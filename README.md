✅ STEP 1 — Add this helper function in TformEmailReceipt

Place this inside the implementation section:

function TformEmailReceipt.CaptureInvoiceToPNG(const FileName: string): Boolean;
var
  Bmp: TBitmap;
begin
  Result := False;
  try
    Bmp := TBitmap.Create;
    try
      // Capture the receipt panel from SelectCashSale form
      if Assigned(formSelectCashSale) then
      begin
        Bmp.Width  := formSelectCashSale.pnlReceiptInner.Width;
        Bmp.Height := formSelectCashSale.pnlReceiptInner.Height;

        // Copy panel content into bitmap
        formSelectCashSale.pnlReceiptInner.PaintTo(Bmp.Canvas, 0, 0);

        // Save as PNG
        with TPngImage.Create do
        try
          Assign(Bmp);
          SaveToFile(FileName);
        finally
          Free;
        end;

        Result := True;
      end;
    finally
      Bmp.Free;
    end;
  except
    Result := False;
  end;
end;

✅ STEP 2 — Modify your SendEmails method

Add these lines before IdSMTP1.Send(IdMessage1).

✔️ Generate PNG file
✔️ Attach to email

Replace your existing code inside SendEmails with this version:

procedure TformEmailReceipt.SendEmails(const EmailList: string);
var
  Rec, ImgFile: string;
  EArr: TArray<string>;
begin
  IdMessage1.Clear;

  IdMessage1.From.Address := 'no-reply@wizbang.co.nz';
  IdMessage1.From.Name    := 'Wizbang Onetap NZ';
  IdMessage1.Subject      := 'Order Receipt';
  IdMessage1.Body.Text :=
    'Dear Customer,' + sLineBreak +
    'Thank you for your order.' + sLineBreak +
    'Attached is your invoice receipt.' + sLineBreak + sLineBreak +
    'Regards,' + sLineBreak +
    'Wizbang Team';

  // Recipients
  EArr := EmailList.Split([';'], TStringSplitOptions.ExcludeEmpty);
  for Rec in EArr do
    IdMessage1.Recipients.Add.Text := Trim(Rec);

  // ---- NEW CODE: SAVE RECEIPT AS PNG ----
  ImgFile := SysUtils.GetTempFileName + '.png';

  if CaptureInvoiceToPNG(ImgFile) then
  begin
    with IdMessage1.MessageParts.Add do
    begin
      FileName := ImgFile;
      ContentType := 'image/png';
    end;
  end
  else
    ShowMessage('Failed to capture receipt image.');

  ConfigureSMTP;

  try
    IdSMTP1.Connect;
    try
      IdSMTP1.Send(IdMessage1);
      ShowMessage('Receipt sent successfully.');
    finally
      IdSMTP1.Disconnect;
    end;
  except
    on E: Exception do
      ShowMessage('Email Sending Failed: ' + E.Message);
  end;
end;

==================================================================================================================================================================================================================

Yes — you can absolutely send the invoice in pure text / tabular format, without creating an image.

This is actually simpler, faster, and produces smaller email size.

Below is a clean approach to do it properly, without touching other units.

✔ How to Send Receipt as Text / Tabular Content (No Image)
You already have:

FrameReceipt: TframeReceipt

It contains labels, grids, totals — the same data shown in cmdLastDes.

You only need a function that extracts textual data from FrameReceipt and converts it into a tabular email-friendly layout.

✔ Step 1: Add Method in TEmailReceiptForm to Convert Receipt to Text
Example (adapt this to your frame components)
function TEmailReceiptForm.BuildReceiptText: string;
begin
  Result := '';
  Result := Result + 'HOTEL XYZ - ORDER RECEIPT' + sLineBreak;
  Result := Result + 'Invoice No: ' + IntToStr(InvoiceID) + sLineBreak;
  Result := Result + 'Date: ' + FormatDateTime('dd/mm/yyyy hh:nn', Now) + sLineBreak;
  Result := Result + '----------------------------------------' + sLineBreak;

  // Example using labels from receipt frame
  Result := Result + Format('%-25s %5s %10s', ['Item', 'Qty', 'Price']) + sLineBreak;
  Result := Result + '----------------------------------------' + sLineBreak;

  // Loop through your item list inside the frame
  if Assigned(ReceiptFrame) then
  begin
    for var I := 0 to ReceiptFrame.ItemGrid.RowCount - 1 do
    begin
      Result := Result + Format('%-25s %5s %10s',
        [
          ReceiptFrame.ItemGrid.Cells[0, I],  // item name
          ReceiptFrame.ItemGrid.Cells[1, I],  // qty
          ReceiptFrame.ItemGrid.Cells[2, I]   // price
        ]) + sLineBreak;
    end;

    Result := Result + '----------------------------------------' + sLineBreak;
    Result := Result + 'Subtotal: ' + ReceiptFrame.lblSubtotal.Caption + sLineBreak;
    Result := Result + 'Tax: ' + ReceiptFrame.lblTax.Caption + sLineBreak;
    Result := Result + 'Total: ' + ReceiptFrame.lblTotal.Caption + sLineBreak;
  end;

  Result := Result + sLineBreak + 'Thank you for dining with us!';
end;


⚠ Replace ItemGrid, lblSubtotal, etc. with the actual components from your TframeReceipt.

✔ Step 2: Send the Text via TIdSMTP

Inside your OK button click or SendEmail method:

procedure TEmailReceiptForm.SendEmail;
var
  ReceiptText: string;
  EmailList: TArray<string>;
  Email: string;
begin
  ReceiptText := BuildReceiptText;
  EmailList := MemoEmails.Text.Split([';'], TStringSplitOptions.ExcludeEmpty);

  IdSMTP1.Host := 'smtp.gmail.com';
  IdSMTP1.Port := 587;
  IdSMTP1.Username := 'your@gmail.com';
  IdSMTP1.Password := 'your-app-password';
  IdSMTP1.UseTLS := utUseStartTLS;

  IdMessage1.Clear;
  IdMessage1.From.Address := 'your@gmail.com';
  IdMessage1.Subject := 'Your Hotel Receipt #' + IntToStr(InvoiceID);
  IdMessage1.Body.Text := ReceiptText;

  try
    IdSMTP1.Connect;

    for Email in EmailList do
    begin
      Email := Trim(Email);
      if Email <> '' then
      begin
        IdMessage1.Recipients.Clear;
        IdMessage1.Recipients.Add.Address := Email;
        IdSMTP1.Send(IdMessage1);
      end;
    end;

  finally
    IdSMTP1.Disconnect;
  end;
end;

✔ Step 3: No Image, No Attachment Needed

Your email will look like this:

HOTEL XYZ - ORDER RECEIPT
Invoice No: 1023
Date: 04/12/2025 18:23
----------------------------------------
Item                      Qty      Price
----------------------------------------
Paneer Butter Masala        1      180.00
Butter Naan                 2       60.00
----------------------------------------
Subtotal: 300.00
Tax: 18.00
Total: 318.00

Thank you for dining with us!


This works beautifully in Gmail, Outlook, Yahoo, and mobile email apps.

✔ Additional Benefits

No need to convert components to bitmaps.

No memory streams.

No PNG attachments.

Text always displays correctly on any device.

Almost zero chance of SMTP size issues.

✔ Answer to Your Question

what if I want to just send the text/tabular format displayed in the invoice receipt and not the image. can we do it

✅ Yes, and it is much easier and cleaner.

All logic stays inside TEmailReceiptForm.
No changes needed in USelectCashSale.
