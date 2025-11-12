function TTabService.clockins: TJSONArray;
begin
  Result := GetClockInOut(GetInvocationMetadata());
end;

// New ClockInOut function:
function TTabService.GetClockInOut(InvokeMetadata: TDSInvocationMetadata)
  : TJSONArray;
var
  JSONArray: TJSONArray;
  // TJSONArray is [{...},{...},....] --> main outer JsonArray
  StaffObject, ClockObj: TJSONObject; // TJSONObject is {key:value, key:value}
  ClockinsArray: TJSONArray; // TJSONArray is [{...},{...},....]
  StartDate, EndDate: TDateTime;
  CurrentStaffID: Integer; // CurrentStaffID tracks the last staff processed.
  sqlvar: String;
begin
  JSONArray := TJSONArray.Create;
  StaffObject := nil;
  ClockinsArray := nil;
  CurrentStaffID := -1;

  try
    // Default values: today
    StartDate := Now;
    EndDate := Now;

    // Read from URL query parameters (if provided)
    if assigned(InvokeMetadata) and assigned(InvokeMetadata.QueryParams) then
    begin
      if InvokeMetadata.QueryParams.Values['start'] <> '' then
        TryISO8601ToDate(InvokeMetadata.QueryParams.Values['start'], StartDate);
      if InvokeMetadata.QueryParams.Values['end'] <> '' then
        TryISO8601ToDate(InvokeMetadata.QueryParams.Values['end'], EndDate);
    end;

    StartDate := StartOfTheDay(StartDate);
    EndDate := EndOfTheDay(EndDate);

    // Prepare SQL query
    with qr do
    begin
      Unprepare;
      SQL.Clear;
      SQL.Add('SELECT C.STAFFID, S.STAFFNO, S.STAFFNAME, P.FIRSTNAME, P.SURNAME, ');
      SQL.Add('       C.CLOCKINID, C.CLOCKINTIME, C.CLOCKOUTTIME, C.WHOCLOCKIN, C.WHOCLOCKOUT, ');
      SQL.Add('       C.WHENCLOCKINTIME, C.WHENCLOCKOUTTIME, S.HOURLYRATE, C.WHENEDITED, C.LOGINID ');
      SQL.Add('FROM CLOCKIN AS C ');
      SQL.Add('JOIN STAFF AS S ON C.STAFFID = S.STAFFID ');
      SQL.Add('JOIN PERORG AS P ON P.PERORGID = S.PERORGID ');
      SQL.Add('WHERE C.CLOCKINTIME >= :startdate AND C.CLOCKOUTTIME <= :enddate ');
      SQL.Add('ORDER BY C.STAFFID, C.CLOCKINTIME ASC');
      Prepare;
      ParamByName('startdate').AsDateTime := StartDate;
      ParamByName('enddate').AsDateTime := EndDate;
      Open;
    end;

    ShowMessage('StartDate: ' + DateTimeToStr(StartDate));
    ShowMessage('EndDate: ' + DateTimeToStr(EndDate));
    ShowMessage('SQL: ' + qr.SQL.Text);
   sqlvar := qr.SQL.Text;

    // Build nested JSON
    if not qr.IsEmpty then
    begin
      qr.First;
      // Loop through all rows
      while not qr.EOF do
      begin
        // If the current row has a different STAFFID, it means we are starting a new staff group.
        if qr.FieldByName('STAFFID').AsInteger <> CurrentStaffID then
        begin
          // Add the previous staff object to JsonArray
          if assigned(StaffObject) then
            JSONArray.AddElement(StaffObject);

          // Create new staff object
          StaffObject := TJSONObject.Create;
          StaffObject.AddPair('StaffID',
            TJSONNumber.Create(qr.FieldByName('STAFFID').AsInteger));
          StaffObject.AddPair('StaffNo', qr.FieldByName('STAFFNO').AsString);
          StaffObject.AddPair('StaffName', qr.FieldByName('STAFFNAME')
            .AsString);
          StaffObject.AddPair('FirstName', qr.FieldByName('FIRSTNAME')
            .AsString);
          StaffObject.AddPair('LastName', qr.FieldByName('SURNAME').AsString);

          ClockinsArray := TJSONArray.Create;
          // Add a nested JSON array (ClockinsArray) as the value for the key "Clockins"
          StaffObject.AddPair('Clockins', ClockinsArray);

          CurrentStaffID := qr.FieldByName('STAFFID').AsInteger;
        end;

        // Add clock-in record
        ClockObj := TJSONObject.Create;
        ClockObj.AddPair('ClockID',
          TJSONNumber.Create(qr.FieldByName('CLOCKINID').AsInteger));
        ClockObj.AddPair('ClockinTime',
          DateTimeToStr(qr.FieldByName('CLOCKINTIME').AsDateTime));
        ClockObj.AddPair('ClockoutTime',
          DateTimeToStr(qr.FieldByName('CLOCKOUTTIME').AsDateTime));
        ClockObj.AddPair('WhoClockin', qr.FieldByName('WHOCLOCKIN').AsString);
        ClockObj.AddPair('WhoClockout', qr.FieldByName('WHOCLOCKOUT').AsString);
        ClockObj.AddPair('WhenClockinTime',
          DateTimeToStr(qr.FieldByName('WHENCLOCKINTIME').AsDateTime));
        ClockObj.AddPair('WhenClockoutTime',
          DateTimeToStr(qr.FieldByName('WHENCLOCKOUTTIME').AsDateTime));
        ClockObj.AddPair('HourlyRate',
          TJSONNumber.Create(qr.FieldByName('HOURLYRATE').AsFloat));
        ClockObj.AddPair('WhenEdited',
          DateTimeToStr(qr.FieldByName('WHENEDITED').AsDateTime));
        ClockObj.AddPair('WhoEdited', qr.FieldByName('LOGINID').AsString);

        // Adds ClockObject to ClockinsArray
        ClockinsArray.AddElement(ClockObj);
        qr.Next;
      end;

      // Add last staff object
      if assigned(StaffObject) then
        JSONArray.AddElement(StaffObject);
    end;

    Result := JSONArray;
  except
    on e: Exception do
    begin
      if assigned(JSONArray) then
        JSONArray.Free;
      ErrorResponse(e.Message);
      LogFileException(e, Self.ClassName, True);
      Result := nil;
    end;
  end;
end;
