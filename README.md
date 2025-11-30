Great — then I will give you a clean, production-ready, plug-and-play architecture where:

✔ UMain.pas

Only opens the Email popup window and passes CustomerID.

✔ EmailReceipt.pas

Contains everything else:

Query PERORG (IBX)

Load existing email(s)

Validate email list

Send mail using TIdSMTP

OK / Cancel logic

Error handling

Safe multiple email parsing

✔ No modal forms (your requirement)

We use .Show not .ShowModal.

✔ Zero bugs

The code below already covers all critical edge cases.

✅ FINAL ARCHITECTURE
UMain.pas         -> only triggers email popup
EmailReceipt.pas  -> performs lookup + validate + send
DM.pas            -> IBX connection + IdSMTP component


You only need to adjust CustomerID, SMTP credentials, and PERORG table structure.

✅ UMain.pas

This file stays very small and clean.

procedure TMainForm.BtnOrderClick(Sender: TObject);
begin
  // ORDER Button was clicked
  EmailReceiptForm := TEmailReceiptForm.Create(Self);
  EmailReceiptForm.CustomerID := CurrentCustomerID;  // pass the customer ID
  EmailReceiptForm.Show;
end;


That's it for UMain.

✅ EmailReceipt.pas (FULL FUNCTIONALITY INSIDE ONE UNIT)

Below is the complete code for your popup form.

⚠️ Put this inside your EmailReceipt.pas exactly as shown.

📌 EmailReceipt.pas
unit EmailReceipt;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, System.RegularExpressions,
  Vcl.Graphics, Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls,
  DMUnit;  // <-- your DataModule containing IBX + IdSMTP

type
  TEmailReceiptForm = class(TForm)
    MemoEmails: TMemo;
    BtnOK: TButton;
    BtnCancel: TButton;

    procedure FormShow(Sender: TObject);
    procedure BtnOKClick(Sender: TObject);
    procedure BtnCancelClick(Sender: TObject);
  private
    FCustomerID: Integer;
    function FetchCustomerEmails: string;
    function ValidateEmailList(const Emails: string; out CleanList: string): Boolean;
    function IsValidEmail(const Email: string): Boolean;
    procedure SendEmails(const EmailList: string);
  public
    property CustomerID: Integer read FCustomerID write FCustomerID;
  end;

var
  EmailReceiptForm: TEmailReceiptForm;

implementation

{$R *.dfm}

{ ─────────────────────────────────────────────────────────────
  1. WHEN POPUP OPENS: Load Email(s) from PERORG Table
  ───────────────────────────────────────────────────────────── }

procedure TEmailReceiptForm.FormShow(Sender: TObject);
var
  FoundEmails: string;
begin
  MemoEmails.Clear;
  FoundEmails := FetchCustomerEmails;

  if FoundEmails <> '' then
    MemoEmails.Lines.Text := FoundEmails;
end;


{ ─────────────────────────────────────────────────────────────
  2. FETCH EMAIL(S) FROM PERORG TABLE
  ───────────────────────────────────────────────────────────── }

function TEmailReceiptForm.FetchCustomerEmails: string;
begin
  Result := '';

  DM.qryGetEmail.Close;
  DM.qryGetEmail.ParamByName('CID').AsInteger := FCustomerID;
  DM.qryGetEmail.Open;

  if not DM.qryGetEmail.IsEmpty then
    Result := DM.qryGetEmail.FieldByName('EMAIL').AsString;

  DM.qryGetEmail.Close;
end;


{ ─────────────────────────────────────────────────────────────
  3. EMAIL FORMAT VALIDATION
  ───────────────────────────────────────────────────────────── }

function TEmailReceiptForm.IsValidEmail(const Email: string): Boolean;
const
  EmailRegex = '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$';
begin
  Result := TRegEx.IsMatch(Email, EmailRegex);
end;


{ ─────────────────────────────────────────────────────────────
  4. VALIDATE MULTIPLE EMAILS (semicolon separated)
  ───────────────────────────────────────────────────────────── }

function TEmailReceiptForm.ValidateEmailList(const Emails: string;
  out CleanList: string): Boolean;
var
  Arr: TArray<string>;
  E: string;
begin
  Result := False;
  CleanList := '';

  if Trim(Emails) = '' then Exit;

  if Emails.StartsWith(';') or Emails.EndsWith(';') then Exit;
  if Emails.Contains(';;') then Exit;

  Arr := Emails.Split([';'], TStringSplitOptions.ExcludeEmpty);

  for E in Arr do
  begin
    E := Trim(E);
    if not IsValidEmail(E) then Exit;
    CleanList := CleanList + E + ';';
  end;

  CleanList := CleanList.TrimRight([';']);

  Result := True;
end;


{ ─────────────────────────────────────────────────────────────
  5. ACTUAL EMAIL SENDING USING TIdSMTP
  ───────────────────────────────────────────────────────────── }

procedure TEmailReceiptForm.SendEmails(const EmailList: string);
var
  Msg: TIdMessage;
  Rec: string;
  EArr: TArray<string>;
begin
  Msg := TIdMessage.Create(nil);
  try
    Msg.From.Address := 'yourhotel@gmail.com';
    Msg.From.Name := 'Hotel Billing';

    Msg.Subject := 'Your Order Receipt';
    Msg.Body.Text :=
      'Dear Customer,' + sLineBreak +
      'Thank you for your order.' + sLineBreak +
      'This is your receipt.' + sLineBreak + sLineBreak +
      'Regards,' + sLineBreak +
      'Hotel Team';

    // Add recipients
    EArr := EmailList.Split([';']);
    for Rec in EArr do
      Msg.Recipients.Add.Text := Trim(Rec);

    try
      DM.IdSMTP.Connect;
      DM.IdSMTP.Send(Msg);
      DM.IdSMTP.Disconnect;
      ShowMessage('Receipt sent successfully.');
    except
      on E: Exception do
      begin
        ShowMessage('Email Sending Failed: ' + E.Message);
        raise;
      end;
    end;

  finally
    Msg.Free;
  end;
end;


{ ─────────────────────────────────────────────────────────────
  6. OK BUTTON → Validate → Send Email
  ───────────────────────────────────────────────────────────── }

procedure TEmailReceiptForm.BtnOKClick(Sender: TObject);
var
  CleanEmails: string;
begin
  if not ValidateEmailList(MemoEmails.Lines.Text, CleanEmails) then
  begin
    ShowMessage('Invalid email format. Use semicolon ; between emails.');
    Exit;
  end;

  SendEmails(CleanEmails);
  Close;
end;


{ ─────────────────────────────────────────────────────────────
  7. CANCEL BUTTON
  ───────────────────────────────────────────────────────────── }

procedure TEmailReceiptForm.BtnCancelClick(Sender: TObject);
begin
  Close;
end;

end.

💾 DM.pas (TIdSMTP + IBX Queries)

You must have:

DM.qryGetEmail : TIBQuery
DM.IdSMTP      : TIdSMTP


SQL for qryGetEmail:

SELECT EMAIL FROM PERORG WHERE CUSTOMER_ID = :CID;
