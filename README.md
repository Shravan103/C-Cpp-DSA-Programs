unit UEmailReceipt;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Buttons, System.RegularExpressions,
  IdSMTP, IdMessage, IdSSLOpenSSL, IdSSL, IdBaseComponent, IdComponent,
  IdTCPConnection, IdTCPClient, IdMessageClient, IdSMTPBase;

type
  TformEmailReceipt = class(TForm)
    MemoEmails: TMemo;
    cmdOK: TButton;
    cmdCancel: TButton;

    IdSMTP1: TIdSMTP;
    IdMessage1: TIdMessage;
    IdSSL: TIdSSLIOHandlerSocketOpenSSL;

    procedure FormShow(Sender: TObject);
    procedure cmdOKClick(Sender: TObject);
    procedure cmdCancelClick(Sender: TObject);

  private
    function IsValidEmail(const Email: string): Boolean;
    function ValidateEmailList(const Emails: string; out CleanList: string): Boolean;
    procedure ConfigureSMTP;
    procedure SendEmails(const EmailList: string);

    function FetchCustomerEmails: string;

  public
    FAccountName: string;
  end;

var
  formEmailReceipt: TformEmailReceipt;

implementation

uses
  DM_WBWaiter, UPaymentTable;

{$R *.dfm}

{ ------------------------------------------------------------------------- }
procedure TformEmailReceipt.FormShow(Sender: TObject);
var
  FoundEmails: string;
begin
  MemoEmails.Clear;

  FAccountName := UPaymentTable.formPaymentTable.txtAccountName.Text;

  if FAccountName <> '' then
  begin
    FoundEmails := FetchCustomerEmails;
    if FoundEmails <> '' then
      MemoEmails.Lines.Text := FoundEmails;
  end;
end;

{ ------------------------------------------------------------------------- }
function TformEmailReceipt.FetchCustomerEmails: string;
begin
  Result := '';

  DM.qryGetEmail.Close;
  DM.qryGetEmail.SQL.Text :=
    'SELECT P.EMAIL ' +
    'FROM PERORG P ' +
    'JOIN ACCOUNT A ON A.PERORGID = P.PERORGID ' +
    'WHERE UPPER(TRIM(A.ACCOUNTNAME)) = UPPER(TRIM(:ACCNAME))';

  DM.qryGetEmail.ParamByName('ACCNAME').AsString := FAccountName;
  DM.qryGetEmail.Open;

  if not DM.qryGetEmail.IsEmpty then
    Result := DM.qryGetEmail.FieldByName('EMAIL').AsString;
end;

{ ------------------------------------------------------------------------- }
function TformEmailReceipt.IsValidEmail(const Email: string): Boolean;
const
  EmailRegex = '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$';
begin
  Result := TRegEx.IsMatch(Email, EmailRegex);
end;

{ ------------------------------------------------------------------------- }
function TformEmailReceipt.ValidateEmailList(const Emails: string;
  out CleanList: string): Boolean;
var
  Arr: TArray<string>;
  E, S: string;
begin
  Result := False;
  CleanList := '';

  if Trim(Emails) = '' then Exit;

  if Emails.StartsWith(';') or Emails.EndsWith(';') then Exit;
  if Emails.Contains(';;') then Exit;

  Arr := Emails.Split([';'], TStringSplitOptions.ExcludeEmpty);

  for E in Arr do
  begin
    S := Trim(E);
    if (S = '') or (not IsValidEmail(S)) then Exit;

    if CleanList <> '' then CleanList := CleanList + ';';
    CleanList := CleanList + S;
  end;

  Result := True;
end;

{ ------------------------------------------------------------------------- }
procedure TformEmailReceipt.ConfigureSMTP;
begin
  // Office 365 settings
  IdSMTP1.Host := 'smtp.office365.com';
  IdSMTP1.Port := 587;
  IdSMTP1.Username := 'alert@wizbang.co.nz';
  IdSMTP1.Password := 'DeV3l0pm3nt88';
  IdSMTP1.UseTLS := utUseExplicitTLS;

  // SSL Setup
  IdSSL.SSLOptions.SSLVersions := [sslvTLSv1_2]; // Office365 requires TLS 1.2+
  IdSSL.SSLOptions.Mode := sslmClient;

  IdSMTP1.IOHandler := IdSSL;     // Attach SSL handler BEFORE connecting
  IdSMTP1.AuthType := satDefault; // Required for Exchange/Office365
end;

{ ------------------------------------------------------------------------- }
procedure TformEmailReceipt.SendEmails(const EmailList: string);
var
  E: string;
  Arr: TArray<string>;
begin
  IdMessage1.Clear;

  IdMessage1.From.Address := 'alert@wizbang.co.nz';
  IdMessage1.From.Name := 'Wizbang Onetap NZ';
  IdMessage1.Subject := 'Order Receipt';

  IdMessage1.Body.Text :=
    'Dear Customer,' + sLineBreak +
    'Thank you for your order.' + sLineBreak +
    'Attached is your receipt.' + sLineBreak +
    'Regards,' + sLineBreak +
    'Wizbang Team';

  Arr := EmailList.Split([';']);
  for E in Arr do
    IdMessage1.Recipients.Add.Address := Trim(E);

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
      ShowMessage('Email sending failed: ' + E.Message);
  end;
end;

{ ------------------------------------------------------------------------- }
procedure TformEmailReceipt.cmdOKClick(Sender: TObject);
var
  CleanList: string;
begin
  if ValidateEmailList(MemoEmails.Text, CleanList) then
    SendEmails(CleanList)
  else
    ShowMessage('Invalid email list. Please correct and try again.');
end;

{ ------------------------------------------------------------------------- }
procedure TformEmailReceipt.cmdCancelClick(Sender: TObject);
begin
  Close;
end;

end.
