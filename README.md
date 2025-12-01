unit UEmailReceipt;

{ ****************************************************************************** }
interface

{ ****************************************************************************** }
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  UDetailForm, StdCtrls, ExtCtrls, Buttons, ImgList,System.RegularExpressions,
  System.ImageList {,IvDictio, IvMulti}, IdSMTP, IdMessage, IdSSLOpenSSL, IdIOHandlerStack,
  IdIOHandler, IdIOHandlerSocket, IdSSL, IdBaseComponent, IdComponent,
  IdTCPConnection, IdTCPClient, IdExplicitTLSClientServerBase, IdMessageClient,
  IdSMTPBase;

{ ****************************************************************************** }
type
  (* *
    Sends Receipt to Customer Emails.
  *)
  TformEmailReceipt = class(TformDetailForm)
    MemoEmails: TMemo;
    cmdAbout: TSpeedButton;
    cmdgmailcomDes: TSpeedButton;
    cmdAtTheRateDes: TSpeedButton;
    IdSMTP1: TIdSMTP;
    IdMessage1: TIdMessage;
    IdSSLIOHandlerSocketOpenSSL1: TIdSSLIOHandlerSocketOpenSSL;
    procedure FormShow(Sender: TObject);
    procedure cmdOkClick(Sender: TObject);
    procedure cmdHideClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);

    procedure cmdTimeClick(Sender: TObject);
    procedure cmdDateClick(Sender: TObject);
    procedure cmdAboutClick(Sender: TObject);
    procedure cmdAtTheRateDesClick(Sender: TObject);
    procedure cmdgmailcomDesClick(Sender: TObject);
    private
      function IsValidEmail(const Email: string): Boolean;
      function ValidateEmailList(const Emails: string; out CleanList: string): Boolean;
      function FetchCustomerEmails: string;
      procedure ConfigureSMTP;
      procedure SendEmails(const EmailList: string);
    public
      FAccountName: String;
  end;

  { ****************************************************************************** }
var
  formEmailReceipt: TformEmailReceipt;

  { ****************************************************************************** }
implementation

{ ****************************************************************************** }
{$R *.DFM}

{ ****************************************************************************** }
uses
  USelectDateTime, LMain, UMain, DM_WBWaiter, UFiscalMenu,
  UBaseDetailForm, UAboutBoxPOS, UPaymentTable;

{ ****************************************************************************** }
procedure TformEmailReceipt.FormShow(Sender: TObject);
var
  FoundEmails: string;
begin
  if ((KBLayout = QWERTY) and (cmdADes.Top <> 44)) or
    ((KBLayout = ALPHA) and (cmdADes.Top <> 0)) then
  begin
    LayOutKeys;
  end;
  cmdFiscalMenu.Visible := AppDetails.EnableFiscalPrinting;

  // "gmail.com" button
  cmdgmailcomDes.Left := cmdLeftBracketDes.Left;
  cmdgmailcomDes.Top := cmdLeftBracketDes.Top;
  cmdgmailcomDes.Width := cmdLeftBracketDes.Width + cmdLeftBracketDes.Width;
  cmdLeftBracketDes.Hide;
  cmdRightBracketDes.Hide;

  // Second "@" button
  cmdAtTheRateDes.Left := cmdDivideDes.Left;
  cmdAtTheRateDes.Top := cmdDivideDes.Top;
  cmdAtTheRateDes.Width := cmdDivideDes.Width;
  cmdAtTheRateDes.Height := cmdDivideDes.Height;
  cmdDivideDes.Hide;

  // Fetch Emails of Users:
  MemoEmails.Clear;
  FAccountName := UPaymentTable.formPaymentTable.txtAccountName.Text;
  if (FAccountName <> '') then
  begin
    FoundEmails := FetchCustomerEmails;
    if FoundEmails <> '' then
    MemoEmails.Lines.Text := FoundEmails;
  end;
end;
{ ****************************************************************************** }
function TformEmailReceipt.IsValidEmail(const Email: string): Boolean;
const
  EmailRegex = '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$';
begin
  Result := TRegEx.IsMatch(Email, EmailRegex);
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.SendEmails(const EmailList: string);
var
  Rec: string;
  EArr: TArray<string>;
begin
  IdMessage1.Clear;
  IdMessage1.From.Address := 'alert@wizbang.co.nz';
  IdMessage1.From.Name := 'Wizbang Onetap NZ';
  IdMessage1.Subject := 'Order Receipt';
  IdMessage1.Body.Text :=
    'Dear Customer,' + sLineBreak +
    'Thank you for your order.' + sLineBreak +
    'This is your receipt.' + sLineBreak + sLineBreak +
    'Regards,' + sLineBreak +
    'Wizbang Team';

  EArr := EmailList.Split([';']);
  for Rec in EArr do
    IdMessage1.Recipients.Add.Text := Trim(Rec);

  ConfigureSMTP;

  try
    IdSMTP1.Connect;
    IdSMTP1.Send(IdMessage1);
    IdSMTP1.Disconnect;
    ShowMessage('Receipt sent successfully.');
  except
    on E: Exception do
      ShowMessage('Email Sending Failed: ' + E.Message);
  end;
end;
{ ****************************************************************************** }
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
    if S = '' then Exit;
    if not IsValidEmail(S) then Exit;

    if CleanList <> '' then
      CleanList := CleanList + ';';
    CleanList := CleanList + S;
  end;

  Result := True;
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.cmdgmailcomDesClick(Sender: TObject);
begin
  KeyStrokeString(TSpeedButton(Sender).Caption);
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.cmdHideClick(Sender: TObject);
begin
  inherited;
  Close;
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.cmdTimeClick(Sender: TObject);
var
  TheNow: TDateTime;
begin
  TheNow := Now;
  if (SelectDateTime(TheNow)) then
  begin
    MemoEmails.SelText := FormatDateTime(DATEFORMAT + ' ' +
      TIMEFORMAT, TheNow);
  end;
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.ConfigureSMTP;
begin
//  IdSMTP1.Host := 'wizbang-co-nz.mail.protection.outlook.com';
  IdSMTP1.Host := 'smtp.office365.com';
  IdSMTP1.Port := 587;
  IdSMTP1.Username := 'alert@wizbang.co.nz';
  IdSMTP1.Password := 'DeV3l0pm3nt88';

  IdSMTP1.UseTLS := utUseExplicitTLS;

  // Configure SSL/TLS handler
  IdSSLIOHandlerSocketOpenSSL1.SSLOptions.Method := sslvTLSv1_2;
  IdSSLIOHandlerSocketOpenSSL1.SSLOptions.Mode := sslmClient;

  // Link SSL handler to SMTP
  IdSMTP1.IOHandler := IdSSLIOHandlerSocketOpenSSL1;

end;

{ ****************************************************************************** }
procedure TformEmailReceipt.cmdAtTheRateDesClick(Sender: TObject);
begin
  ShiftDown;
  KeyStroke(TSpeedButton(Sender).Caption[1]);
end;

{ ****************************************************************************** }
procedure TformEmailReceipt.cmdDateClick(Sender: TObject);
var
  TheNow: TDateTime;
begin
  TheNow := Now;
  if (SelectDate(TheNow)) then
  begin
    MemoEmails.SelText := FormatDateTime(DATEFORMAT, TheNow);
  end;
end;

{ ****************************************************************************** }
procedure TformEmailReceipt.cmdOkClick(Sender: TObject);
var
  CleanList: string;
begin
  inherited;
  if ValidateEmailList(MemoEmails.Text, CleanList) then
    SendEmails(CleanList)
  else
    ShowMessage('Invalid email list. Please correct and try again.');
end;

{ ****************************************************************************** }
procedure TformEmailReceipt.cmdAboutClick(Sender: TObject);
begin
  inherited;
  ShowAboutBox('onetap POS');
end;
{ ****************************************************************************** }
function TformEmailReceipt.FetchCustomerEmails: string;
begin
  Result := '';

  DM.qryGetEmail.Close;
  DM.qryGetEmail.SQL.Clear;
  DM.qryGetEmail.SQL.Add('SELECT P.EMAIL');
  DM.qryGetEmail.SQL.Add('FROM PERORG P');
  DM.qryGetEmail.SQL.Add('JOIN ACCOUNT A ON A.PERORGID = P.PERORGID');
  DM.qryGetEmail.SQL.Add('WHERE UPPER(TRIM(A.ACCOUNTNAME)) = UPPER(TRIM(:ACCNAME))');

  DM.qryGetEmail.ParamByName('ACCNAME').AsString := FAccountName;
  DM.qryGetEmail.Open;

  if not DM.qryGetEmail.IsEmpty then
    Result := DM.qryGetEmail.FieldByName('EMAIL').AsString;

  DM.qryGetEmail.Close;
end;
{ ****************************************************************************** }
procedure TformEmailReceipt.FormCreate(Sender: TObject);
begin
  inherited;
  pnlControlButtons.Width := 208;
  cmdLayout.Left := 0;
  cmdAbout.Left := 52;
  cmdHide.Left := 104;
  cmdOk.Left := 156;
end;

end.
