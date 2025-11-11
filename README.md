This repository consists of following codes:
1. Object-Oriented-Programming(C++)
2. Data Structures(C)

function TTabService.GetClockInOut(InvokeMetadata: TDSInvocationMetadata): TJSONArray;
var
  JSONArray: TJSONArray;
  JSONObject: TJSONObject;
  StartDate, EndDate: TDateTime;
begin
  JSONArray := nil;
  try
    // Default values: today and now
    StartDate := Now;
    EndDate := Now;

    // Read from URL query parameters (if provided)
    if (InvokeMetadata.QueryParams.Values['start'] <> '') then
      StartDate := StrToDate(InvokeMetadata.QueryParams.Values['start']);
    if (InvokeMetadata.QueryParams.Values['end'] <> '') then
      EndDate := StrToDate(InvokeMetadata.QueryParams.Values['end']);

    // Prepare the query
    with qr do
    begin
      Unprepare;
      with SQL do
      begin
        Clear;
        Add('SELECT employeeid, clockin_time, clockout_time');
        Add('FROM clockin');
        Add('WHERE clockin_time BETWEEN :startdate AND :enddate');
        Add('ORDER BY clockin_time ASC');
      end;
      Prepare;
      ParamByName('startdate').AsDateTime := StartDate;
      ParamByName('enddate').AsDateTime := EndDate;
      Open;
    end;

    // Build JSON result
    if not qr.IsEmpty then
    begin
      JSONArray := TJSONArray.Create;
      qr.First;
      while not qr.EOF do
      begin
        JSONObject := TJSONObject.Create;
        JSONObject.AddPair('EmployeeID', TJSONNumber.Create(qr.FieldByName('employeeid').AsInteger));
        JSONObject.AddPair('ClockIn', DateTimeToStr(qr.FieldByName('clockin_time').AsDateTime));
        JSONObject.AddPair('ClockOut', DateTimeToStr(qr.FieldByName('clockout_time').AsDateTime));
        JSONArray.AddElement(JSONObject);
        qr.Next;
      end;
    end
    else
      JSONArray := TJSONArray.Create;

    Result := JSONArray;

  except
    on E: Exception do
    begin
      if Assigned(JSONArray) then JSONArray.Free;
      ErrorResponse(E.Message);
      LogFileException(E, Self.ClassName, True);
      Result := nil;
    end;
  end;
end;
