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
  SSLORTLS: Integer;

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
  //  IdMessage1.From.Address := 'alert@wizbang.co.nz';
  IdMessage1.From.Address := 'no-reply@wizbang.co.nz';
  IdMessage1.From.Name := 'Wizbang Onetap NZ';
  IdMessage1.Subject := 'Order Receipt';
  IdMessage1.Body.Text :=
    'Dear Customer,' + sLineBreak +
    'Thank you for your order.' + sLineBreak +
    'This is your receipt.' + sLineBreak + sLineBreak +
    'Regards,' + sLineBreak +
    'Wizbang Team';

  // Recipients (semicolon-separated)
  EArr := EmailList.Split([';'], TStringSplitOptions.ExcludeEmpty);
  for Rec in EArr do
    IdMessage1.Recipients.Add.Text := Trim(Rec);

  // Configure SMTP/SSL based on SSLORTLS flag
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
{ ****************************************************************************** }
procedure TformEmailReceipt.ConfigureSMTP;
begin
  SSLORTLS := 0;
  // Ensure SSL handler exists
  if not Assigned(IdSSLIOHandlerSocketOpenSSL1) then
    raise Exception.Create('IdSSLIOHandlerSocketOpenSSL1 is not created.');

  if IdSMTP1.Connected then
    IdSMTP1.Disconnect(False);
  IdSMTP1.IOHandler := nil;
  IdSMTP1.UseTLS := utNoTLSSupport;

  IdSMTP1.Host := 'wizbang-co-nz.mail.protection.outlook.com';
//  IdSMTP1.Host := 'smtp.office365.com';
  IdSMTP1.AuthType := satDefault;
  // IdSMTP1.Username := 'alert@wizbang.co.nz';
  IdSMTP1.Username := 'no-reply@wizbang.co.nz';
  IdSMTP1.Password := 'DeV3l0pm3nt88';
  IdSMTP1.ConnectTimeout := 30000;

  if SSLORTLS = 1 then
  begin
    IdSMTP1.Port := 587;

    // TLS/SSL settings
    IdSSLIOHandlerSocketOpenSSL1.SSLOptions.Method := sslvSSLv23;
    IdSSLIOHandlerSocketOpenSSL1.SSLOptions.Mode := sslmClient;
    IdSSLIOHandlerSocketOpenSSL1.SSLOptions.VerifyMode := [];
    IdSSLIOHandlerSocketOpenSSL1.SSLOptions.VerifyDepth := 0;

    // Link SSL handler and enable STARTTLS
    IdSMTP1.IOHandler := IdSSLIOHandlerSocketOpenSSL1;
    IdSMTP1.UseTLS := utUseExplicitTLS; // STARTTLS
  end
  else
  begin
    // Plain SMTP (no SSL/TLS)
    IdSMTP1.IOHandler := nil;
    IdSMTP1.UseTLS := utNoTLSSupport;
    IdSMTP1.Port := 25;
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

-----------------------------------------------------------------------------------------------------------


unit UPaymentTable;
{******************************************************************************}
interface
{******************************************************************************}
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  UBaseDetailForm, ExtCtrls, StdCtrls, Buttons, Mask, IB_Controls, ComCtrls,
  Grids, IB_Grid, fcImgBtn, fcShapeBtn, Contnrs,
  UTenderLine, UTendering, USelectLoyaltyReward, UIntegerList,
  USkin{,IvDictio, IvMulti},  UGlobalVariables,
  UTableManagementBase, UTableManagementIntegration, UDiscountCalc, UOrderLine,
  UCombos, USubsidyCalc, IB_EditButton;
{******************************************************************************}
procedure PaymentTable;
{******************************************************************************}
type
  TVisibleWindow = (vwToPay, vwTender);
  TRecType = (rtUnTenderedItems, rtTenderedItems, rtBoth);

  TManagerAuthorise = record
    Authorise: Boolean;
    AuthoriseLoginID: Integer;
    AuthorisePriv6: Boolean;
    AuthoriseAccountRank: Integer;
    AuthoriseAccChargeRank: Integer;
    AuthoriseAccPaymentRank: Integer;
    AuthorisePriv13: Boolean;
  end;

  (**
    Used to make payments against an account.
    It is a Key form, used often.
    By default we are paying off the entire bill. If we only want to pay a part
    of it we can select items, items belonging to a position, or state we are
    paying a certain amount. We are able to select and pay on a Debtor Account.
  *)
  TformPaymentTable = class(TformBaseDetailForm)
    pnlTotals: TPanel;
    pnlTotalsRight: TPanel;
    cmdOkDes: TSpeedButton;
    lblSumToPay: TLabel;
    lblStillDue: TLabel;
    lblPayment: TLabel;
    txtSumToPay: TEdit;
    txtStillDue: TEdit;
    txtPayment: TEdit;
    lblBalDue: TLabel;
    txtBalDue: TEdit;
    cmdAuthoriseDes: TSpeedButton;
    pnlAvail: TPanel;
    lblAvailDollarSign: TLabel;
    lblAvail: TLabel;
    txtAvail: TEdit;
    pnlToPay: TPanel;
    pnlOLs: TPanel;
    pnlOLControls: TPanel;
    cmdOLUpDes: TSpeedButton;
    cmdOLDownDes: TSpeedButton;
    cmdAddPosDes: TSpeedButton;
    pnlToPayBorder: TPanel;
    pnlOLsToPay: TPanel;
    pnlToPayControls: TPanel;
    cmdTenderDes: TSpeedButton;
    cmdOLClearDes: TSpeedButton;
    cmdOLDeleteDes: TSpeedButton;
    cmdAllDes: TSpeedButton;
    grdToPay: TStringGrid;
    pnlToPayQty: TPanel;
    cmdToPayUpDes: TSpeedButton;
    cmdToPayDownDes: TSpeedButton;
    lblToPay: TLabel;
    lblToPayCopy: TLabel;
    lblQty: TLabel;
    lblSalesTax: TLabel;
    txtToPay: TEdit;
    txtToPayCopy: TEdit;
    txtQty: TEdit;
    txtSalesTax: TEdit;
    pnlToPayPadding: TPanel;
    pnlTender: TPanel;
    pnlTenderLinesBorder: TPanel;
    pnlTenderLines: TPanel;
    grdTLs: TStringGrid;
    pnlTenderTenders: TPanel;
    cmdToPayDes: TSpeedButton;
    cmdTLDeleteDes: TSpeedButton;
    cmdTLClearDes: TSpeedButton;
    cmdDetailsDes: TSpeedButton;
    cmdCashDes: TSpeedButton;
    cmdEFTPOSDes: TSpeedButton;
    cmdVisaDes: TSpeedButton;
    cmdAMEXDes: TSpeedButton;
    cmdMasterCardDes: TSpeedButton;
    cmdDinersDes: TSpeedButton;
    cmdChequeDes: TSpeedButton;
    cmdOtherDes: TSpeedButton;
    cmdVoucherDes: TSpeedButton;
    cmdTLUpDes: TSpeedButton;
    cmdTLDownDes: TSpeedButton;
    cmdAmountToPayDes: TSpeedButton;
    cmdEditDes: TSpeedButton;
    cmdPMSDes: TSpeedButton;
    txtTTender: TEdit;
    txtTChange: TEdit;
    txtTTip: TEdit;
    txtTPayment: TEdit;
    pnlAccounts: TPanel;
    pnlAccountControlsBorder: TPanel;
    pnlAccountControls: TPanel;
    lblAccountName: TLabel;
    cmdUpAccountsDes: TSpeedButton;
    cmdDownAccountsDes: TSpeedButton;
    cmdSelectAccountDes: TSpeedButton;
    cmdDeselectAccountDes: TSpeedButton;
    lblDiscountPercent: TLabel;
    lblDiscountAmount: TLabel;
    cmdDiscountPercentDes: TSpeedButton;
    cmdDiscountAmountDes: TSpeedButton;
    lblAllowCredit: TLabel;
    cmdAccountNoLookupDes: TSpeedButton;
    cmdOnAccountDes: TSpeedButton;
    txtAccountName: TEdit;
    txtDiscountPercent: TEdit;
    txtDiscountAmount: TEdit;
    pnlAccountButtons: TPanel;
    scrAccounts: TScrollBox;
    cmdSummaryTableDes: TSpeedButton;
    cmdLastDes: TSpeedButton;
    grdUnInvoicedList: TStringGrid;
    lblComboDisc: TLabel;
    txtComboDisc: TEdit;
    cmdComboIndexDes: TSpeedButton;
    lblAllowItemLevelDiscount: TLabel;
    cmdGetCPFDes: TSpeedButton;
    cmdEventDes: TSpeedButton;
    pnlTabDetails: TPanel;
    lblTableName: TLabel;
    lblTableNo: TLabel;
    lblStaff: TLabel;
    txtTableName: TEdit;
    txtTableNo: TEdit;
    txtStaff: TIB_Edit;
    pnlLoyalty: TPanel;
    cmdLoyaltyDes: TSpeedButton;
    cmdExternalAccLookupDes: TSpeedButton;
    cmdExternalAccRIPDes: TSpeedButton;
    cmdTaxCoreBuyerDetailDes: TSpeedButton;
    cmdGoodyDes: TSpeedButton;

    procedure cmdOkClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure cmdOLDeleteClick(Sender: TObject);
    function AddOrderLine(AList:TList;AOLIndex: Integer): Boolean;
    procedure AddAllOrderLines;
    procedure DeleteOrderLine;
    procedure SetOLCols;
    procedure SetUnInvoicedOLCols;
    procedure cmdAllClick(Sender: TObject);
    procedure cmdOLClearClick(Sender: TObject);
    procedure cmdToPayClick(Sender: TObject);
    procedure grdToPayClick(Sender: TObject);
    procedure txtToPayChange(Sender: TObject);
    procedure txtToPayExit(Sender: TObject);
    procedure cmdOLUpClick(Sender: TObject);
    procedure cmdOLDownClick(Sender: TObject);
    procedure txtSumToPayChange(Sender: TObject);
    procedure cmdToPayUpClick(Sender: TObject);
    procedure cmdToPayDownClick(Sender: TObject);
    procedure cmdTenderClick(Sender: TObject);
    procedure cmdHideClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure cmdDeselectAccountClick(Sender: TObject);
    procedure cmdUpAccountsClick(Sender: TObject);
    procedure cmdDownAccountsClick(Sender: TObject);
    procedure cmdSelectAccountClick(Sender: TObject);
    procedure cmdDiscountPercentClick(Sender: TObject);
    procedure cmdDiscountAmountClick(Sender: TObject);
    procedure cmdAmountToPayClick(Sender: TObject);
    procedure cmdAddPosClick(Sender: TObject);
    procedure cmdAccountNoLookupClick(Sender: TObject);
    procedure TryLookupPaymentTableAccount(AccountID: Integer; AShowAcctScanned: Boolean = True; PromptLoyalty: Boolean = True; VerifyNO: Boolean = True);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);

    procedure cmdTLUpClick(Sender: TObject);
    procedure cmdTLDownClick(Sender: TObject);
    procedure cmdTLDeleteClick(Sender: TObject);
    procedure cmdTLClearClick(Sender: TObject);
    procedure cmdDetailsClick(Sender: TObject);
    procedure cmdCashClick(Sender: TObject);
    procedure cmdEFTPOSClick(Sender: TObject);
    procedure cmdVisaClick(Sender: TObject);
    procedure cmdDinersClick(Sender: TObject);
    procedure cmdAMEXClick(Sender: TObject);
    procedure cmdMasterCardClick(Sender: TObject);
    procedure cmdChequeClick(Sender: TObject);
    procedure cmdVoucherClick(Sender: TObject);
    procedure cmdOtherClick(Sender: TObject);
    procedure cmdEditClick(Sender: TObject);
    procedure grdTLsDblClick(Sender: TObject);
    procedure cmdAuthoriseClick(Sender: TObject);
    procedure txtToPayEnter(Sender: TObject);
    procedure txtToPayClick(Sender: TObject);
    procedure txtQtyEnter(Sender: TObject);
    procedure txtQtyClick(Sender: TObject);
    procedure txtQtyExit(Sender: TObject);
    procedure txtQtyChange(Sender: TObject);
    procedure cmdPMSClick(Sender: TObject);
    procedure cmdOnAccountClick(Sender: TObject);
    procedure cmdLoyaltyDesClick(Sender: TObject);
    procedure cmdSummaryTableDesClick(Sender: TObject);
    procedure cmdLastDesClick(Sender: TObject);
    procedure cmdGetCPFDesClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure grdToPayDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
    procedure grdUnInvoicedListDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
    procedure grdUnInvoicedListClick(Sender: TObject);
    procedure cmdComboIndexDesClick(Sender: TObject);
    procedure grdTLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
    procedure cmdEventDesClick(Sender: TObject);
    procedure cmdExternalAccLookupDesClick(Sender: TObject);
    procedure cmdExternalAccRIPDesClick(Sender: TObject);
    procedure DoExternalAccountButtonClick(RIPButtonClicked: Boolean = False);
    procedure cmdTaxCoreBuyerDetailDesClick(Sender: TObject);
    procedure grdToPayEnter(Sender: TObject);
    procedure grdToPayExit(Sender: TObject);
    procedure grdToPayKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure grdToPayMouseWheelDown(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
    procedure grdToPayMouseWheelUp(Sender: TObject; Shift: TShiftState;
      MousePos: TPoint; var Handled: Boolean);
  private
    FToPay, FPayment, FTip, FChange, FTender, FRounding, FStillDue, FRequired: Currency;
    FSalesTax: Currency;
    FSalesTaxCalculated: Boolean;
    FSurcharge: Currency;
    FormActivated: Boolean;
    OldLoyaltyRewardOffer: TRewardOffer;
    FoundFirstLoyaltyFreeItem: Boolean;
    FDiscountPercent: Currency;
    FOrigDiscountAmount, FDiscountAmount: Currency;
    AddingTender: Boolean;
    FLoyaltyFreeItemDiscount: Currency;
    FUseDiscountAmount: Boolean;
    FUseDiscountPeriods: Boolean;
    FUseItemDiscounts: Boolean;
    FLimitMaximum: Boolean;
    FMaxDiscount: Currency;
    FOrigDiscountPercent: Currency;
    FCurrentBalance: Currency;
    GridUpdating: Boolean;

    DoCalcToPay: Boolean;
    DoCalcQty: Boolean;

    DoingTablePayment: Boolean;

    CurrentHour, CurrentMin, CurrentSec, CurrentMSec: Word;
    CurrentTime: Integer;
    CurrentDay: Integer;

    LibAccount, LibRoom, LibName, LibStatus: string;
    NZAAccount: Integer;
    NZAAkey: string;
    NZAName: string;

    EvolutionAccountNo: Integer;
    EvolutionAccount: string;
    EvolutionName: string;
    EvolutionAccExtra: string;

    SurchargeList: TList;
    SaleCategorySurchargeItem: TSurchargeItem;

    OnAccount: Boolean;
    SurplusOnAccount: Boolean;

    LoyaltyReward: TLoyaltyReward;

    cmdHideSkin: TfcCustomImageBtn;
    cmdAuthorise: TfcCustomImageBtn;
    cmdLast: TfcCustomImageBtn;
    cmdSummaryTable: TfcCustomImageBtn;
    cmdLoyalty: TfcCustomImageBtn;

    cmdAmountToPay: TfcCustomImageBtn;
    cmdTLDelete: TfcCustomImageBtn;
    cmdTLClear: TfcCustomImageBtn;
    cmdDetails: TfcCustomImageBtn;
    cmdTLUp: TfcCustomImageBtn;
    cmdTLDown: TfcCustomImageBtn;
    cmdCheque: TfcCustomImageBtn;
    cmdVoucher: TfcCustomImageBtn;
    cmdOther: TfcCustomImageBtn;
    cmdPMS: TfcCustomImageBtn;
    cmdEdit: TfcCustomImageBtn;
    cmdDiners: TfcCustomImageBtn;
    cmdVisa: TfcCustomImageBtn;
    cmdEFTPOS: TfcCustomImageBtn;
    cmdToPay: TfcCustomImageBtn;
    cmdMasterCard: TfcCustomImageBtn;
    cmdAMEX: TfcCustomImageBtn;
    cmdCash: TfcCustomImageBtn;
    cmdGetCPF: TfcCustomImageBtn;
    cmdFiscalSkin: TfcCustomImageBtn;

    cmdUpAccounts: TfcCustomImageBtn;
    cmdDownAccounts: TfcCustomImageBtn;
    cmdSelectAccount: TfcCustomImageBtn;
    cmdAccountNoLookup: TfcCustomImageBtn;
    cmdDeselectAccount: TfcCustomImageBtn;
    cmdExternalAccLookup: TfcCustomImageBtn;
    cmdExternalAccRIP: TfcCustomImageBtn;
    cmdOnAccount: TfcCustomImageBtn;
    cmdDiscountPercent: TfcCustomImageBtn;
    cmdDiscountAmount: TfcCustomImageBtn;

    cmdOLUp: TfcCustomImageBtn;
    cmdOLDown: TfcCustomImageBtn;
    cmdAddPos: TfcCustomImageBtn;

    cmdToPayUp: TfcCustomImageBtn;
    cmdToPayDown: TfcCustomImageBtn;

    cmdAll: TfcCustomImageBtn;
    cmdOLDelete: TfcCustomImageBtn;
    cmdOLClear: TfcCustomImageBtn;
    cmdOLZero: TfcCustomImageBtn;
    cmdOL1: TfcCustomImageBtn;
    cmdOL2: TfcCustomImageBtn;
    cmdOL3: TfcCustomImageBtn;
    cmdOL4: TfcCustomImageBtn;
    cmdOL5: TfcCustomImageBtn;
    cmdOL6: TfcCustomImageBtn;
    cmdOL7: TfcCustomImageBtn;
    cmdOL8: TfcCustomImageBtn;
    cmdOL9: TfcCustomImageBtn;
    cmdOLDoubleZero: TfcCustomImageBtn;
    cmdOLDot: TfcCustomImageBtn;
    cmdOLBack: TfcCustomImageBtn;
    cmdTender: TfcCustomImageBtn;
    cmdComboIndex: TfcCustomImageBtn;
    cmdEvent: TfcCustomImageBtn;
    cmdGoody: TfcCustomImageBtn;
    cmdTaxCoreBuyerDetail: TfcCustomImageBtn;
    cmdOK: TfcCustomImageBtn;
    FVisibleWindow: TVisibleWindow;

    FCPFNumber: string;  // for fiscal printing
    FAccountCPFNumberChanged: Boolean; //for fiscal printing
    FTenderSeqID, FTenderGrpID: Integer;

    FRecordType: TRecType;
    AmountSplitingStarted: Boolean;

    NFManual: Boolean;
    NFEMode: Boolean;
    ShowManualSelection, NFManualSecond: Boolean;
    FPerorgID: Integer;
    FAmtToPay, FGroupPayment: Currency;
    FManualSeries, FManualSubSeries, FManualInvNo: string;
    FLoyaltyDiscount: Currency;
    FDiscCalculator: TDiscountCalc;
    SubsidyCalc: TSubsidyCalc;

    AccountChange, RecalculateDiscount: Boolean;
    FAllCombos: TCombos;
    FComboDiscount: Currency;
	  FCreditCardDiscount: Currency;
    SuppressClearTenderMessage: Boolean;
    FForceUnSelectAccount, FOneBill: Boolean;
    FTotalSubsidyAllowed, FHostSubsidy, FGuestSubsidy : Currency;
    ToPayString: string;
    QtyChangedManully: Boolean;
    grdItemAutoClick: Boolean;
    grdItemManualClick: Boolean;
    isbtnClick: Boolean;
    function GetOriginalOL(aOrderLineID: Integer; var ASrcIndex: Integer): TOrderLine;
    procedure LoadAllComboItems(aComboID: Integer; aComboIndex: Integer);

    procedure SetComboColourIndex(AList:TList);
    procedure OLKeyStroke(Key: Char);
    function ValidateGoodySale: Boolean;
    function TryPaymentTable: Boolean;

    function SendLibicaTransactions(AOLList: TList): Boolean;
    function SendFidelioTransactions(AOLList: TList): Boolean;
    function SendPhoenixTransactions(AOLList: TList): Boolean;
    function SendMicros4700Transactions(AOLList: TList): Boolean;
    function SendNZATransactions(AOLList: TList): Boolean;
    function SendEvolutionTransactions(AOLList: TList): Boolean;
    function SendICRTouchTransactions(AOLList: TList): Boolean;
    function SendMewsTransactions(AOLList: TList; SPNumber, TheTenderID: Integer): Boolean;
    procedure SendTableManagementTransactions(aOLList: TList);

    function SendTaxCoreTransactions(AOLList: TList): Boolean;

    procedure ClearAllOrderLines;
    procedure ClearUnTenderedOrderLines;
    procedure ClearAllSurcharges;
    procedure ClearVoidLines; //Added for fiscal
    procedure LoadFromUnInvoicedList(aOrderLineID: Integer);
    procedure ClearItemLevelSubsidy(var AOLList: TList);

    procedure CalculateToPay(LoadAllowedDisc: Boolean = False; DisplayDiscount: Boolean = True);
    procedure UpdateScreenAfterCalculateDiscount;
    procedure CalculateAccTypeItemDiscount(AList: TList);
    procedure CalculateComboAndAccountDiscounts(AList:TList);
    procedure CalculateSubsidy(AList: TList;LoadAllowedDisc, DisplayDiscount:Boolean);
    function ApplySubsidyOnItems: Boolean;

    procedure BuildAccountButtons;
    function CheckLoyaltyReward(Prompt: Boolean) : Boolean;
    procedure ApplyLoyalty(tmpList: TList; FindNewOnly: Boolean = False);
    procedure AccountClick(Sender: TObject);
    procedure InitialiseAccountInfo(AShowAcctScanned: Boolean = True; PromptLoyalty: Boolean = True; VerifyNo: Boolean = True);
    procedure UpdateUnInvoicedListComboDetails;
    function CanSelectAccount: Boolean;
    function GetSelectedAccount(AAccountID: Integer; AShowAcctScanned: Boolean; PromptLoyalty: Boolean; PrevAccountID: Integer;
      VerifyPin: Boolean = False; ForGoodyDiscount: Boolean = False): Boolean;
    procedure ResetLoyaltyReward;
    procedure SetTLCols;
    function GetDue(IncludeExternalTenders: Boolean = True): Currency;
    procedure LoadTLs(LAction: TLA);
    procedure RecalculateTotals;
    procedure AddTenderLine(TLType: TTLT; IntendedTenderTypeProviderID: Integer = 0; pTL: TTL = nil);
    procedure EditTenderLine;
    procedure DeleteTenderLine;
    procedure ClearAllTenderLines;
    procedure PaymentTableCashCam;
    function EFTPOSTransExists: Boolean;
    function TenderTypeExists(TLType: TTLT): Boolean;

    procedure ShowPaymentTable;

    procedure SetCurrentTime;
    procedure BuildTenderSurchargeList;
    procedure ChangeAllowCreditLabel(AllowCredit: Boolean;VerifyPin:Boolean=True);
    procedure SetOnAccount(State: Boolean);
    procedure TestAllOLsLoyaltyItem(AList: Tlist;FindNewOnly: Boolean);
    function TestLoyaltyItem(LoyaltyRewardID: Integer; ItemID: Integer): Integer;
    function AllocateItemLoyalty(var OL: TOrderLine): Boolean;
    procedure UpdateGrdToPayCells(Row: Integer);
    procedure FSetVisibleWindow(Value: TVisibleWindow);
    procedure LoadAllOlsToPay;
    procedure LoadUnInvoicedOlsToGrid(AList: TList; APaid, AToPay: Currency);
    procedure LoadUnInvoicedOlsToOLToPayList(AList: TList);
    procedure ClearUnInvoicedOls;

    procedure ClearTempItemList;
    procedure ClearUnInvoicedGrid;
    procedure ClearOLsWithoutTenderSeqNo;
    procedure UpdateUnInvoicedOLValues(Apply: Boolean);
    procedure ReloadItemsForThePayment(AList: TList; aPaid: Currency);

    procedure AssociateOLSToTender(var TL: TTL; ATenderSeqNo: Integer);
    function CheckPrinterFunctions:Boolean;
    function GetCPFNumber(UpdateCPF: Boolean):Boolean; // for fiscal printing
    function GetNFManualExtraDetails:Boolean;
    function GetNFEDetails:Boolean;
    procedure RetrieveCPFAccount;
    function SendFiscalInvoice: Boolean;
    procedure UpdateOLTenderID(ATenderID:Integer);
    procedure CopyOL(aDestOL: TOrderLine; aSourceOL: TOrderLine);
    procedure DeleteOLsOfTender(TL:TTL;ASeqNo: Integer);
    function CheckForUnTenderedItems:Boolean;
    procedure GetAllSelectedOLs(var AList: Tlist);
    procedure CombineSameOls(var AList: TList);
    procedure CalculateAmountForUntenderedItems;
    procedure ProcessTheRemainingItems(TLPay: Currency);
    procedure CopyItemSToTempList;
    procedure LoadSelectedToPayList;

    procedure ApplyPreviousNonAppliedDiscount;
    procedure SetItemSelection;
    procedure DoLoyalty(Prompt: Boolean);
    procedure ClearItemLevelDiscounts(AList: TList);
    function CheckForUnFinishedCombos(aList: TList; var aCombo: string; var aComboIndex: Integer): Boolean;
    procedure BroadcastCDAMessage;
    procedure ClearCDAMessage;
    procedure DeselectEvent;
    procedure ApplyItemLevelOneBillDiscountAmount;
    procedure SetGuestCount;
    procedure AddExternalTenders;
    procedure DeleteCurrentExternalTenders;
    procedure ReOrderOLtoPayList;
  public
    ItemsSelectedManually, FProcessedAllItems, FSelectedItemsNotPaidFull, PaymentOnSelectedItems, FGetAllItems: Boolean;
    TempItemList, UnInvoicedOlsToPay, OLToPayList, VoidList: TList;
    ComboFreeItems: TObjectList;
    TLList, OneBillTLList: TTLList;
    JustSelected, ReloadItems: Boolean;
    ManagerAuthorise: TManagerAuthorise;
    IsNormalLoyalty: Boolean;
    property VisibleWindow: TVisibleWindow read FVisibleWindow write FSetVisibleWindow;
    property IsDoingTablePayment: Boolean read DoingTablePayment;
    procedure ShowExternalAccounts(TL: TTL = nil);
    procedure CloseExternalAccounts;
    procedure DeselectAccount(SuppressConfirmation: Boolean=True; ConfirmationResult: TModalResult=mrNone);
    procedure UpdateTLListExternalAccounts;

    procedure SetGoodyAccountDiscountAmount;
    procedure RefreshGoodyAccountDiscountAmount;
    function TryAddGoodyDiscountAmount(ScannedAmount: string): Boolean;
    procedure DeselectGoodyAccount;
  end;
{******************************************************************************}
var
  formPaymentTable: TformPaymentTable;
{******************************************************************************}
implementation
{******************************************************************************}
uses
  IB_Components, DateUtils,
  UConfirmation, LWBQuery, USelectTable, USelectName, LMain, UAppDetails, DM_WBWaiter,
  UMessage, UQuickMessage, USingleNote, UTables, UPrintSummary,
  UDiscountPercent, UDiscountAmount, USelectAccount, LErrors, UGetAmountToPay, UGetItemSplitPayment,
  UGetSelectPos, UGetAccountNo, UEFTPOSIntegration, UDMCashCam, USmartCardTransaction, UPocketVoucherTransaction,
  ULibicaTransaction, UFidelioTransaction, UPhoenixTransaction, UMicros4700Transaction,
  UNZATransaction, UEvolutionTransaction, UICRTouchPMSInterface, UDialogPrepay, UNumPadText, UAccountScanned,
  UMain, UAlphaBlend, UResources,
  UGetCPFNumber, UNFEDetail, UFiscalPrinting, IB_Access, USelectEvent,
  UEncryption, ULogFile, ULogTypes, Math, uTEFDaruma, UCombosInSale, UCDAServer, UFiscalDeletedData,
  UGlobalMenuObjects, UGlobalMenuList, UGetNewGuests, USelectExternalAccount, UExternalAccountIntegration, UExternalAccount,
  USearchExternalAccount, wiGroupPOS, UTaxCoreHTTPSocket, UTaxCoreTransaction, UTaxCoreBuyerDetail, USelectCashSale,
  LGoody, UMewsHTTP, UMewsTransaction;
{******************************************************************************}
{$R *.DFM}
{******************************************************************************}
procedure PaymentTable;
begin
  if ((GlbTable.Uninvoiced <= 0) and (not AppDetails.PaymentAllowZeroPriceInvoices)) then begin
    ShowQuickMsg(Format(sNoDueOnTab, [GlbTable.TableNo]));
  end
  else if (not CheckTaxCoreReadyForInvoice) then
  begin
    ShowQuickMsg('Error with Fiscal device, unable to take payment');
  end
  else begin
    formPaymentTable := TformPaymentTable.Create(Application);
    DoingServiceTransaction := True;
    with formPaymentTable do
    begin
      ShowModal;
      Release;
      formPaymentTable := nil;
    end;
    DoingServiceTransaction := False;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.FormCreate(Sender: TObject);

  procedure HideUsualTenderLineTypes;
  begin
    cmdMasterCard.Visible := False;
    cmdAmex.Visible := False;
    cmdDiners.Visible := False;
    cmdVisa.Visible := False;
    cmdCash.Visible := False;
    cmdEFTPOS.Visible := False;
    cmdCheque.Visible := False;
    cmdVoucher.Visible := False;
    cmdOther.Visible := False;
  end;

begin
  LogMessage(ltPaymentTrace, '1' + #9);
  inherited;

  FAllCombos := TCombos.Create;
  FGetAllItems := False;
  FRecordType := rtUnTenderedItems;
  FormActivated := False; // Added for Manual NF Mode - Brazil to close the form in formactivate itself using postmessage
  UnInvoicedOlsToPay := TList.Create;
  ComboFreeItems := TObjectList.Create;
  TempItemList := TList.Create;

  ToPayString := '0.00';
  QtyChangedManully := False;

  FCPFNumber := ''; // for fiscal printing
  FAccountCPFNumberChanged := False;

  DoCalcToPay := False;
  DoCalcQty := False;
  grdItemAutoClick := False;
  grdItemManualClick := False;
  OLToPayList := TList.Create;
  VoidList := TList.Create;
  TLList := TTLList.Create;
  OneBillTLList := TTLList.Create;
  SurchargeList := TList.Create;
  FDiscCalculator := TDiscountCalc.Create;
  SubsidyCalc := TSubsidyCalc.Create;
  SaleCategorySurchargeItem := Nil;
  SetOLCols;
  SetTLCols;
  if (dm.qrOLsToPay.SQL.Text = '') then begin
    SetQuery(qtOLsToPay);
  end;
  if (WBAccountButtons.QueryType <> qtAccountButtonsCurrent) then begin
    FilterQuery(dm.qrAccountButtons, qtAccountButtonsCurrent);
  end;

  pnlToPay.Visible := False;
  pnlTender.Visible := True;
  FVisibleWindow := vwTender;
  pnlToPay.Align := alClient;
  pnlTender.Align := alClient;

  if (Skin.FormFormBorder <> -1) then begin
    pnlForm.BorderWidth := Skin.FormFormBorder;
    pnlAccountButtons.BorderWidth := (4 - Skin.FormFormBorder);
    pnlAccountControlsBorder.BorderWidth := (4 - Skin.FormFormBorder);
    pnlAccountControlsBorder.Height := pnlAccountControlsBorder.Height + (2 * (4 - Skin.FormFormBorder));

    pnlTotals.BorderWidth := (4 - Skin.FormFormBorder);
    pnlTotals.Height := pnlTotals.Height + (2 * (4 - Skin.FormFormBorder));
    if (Skin.FormFormBorder = 0) then begin
      pnlTender.BevelOuter := bvNone;
      pnlToPay.BevelOuter := bvNone;
    end;
  end;
  if (Skin.FormColour <> -1) then begin
    pnlForm.Color := Skin.FormColour;
    pnlForm.BorderStyle := bsNone;
  end;

  if (Skin.FormToolbarColour <> -1) then begin
    pnlTitle.ParentColor := False;
    pnlTitle.ParentBackground := False;
    pnlTitle.Color := Skin.FormToolbarColour;
  end;
  if (Skin.FormToolbarEditBoxColour <> -1) then begin
    txtAvail.Color := Skin.FormToolbarEditBoxColour;
    txtTableName.Color := Skin.FormToolbarEditBoxColour;
    txtTableNo.Color := Skin.FormToolbarEditBoxColour;
    txtStaff.Color := Skin.FormToolbarEditBoxColour;
  end;

  if (Skin.FormFunctionsBorder <> -1) then begin
    pnlTenderLinesBorder.BorderWidth := Skin.FormFunctionsBorder;
    pnlTenderLinesBorder.Width := pnlTenderLinesBorder.Width + (2 * Skin.FormFunctionsBorder);

    pnlToPay.BorderWidth := Skin.FormFunctionsBorder;
  end;
  if (Skin.FormFunctionsColour <> -1) then begin
    pnlTenderLinesBorder.ParentColor := False;
    pnlTenderLinesBorder.ParentBackground := False;
    pnlTenderLinesBorder.Color := Skin.FormFunctionsColour;

    pnlToPay.ParentColor := False;
    pnlToPay.ParentBackground := False;
    pnlToPay.Color := Skin.FormFunctionsColour;

    pnlTotals.ParentColor := False;
    pnlTotals.ParentBackground := False;
    pnlTotals.Color := Skin.FormFunctionsColour;
  end;
  if (Skin.FormFunctionsEditBoxColour <> -1) then begin
    txtTPayment.Color := Skin.FormFunctionsEditBoxColour;
    txtTTip.Color := Skin.FormFunctionsEditBoxColour;
    txtTTender.Color := Skin.FormFunctionsEditBoxColour;
    txtTChange.Color := Skin.FormFunctionsEditBoxColour;

    txtToPayCopy.Color := Skin.FormFunctionsEditBoxColour;
    txtSalesTax.Color := Skin.FormFunctionsEditBoxColour;

    txtSumToPay.Color := Skin.FormFunctionsEditBoxColour;
    txtBalDue.Color := Skin.FormFunctionsEditBoxColour;
    txtPayment.Color := Skin.FormFunctionsEditBoxColour;
    txtStillDue.Color := Skin.FormFunctionsEditBoxColour;
  end;
  if (Skin.FormFunctionsEditBoxEditColour <> -1) then begin
    txtQty.Color := Skin.FormFunctionsEditBoxColour;
    txtToPay.Color := Skin.FormFunctionsEditBoxColour;
  end;

  if (Skin.AccountsBGColour <> -1) then begin
    pnlAccounts.ParentColor := False;
    pnlAccounts.ParentBackground := False;
    pnlAccounts.Color := Skin.AccountsBGColour;
  end;
  if (Skin.AccountsControlsColour <> -1) then begin
    pnlAccountControlsBorder.ParentColor := False;
    pnlAccountControlsBorder.ParentBackground := False;
    pnlAccountControlsBorder.Color := Skin.AccountsControlsColour;
  end;
  if (Skin.AccountsEditBoxColour <> -1) then begin
    txtAccountName.Color := Skin.AccountsEditBoxColour;
    txtDiscountPercent.Color := Skin.AccountsEditBoxColour;
    txtDiscountAmount.Color := Skin.AccountsEditBoxColour;
  end;

  Skin.NewButtonFromOldButton(cmdHideSkin, cmdHide, bstToolbar, gstClose);
  Skin.NewButtonFromOldButton(cmdAuthorise, cmdAuthoriseDes, bstToolbar, gstAuthorise);
  Skin.NewButtonFromOldButton(cmdLast, cmdLastDes, bstToolbar, gstLast);
  Skin.NewButtonFromOldButton(cmdSummaryTable, cmdSummaryTableDes, bstToolbar, gstSummary);
  Skin.NewButtonFromOldButton(cmdLoyalty, cmdLoyaltyDes, bstToolbar, gstAccount);
  Skin.NewButtonFromOldButton(cmdComboIndex, cmdComboIndexDes, bstToolbar, gstSummary);
  Skin.NewButtonFromOldButton(cmdEvent, cmdEventDes, bstToolbar, gstChargeAccs);
  Skin.NewButtonFromOldButton(cmdGoody, cmdGoodyDes, bstToolbar, gstGoody);
  Skin.NewButtonFromOldButton(cmdTaxCoreBuyerDetail, cmdTaxCoreBuyerDetailDes, bstToolBar, gstRefund);
  Skin.NewButtonFromOldButton(cmdGetCPF, cmdGetCPFDes, bstToolbar);
  Skin.NewButtonFromOldButton(cmdFiscalSkin, cmdFiscalMenu, bstToolbar);
  Skin.NewButtonFromOldButton(cmdAmountToPay, cmdAmountToPayDes, bstAmount);
  Skin.NewButtonFromOldButton(cmdTLDelete, cmdTLDeleteDes, bstClear56);
  Skin.NewButtonFromOldButton(cmdTLClear, cmdTLClearDes, bstAll56);
  Skin.NewButtonFromOldButton(cmdDetails, cmdDetailsDes, bstDetails);
  Skin.NewButtonFromOldButton(cmdTLUp, cmdTLUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdTLDown, cmdTLDownDes, bstDown);

  if ((AppDetails.PaymentAllowPrePay) and (GlbTable.PrepaidBalance <> 0)) then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Caption := sPrePaid;
  end
  else if (AppDetails.SmartCardInterface > 0) then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Caption := sSmartCard;
  end
  else if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '') and (AppDetails.PocketVoucherTerminalID <> '') and (AppDetails.PocketVoucherPassword <> '') ) then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Caption := 'POCKETvoucher';
  end
  else if AppDetails.wiGroupEnabled then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Caption := swiGroupTL;
  end
  else
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstCheque);
  end;

  Skin.NewButtonFromOldButton(cmdVoucher, cmdVoucherDes, bstVoucher);
  Skin.NewButtonFromOldButton(cmdOther, cmdOtherDes, bstOther);
  Skin.NewButtonFromOldButton(cmdPMS, cmdPMSDes, bstGen112);
  Skin.NewButtonFromOldButton(cmdEdit, cmdEditDes, bstEdit);
  Skin.NewButtonFromOldButton(cmdDiners, cmdDinersDes, bstDiners);
  Skin.NewButtonFromOldButton(cmdVisa, cmdVisaDes, bstVisa);
  Skin.NewButtonFromOldButton(cmdEFTPOS, cmdEFTPOSDes, bstEFTPOS);
  Skin.NewButtonFromOldButton(cmdToPay, cmdToPayDes, bstItems48x96);
  Skin.NewButtonFromOldButton(cmdMasterCard, cmdMasterCardDes, bstMasterCard);
  Skin.NewButtonFromOldButton(cmdAMEX, cmdAMEXDes, bstAmex);
  Skin.NewButtonFromOldButton(cmdCash, cmdCashDes, bstCash);

  Skin.NewButtonFromOldButton(cmdUpAccounts, cmdUpAccountsDes, bstUp);
  Skin.NewButtonFromOldButton(cmdDownAccounts, cmdDownAccountsDes, bstDown);
  Skin.NewButtonFromOldButton(cmdSelectAccount, cmdSelectAccountDes, bstSelectAcc);

  Skin.NewButtonFromOldButton(cmdAccountNoLookup, cmdAccountNoLookupDes, bstAccNoLookup);
  Skin.NewButtonFromOldButton(cmdDeselectAccount, cmdDeselectAccountDes, bstDeselectAcc);
  Skin.NewButtonFromOldButton(cmdExternalAccLookup, cmdExternalAccLookupDes, bstExternalAccount);
  Skin.NewButtonFromOldButton(cmdExternalAccRIP, cmdExternalAccRIPDes, bstExternalAccountRIP);
  Skin.NewButtonFromOldButton(cmdOnAccount, cmdOnAccountDes, bstPayOnAcc);
  Skin.NewButtonFromOldButton(cmdDiscountPercent, cmdDiscountPercentDes, bstDiscPercent);
  Skin.NewButtonFromOldButton(cmdDiscountAmount, cmdDiscountAmountDes, bstDiscAmount);

  Skin.NewButtonFromOldButton(cmdOLUp, cmdOlUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdOLDown, cmdOLDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdAddPos, cmdAddPosDes, bstAddPosition);

  Skin.NewButtonFromOldButton(cmdToPayUp, cmdToPayUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdToPayDown, cmdToPayDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdAll, cmdAllDes, bstSettle);
  Skin.NewButtonFromOldButton(cmdOLDelete, cmdOLDeleteDes, bstClear72);
  Skin.NewButtonFromOldButton(cmdOLClear, cmdOLClearDes, bstAll72);
  Skin.NewButtonFromOldButton(cmdTender, cmdTenderDes, bstTender102x48);

  if AppDetails.FiscalType = fiscalNone then
    pnlControlButtons.Width := pnlControlButtons.Width - cmdGetCPF.Width;
  if not AppDetails.EnableCombo then
    pnlControlButtons.Width := pnlControlButtons.Width - cmdComboIndex.Width;

  cmdHideSkin.Left := pnlControlButtons.Width - 48;

  cmdEvent.Visible := AppDetails.EnableEvent;
  cmdTaxCoreBuyerDetail.Visible := AppDetails.FiscalType = fiscalTaxCore;
  cmdComboIndex.Visible := AppDetails.EnableCombo;
  cmdComboIndex.Enabled := False;

  IniGoodyUI(Self, cmdGoody);

  PositionControlAlign(TControl(pnlTabDetails), TControl(cmdHideSkin), alLeft);
  PositionControlAlign(TControl(cmdAuthorise), TControl(pnlTabDetails), alLeft);
  PositionControlAlign(TControl(cmdLast), TControl(cmdAuthorise), alLeft);
  PositionControlAlign(TControl(cmdSummaryTable), TControl(cmdLast), alLeft);
  PositionControlAlign(TControl(cmdEvent), TControl(cmdSummaryTable), alLeft);
  PositionControlAlign(TControl(cmdComboIndex), TControl(cmdEvent), alLeft);
  PositionControlAlign(TControl(cmdGoody), TControl(cmdComboIndex), alLeft);
  PositionControlAlign(TControl(cmdTaxCoreBuyerDetail), TControl(cmdGoody), alLeft);
  PositionControlAlign(TControl(cmdGetCPF), TControl(cmdTaxCoreBuyerDetail), alLeft);


  if (Screen.Width > 800) then
  begin
    pnlTotalsRight.Width := pnlTotalsRight.Width + (160 - 48);
    Skin.NewButtonFromOldButton(cmdOK, cmdOKDes, bstOK);
    cmdOK.Width := 160;
  end
  else
  begin
    pnlTitleText.Visible := False;
    Skin.NewButtonFromOldButton(cmdOK, cmdOKDes, bstGen48);
  end;
  cmdGetCPF.Visible := AppDetails.EnableFiscalPrinting;

  Skin.ApplySkinToGrid(grdToPay);
  Skin.ApplySkinToGrid(grdUnInvoicedList);
  Skin.ApplySkinToGrid(grdTLs);

  BuildAccountButtons;
  txtSalesTax.Visible := AppDetails.TaxExclusivePrices;
  lblSalesTax.Visible := AppDetails.TaxExclusivePrices;

  txtTableNo.Text := IntToStr(GlbTable.TableNo);
  txtTableName.Text := GlbTable.TableName;
  txtStaff.Text := GlbLogin.StaffName;

  cmdEFTPOS.Enabled := GetTenderLineTypeRecord(TLTEFTPOS).TenderTypeEnabled;
  cmdCash.Enabled := GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled;
  if ((AppDetails.PaymentAllowPrePay) and (GlbTable.PrepaidBalance <> 0)) then begin
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTPrepaid).TenderTypeEnabled;
  end
  else if (AppDetails.SmartCardInterface > 0) then begin
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTSmartCard).TenderTypeEnabled;
  end
  else if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '') and (AppDetails.PocketVoucherTerminalID <> '') and (AppDetails.PocketVoucherPassword <>'')) then
  begin
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTPocketVoucher).TenderTypeEnabled;
  end
  else if AppDetails.wiGroupEnabled then
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTwiGroup).TenderTypeEnabled
  else begin
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTCheque).TenderTypeEnabled;
  end;

  cmdVoucher.Enabled := GetTenderLineTypeRecord(TLTVoucher).TenderTypeEnabled;
  cmdOther.Enabled := GetTenderLineTypeRecord(TLTOther).TenderTypeEnabled;
  cmdVisa.Enabled := (GetTenderLineTypeRecord(TLTVisaEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTVisaManual).TenderTypeEnabled);
  cmdMasterCard.Enabled := (GetTenderLineTypeRecord(TLTMasterCardEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTMasterCardManual).TenderTypeEnabled);
  cmdAMEX.Enabled := (GetTenderLineTypeRecord(TLTAmexEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTAmexManual).TenderTypeEnabled);
  cmdDiners.Enabled := (GetTenderLineTypeRecord(TLTDinersEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTDinersManual).TenderTypeEnabled);

  if (AppDetails.PMSType <> pmsNone) then begin
    PositionControl(TControl(cmdVoucher), 112, -1, 76, -1);
    PositionControl(TControl(cmdOther), 188, -1, 76, -1);
    PositionControl(TControl(cmdPMS), 264, 82, 72, 48);
    cmdPMS.Visible := True;
    case AppDetails.PMSType of
      pmsLibica: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biLibica);
      pmsFidelio, pmsPhoenix, pmsMicros4700, pmsICRTouch, pmsMews: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biRoomCharge);
      pmsNZAGold: begin
        if (AppDetails.NZAOnly) then begin
          HideUsualTenderLineTypes;
          Skin.ChangeButton(cmdPMS, bstGen96x72, gstNoChange, biNZAGold);
          PositionControl(TControl(cmdPMS), 84, 118, 168, 72);
        end
        else begin
          Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biNZAGold);
        end;
      end;
      pmsEvolution, pmsMiniBar:
      begin
        if (AppDetails.EvolutionOnly or AppDetails.MinibarRoomTransferOnly) then
        begin
          HideUsualTenderLineTypes;
          Skin.ChangeButton(cmdPMS, bstGen96x72, gstNoChange, biEvolution);
          PositionControl(TControl(cmdPMS), 84, 118, 168, 72);
        end
        else begin
          Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biEvolution);
        end;
      end;
    end;
  end;
  if (AppDetails.TenLnTypOther <> 'Other') then begin
    Skin.ChangeButton(cmdOther, bstGen112, gstNoChange, biNone, AppDetails.TenLnTypOther);
  end;

  lblDiscountAmount.Caption := Format(sDiscountAmount, [AppDetails.CurrencyType]);
  lblAvailDollarSign.Caption := AppDetails.CurrencyType;
  LogMessage(ltPaymentTrace, '2' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.FormShow(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '3' + #9 + 'GlbTable.GroupID=' + IntToStr(GlbTable.GroupID) + ', GlbTable.TableNo=' + IntToStr(GlbTable.TableNo));
  inherited;
  GlbAccount.AccountID := -1;
  GlbCustomer.AccountID := 0;
  GlbAccount.PerorgName := '';
  GlbAccount.PerorgAddress := '';

  DeselectEvent;

  AmountSplitingStarted := False;
  FRecordType := rtBoth;

  IsNormalLoyalty := True;
  FTenderSeqID := 0;
  FTenderGrpID := 1;
  FCPFNumber := '';
  FAccountCPFNumberChanged := False;
  FDiscCalculator.PrevOrderItemsLoaded := False;
  FDiscCalculator.LoadPreviousOrders := False;

  if (AppDetails.FiscalType = fiscalTaxCore) then
    TaxCoreTransactionData.Clear;

  TheActiveDetail := adPaymentTable;
  FAllCombos.LoadCombos;

  LogMessage(ltPaymentTrace, '332' + #9);
  ShowPaymentTable;
  LogMessage(ltPaymentTrace, '333' + #9);

  if ((AppDetails.PaymentGuestCount) and (GlbTable.RequireGuests)) then
    SetGuestCount;

  if (GlbTable.IntendedTenderTypeID > 0) then begin
    LogMessage(ltPaymentTrace, '29' + #9);
    AddTenderLine(GetTLT(GlbTable.IntendedTenderTypeID), GlbTable.IntendedTenderTypeProviderID);
    LogMessage(ltPaymentTrace, '30' + #9);
  end;
  cmdSelectAccount.Enabled := True;
  cmdAccountNoLookup.Enabled := cmdSelectAccount.Enabled;
  cmdDeselectAccount.Enabled := cmdSelectAccount.Enabled;
  cmdDiscountPercent.Enabled := cmdSelectAccount.Enabled;
  cmdDiscountAmount.Enabled := cmdSelectAccount.Enabled;

  //show 'External Accounts' button if there's at least one active external account other than RIP
  cmdExternalAccLookup.Visible := sExternalAccountIntegration.IsActive and
    (not (sExternalAccountIntegration.IsActive(eptRIP) and (sExternalAccountIntegration.FExternalProviders.Count = 1)));
  //show 'RIP Expenses' button in account area if RIP active
  cmdExternalAccRIP.Visible := sExternalAccountIntegration.IsActive(eptRIP);

  //reposition 'RIP Expense' button if 'External Accounts' button is not visible
  if (not cmdExternalAccLookup.Visible) and cmdExternalAccRIP.Visible then
    PositionControl(TControl(cmdExternalAccRIP), 392, 4, 128, 48);

  if (AppDetails.PaymentDefaultPayByItem) then
    cmdToPayClick(Self);
  LogMessage(ltPaymentTrace, '4' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  LogMessage(ltPaymentTrace, '5' + #9);
  inherited;

  TheActiveDetail := adNoDetail;
  ClearStatusPage;

  //CheckTableLock(False, True);
  if (ReturnToCashSaleAfterPayment) then begin
    ReturnToCashSaleAfterPayment := False;
    PostMessage(formMain.Handle, WM_TOCASHSALE, 0, 0);
  end;
  LogMessage(ltPaymentTrace, '6' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.FormDestroy(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '7' + #9);
  ClearAllOrderLines;
  OLToPayList.Free;
  OLToPayList := nil;

  ClearVoidLines;
  VoidList.Free;
  VoidList := nil;

  TLList.Free;
  TLList := nil;
  OneBillTLList.Free;
  OneBillTLList := nil;

  ClearAllSurcharges;
  SurchargeList.Free;
  SurchargeList := nil;
  dm.qrOLsToPay.Active := False;

  ComboFreeItems.Clear;
  ComboFreeItems.Free;
  ComboFreeItems := nil;
  ClearTempItemList;
  ClearUnInvoicedOls;

  if Assigned(UnInvoicedOlsToPay) then
  begin
    UnInvoicedOlsToPay.Free;
    UnInvoicedOlsToPay := nil;
  end;

  if Assigned(FDiscCalculator) then
  begin
    FDiscCalculator.Free;
    FDiscCalculator := nil;
  end;
  SubsidyCalc.Free;
  if Assigned(FAllCombos) then
  begin
    FAllCombos.Free;
    FAllCombos := nil;
  end;
  ClearCDAMessage;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.ClearPayment;

  inherited;
  LogMessage(ltPaymentTrace, '8' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.OLKeyStroke(Key: Char);
begin
  if txtToPay.Enabled then begin
    //akm 12.5.1 - if nothing focused, focus qty field before typing char
    if ((not txtQty.Focused) and (not txtToPay.Focused) and (VisibleWindow = vwToPay)) then begin
      txtQty.SetFocus;
      txtQty.SelectAll;
    end;

    Keybd_event(VkKeyScan(Key), 0, 0, 0);
  end;
end;
{******************************************************************************}
function TformPaymentTable.ValidateGoodySale: Boolean;
const
  GoodyValFail = 'ValidateTabPayment False/Goody - ';
begin
  LogMessage(ltPaymentVerbose, 'TformPaymentTable.ValidateGoodySale begin');
  Result := False;

  //Check goody discount account is only used if a goody discounted sale
  if (not InGoodyDiscountedSale) and ((GlbAccount.AccountID > 0) and (GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID)) then
  begin
    LogMessage(ltPaymentVerbose, GoodyValFail + sGoodyDiscAcctExclusiveToGoodyDiscSale);
    ShowMsg(sGoodyDiscAcctExclusiveToGoodyDiscSale);
    Exit;
  end;

  //Check goody discount account is used if a goody discount sale
  if InGoodyDiscountedSale and (not ((GlbAccount.AccountID > 0) and (GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID))) then
  begin
    LogMessage(ltPaymentVerbose, GoodyValFail + sGoodyDiscAcctRequiredForGoodyDiscSale);
    ShowMsg(sGoodyDiscAcctRequiredForGoodyDiscSale);
    Exit;
  end;

  //If in Goody Sale
  if InGoodySale then
  begin
    //Check don't have an Event selected
    if GlbEvent.EventID <> 0 then
    begin
      LogMessage(ltPaymentVerbose, GoodyValFail + sNoEventForGoodyScan);
      ShowMsg(sNoEventForGoodyScan);
      Exit;
    end;

    //If in Goody Discounted Sale
    if InGoodyDiscountedSale then
    begin
      //check account discount equals Goody discount
      RefreshGoodyAccountDiscountAmount;
      if FDiscountAmount <> GlbGoody.TotalDiscount then
      begin
        LogMessage(ltPaymentVerbose, GoodyValFail + sGoodyDiscDiffersFromAcctDisc);
        ShowMsg(sGoodyDiscDiffersFromAcctDisc);
        Exit;
      end;

      //check total discount doesn't exceed limit
      if FLimitMaximum and (FDiscountAmount > GlbAccount.MaxDiscount) then
      begin
        LogMessage(ltPaymentVerbose, GoodyValFail + Format(sGoodyDiscExceedsAcctLimit, [FormatCurrencySign(GlbAccount.MaxDiscount)]));
        ShowMsg(Format(sGoodyDiscExceedsAcctLimit, [FormatCurrencySign(GlbAccount.MaxDiscount)]));
        Exit;
      end;
    end;
  end;

  Result := True;
  LogMessage(ltPaymentVerbose, 'TformPaymentTable.ValidateGoodySale end, Result = True');
end;
{*******************************************************************************
Creates an invoice for the table
*******************************************************************************}
function TformPaymentTable.TryPaymentTable: Boolean;
var
  I: Integer;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;

  BalanceDelta: Currency;
  LibExists: Boolean;
  NZAExists: Boolean;
  EvolutionExists: Boolean;
  FidelioExists: Boolean;
  PhoenixExists: Boolean;
  Micros4700Exists: Boolean;
  RoomChargeExists: Boolean;
  TheTenderID: Integer;

  TheInvoiceID, TheInvoiceNo: Integer;
  TheInvoiceDate: TDateTime;

  OrderID: Integer;
  TheGroupID: Integer;
  GenTR: TIB_Transaction;
  aOL, tmpOL: TOrderLine;
  VOL: TVoidOL;
  TL: TTL;
  SurchargeItem: TSurchargeItem;
  ClearPreAuthTxnRef: Boolean;
  aTotalTender, PrepaidBalanceChange: Currency;
  aPromoMessage, aNotes: string; // for fiscal printing
  tmpItemGroup: TItemGroup;
  FiscalTLList: TTLList;
  TenderLine: TTenderLineType;
  aTotalTip: Currency;
  aUnit, aItemName, aQtyType, aRounding: string;
  tmpList: TList;
  aTax: string;
  FiscalPaymentInitialized: Boolean;
  NFEItem: TNFEItem;
  aAmtCancelled: Currency;
  aHash: string;
  CupomCOO, aTEFReport: string;
  TEFReportData: TStringList;
  PrintOK: Boolean;
  iOldTenderID, iRefundTenderID, iRefundInvoiceID: Integer;
  OrgInvCOO, tmpTefFlag: string;
  aQty: Currency;
  aUnitPrice, aDiscount, NCMTax, TotalNCMTax: Currency;
  TheSP: TIB_StoredProc;
  TheTR: TIB_Transaction;
  SPNumber: Integer;
  extAccountsMsg: string;
  MewsOrderId: string;
begin
  LogMessage(ltPaymentTrace, '181' + #9);
  aTotalTender := 0;
  aDiscount := 0;
  TotalNCMTax := 0;
  FiscalPaymentInitialized := False;
  Result := False;
  TL := nil;
  BalanceDelta := 0.00;
  TheTenderID := 0;

  if (AppDetails.OrdersLockWait) then begin
    SPNumber := 3;
  end
  else begin
    SPNumber := 1;
  end;

  TheSP := dm.GetSP(SPNumber);
  TheTR := dm.GetTR(SPNumber);
  LogMessage(ltPaymentTrace, '182' + #9);

  ClearPreAuthTxnRef := False;

  SetStatus(sSaving);
  GenTR := DM.qrGeneral.IB_Transaction;
  dm.qrGeneral.IB_Transaction := TheTR;

  SFiscalPrinting.FiscalOpTryAgainCommand := True;

  SFiscalPrinting.SupressFiscalErrorMessage := False;
  if SWBEncryption.AuxiliaryData.TEFDone = '1' then
    SFiscalPrinting.SupressFiscalErrorMessage := True;

  if ((TLList.Count > 0) and (not OnAccount)) then
  begin
    if ItemsSelectedManually or FSelectedItemsNotPaidFull then
    begin
      LogMessage(ltPaymentTrace, '183' + #9);
      LoadUnInvoicedOlsToOLToPayList(TempItemList)
    end
    else
    begin
      LogMessage(ltPaymentTrace, '218' + #9);
      AddAllOrderLines;
    end;
  end;

  LogMessage(ltPaymentTrace, '184' + #9);
  SetItemSelection;

  LogMessage(ltPaymentTrace, '185' + #9);
  CalculateToPay;

  tmpList := TList.Create;
  FiscalTLList := TTLList.Create;
  try
    LogMessage(ltPaymentTrace, '186' + #9);
    GetAllSelectedOLs(tmpList);

    LogMessage(ltPaymentTrace, '187' + #9);
    CombineSameOls(tmpList);          //copies of OLs now in tmpList so need to free them

    //Allocate and apply wiGroup tender line discounts to consolidated OL list.
    //NB: Assumes no other discount is/will be applied to the selected order
    //lines to pay, EnableFiscalPrinting = False, no Event selected (no subsidy),
    //not used by a ROSS system, nor interfacing with other external parties
    //(e.g. Fidelio, Phoenix, Micros, NZA, Evolution, ICR Touch).
    if TLList.TenderTypeExists(TLTwiGroup) then
    begin
      if not (wiGroupPOSSrv.AllocateDiscount(tmpList) and wiGroupPOSSrv.ApplyDiscount(tmpList, FSalesTax)) then
      begin
        ShowMsg(Format(swiGroupDiscAllocError, [swiGroupTL]));
        Exit;
      end;
    end;

    //if there are tender lines, kick cash draw ASAP
    if (TLList.OpenDrawerExists(not SurplusOnAccount)) or
          ((AppDetails.PaymentAllowZeroPriceInvoices) and (AppDetails.PaymentZPIOpenCashDrawer) and (TLList.Count = 0)) then
    begin
      if not AppDetails.EnableFiscalPrinting then
        KickCashDraw(GlbLogin.TillDriverName, GlbLogin.TillDrawKickString);
    end;

    // for Fiscal Printing - Open an Invoice in the Fiscal Printer
    if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
    begin
      tmpTefFlag := SWBEncryption.AuxiliaryData.TEFDone;
      SWBEncryption.AuxiliaryData.TEFDone := '0';
      if (not SFiscalPrinting.OpenFiscalPrinterInvoice(FCPFNumber, GlbAccount.PerorgName, GlbAccount.PerorgAddress)) then
      begin
        SWBEncryption.AuxiliaryData.TEFDone := tmpTefFlag;
        Exit;
      end;
      SFiscalPrinting.GetMovementDate;
      SWBEncryption.GenerateEncryptedAuxFile;

      SWBEncryption.AuxiliaryData.TEFDone := tmpTefFlag;
      SFiscalPrinting.GetCOOForInvoice;
      SFiscalPrinting.GetCCFForInvoice;
    end;

    TheGroupID := GlbTable.GroupID;
    try
      if (not dm.TRStartTest(SPNumber)) then begin
        LogMessage(ltPaymentTrace, '188' + #9);
        Exit;
      end;

      LogMessage(ltPaymentTrace, '189' + #9);
      with TheSP do begin
        if ((SurchargeList.Count > 0) and (FSurcharge > 0)) then begin
          SetStoredProcName('INSERT_ORDER', SPNumber);

          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ParamByName('rush').AsInteger := 0;

          ParamNull(ParamByName('location'));
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);
          ParamByName('tableno').AsInteger := GlbTable.TableNo;
          ParamNull(ParamByName('originaltime'));
          ParamNull(ParamByName('eatintakeaway'));
          ParamNull(ParamByName('heldgroupid'));
          ParamByName('CPFNumber').AsString := Trim(FCPFNumber);
          ExecProc;
          OrderID := FieldByName('orderid').AsInteger;

          //Don't do stock decrement on a surcharge  ;)

          for i := 0 to (SurchargeList.Count - 1) do begin
            SurchargeItem := SurchargeList.Items[I];
            if (SurchargeItem.Amount <> 0) then begin
              //create order line
              SetStoredProcName('INSERT_ORDERLINE', SPNumber);

              ParamNull(ParamByName('courseid'));
              ParamByName('qty').AsCurrency := 1;
              ParamNull(ParamByName('notes'));
              ParamByName('happyhour').AsInteger := 0;
              ParamNull(ParamByName('positions'));
              ParamNull(ParamByName('numpos'));
              ParamByName('itemid').AsInteger := SurchargeItem.ItemID;
              ParamByName('orderid').AsInteger := OrderID;
              ParamNull(ParamByName('printerid'));
              ParamNull(ParamByName('repeatprinterid'));
              ParamByName('held').AsInteger := 0;
              ParamByName('openprice').AsCurrency := SurchargeItem.Amount;
              ParamNull(ParamByName('openpriceitemabbrev'));
              ParamByName('pricelevel').AsInteger := 0;
              ParamNull(ParamByName('currenthour'));
              ParamNull(ParamByName('currentminute'));
              ParamNull(ParamByName('currentday'));
              ParamNull(ParamByName('orderlineorder'));

              if AppDetails.EnableFiscalPrinting then
              begin
                SWBEncryption.Initialise;
                SWBEncryption.AddInt(SurchargeItem.ItemID);
                SWBEncryption.AddStr(SurchargeItem.ItemAbbrev);
                SWBEncryption.AddStr('T');
                SWBEncryption.AddStr(FloatToStr(SurchargeItem.SalesTaxPercent));
                SWBEncryption.AddCurrency(1);
                SWBEncryption.AddCurrency(SurchargeItem.Amount);
                SWBEncryption.AddCurrency(1 * SurchargeItem.Amount);
                SWBEncryption.AddCurrency(0);
                aHash := SWBEncryption.GetHash;
                ParamByName('Checksum').AsString := aHash;
              end;

              ExecProc;
              SurchargeItem.OrderLineID := FieldByName('orderlineid').AsInteger;
            end;
          end;
        end;

        //create invoice record
        SetStoredProcName('INSERT_INVOICE', SPNumber);

        ParamByName('discountrate').AsCurrency := FDiscountPercent;
        IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
        ParamByName('outletid').AsInteger := AppDetails.OutletID;
        ParamByName('groupid').AsInteger := GlbTable.GroupID;

        //may be null from tables already open when Sale Categories introduced
        IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);

        ParamNull(ParamByName('refundnote'));
        ParamByName('invoicetype').AsString := 'I';
        ParamByName('loginid').AsInteger := GlbLogin.LoginID;
        ParamByName('salestax').AsCurrency := FSalesTax;
        ParamNull(ParamByName('cashsaleorderid'));
        StringAsParam(ParamByName('cpfnumber'), FCPFNumber);
        IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);
        aNotes := '';

       // StringAsParam(ParamByName('REFERENCENO'), aZapaReferences);
        StringAsParam(ParamByName('Notes'), aNotes);
        StringAsParam(ParamByName('FiscalCOO'), SFiscalPrinting.COO);
        StringAsParam(ParamByName('FiscalCCF'), SFiscalPrinting.CCF);

        ExecProc;
        if (not FieldByName('errorcode').IsNull) then begin
          LogMessage(ltPaymentTrace, '190' + #9);
          ShowErrorMsg(FieldByName('errorcode').AsInteger);
          TheTr.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice

          Exit;
        end
        else
        begin
          TheInvoiceID := FieldByName('invoiceid').AsInteger;
          WBInvoices.RefreshID := TheInvoiceID;
          TheInvoiceNo := FieldByName('InvoiceNo').AsInteger;
          TheInvoiceDate := FieldByName('InvoiceDate').AsDateTime;

          LogMessage(ltPaymentTrace, '616' + #9 + 'TheInvoiceID=' + IntToStr(TheInvoiceID) + ', TheInvoiceNo=' + IntToStr(TheInvoiceNo)
            + ', TheInvoiceDate=' + FormatDateTime('dd-mm-yyyy hh:nn:ss.zzz', TheInvoiceDate));

          //process External Accounts here
          try
            sExternalAccountIntegration.SPNumber := SPNumber;
            if (sExternalAccountIntegration.ActionPresentedCards(TheInvoiceID, TheInvoiceNo, tmplist, SurchargeList, TLList)) then
            begin
              if (sExternalAccountIntegration.CanContinueSale) then
              begin
                //shall continue sale here as transaction to external accounts have been committed on their side, we just might not have a valid transaction number
                extAccountsMsg := sExternalAccountIntegration.ValidateProcessedCards;
                if ((extAccountsMsg <> '')) then
                begin
                  LogMessage(ltExternalAccounts, extAccountsMsg);
                end;
                //ShowQuickMsg(sExtAccountTransSUCCESSFUL);
              end
              else
              begin
                LogMessage(ltPaymentTrace, '191' + #9);
                TheTr.Rollback;
                Exit;
              end;
            end
            else if (not sExternalAccountIntegration.CanContinueSale) then
            begin
              LogMessage(ltPaymentTrace, '192' + #9);
              TheTr.Rollback;
              Exit;
            end;
          except
            LogMessage(ltPaymentTrace, '193' + #9);
            ShowMsg('Un-expected External Accounts issue');
            LogMessage(ltExternalAccounts, 'Un-expected External Accounts issue');
            TheTr.Rollback;
            Exit;
          end;

          if AppDetails.EnableFiscalPrinting then
          begin
            SetStoredProcName('UPDATEFISCALDETAILS', SPNumber);
            ParamByName('MANUFACTURENO').AsString := GlbFiscal.ManufactureNo;
            ParamByName('ECFMODEL').AsString := GlbFiscal.ECFModel;
            ParamByName('ADDITIONALMF').AsString := GlbFiscal.MFAdditional;
            ParamByName('THEID').AsInteger := TheInvoiceID;
            ParamByName('TABLETYPE').AsInteger := Ord(fttInvoice); // INVOICE Table
            ParamByName('FISCALSEQNO').AsInteger := AppDetails.FiscalPrinterSeqNo;
            ParamByName('MANUALSERIES').AsString := '';
            ParamByName('MANUALSUBSERIES').AsString := '';
            ParamByName('FISCALINVNO').AsString := '';
            if NFManual then
            begin
              ParamByName('MANUFACTURENO').AsString := '';
              ParamByName('ECFMODEL').AsString := '';
              ParamByName('ADDITIONALMF').AsString := '';
              ParamByName('MANUALSERIES').AsString := FManualSeries;
              ParamByName('MANUALSUBSERIES').AsString := FManualSubSeries;
              ParamByName('FISCALINVNO').AsString := FManualInvNo;
            end;

            ParamByName('FISCALINVTYPE').AsString := '2D';
            if NFManual then
              ParamByName('FISCALINVTYPE').AsString := '02'
            else if NFEMode then
              ParamByName('FISCALINVTYPE').AsString := '55';

            Execute;

            if (TheInvoiceID > 0) then
            begin
              SetStoredProcName('UPDATEVOIDLNINVOICEID', SPNumber);
              ParamByName('pinvoiceid').AsInteger := TheInvoiceID;
              ParamByName('pgroupid').AsInteger := GlbTable.GroupID;
              ExecProc;
            end;
          end;

          //if a Goody invoice, save details
          if InGoodySale then
          begin
            SetStoredProcName('UPDATEGOODYINVOICEDETAILS', SPNumber);
            ParamByName('INVOICEID').AsInteger := TheInvoiceID;
            ParamByName('CUSTOMERID').AsString := GlbGoody.CustomerID;
            ParamByName('DISCOUNTAMOUNTS').AsString := GlbGoody.DiscountAmounts.CommaText;
            ExecSQL;
          end;
        end;

        //create invoice lines
        SoFarDiscount := 0.000;
        SoFarComboDiscount := 0;

        for I := 0 to (tmpList.Count - 1) do
        begin
          aOL := tmpList.Items[I];
          // no combo or combos resetted due to 100 % account discount
          if ((FDiscountPercent = 100) and (not FUseDiscountAmount) and (not FUseItemDiscounts) and (not FUseDiscountPeriods) and (aOL.ComboID <= 0)) then
          begin
            with DM.qrGeneral do
            begin
              Close;
              SQL.Clear;
              SQL.Add('UPDATE ORDLN SET COMBOID = null, COMBOGROUP = null, COMBOINDEX = null, COMBOQTY = null');
              SQL.Add(' WHERE ORDERLINEID = :orderlineid ');
              ParamByName('orderlineid').AsInteger := aOL.OrderLineID;
              ExecSQL;
            end;
          end;

          if ((aOL.ToPay > aOL.LoyaltyFreeDiscount) or (aOL.LoyaltyFreeQty <= 0) or (LoyaltyReward.LoyaltyRewardID <= 0) or (LoyaltyReward.RewardOffer <> roItem)) then
          begin
            SetStoredProcName('INSERT_INVOICELINE', SPNumber);   //insert_invoiceline
            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            ParamByName('orderlineid').AsInteger := aOL.OrderLineID;

            aOL.ILAmount := GetRoundedUpDown(aOL.ToPay - aOL.LoyaltyFreeDiscount, AppDetails.DecimalPlaces);
            ParamByName('ilamount').AsCurrency := aOL.ILAmount;

            //NB: if wiGroup TLs exist, don't enter this loop, as aOL.ILDiscount has already been set above
            if IsNormalLoyalty and (not TLList.TenderTypeExists(TLTwiGroup)) and (not sExternalAccountIntegration.OverrideDiscount) then
            begin
              if aOL.ComboID > 0 then
              begin
                if aOL.LastComboItem then
                begin
                  ILDiscount := FComboDiscount - SoFarComboDiscount;
                end
                else
                begin
                  ILDiscount := aOL.AllowedDiscount;
                end;

                ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
                SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
              end
              else
              begin
                //if it's the last line, allocate whatever hasn't been pro rata-ed
                if aOL.LastNormalItem then
                  ILDiscount := FDiscountAmount - aOL.LoyaltyFreeDiscount - SoFarDiscount
                else
                begin              //pro rata discount and accumulate discount pro rata-ed so far
                  if ((AppDetails.EnableFiscalPrinting) and (FUseDiscountAmount) and (aOL.ILAmount = aOL.AllowedDiscount)) then
                  begin
                    aOL.AllowedDiscount := ((aOL.ILAmount * (FDiscountAmount- aOL.LoyaltyFreeDiscount)) / FToPay);
                    ILDiscount := aOL.AllowedDiscount;
                  end
                  else
                    ILDiscount := aOL.AllowedDiscount - aOL.LoyaltyFreeDiscount;

                  ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                  SoFarDiscount := SoFarDiscount + ILDiscount;
                  SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
                end;
              end;

              ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
              aOL.ILDiscount := ILDiscount;
            end;

            ParamByName('discountamount').AsCurrency := aOL.ILDiscount;
            if (aOL.DiscountSchemeID > 0) then
              IntegerAsParam(ParamByName('discountschemeid'), aOL.DiscountSchemeID)
            else IntegerAsParam(ParamByName('discountschemeid'), GlbAccount.DiscountSchemeID);

            if (AppDetails.TaxExclusivePrices) then begin
              BalanceDelta := BalanceDelta + GetRoundedUpDown(((aOL.ILAmount - aOL.ILDiscount) * (100 + aOL.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
            end
            else begin
              BalanceDelta := BalanceDelta + GetRoundedUpDown(aOL.ILAmount - aOL.ILDiscount, AppDetails.DecimalPlaces);
            end;
            // for Fiscal Printing - Open an Invoice in the Fiscal Printer
            if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
            begin
              if (not aOL.WeighedItem) then
              begin
                aUnit := 'UN';
                aQtyType := 'I';
              end
              else
              begin
                aUnit := 'KG';              //0 = in Kilos 1 - in 10gm, 2 - in gm
                aQtyType := 'F';
              end;

              if ((aOL.Qty > 0) and (aOL.UnitPrice > 0) and (aOL.ItemID > 0)) then
              begin
                if aOL.OrderLineID > 0 then
                begin
                  with DM.qrGeneral do
                  begin
                    Close;
                    SQL.Clear;
                    SQL.Add('SELECT MODTEXT FROM GETALLMODIFIERTEXT(:orderlineid, :onlypriced)');
                    ParamByName('orderlineid').AsInteger := aOL.OrderLineID;
                    ParamNull(ParamByName('onlypriced'));
                    Active := True;
                  end;
                  aItemName := Trim(Copy(aOL.ItemString, 1, 29));

                  if not DM.qrGeneral.Eof then
                  begin
                    if Trim(DM.qrGeneral.FieldByName('ModText').AsString) <> '' then
                    begin
                      if AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC] then
                      begin
                        aItemName := Trim(Copy(aOL.ItemString, 1, 20)) + ':' + Trim(Copy(DM.qrGeneral.FieldByName('ModText').AsString, 1, 179));
                        SFiscalPrinting.ExtendItemDescription(aItemName);
                        aItemName := Trim(Copy(aOL.ItemString, 1, 29));
                      end
                      else if AppDetails.FiscalPrinterType = fpDaruma then
                        aItemName := Trim(Copy(aOL.ItemString, 1, 30)) + ':' + Trim(Copy(DM.qrGeneral.FieldByName('ModText').AsString, 1, 200));

                      {if aOL.ModPrice > 0 then
                        ChangeInvoiceItemDiscount(IntToStr(i+1), 'A', '$', FormatFloat('################0.00', aOL.ModPrice));}
                    end;
                  end;
                end;
                if Trim(aOL.TaxSituation) = '' then
                  aOL.TaxSituation := 'T';
                if AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC] then
                begin
                  if aOL.TaxSituation = 'T' then
                    aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 0)
                  else if aOL.TaxSituation = 'S' then
                    aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 1)
                  else
                    aTax := aOL.TaxSituation;
                end
                else if AppDetails.FiscalPrinterType = fpDaruma then
                begin
                  if aOL.TaxSituation = 'T' then
                    aTax := 'I' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100))
                  else if aOL.TaxSituation = 'S' then
                    aTax := 'S' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100))
                  else if ((aOL.TaxSituation = 'II') or (aOL.TaxSituation = 'NN') or (aOL.TaxSituation = 'FF')) then
                    aTax := aOL.TaxSituation
                  else if aOL.TaxSituation = 'SI' then
                    aTax := 'ISS'
                  else if aOL.TaxSituation = 'SN' then
                    aTax := 'NS'
                  else if aOL.TaxSituation = 'SF' then
                    aTax := 'FS';
                end;

                if AppDetails.TruncateAmount = 1 then
                begin
                  aQty := TruncateTo(aOL.Qty, 3);
                  aUnitPrice := TruncateTo(aOL.UnitPrice, 3);
                  aDiscount := TruncateTo(aOL.ILDiscount, 2);
                  aRounding := 'T';
                end
                else
                begin
                  aQty := CRoundTo(aOL.Qty, 4);
                  aUnitPrice := CRoundTo(aOL.UnitPrice, 3);
                  aDiscount := CRoundTo(aOL.ILDiscount, 2);
                  aRounding := 'A';
                end;

                if (AppDetails.FiscalPrinterType = fpBematechNFC)then
                begin
                  NCMTax := SFiscalPrinting.GetNCMTax(aOL.NCMProductCode, aOL.CSOSN, aOL.ToPay);
                  TotalNCMTax := TotalNCMTax + NCMTax;
                  if not SFiscalPrinting.AddFiscalPrinterNFCInvoiceItems(aOL.ItemID, aOL.GTIN, aItemName,
                      '01', aTax, aUnit, aQtyType, '3', aQty, '3', aUnitPrice, '$', '0', aDiscount,
                      aRounding, aOL.NCMProductCode, IntToStr(aOL.CFOP), aOL.Notes, aOL.CSTICMS, IntToStr(aOL.ProductOrigin), '', '', '', '',
                      '', aOL.CSOSN, '', '', '', '', '', '', '', '',
                      '', '', '', '', '', '', FormatFloat(PRICEFORMAT,NCMTax), aOL.CST_PIS, '', '',
                      '', '', '', aOL.CST_COFFINS, '', '', '', '', '', aOL.CEST) then
                  begin
                    LogMessage(ltPaymentTrace, '194' + #9);
                    TheTr.Rollback;
                    if SFiscalPrinting.OpenedFiscalInvoice then
                      SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                    Result := False;
                    Exit;
                  end;
                end
                else if not SFiscalPrinting.AddFiscalPrinterInvoiceItems(aOL.ItemID, aItemName, aTax, aQty, aUnit, aUnitPrice, aDiscount) then
                begin
                  LogMessage(ltPaymentTrace, '195' + #9);
                  TheTR.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end;
                LogMessage(ltFiscal, 'Printed item '+ aItemName);
              end;
            end;
            ParamByName('FISCALSEQNO').AsString := SFiscalPrinting.FiscalItemIndex;
            if AppDetails.EnableFiscalPrinting then
            begin
              aAmtCancelled := 0;
              with dm.qr do
              begin
                Close;
                SQL.Clear;
                SQL.Add('SELECT p.ITEMID, p.ITEMABBREV, p.QTY, p.UNITPRICE, p.TOTAL, p.AMOUNTCANCELLED, p.qtycancelled from GETFISCALORDERLINEAMOUNT (:orderlineid) p ');
                ParamByName('orderlineid').AsInteger := aOL.OrderLineID;
                Active := True;
                if not Eof then
                  aAmtCancelled := FieldByName('AMOUNTCANCELLED').AsFloat;
              end;

              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(SFiscalPrinting.COO);
              SWBEncryption.AddStr(SFiscalPrinting.CCF);
              SWBEncryption.AddInt(aOL.ItemID);
              SWBEncryption.AddStr(aOL.ItemString);
              SWBEncryption.AddStr(aOL.TaxSituation);
              SWBEncryption.AddStr(FloatToStr(aOL.SalesTaxPercent));
              SWBEncryption.AddCurrency(aOL.Qty);
              SWBEncryption.AddCurrency(aOL.UnitPrice);
              SWBEncryption.AddCurrency(ParamByName('ilamount').AsFloat-ParamByName('discountamount').AsFloat);
              SWBEncryption.AddCurrency(aAmtCancelled);
              aHash := SWBEncryption.GetHash;

              ParamByName('CHECKSUM').AsString := aHash;
              ParamByName('FISCALTAX').Asinteger := 1;

              SWBEncryption.Initialise;
              SWBEncryption.AddInt(aOL.ItemID);
              SWBEncryption.AddStr(aOL.ItemString);
              SWBEncryption.AddCurrency(aOL.Qty);
              SWBEncryption.AddCurrency(aOL.UnitPrice);
              SWBEncryption.AddCurrency(aOL.Qty * aOL.UnitPrice);
              SWBEncryption.AddCurrency(aAmtCancelled);
              aHash := SWBEncryption.GetHash;
              SFiscalPrinting.UpdateOrderLineChecksumToDb(aHash, aOL.OrderLineID);
            end;

            ExecProc;
            aOL.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

            if (AppDetails.FiscalType = fiscalTaxCore) then
            begin
              SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
              ParamByName('invoicelineid').AsInteger := aOL.InvoiceLineID;
              StringAsParam(ParamByName('taxlabel'), aOL.TaxCoreLabel);
              ExecProc;
            end;
          end;

          if ((aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0) and (aOL.LoyaltyFreeDiscount > 0)) then
          begin
            SetStoredProcName('INSERT_INVOICELINE', SPNumber);
            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            ParamByName('orderlineid').AsInteger := aOL.OrderLineID;

            aOL.ILAmount := aOL.LoyaltyFreeDiscount;
            ParamByName('ilamount').AsCurrency := aOL.ILAmount;

            aOL.ILDiscount := aOL.LoyaltyFreeDiscount;
            ParamByName('discountamount').AsCurrency := aOL.ILDiscount;
            ParamNull(ParamByName('discountschemeid'));
            ParamByName('FISCALSEQNO').AsString := '';
            SoFarDiscount := SoFarDiscount + aOL.ILDiscount;
            // for Fiscal Printing - Open an Invoice in the Fiscal Printer
            if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
            begin
              if (not aOL.WeighedItem) then
              begin
                aUnit := 'UN';
                aQtyType := 'I';
              end
              else
              begin
                aUnit := 'KG';
                aQtyType := 'F';                      //0 = in Kilos 1 - in 10gm, 2 - in gm
              end;

              if Trim(aOL.TaxSituation) = '' then
                aOL.TaxSituation := 'T';

              if AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC] then
              begin
                if aOL.TaxSituation = 'T' then
                  aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 0)
                else if aOL.TaxSituation = 'S' then
                  aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 1)
                else
                  aTax := aOL.TaxSituation;
              end
              else if AppDetails.FiscalPrinterType = fpDaruma then
              begin
                if aOL.TaxSituation = 'T' then
                  aTax := 'I' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100))
                else if aOL.TaxSituation = 'S' then
                  aTax := 'S' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100))
                else if ((aOL.TaxSituation = 'II') or (aOL.TaxSituation = 'NN') or (aOL.TaxSituation = 'FF')) then
                  aTax := aOL.TaxSituation
                else if aOL.TaxSituation = 'SI' then
                  aTax := 'ISS'
                else if aOL.TaxSituation = 'SN' then
                  aTax := 'NS'
                else if aOL.TaxSituation = 'SF' then
                  aTax := 'FS';
              end;

              if AppDetails.TruncateAmount = 1 then
              begin
                aQty := TruncateTo(aOL.Qty, 3);
                aUnitPrice := TruncateTo(aOL.UnitPrice, 3);
                aDiscount := TruncateTo(aOL.ILDiscount, 2);
                aRounding := 'T';
              end
              else
              begin
                aQty := CRoundTo(aOL.Qty, 4);
                aUnitPrice := CRoundTo(aOL.UnitPrice, 3);
                aDiscount := CRoundTo(aOL.ILDiscount, 2);
                aRounding := 'A';
              end;

              if ((aOL.Qty > 0) and (aOL.UnitPrice > 0) and (aOL.ItemID > 0)) then
              begin
                if (AppDetails.FiscalPrinterType = fpBematechNFC)then
                begin
                  NCMTax := SFiscalPrinting.GetNCMTax(aOL.NCMProductCode, aOL.CSOSN, aOL.ToPay);
                  TotalNCMTax := TotalNCMTax + NCMTax;
                  if not SFiscalPrinting.AddFiscalPrinterNFCInvoiceItems(aOL.ItemID, aOL.GTIN, aItemName,
                      '01', aTax, aUnit, aQtyType, '3', aQty, '3', aUnitPrice, '$', '0', aDiscount,
                      aRounding, aOL.NCMProductCode, IntToStr(aOL.CFOP), aOL.Notes, aOL.CSTICMS, IntToStr(aOL.ProductOrigin), '', '', '', '',
                      '', aOL.CSOSN, '', '', '', '', '', '', '', '',
                      '', '', '', '', '', '', FormatFloat(PRICEFORMAT,NCMTax), aOL.CST_PIS, '', '',
                      '', '', '', aOL.CST_COFFINS, '', '', '', '', '', aOL.CEST) then
                  begin
                    LogMessage(ltPaymentTrace, '196' + #9);
                    TheTR.Rollback;
                    if SFiscalPrinting.OpenedFiscalInvoice then
                      SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                    Result := False;
                    Exit;
                  end;
                end
                else if not SFiscalPrinting.AddFiscalPrinterInvoiceItems(aOL.ItemID, Copy(aOL.ItemString, 1, 29), aTax, aQty, aUnit, aUnitPrice, aDiscount) then
                begin
                  LogMessage(ltPaymentTrace, '197' + #9);
                  TheTr.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end;
              end;
            end;

            if AppDetails.EnableFiscalPrinting then
            begin
              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(SFiscalPrinting.COO);
              SWBEncryption.AddStr(SFiscalPrinting.CCF);
              SWBEncryption.AddInt(aOL.ItemID);
              SWBEncryption.AddStr(aOL.ItemString);
              SWBEncryption.AddStr(aOL.TaxSituation);
              SWBEncryption.AddStr(FloatToStr(aOL.SalesTaxPercent));
              SWBEncryption.AddCurrency(aOL.Qty);
              SWBEncryption.AddCurrency(aOL.UnitPrice);
              SWBEncryption.AddCurrency(ParamByName('ilamount').AsFloat-ParamByName('discountamount').AsFloat);
              SWBEncryption.AddCurrency(0);
              aHash := SWBEncryption.GetHash;

              ParamByName('CHECKSUM').AsString := aHash;
              ParamByName('FISCALTAX').Asinteger := 1;
            end;

            ExecProc;
            aOL.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

            if (AppDetails.FiscalType = fiscalTaxCore) then
            begin
              SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
              ParamByName('invoicelineid').AsInteger := aOL.InvoiceLineID;
              StringAsParam(ParamByName('taxlabel'), aOL.TaxCoreLabel);
              ExecProc;
            end;
          end;
        end;

        //create invoice lines for surcharges
        for i := 0 to (SurchargeList.Count - 1) do begin
          if (TSurchargeItem(SurchargeList[I]).Amount <> 0) then begin
            SetStoredProcName('INSERT_INVOICELINE', SPNumber);
            ParamByName('ilamount').AsCurrency := GetRoundedUpDown(TSurchargeItem(SurchargeList[i]).Amount, AppDetails.DecimalPlaces);
            ParamByName('discountamount').AsCurrency := TSurchargeItem(SurchargeList[i]).Discount;
            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            ParamByName('orderlineid').AsInteger := TSurchargeItem(SurchargeList[i]).OrderLineID;
            ParamNull(ParamByName('discountschemeid'));
            ParamByName('FISCALSEQNO').AsString := '';
            if AppDetails.EnableFiscalPrinting then
              ParamByName('FISCALTAX').Asinteger := 1;
            //AM 23Dec99 Account Balance
            if (AppDetails.TaxExclusivePrices) then begin   //Jon 18-03-2002
              BalanceDelta := BalanceDelta + GetRoundedUpDown((TSurchargeItem(SurchargeList[i]).Amount * (100 + TSurchargeItem(SurchargeList[i]).SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
            end
            else begin
              BalanceDelta := BalanceDelta + GetRoundedUpDown(TSurchargeItem(SurchargeList[i]).Amount, AppDetails.DecimalPlaces);
            end;

            if AppDetails.EnableFiscalPrinting then
            begin
              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(SFiscalPrinting.COO);
              SWBEncryption.AddStr(SFiscalPrinting.CCF);
              SWBEncryption.AddInt(TSurchargeItem(SurchargeList[i]).ItemID);
              SWBEncryption.AddStr(TSurchargeItem(SurchargeList[i]).ItemAbbrev);
              SWBEncryption.AddStr('T');
              SWBEncryption.AddStr(FloatToStr(TSurchargeItem(SurchargeList[i]).SalesTaxPercent));
              SWBEncryption.AddCurrency(1);
              SWBEncryption.AddCurrency(TSurchargeItem(SurchargeList[i]).Amount);
              SWBEncryption.AddCurrency(TSurchargeItem(SurchargeList[i]).Amount);
              SWBEncryption.AddCurrency(0);
              aHash := SWBEncryption.GetHash;
              ParamByName('CHECKSUM').AsString := aHash;
            end;
            ExecProc;
            TSurchargeItem(SurchargeList[i]).InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

            if (AppDetails.FiscalType = fiscalTaxCore) then
            begin
              SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
              ParamByName('invoicelineid').AsInteger := TSurchargeItem(SurchargeList[i]).InvoiceLineID;
              tmpItemGroup := GlobalMenuList.GetItemsItemGroup( TSurchargeItem(SurchargeList[i]).ItemID);
              if (Assigned(tmpItemGroup)) then
                StringAsParam(ParamByName('taxlabel'), tmpItemGroup.TaxCoreLabel)
              else
                ParamNull(ParamByName('taxlabel'));
              ExecProc;
            end;

          end;
        end;

        //create tender record ONLY if there are tender lines
        if ((TLList.Count > 0) and (not TLList.TenderTypeExistsOnly(TLTNone))) then
        begin
          PrepaidBalanceChange := 0.00;
          SetStoredProcName('INSERT_TENDER', SPNumber);
          ParamByName('tendertype').AsString := 'P';

          IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
          ParamByName('groupid').AsInteger := GlbTable.GroupID;

          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          ParamNull(ParamByName('nosaletypeid'));
          ParamNull(ParamByName('nosalenotes'));
          // ParamByName('printed').AsInteger := 1;

          BoolAsParam(ParamByName('printed'), AppDetails.PrintReceipt);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;

          LibExists := ((AppDetails.PMSType = pmsLibica) and (TenderTypeExists(TLTLibica)));
          StringAsParamIfBool(ParamByName('libaccount'), LibAccount, LibExists);   //Jon 03-10-2002
          StringAsParamIfBool(ParamByName('libroom'), LibRoom, LibExists);         //Jon 03-10-2002
          StringAsParamIfBool(ParamByName('libname'), LibName, LibExists);         //Jon 03-10-2002
          StringAsParamIfBool(ParamByName('libstatus'), LibStatus, LibExists);     //Jon 03-10-2002

          NZAExists := ((AppDetails.PMSType = pmsNZAGold) and (TenderTypeExists(TLTNZAGold)));
          IntegerAsParamIfBool(ParamByName('nzaaccount'), NZAAccount, NZAExists);       //Jon 01-02-2003
          StringAsParamIfBool(ParamByName('nzaakey'), NZAAkey, NZAExists);              //Jon 01-02-2003
          StringAsParamIfBool(ParamByName('nzaname'), Copy(NZAName, 1, 30), NZAExists); //Jon 01-02-2003

          EvolutionExists := ((AppDetails.PMSType = pmsEvolution) and (TenderTypeExists(TLTEvolution)));
          IntegerAsParamIfBool(ParamByName('evolutionaccountno'), EvolutionAccountNo, EvolutionExists);
          StringAsParamIfBool(ParamByName('evolutionaccount'), Copy(EvolutionAccount, 1, 30), EvolutionExists);
          StringAsParamIfBool(ParamByName('evolutionname'), Copy(EvolutionName, 1, 60), EvolutionExists);
          if ((EvolutionExists) and (AppDetails.EvolutionAccExtraField <> '')) then begin
            if (AppDetails.EvolutionAccExtraFieldSortInt) then begin
              IntegerAsParam(ParamByName('nzaaccount'), StrToIntDef(EvolutionAccExtra, 0));  //Jon 06-06-2008
            end
            else begin
              StringAsParam(ParamByName('nzaname'), Copy(EvolutionAccExtra, 1, 30));         //Jon 06-06-2008
            end;
          end;

          FidelioExists := ((AppDetails.PMSType = pmsFidelio) and (TenderTypeExists(TLTFidelio)));
          PhoenixExists := ((AppDetails.PMSType = pmsPhoenix) and (TenderTypeExists(TLTPhoenix)));
          Micros4700Exists := ((AppDetails.PMSType = pmsMicros4700) and ((TenderTypeExists(TLTMicros4700)) or (TenderTypeExists(TLTMicros4700Account))));
          RoomChargeExists := (TenderTypeExists(TLTRoomCharge));

          if (PhoenixExists) then begin
            ParamByName('fidelioguestnumber').AsInteger := GlbPhoenixAccount.GuestNumber;
            ParamByName('fidelioroomnumber').AsString := GlbPhoenixAccount.RoomNumber;
            ParamByName('fidelioguestname').AsString := GlbPhoenixAccount.GuestName;
          end
          else if (Micros4700Exists) then begin
            ParamNull(ParamByName('fidelioguestnumber'));
            ParamByName('fidelioroomnumber').AsString := GlbMicros4700Account.RoomNumber;
            ParamByName('fidelioguestname').AsString := GlbMicros4700Account.SubFolio;
          end
          else if RoomChargeExists then
          begin
            if (AppDetails.PMSType = pmsICRTouch) then
            begin
              IntegerAsParam(ParamByName('fidelioguestnumber'), GlbICRTouchFolio.FolioNumber);
              StringAsParam(ParamByName('fidelioroomnumber'), GlbICRTouchFolio.RoomNumber);
              StringAsParam(ParamByName('fidelioguestname'), GlbICRTouchFolio.DisplayName);
            end
            else
            begin
              IntegerAsParam(ParamByName('fidelioguestnumber'), GlbTable.FidelioGuestNumber);
              StringAsParam(ParamByName('fidelioroomnumber'), GlbTable.FidelioRoomNumber);
              StringAsParam(ParamByName('fidelioguestname'), GlbTable.FidelioGuestName);
            end;
          end
          else
          begin
            IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbFidelioAccount.GuestNumber, FidelioExists);
            StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbFidelioAccount.RoomNumber, FidelioExists);
            StringAsParamIfBool(ParamByName('fidelioguestname'), GlbFidelioAccount.GuestName, FidelioExists);
          end;

          // for Fiscal Printing - Open an Invoice in the Fiscal Printer
          if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          begin
            aTotalTip := 0;
            FCreditCardDiscount := 0;
            //Calculate total tip amount and add this as an item to the fiscal printer
            for I := 0 to (TLList.Count - 1) do
            begin
              TL := TLList.Items[I];
              if (TL.TLTender > 0) then
              begin
                aTotalTip := aTotalTip + TL.TLTip;
                FCreditCardDiscount := FCreditCardDiscount + TL.TEFCreditCardDiscount;
              end;
            end;
            if FCreditCardDiscount > 0 then
            begin
              PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('D', '$', FCreditCardDiscount);
            end
            else
            begin
              if (AppDetails.TipsAsChange) then
                PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('A', '$', 0)     //dont send value of tip thus extra amount tendered is seen as change in fiscal printer
              else
                PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('A', '$', aTotalTip);
            end;

            if not PrintOK then
            begin
              LogMessage(ltPaymentTrace, '198' + #9);
              TheTR.Rollback;
              if SFiscalPrinting.OpenedFiscalInvoice then
                SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
              Result := False;
              Exit;
            end;
            LogMessage(ltFiscal, 'Opened fiscal invoice payment');
            FiscalPaymentInitialized := True;
          end;
          ParamByName('COO').AsString := SFiscalPrinting.COO;
          ParamByName('GNF').AsString := '';

          SWBEncryption.Initialise;
          SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
          SWBEncryption.AddStr(SFiscalPrinting.COO);
          SWBEncryption.AddStr(SFiscalPrinting.CCF); //CCF
          SWBEncryption.AddStr(''); // CDC
          SWBEncryption.AddStr(''); // GNF
          aHash := SWBEncryption.GetHash;
          ParamByName('CHECKSUM').AsString := aHash;
          IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);
          ExecProc;
          WBReceipts.RefreshID := FieldByName('tenderid').AsInteger;
          TheTenderID := WBReceipts.RefreshID;

          if (AppDetails.EnableFiscalPrinting) then
          begin
            SetStoredProcName('UPDATEFISCALDETAILS', SPNumber);
            ParamByName('MANUFACTURENO').AsString := GlbFiscal.ManufactureNo;
            ParamByName('ECFMODEL').AsString := GlbFiscal.ECFModel;
            ParamByName('ADDITIONALMF').AsString := GlbFiscal.MFAdditional;
            ParamByName('THEID').AsInteger := TheTenderID;
            ParamByName('TABLETYPE').AsInteger := Ord(fttTender); // tender Table
            ParamByName('FISCALSEQNO').AsInteger := AppDetails.FiscalPrinterSeqNo;
            ParamByName('MANUALSERIES').AsString := '';
            ParamByName('MANUALSUBSERIES').AsString := '';
            ParamByName('FISCALINVNO').AsString := '';
            ParamByName('FISCALINVTYPE').AsString := '';
            Execute;
          end;
          //create tender lines
          FiscalTLList.Copy(TLList);

          for I := 0 to (TLList.Count - 1) do
          begin
            TL := TLList.Items[I];
            //only create a record if amount > 0
            if (TL.TLType = TLTPrepaid) then begin
              PrepaidBalanceChange := PrepaidBalanceChange - TL.TLTender;
            end;

            //Save tender line
            //NB: treat wiGroup tenders differently: if a wiGroup 'payment' was
            //all discount and no actual payment, then don't save tender line
            //(else do save, including zero payments (to record trans occurred)
            if ((TL.TLType <> TLTwiGroup) and (TL.TLType <> TLTExternalProviders) and (TL.TLTender > 0)) or
                ((TL.TLType = TLTwiGroup) and not (
                   (TwiGroupTrans(TL.wiGroupTrans).Discount > 0) and (TwiGroupTrans(TL.wiGroupTrans).Payment = 0))) or
                ((TL.TLType = TLTExternalProviders) and (TL.TLTender <> 0) and not (sExternalAccountIntegration.OverrideDiscount)) then
            begin
              if (TL.TLTypeID <> GetTLTID(TLTLoyaltyPoints)) then
                aTotalTender := aTotalTender + GetRoundedUpDown(TL.TLPayment, AppDetails.DecimalPlaces);

              SetStoredProcName('INSERT_TENDERLINE', SPNumber);
              ParamByName('tenderid').AsInteger := WBReceipts.RefreshID;
              ParamByName('tenderlinetypeid').AsInteger := TL.TLTypeID;
              ParamByName('tenderlineamount').AsCurrency := TL.TLTender;
              ParamByName('tenderlinetip').AsCurrency := TL.TLTip;
              //ParamByName('roundingamount').AsFloat := 0;
              ParamByName('roundingamount').AsCurrency := TL.TLRounding;    //Jon 30-3-2001
              BoolAsParam(ParamByName('changeamount'), False);

              if (SurplusOnAccount) then begin      //Jon 04-11-2003
                ParamByName('tenderlinechange').AsCurrency := 0;
              end
              else begin
                ParamByName('tenderlinechange').AsCurrency := TL.TLChange; //akm 01.12.01
              end;

              StringAsParam(ParamByName('tenderlinenotes'), TL.TLNotes);
              StringAsParam(ParamByName('eftpostxnref'), TL.EFTPOSTxnRef);
              if (TL.TLType <> TLTExternalProviders) then
              begin
                StringAsParam(ParamByName('eftpostxnauthcode'), TL.EFTPOSTxnAuthCode);
                StringAsParam(ParamByName('eftposcardref'), TL.EFTPOSCardRef);
              end
              else
              begin
                StringAsParam(ParamByName('eftpostxnauthcode'), TL.ExternalTransactionNo);
                StringAsParam(ParamByName('eftposcardref'), TL.EFTPOSCardRef);
              end;
              //AM 23Dec99 Account Balance
              BalanceDelta := BalanceDelta - GetRoundedUpDown(TL.TLTender - TL.TLTip + TL.TLRounding, AppDetails.DecimalPlaces);

              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(SFiscalPrinting.COO);
              SWBEncryption.AddStr(SFiscalPrinting.CCF); //CCF
              SWBEncryption.AddStr(''); // CDC
              SWBEncryption.AddStr(''); // GNF
              StringAsParam(ParamByName('COO'), SFiscalPrinting.COO);
              ParamNull(ParamByName('GNF'));
              StringAsParam(ParamByName('CCF'), SFiscalPrinting.CCF);
              StringAsParam(ParamByName('CHECKSUM'), SWBEncryption.GetHash);
              IntegerAsParam(ParamByName('tenlinetypeproviderid'), TL.TLProviderID);
              IntegerAsParam(ParamByName('instalments'), TL.TLInstalments);
              TL.COO := SFiscalPrinting.COO;
              TL.CCF := SFiscalPrinting.CCF;
              ExecProc;
              TL.TLID := FieldByName('tenderlineid').AsInteger;
            end;

            if (TL.PreAuthPayment) then begin
              ClearPreAuthTxnRef := True;
            end;
          end;

          //Fiscal tenders
          if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          begin
            FiscalTLList.DoCombine;
            FiscalTLList.DoSort(True);
            if (FiscalTLList.Count > 1) and ((FiscalTLList.TotalTender - FiscalTLList.Items[FiscalTLList.Count - 1].TLTender) >= FiscalTLList.TotalPayment) then
            begin
              FiscalTLList.DoCombine(True);
              FiscalTLList.DoSort;
            end;

            for I := 0 to (FiscalTLList.Count - 1) do
            begin
              TL := FiscalTLList.Items[I];

              TenderLine := GetTenderLineTypeRecord(GetTLT(TL.TLTypeID));
              if Assigned(TenderLine) then
              begin
                if (not SFiscalPrinting.AddFiscalPrinterPayment(TenderLine.TenderLineType, TL.TLTender-TL.TEFCreditCardDiscount, TL.TLTypeID = GetTLTID(TLTCash))) then
                begin
                  LogMessage(ltPaymentTrace, '199' + #9);
                  TheTR.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end;
                LogMessage(ltFiscal, 'Send payment details of ' + TenderLine.TenderLineType);
              end;
            end;

            SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '1';
            SWBEncryption.GenerateEncryptedAuxFile;
          end;

          //create a tender line for the change
          if ((FChange > 0) and (not SurplusOnAccount)) then begin
            SetStoredProcName('INSERT_TENDERLINE', SPNumber);
            ParamByName('tenderid').AsInteger := WBReceipts.RefreshID;
            ParamByName('tenderlinetypeid').AsInteger := 4; //cash
            ParamByName('tenderlineamount').AsCurrency := -FChange; //total change
            ParamByName('tenderlinetip').AsCurrency := 0;
            ParamByName('roundingamount').AsCurrency := 0; // JEH 24/11/01 Removed FRounding
            BoolAsParam(ParamByName('changeamount'), True);
            ParamByName('tenderlinechange').AsCurrency := 0; //akm 01.12.01
            ParamNull(ParamByName('tenderlinenotes'));
            ParamNull(ParamByName('eftpostxnref'));
            ParamNull(ParamByName('eftposcardref'));
            //AM 23Dec99 Account Balance
            BalanceDelta := BalanceDelta - GetRoundedUpDown(ParamByName('tenderlineamount').AsCurrency - ParamByName('tenderlinetip').AsCurrency + ParamByName('roundingamount').AsCurrency, AppDetails.DecimalPlaces);

            if AppDetails.EnableFiscalPrinting then
            begin
              StringAsParam(ParamByName('COO'), SFiscalPrinting.COO);
              ParamNull(ParamByName('GNF'));
              StringAsParam(ParamByName('CCF'), SFiscalPrinting.CCF);
              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(SFiscalPrinting.COO);
              SWBEncryption.AddStr(SFiscalPrinting.CCF); //CCF
              // CDC
              // GNF
              StringAsParam(ParamByName('CHECKSUM'), SWBEncryption.GetHash);
            end
            else
            begin
              ParamNull(ParamByName('COO'));
              ParamNull(ParamByName('GNF'));
              ParamNull(ParamByName('CCF'));
              ParamNull(ParamByName('checksum'));
            end;
            ParamNull(ParamByName('tenlinetypeproviderid'));
            ParamNull(ParamByName('instalments'));
            ExecProc;
          end;

          if (PrepaidBalanceChange <> 0) then begin
            SetStoredProcName('CHANGETABLEPREPAYBALANCE', SPNumber);         //changetableprepaybalance
            ParamByName('groupid').AsInteger := GlbTable.GroupID;
            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            ParamByName('prepaidbalancechange').AsCurrency := PrepaidBalanceChange;
            ExecProc;
            if (not FieldByName('errorcode').IsNull) then begin
              LogMessage(ltPaymentTrace, '200' + #9);
              Result := False;
              ShowErrorMsg(FieldByName('errorcode').AsInteger);
              TheTr.Rollback;
              if SFiscalPrinting.OpenedFiscalInvoice then
                SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
              Exit;
            end;
          end;
        end;

        //Save wiGroup transaction database records
        if TLList.TenderTypeExists(TLTwiGroup) then
          wiGroupPOSSrv.SaveTransRecords(TheSP, SPNumber, tmpList);

        //For fiscal Printing
        if ((TheInvoiceID > 0) and (AppDetails.EnableFiscalPrinting)) then
        begin
          if NFEMode then
          begin
            if not GetNFEDetails then
            begin
              LogMessage(ltPaymentTrace, '201' + #9);
              Result := False;
              TheTr.Rollback;
              Exit;
            end;

            if FPerorgID > 0 then
              SetGlbPerson(FPerorgID);

            if Assigned(GlbNFEData) then // global variable
            begin
              GlbNFEData.InvoiceID := TheInvoiceID;
              GlbNFEData.InvoiceNo := TheInvoiceNo;
              GlbNFEData.InvoiceDate := TheInvoiceDate;
              GlbNFEData.InvoiceTotal := FToPay;
              GlbNFEData.PaidOnAccount := False;
              if ((GlbAccount.AccountID > 0) and (BalanceDelta > 0) and (OnAccount)) then
                GlbNFEData.PaidOnAccount := True;

              try
                for I := 0 to (tmpList.Count - 1) do
                begin
                  aOL := tmpList.Items[I];
                  if (aOL.ItemID > 0) then
                  begin
                    NFEItem := TNFEItem.Create;

                    NFEItem.ItemID := aOL.ItemID;
                    NFEItem.ItemName := aOL.ItemString;
                    NFEItem.Qty := aOL.Qty;
                    NFEItem.CFOP := GlbNFEData.CFOP;
                    NFEItem.FiscalSeq := IntToStr(i);
                    NFEItem.WeightedItem := Boolean(aOL.WeighedItem) ;
                    NFEItem.UnitPrice := aOL.UnitPrice;
                    NFEItem.Discount := aOL.ILDiscount;
                    NFEItem.Total := aOL.Qty * aOL.UnitPrice;//aOL.ILAmount;
                    NFEItem.ItemGrpType := aOL.ItemGrpType;

                    NFEItem.TaxSituation := aOL.TaxSituation;
                    NFEItem.TaxRate := aOL.SalesTaxPercent;
                    NFEItem.CSOSN := aOL.CSOSN;
                    NFEItem.CSTICMS := aOL.CSTICMS;
                    NFEItem.NCMProductCode := aOL.NCMProductCode;
                    SFiscalPrinting.NFEItems.Add(NFEItem);
                  end;
                end;

                if not SFiscalPrinting.SendNFEDetails then
                begin
                  LogMessage(ltPaymentTrace, '202' + #9);
                  ShowQuickMsg(sErrorInSendingNFEData);
                  TheTR.Rollback;
                  Result := False;
                  Exit;
                end;
              finally
                SFiscalPrinting.NFEItems.Clear;
              end;
            end;

            SetStoredProcName('UPDATENFEDETAILS', SPNumber);
            ParamByName('INVOICEID').AsInteger := TheInvoiceID;
            ParamByName('INVOICESTATUS').AsString := GlbNFEData.InvoiceStatus;
            ParamByName('DISPATCHDATE').AsDateTime := GlbNFEData.DeliveryDate;
            ParamByName('BOXQTY').AsFloat := GlbNFEData.BoxQty;
            ParamByName('LOTNUMBER').AsFloat := GlbNFEData.LotNumber;
            ParamByName('GROSSWEIGHT').AsFloat := GlbNFEData.GrossWeight;
            ParamByName('NETWEIGHT').AsFloat := GlbNFEData.NetWeight;
            ParamByName('NUMBERPLATE').AsString := GlbNFEData.NumberPlate;
            ParamByName('NUMBERPLATESTATE').AsString := GlbNFEData.NumberPlateState;
            ParamByName('MARCA').AsString := GlbNFEData.Marca;
            ParamByName('NOTES').AsString := GlbNFEData.Notes;
            ParamByName('FREIGHTVALUE').AsFloat := GlbNFEData.FreightValue;
            ParamByName('INSURANCEVALUE').AsFloat := GlbNFEData.InsuranceValue;
            ParamByName('OTHEREXPVALUE').AsFloat := GlbNFEData.OtherExpense;
            ParamByName('SHIPPINGMODE').AsString := GlbNFEData.FreightType;
            ParamByName('DANFENUMBER').AsString := GlbNFEData.DANFENumber;
            ParamByName('INVOICEKEY').AsString := GlbNFEData.InvoiceKey;
            ParamByName('PROTOCOLNUMBER').AsString := GlbNFEData.ProtocolNumber;
            ParamByName('PROTOCOLDATE').AsDateTime := GlbNFEData.ProtocolDate;
            ParamByName('PROTOCOLSTATUS').AsString := GlbNFEData.ProtocolStatus;
            ParamByName('CFOP').AsInteger := GlbNFEData.CFOP;
            ExecProc;
          end;
        end;

        //If Needing to clear PreAuth txn reference from Tab, then do it here
        if (ClearPreAuthTxnRef) then begin
          SetStoredProcName('SETTABLEEFTPOSTXNREF', SPNumber);             //settableeftpostxnref
          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamNull(ParamByName('eftpostxnref'));
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then begin
            LogMessage(ltPaymentTrace, '203' + #9);
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if SFiscalPrinting.OpenedFiscalInvoice then
              SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
            Exit;
          end;
        end;

        //try to close table
        if ((AppDetails.PaymentAutoCloseTable) and (GlbTable.WhenOpened <> 0)) or
           ((not AppDetails.PaymentAutoCloseTable) and
            ((AppDetails.PaymentSaleCategoryID <> 0) and (AppDetails.PaymentSaleCategoryID <> GlbTable.SaleCategoryID))) then
            begin
          SetStoredProcName('CLOSE_TABLE', SPNumber);      //close_table

          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
        end;

        if ((AppDetails.FiscalType = fiscalNone) and ((AppDetails.PrintReceipt) or
            ((GlbTable.SectionID = AppDetails.DeliverySectionID) and (AppDetails.PhoneOrderDeliveryPrintInvoice)) or
            ((Appdetails.PaymentTenderOverRidePrints) and (TLList.PrintCopiesExists)) or
            ((Appdetails.PaymentAccountPrints = 1) and ((FDiscountAmount > 0) or (FLoyaltyFreeItemDiscount > 0))) or
            ((Appdetails.PaymentAccountPrints = 2) and (GlbAccount.AccountID > 0)))) then begin
          //insert print job
          SetStoredProcName('INSERT_PRINTJOBWITHLOGINID', SPNumber);   //Jon 17-4-2001                //insert_printjobwithloginid

          ParamByName('printjobtype').AsString := 'Invoice';
          ParamByName('theid').AsInteger := TheInvoiceID;
          IntegerAsParamIfBool(ParamByName('loginid'), ManagerAuthorise.AuthoriseLoginID, ManagerAuthorise.Authorise);
          ParamByName('printerid').AsInteger := AppDetails.PrinterID;
          ExecProc;
          if ((GlbTable.SectionID = AppDetails.DeliverySectionID) and (AppDetails.PhoneOrderDeliveryPrintInvoice)) then begin
            //Do it again
            ExecProc;
          end;
        end;

        //AM 23Dec99 Account Balance
        //totalinvoices = insert_invoiceline: add sum(ilamount-discountamount)
        //totaltenders = insert_tenderline: less sum(tenderlineamount-tenderlinetip+roundingamount)
        //account balance delta = totalinvoices - totaltenders
        if (GlbAccount.AccountID > 0) then begin // if going on an account, set account balance
          SetStoredProcName('INSERTACCOUNTINVOICE', SPNumber);            //insertaccountinvoice

          ParamByName('accountid').AsInteger := GlbAccount.AccountID;
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          IntegerAsParam(ParamByName('tenderid'), TheTenderID);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          CurrencyAsParam(ParamByName('currentbalancechange'), GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces));

          //for Fiscal Printing - Open an Invoice in the Fiscal Printer
          if BalanceDelta > 0.005 then
          begin
            if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
            begin
              SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '0';
              SWBEncryption.GenerateEncryptedAuxFile;

              if not FiscalPaymentInitialized then
              begin
                if FCreditCardDiscount > 0 then
                begin
                  PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('D', '$', FCreditCardDiscount);
                end
                else
                begin
                  if (AppDetails.TipsAsChange) then
                    PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('A', '$', 0)     //dont send value of tip thus extra amount tendered is seen as change in fiscal printer
                  else
                    PrintOK := SFiscalPrinting.OpenFiscalPrinterPayment('A', '$', aTotalTip);
                  end;

                if not PrintOK then
                begin
                  LogMessage(ltPaymentTrace, '204' + #9);
                  TheTR.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end;
              end;
              if Trim(AppDetails.FiscalAccountPaymentMethod) <> '' then
              begin
                if ((AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC]) and (not SFiscalPrinting.AddFiscalPrinterPayment(AppDetails.FiscalAccountPaymentMethod, BalanceDelta))) then
                begin
                  LogMessage(ltPaymentTrace, '205' + #9);
                  TheTr.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end
                else if ((AppDetails.FiscalPrinterType = fpDaruma) and (not SFiscalPrinting.AddFiscalPrinterPayment(AppDetails.FiscalAccountPaymentMethod, BalanceDelta))) then
                begin
                  LogMessage(ltPaymentTrace, '206' + #9);
                  TheTr.Rollback;
                  if SFiscalPrinting.OpenedFiscalInvoice then
                    SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                  Result := False;
                  Exit;
                end;
              end;
              SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '1';
              SWBEncryption.GenerateEncryptedAuxFile;
            end;
          end;
          ExecProc;

          if ((LoyaltyReward.LoyaltyRewardID > 0) and {(not LoyaltyReward.Processed)} (TheInvoiceID > 0)) then begin
            SetStoredProcName('REDEEMLOYALTYPOINTS', SPNumber);             //redeemloyaltypoints

            ParamByName('accountid').AsInteger := GlbAccount.AccountID;
            ParamByName('loyaltyrewardid').AsInteger := LoyaltyReward.LoyaltyRewardID;
            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            IntegerAsParam(ParamByName('tenderid'), TheTenderID);
            IntegerAsParamIfBool(ParamByName('loyaltypoints1change'), LoyaltyReward.DecrementLoyalty1Points, ((LoyaltyReward.RewardOffer = roVariableDiscount) and (LoyaltyReward.DecrementLoyalty1Points > 0)));
            IntegerAsParamIfBool(ParamByName('loyaltypoints2change'), LoyaltyReward.DecrementLoyalty2Points, ((LoyaltyReward.RewardOffer = roVariableDiscount) and (LoyaltyReward.DecrementLoyalty2Points > 0)));
            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            ExecProc;
          end;
        end;

        SFiscalPrinting.UpdateInvoiceChecksum(TheSP, TheInvoiceID, False);

        if AppDetails.OrderDiscountEnable then
        begin
          if (GlbTable.AccountID <> GlbAccount.AccountID ) then
          begin
            FDiscCalculator.AccountID := GlbAccount.AccountID;
            FDiscCalculator.Account := GlbAccount;
            FDiscCalculator.LoadUninvoicedOls := True;
            FDiscCalculator.RecalculateDiscountsOfTheGroup(ComboFreeItems, False,True); // No Update db
          end;
        end;
      end;

      if ((AppDetails.PMSType = pmsLibica) and (TenderTypeExists(TLTLibica))) then begin
        if (not SendLibicaTransactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '207' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Result := False;
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsFidelio) and
               ((AppDetails.FidelioAllTransactions = 1) or
                ((AppDetails.FidelioExtendedTender > 0) and (AppDetails.FidelioAllTransactions < 2) and (TLList.Count > 0)) or
                (TenderTypeExists(TLTFidelio)))) then begin
        if (not SendFidelioTransactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '208' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsPhoenix) and ((AppDetails.PhoenixAllTransactions) or ((AppDetails.PhoenixExtendedTender) and (TLList.Count > 0)) or (TenderTypeExists(TLTPhoenix)))) then begin
        if (not SendPhoenixTransactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '209' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsMicros4700) and ((AppDetails.Micros4700AllTransactions = 1) or ((AppDetails.Micros4700ExtendedTender) and (AppDetails.Micros4700AllTransactions < 2) and (TLList.Count > 0)) or (TenderTypeExists(TLTMicros4700)) or (TenderTypeExists(TLTMicros4700Account)))) then begin
        if (not SendMicros4700Transactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '210' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsNZAGold) and (TenderTypeExists(TLTNZAGold))) then begin
        if (not SendNZATransactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '211' + #9);
          TheTr.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsEvolution) and (TenderTypeExists(TLTEvolution))) then begin
        if (not SendEvolutionTransactions(tmpList)) then begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '212' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Result := False;
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsICRTouch) and (TenderTypeExists(TLTRoomCharge))) then
      begin
        if (not SendICRTouchTransactions(tmpList)) then
        begin
          //Something wrong happened while putting in the Transactions.
          LogMessage(ltPaymentTrace, '213' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Result := False;
          Exit;
        end;
      end
      else if ((AppDetails.PMSType = pmsMews) and ((AppDetails.MewsAddOutletBills) or (TenderTypeExists(TLTMews)))) then begin
        if (not SendMewsTransactions(tmpList, SPNumber, TheTenderID)) then
        begin
          //Something wrong happened while putting in the Transactions.
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Exit;
        end;
      end;

      if (AppDetails.TableManagementType <> tabmanNone) then
      begin
        SendTableManagementTransactions(tmpList);
      end;

      if (AppDetails.FiscalType = fiscalTaxCore) then
      begin
        if (SendTaxCoreTransactions(tmpList)) and TaxCoreTransactionData.SaveInvoice(False, SPNumber) and TaxCoreTransactionData.InsertPrintJob(SPNumber) then
        begin
          if (TaxCoreTransactionData.Messages <> '') then
            ShowQuickMsg('Tax Core Transaction' + #13#10 + TaxCoreTransactionData.Messages);
          TaxCoreTransactionData.Clear;
        end
        else begin
          // transaction failed
          LogMessage(ltPaymentTrace, '214' + #9);
          if (TaxCoreTransactionData.Messages <> '') then
            ShowMsg('Tax Core Transaction' + #13#10 + TaxCoreTransactionData.Messages)
          else
            ShowMsg('Tax Core Transaction' + #13#10 + 'Failed');
          TheTR.Rollback;
          Result := False;
          Exit;
        end;
      end;

      // for Fiscal Printing - Open an Invoice in the Fiscal Printer
      if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
      begin
        //TSurchargeItem(SurchargeList[I]).
        aPromoMessage := SFiscalPrinting.GetFiscalInvoiceFooter(OLToPayList, Self, '', TheGroupID, TheInvoiceID);
        if not SFiscalPrinting.CloseFiscalPrinterPayment(aPromoMessage, TotalNCMTax) then
        begin
          LogMessage(ltPaymentTrace, '215' + #9);
          TheTR.Rollback;
          if SFiscalPrinting.OpenedFiscalInvoice then
            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          Result := False;
          Exit;
        end;
        SFiscalPrinting.FiscalOpTryAgainCommand := False;
        LogMessage(ltFiscal, 'Closed fiscal invoice');
      end;

      if (TLList.OpenDrawerExists(not SurplusOnAccount)) then
      begin
        if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          SFiscalPrinting.KickDrawer;
      end;

      LogMessage(ltPaymentTrace, '216' + #9);
      TheTR.Commit;
      LogMessage(ltPaymentTrace, '217' + #9);

      if (GlbEvent.EventID > 0) then
      begin
        SetGlbEvent(GlbEvent.EventID, True);

        if ((GlbEvent.Available) < 0) then
        begin
          ShowMsg(sEventSpendLimitReachedDeSelect);
          txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available);
          DeselectEvent;
        end;
      end;

      FoundFirstLoyaltyFreeItem := False;
      formEFTPOSIntegration.ClearTransactionID;
      LogMessage(ltPaymentTrace, '219' + #9);

      // send paymark loyalty amount to eftpos machine again
      if ((AppDetails.EFTPOSIntegrated and AppDetails.EFTPOSPaymarkLoyaltyEnabled) and
          (aTotalTender > 0) and (ShowConf(sIsLoyaltyCardAvailable) = mrYes)) then
      begin
        LogMessage(ltPaymentTrace, '220' + #9);
        formEFTPOSIntegration.EFTPOSTLType := TLTLoyaltyPoints;
        formEFTPOSIntegration.EFTPOSTxnRef := ''; // empty reference for loyalty points
        formEFTPOSIntegration.EFTPOSTxnAuthCode := '';
        formEFTPOSIntegration.AllowTipAuthorisation := False;
        formEFTPOSIntegration.DoEftposTrans(aTotalTender, 0, ettEftpos);
      end;

      if (AppDetails.EnableFiscalPrinting and AppDetails.EFTPOSIntegrated and EFTPOSTransExists) then
      begin
        SWBEncryption.AuxiliaryData.DBInvoiceDone := '1';
        SWBEncryption.GenerateEncryptedAuxFile;

        if (not SFiscalPrinting.PrintAllCDCs(TLList)) then
        begin
          SFiscalPrinting.LockKeyBoardWhilePrinting(False);
          SFiscalPrinting.CancelNonInvoiceTEF(True);
          SFiscalPrinting.RefundPOSSale(iOldTenderID, iRefundTenderID, iRefundInvoiceID, OrgInvCOO);
          SFiscalPrinting.SupressFiscalErrorMessage := True;
          PrintOK := SFiscalPrinting.FiscalPrinterEnabled;
          SFiscalPrinting.SupressFiscalErrorMessage := False;

          if PrintOK then
            SFiscalPrinting.CancelFiscalPrinterInvoice(SWBEncryption.AuxiliaryData.InvoiceCPF, SWBEncryption.AuxiliaryData.InvoiceName, SWBEncryption.AuxiliaryData.InvoiceAddress, True);

          // Here lets start the timer again!
          Result := False;
          Exit;
        end;
        SFiscalPrinting.LockKeyBoardWhilePrinting(False);
      end;

      if AppDetails.EnableFiscalPrinting then
      begin
        SWBEncryption.AuxiliaryData.TEFReferences := '';

        SWBEncryption.AuxiliaryData.DBInvoiceDone := '0';
        SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '0';
        SWBEncryption.AuxiliaryData.TEFDone := '0';
        SWBEncryption.AuxiliaryData.CDCDone := '0';

        SWBEncryption.AuxiliaryData.InvoiceCPF := '';
        SWBEncryption.AuxiliaryData.InvoiceName := '';
        SWBEncryption.AuxiliaryData.InvoiceAddress := '';
        SWBEncryption.GenerateEncryptedAuxFile;
      end;

      if ((BalanceDelta > 0.005) and (AppDetails.EnableFiscalPrinting) and (GlbAccount.AccountID > 0)) then
        SFiscalPrinting.PrintAccountPaymentReport(sPayOnAccount, BalanceDelta, FCPFNumber);

      SFiscalPrinting.COO := '';
      SFiscalPrinting.CCF := '';
      SFiscalPrinting.SupressFiscalErrorMessage := False;

      FSelectedItemsNotPaidFull := False;
      FUseItemDiscounts := False;
      Result := True;
      LogMessage(ltPaymentTrace, '221' + #9);
    except
      on e: exception do
      begin
        LogMessage(ltPaymentTrace, '222' + #9);
        ShowMessage(e.message);
        TheTr.Rollback;
        SFiscalPrinting.FiscalOpTryAgainCommand := False;

        if SFiscalPrinting.OpenedFiscalInvoice then
          SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice

        SelectFirsts;
      end;
    end;
    SFiscalPrinting.FiscalOpTryAgainCommand := False;
  finally
    sExternalAccountIntegration.FinalisePresentedCards;

    if Assigned(GlbNFEData) then
      GlbNFEData.Free;

    ClearListObjects(tmpList, True); //copies of OLs now in tmpList so need to free them
    FiscalTLList.Clear;
    FiscalTLList.Free;

    DM.qrGeneral.IB_Transaction := GenTR;

    ClearStatusPage;
    ClearStatus;
    LogMessage(ltPaymentTrace, '223' + #9);
  end;
end;
{******************************************************************************}
function TformPaymentTable.SendLibicaTransactions(AOLList: TList): Boolean;
var
  LibFood, LibBev, LibTip: Currency;
  TotalFood, TotalBev, LibAmt: Currency;
  LibDesc: string;
  LibGLCodeF, LibGLCodeB, LibGLCodeTip: string;
  ListCount: Integer;
  I: Integer;
  TL: TTL;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  LibicaSourceList: TList;
begin
  I := 0;
  ListCount := TLList.Count;
  while ((I < ListCount) and (TTL(TLList.Items[I]).TLTypeID <> GetTLTID(TLTLibica))) do begin
    Inc(I);
  end;
  if (I >= ListCount) then begin
  //This should never happen cos we should only come here if libicatransexist
    Result := False;
    Exit;
  end;
  TL := TLList.Items[I];

  LibAmt := TL.TLPayment;
  LibTip := TL.TLTip;

  ListCount := AOLList.Count;

  TotalFood := 0;
  TotalBev := 0;
  SoFarDiscount := 0;
  SoFarComboDiscount := 0;

  for I := 0 to (ListCount - 1) do
  begin
    if TOrderLine(AOLList[I]).ComboID > 0 then
    begin
      if TOrderLine(AOLList[I]).LastComboItem then
      begin
        ILDiscount := FComboDiscount - SoFarComboDiscount;
      end
      else
      begin
        ILDiscount := TOrderLine(AOLList[I]).AllowedDiscount;
      end;

      ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
      SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
      SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
    end
    else
    begin
      //if it's the last line, allocate whatever hasn't been pro rata-ed
      if TOrderLine(AOLList[I]).LastNormalItem then
        ILDiscount := FDiscountAmount - SoFarDiscount
      else
      begin              //pro rata discount and accumulate discount pro rata-ed so far
        ILDiscount := TOrderLine(AOLList[I]).AllowedDiscount;
        ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
        SoFarDiscount := SoFarDiscount + ILDiscount;
        SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
      end;
    end;

    ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
    SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);

    if (TOrderLine(AOLList[I]).ForB = 'F') then
    begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalFood := TotalFood + (((TOrderLine(AOLList[I]).ToPay - ILDiscount) * (100 + TOrderLine(AOLList[I]).SalesTaxPercent)) / 100);
      end
      else begin
        TotalFood := TotalFood + TOrderLine(AOLList[I]).ToPay - ILDiscount;
      end;
    end
    else
    begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalBev := TotalBev + (((TOrderLine(AOLList[I]).ToPay - ILDiscount) * (100 + TOrderLine(AOLList[I]).SalesTaxPercent)) / 100);
      end
      else begin
        TotalBev := TotalBev + TOrderLine(AOLList[I]).ToPay - ILDiscount;
      end;
    end;
  end;

  try
    //We are already in a Transaction so another one does not need to be started
    with dm.sp do begin
      SetStoredProcName('GETSALECATEGORYINFO');

      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := CurrentDay;
      Execute;

      if (FieldByName('salecategoryid').IsNull) then begin
        ShowQuickMsg(sSaleCatNotFound);
        Result := False;
        Exit;
      end;

      LibGLCodeF := FieldByName('libglf').AsString;
      LibGLCodeB := FieldByName('libglb').AsString;
      LibGLCodeTip := FieldByName('libgltip').AsString;

      if ((FieldByName('tplibglf').AsString <> '') and
        (FieldByName('tp1libglb').AsString <> '')) then begin
        LibGLCodeF := FieldByName('tplibglf').AsString;
        LibGLCodeB := FieldByName('tp1libglb').AsString
      end;
    end;
  except
    on e: exception do begin
      Result := False;
      ShowMessage(e.message);
      Exit;
    end;
  end;

  if (LibAmt > TotalFood) then begin
    LibFood := TotalFood;
    LibBev := LibAmt - TotalFood;
  end
  else begin
    LibFood := LibAmt;
    LibBev := 0;
  end;

  LibDesc := Copy('Invoice ' + IntToStr(WBInvoices.RefreshID) + ', ' + AppDetails.TerminalName, 1, 30);

  if (((LibFood > 0) and (LibGLCodeF = '')) or
      ((LibBev > 0) and (LibGLCodeB = '')) or
      ((LibTip > 0) and (LibGLCodeTip = ''))) then begin
    ShowQuickMsg(sGLCodeNotProvided);
    Result := False;
    Exit;
  end;

  LibicaSourceList := TList.Create;

  if (LibFood > 0) then begin
    LibicaSourceList.Add(NewPLibicaSourceData(LibAccount, LibGLCodeF, LibDesc, LibFood));
  end;
  if (LibBev > 0) then begin
    LibicaSourceList.Add(NewPLibicaSourceData(LibAccount, LibGLCodeB, LibDesc, LibBev));
  end;
  if (LibTip > 0) then begin
    LibicaSourceList.Add(NewPLibicaSourceData(LibAccount, LibGLCodeTip, LibDesc, LibTip));
  end;

  Result := DoLibicaTransactions(LibicaSourceList);
//  DestroyList(LibicaSourceList, True);
  ClearLibicaSourceDataList(LibicaSourceList);
  LibicaSourceList.Free;
end;
{******************************************************************************}
function TformPaymentTable.SendFidelioTransactions(AOLList: TList): Boolean;
var
  FidelioTransaction: TFidelioTransaction;
  ListFT: TList;

  FidTotal, FidSubtotal, FidTax: Currency;
  ListCount: Integer;
  I: Integer;
  ArrayI: Integer;
  TL: TTL;

  LineSub, LineTax, LineDisc, LineTotal: Currency;
  aSub: array [1 .. CN_FidelioArraySize] of Currency;
  aTax: array [1 .. CN_FidelioArraySize] of Currency;
  aDisc: array [1 .. CN_FidelioArraySize] of Currency;
  aTotal: array [1 .. CN_FidelioArraySize] of Currency;
  ServingTime: Integer;
  TenderLineType: TTenderLineType;

  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  OL: TOrderLine;
  SurchargeItem: TSurchargeItem;

  procedure AddFidelioValues;
  var
    Ratio: Double;
    J: Integer;
    LocalSub: Currency;
    LocalTax: Currency;

  begin
    for J := 1 to CN_FidelioArraySize do
    begin
      if (FidTotal > aTotal[J]) then begin
        Ratio := 1;
      end
      else if (aTotal[J] > 0) then begin
        Ratio := (FidTotal / aTotal[J]);
      end
      else begin
        Ratio := 0;
      end;
      LocalSub := RoundCurrency(aSub[J] * Ratio, 2);
      LocalTax := RoundCurrency(aTax[J] * Ratio, 2);
      FidelioTransaction.Sub[J] := FidelioTransaction.Sub[J] + LocalSub;
      FidelioTransaction.Tax[J] := FidelioTransaction.Tax[J] + LocalTax;
      FidelioTransaction.Disc[J] := RoundCurrency(LocalSub + LocalTax - (aTotal[J] * Ratio), 2);
      FidTotal := FidTotal - (aTotal[J] * Ratio);
      aTotal[J] := aTotal[J] * (1 - Ratio);
      aSub[J] := aSub[J] * (1 - Ratio);
      aTax[J] := aTax[J] * (1 - Ratio);
      aDisc[J] := aDisc[J] * (1 - Ratio);
    end;
  end;

begin
  ListFT := TList.Create;
  try
    I := 0;
    ListCount := TLList.Count;
    while ((I < ListCount) and (TTL(TLList.Items[I]).TLTypeID <> GetTLTID(TLTFidelio))) do begin
      Inc(I);
    end;
    if ((I >= ListCount) and (AppDetails.FidelioExtendedTender = 0) and (AppDetails.FidelioAllTransactions <> 1)) then begin
    //This should never happen cos we should only come here if Fideliotransexist
      Result := False;
      Exit;
    end
    else begin
      if (I >= ListCount) then begin
        TL := Nil;
      end
      else begin
        TL := TLList.Items[I];
      end;
      if ((AppDetails.FidelioBreakdownType = 0) and (Assigned(TL))) then begin
        FidelioTransaction := TFidelioTransaction.Create;

        FidelioTransaction.GuestNumber := GlbFidelioAccount.GuestNumber;
        FidelioTransaction.RoomNumber := GlbFidelioAccount.RoomNumber;
        FidelioTransaction.GuestName := GlbFidelioAccount.GuestName;
        FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
        if (GlbTable.Invoiced > 0) then begin
          FidelioTransaction.Covers := 0;
        end
        else begin
          FidelioTransaction.Covers := GlbTable.Guests;
        end;

        FidTotal := TL.TLPayment + TL.TLTip;
        if ((FToPay + FSurcharge - FDiscountAmount) <> 0) then begin
          FidTax := FSalesTax * (TL.TLPayment) / (FToPay + FSurcharge - FDiscountAmount);
        end
        else begin
          FidTax := 0;
        end;
        FidSubtotal := FidTotal - FidTax;

        FidelioTransaction.FoodSub := FidSubtotal;
        FidelioTransaction.FoodTax := FidTax;
        FidelioTransaction.BreakdownType := 0;
        FidelioTransaction.Refund := False;
        ListFT.Add(FidelioTransaction);
      end
      else if (AppDetails.FidelioBreakdownType > 0) then begin
        ListCount := AOLList.Count - 1;

        for I := 1 to CN_FidelioArraySize do
        begin
          aSub[I] := 0.00;
          aTax[I] := 0.00;
          aDisc[I] := 0.00;
          aTotal[I] := 0.00;
        end;
        SoFarDiscount := 0;
        SoFarComboDiscount := 0.00;

        for I := 0 to ListCount do
        begin
          OL := AOLList.Items[I];

          if OL.ComboID > 0 then
          begin
            if OL.LastComboItem then
            begin
              ILDiscount := FComboDiscount - SoFarComboDiscount;
            end
            else
            begin
              ILDiscount := OL.AllowedDiscount;
            end;

            ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
            SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
            SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
          end
          else
          begin            //if it's the last line, allocate whatever hasn't been pro rata-ed
            if OL.LastNormalItem then
              ILDiscount := FDiscountAmount - SoFarDiscount
            else
            begin              //pro rata discount and accumulate discount pro rata-ed so far
              ILDiscount := OL.AllowedDiscount;
              ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
              SoFarDiscount := SoFarDiscount + ILDiscount;
              SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
            end;
          end;

          if (AppDetails.TaxExclusivePrices) then
          begin
            LineSub := OL.ToPay;
            LineTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
            LineDisc := ILDiscount;
            LineTotal := LineSub + LineTax - LineDisc;
          end
          else begin
            LineSub := GetRoundedUpDown(((OL.ToPay * 100) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
            LineTotal := (OL.ToPay - ILDiscount);
            LineTax := GetRoundedUpDown(LineTotal * ((OL.SalesTaxPercent) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
            LineDisc := GetRoundedUpDown((ILDiscount * 100) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
          end;

          ArrayI := StrToIntDef(OL.GLCode, 0);
          if (not (ArrayI in [1 .. CN_FidelioArraySize])) then
          begin
            if (OL.ForB = 'F') then
            begin
              ArrayI := 1;
            end
            else
            begin
              ArrayI := 2;
            end;
          end;

          aSub[ArrayI] := aSub[ArrayI] + LineSub;
          aTax[ArrayI] := aTax[ArrayI] + LineTax;
          aDisc[ArrayI] := aDisc[ArrayI] + LineDisc;
          aTotal[ArrayI] := aTotal[ArrayI] + LineTotal;
        end;

        ListCount := (SurchargeList.Count - 1);
        for I := 0 to (ListCount) do begin
          SurchargeItem := SurchargeList.Items[I];

          if (AppDetails.TaxExclusivePrices) then begin
            LineSub := SurchargeItem.Amount;
            LineTax := GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
            LineDisc := 0;
            LineTotal := LineSub + LineTax;
          end
          else begin
            LineSub := ((SurchargeItem.Amount * 100) / (100 + SurchargeItem.SalesTaxPercent));
            LineTotal := GetRoundedUpDown(SurchargeItem.Amount, AppDetails.DecimalPlaces);
            LineTax := GetRoundedUpDown(LineTotal * ((SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent)), AppDetails.DecimalPlaces);
            LineDisc := 0;
          end;

          aSub[3] := aSub[3] + LineSub;
          aTax[3] := aTax[3] + LineTax;
          aDisc[3] := aDisc[3] + LineDisc;
          aTotal[3] := aTotal[3] + LineTotal;
        end;

        Try
          //We are already in a Transaction so another one does not need to be started
          with dm.sp do begin
            SetStoredProcName('GETSALECATEGORYINFO');

            ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
            ParamByName('currenttime').AsInteger := CurrentTime;
            ParamByName('currentday').AsInteger := CurrentDay;
            Execute;

            if (FieldByName('salecategoryid').IsNull) then begin
              dm.tr.Rollback;   //Jon 14-07-2003
              ShowQuickMsg(sSaleCatNotFound);
              Result := False;
              Exit;
            end;

            ServingTime := FieldAsInt(FieldByName('fidservingtime'), 0);

            if ((FieldAsInt(FieldByName('tpfidservingtime'))) > 0) then begin
              ServingTime := FieldAsInt(FieldByName('tpfidservingtime'));
            end;
          end;
        except
          on e: exception do begin
            dm.tr.Rollback;   //Jon 14-07-2003
            Result := False;
            ShowMessage(e.message);
            Exit;
          end;
        end;
        if (Assigned(TL)) then begin
          FidelioTransaction := TFidelioTransaction.Create;

          FidelioTransaction.GuestNumber := GlbFidelioAccount.GuestNumber;
          FidelioTransaction.RoomNumber := GlbFidelioAccount.RoomNumber;
          FidelioTransaction.GuestName := GlbFidelioAccount.GuestName;
          FidelioTransaction.ServingTime := ServingTime;
          FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
          if (GlbTable.Invoiced > 0) then begin
            FidelioTransaction.Covers := 0;
          end
          else begin
            FidelioTransaction.Covers := GlbTable.Guests;
          end;

          FidTotal := TL.TLPayment;
          AddFidelioValues;

          FidelioTransaction.Tip := TL.TLTip;

          FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
          FidelioTransaction.Refund := False;

          ListFT.Add(FidelioTransaction);
        end;
        if ((AppDetails.FidelioExtendedTender > 0) and
            ((AppDetails.FidelioAllTransactions < 2) or
             ((AppDetails.FidelioAllTransactions = 2) and (TLList.TenderTypeExists(TLTFidelio))))) then
        begin
          ListCount := (TLList.Count - 1);
          for I := 0 to ListCount do
          begin
            TL := TLList.Items[I];
            if (TL.TLTypeID <> GetTLTID(TLTFidelio)) then
            begin
              FidelioTransaction := TFidelioTransaction.Create;
              TenderLineType := GetTenderLineTypeRecord(TL.TLType);

              FidelioTransaction.GuestNumber := StrToIntDef(TenderLineType.LibGLCode, AppDetails.FidelioLIBGLCode);
              FidelioTransaction.RoomNumber := '';
              FidelioTransaction.GuestName := '';
              FidelioTransaction.ServingTime := ServingTime;
              FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
              if ((ListFT.Count > 0) or (GlbTable.Invoiced > 0)) then begin
                FidelioTransaction.Covers := 0;
              end
              else begin
                FidelioTransaction.Covers := GlbTable.Guests;
              end;
              FidelioTransaction.PaymentMethod := TenderLineType.MiniName;

              FidTotal := TL.TLPayment;
              AddFidelioValues;

              FidelioTransaction.Tip := TL.TLTip;

              FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
              FidelioTransaction.Refund := False;

              ListFT.Add(FidelioTransaction);
            end;
          end;
        end;
        //Post a transaction with 0 value to show the till was opened.
        if (((AppDetails.FidelioAllTransactions = 1) or
             ((AppDetails.FidelioAllTransactions = 2) and (TLList.TenderTypeExists(TLTFidelio)))) and
            (ListFT.Count <= 0)) then begin
          FidelioTransaction := TFidelioTransaction.Create;
          TenderLineType := GetTenderLineTypeRecord(TLTCash);

          FidelioTransaction.GuestNumber := StrToIntDef(TenderLineType.LibGLCode, AppDetails.FidelioLIBGLCode);
          FidelioTransaction.RoomNumber := '';
          FidelioTransaction.GuestName := '';
          FidelioTransaction.ServingTime := ServingTime;
          FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
          if (GlbTable.Invoiced > 0) then begin
            FidelioTransaction.Covers := 0;
          end
          else begin
            FidelioTransaction.Covers := GlbTable.Guests;
          end;
          FidelioTransaction.PaymentMethod := TenderLineType.MiniName;

          FidTotal := 0;
          AddFidelioValues;

          FidelioTransaction.Tip := 0;

          FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
          FidelioTransaction.Refund := False;

          ListFT.Add(FidelioTransaction);
        end;
      end;
    end;
    Result := DoFidelioTransactionList(ListFT);
    //RPC Here if we failed, we removed the room transfer tender to force to re enter room details
    if (not Result) then begin
      ListCount := TLList.Count;
      I := 0;
      while (I < ListCount) do begin
        if (TTL(TLList.Items[I]).TLTypeID = GetTLTID(TLTFidelio)) then begin
          grdTLs.Row := I+1;
          DeleteTenderLine;
          Dec(ListCount);
        end;
        Inc(I);
      end;
    end;
  finally
    ClearListObjects(ListFT, True);
  end;
end;
{******************************************************************************}
function TformPaymentTable.SendPhoenixTransactions(AOLList: TList): Boolean;
var
  PhoenixTransaction: TPhoenixTransaction;
  ListPT: TList;

  PhxTotal: Currency;
  ListCount: Integer;
  I: Integer;
  TL: TTL;

  GLFoodTotal, GLBevTotal, FoodTotal, BevTotal, MiscTotal, TipTotal: Currency;
  LineSub, LineTax, LineDisc, LineTotal: Currency;
  TenderLineType: TTenderLineType;

  FoodCat, BevCat, MiscCat, TipCat: Integer;
  tmpFoodCat, tmpBevCat: Integer;
  OverrideIGGLCode: Boolean;

  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  OL: TOrderLine;
  SurchargeItem: TSurchargeItem;
  tmpItemGroup: TItemGroup;

  procedure AddPhoenixValues(DoEverything: Boolean);

    procedure AllocateAgainstPhxTotal(ACategory: Integer; ATotal: Currency);
    var
      Ratio: Double;
      Value: Currency;
    begin
      if ((PhxTotal > ATotal) or (DoEverything)) then begin
        Ratio := 1;
      end
      else if (ATotal > 0) then begin
        Ratio := (PhxTotal / ATotal);
      end
      else begin
        Ratio := 0;
      end;

      Value := RoundCurrency(ATotal * Ratio, 2);
      if (Value <> 0) then begin
        PhoenixTransaction.AddPhoenixCategory(ACategory, Value, True);
      end;

      PhxTotal := PhxTotal - (ATotal * Ratio);
    end;

    procedure AllocateGLOLsAgainstPhxTotal(ForB: string; ATotal: Currency);
    var
      iIDx: Integer;
      cAmtToAlloc: Currency;
    begin
      //if there is a GL Food/Bev total, it is distributed across the order
      //lines below, where there is a GLCode, and the ForB param matches
      if ATotal > 0 then
      begin
        //loop through each order line
        ListCount := (AOLList.Count - 1);
        for iIDx := 0 to ListCount do
        begin
          OL := AOLList.Items[iIDx];

          //if find an order line with a GLCode (from item's itemgroup)
          if (OL.GLCode <> '') and (OL.FORB = ForB) then
          begin
            //allocate the net amount to pay on the orderline (or less, if remaining total is smaller)
            if OL.NetToPay <= ATotal then
              cAmtToAlloc := OL.NetToPay
            else
              cAmtToAlloc := ATotal;
            AllocateAgainstPhxTotal(StrToIntDef(OL.GLCode, 0), cAmtToAlloc);

            //remove amount just allocated from remaining total
            ATotal := ATotal - cAmtToAlloc;
            if ATotal <= 0 then
              Break;
          end;
        end;
      end;
    end;

  begin
    AllocateGLOLsAgainstPhxTotal('F', GLFoodTotal);
    AllocateGLOLsAgainstPhxTotal('B', GLBevTotal);
    AllocateAgainstPhxTotal(FoodCat, FoodTotal);
    AllocateAgainstPhxTotal(BevCat, BevTotal);
    AllocateAgainstPhxTotal(MiscCat, MiscTotal);
  end;

begin
  ListPT := TList.Create;
  PhoenixTransaction := nil;
  try
    I := 0;
    ListCount := TLList.Count;
    while ((I < ListCount) and (TTL(TLList.Items[I]).TLTypeID <> GetTLTID(TLTPhoenix))) do
    begin
      Inc(I);
    end;
    if ((I >= ListCount) and (not (AppDetails.PhoenixExtendedTender or AppDetails.PhoenixAllTransactions))) then begin
    //This should never happen cos we should only come here if Phoenixtransexist
      Result := False;
      Exit;
    end
    else begin
      if (I >= ListCount) then begin
        TL := Nil;
      end
      else begin
        TL := TLList.Items[I];
      end;

      try
        //We are already in a Transaction so another one does not need to be started
        with dm.sp do begin
          SetStoredProcName('GETSALECATEGORYINFO');

          ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
          ParamByName('currenttime').AsInteger := CurrentTime;
          ParamByName('currentday').AsInteger := CurrentDay;
          Execute;

          if (FieldByName('salecategoryid').IsNull) then begin
            dm.tr.Rollback;   //Jon 14-07-2003
            ShowQuickMsg(sSaleCatNotFound);
            Result := False;
            Exit;
          end;

          FoodCat := StrToIntDef(FieldAsString(FieldByName('libglf')), 1);
          BevCat := StrToIntDef(FieldAsString(FieldByName('libglb')), 2);
          MiscCat := StrToIntDef(FieldAsString(FieldByName('libglmisc')), 3);
          TipCat := StrToIntDef(FieldAsString(FieldByName('libgltip')), 4);

          tmpFoodCat := StrToIntDef(FieldAsString(FieldByName('tplibglf')), FoodCat);
          tmpBevCat := StrToIntDef(FieldAsString(FieldByName('tplibglb')), BevCat);
          OverrideIGGLCode := FieldAsBool(FieldByName('overrideigglcode'));
          if ((tmpFoodCat <> 0) and (tmpBevCat <> 0)) then begin
            FoodCat := tmpFoodCat;
            BevCat := tmpBevCat;
          end;
        end;
      except
        on e: exception do begin
          dm.tr.Rollback;   //Jon 14-07-2003
          Result := False;
          ShowMessage(e.message);
          Exit;
        end;
      end;

      if ((AppDetails.PhoenixBreakdownType = 0) and (Assigned(TL))) then begin
        PhoenixTransaction := TPhoenixTransaction.Create;

        PhoenixTransaction.GuestNumber := GlbPhoenixAccount.GuestNumber;
        PhoenixTransaction.RoomNumber := GlbPhoenixAccount.RoomNumber;
        PhoenixTransaction.GuestName := GlbPhoenixAccount.GuestName;
        PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
        if (GlbTable.Invoiced > 0) then begin
          PhoenixTransaction.Covers := 0;
        end
        else begin
          PhoenixTransaction.Covers := GlbTable.Guests;
        end;

        PhxTotal := TL.TLPayment + TL.TLTip;

        PhoenixTransaction.AddPhoenixCategory(FoodCat, PhxTotal);

        PhoenixTransaction.BreakdownType := 0;
        PhoenixTransaction.Refund := False;
        ListPT.Add(PhoenixTransaction);
      end
      else if (AppDetails.PhoenixBreakdownType = 1) then begin
      //Do this right then remove the same code where it exists below
        ListCount := (AOLList.Count - 1);

        GLFoodTotal := 0;
        GLBevTotal := 0;
        FoodTotal := 0;
        BevTotal := 0;
        MiscTotal := 0;
        TipTotal := 0;
        SoFarDiscount := 0;
        SoFarComboDiscount := 0;

        for I := 0 to ListCount do begin
          OL := AOLList.Items[I];

          if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
          begin
            ILDiscount := 0;
          end
          else if OL.ComboID > 0 then
          begin
            if OL.LastComboItem then
            begin
              ILDiscount := FComboDiscount - SoFarComboDiscount;
            end
            else
            begin
              ILDiscount := OL.AllowedDiscount;
            end;

            ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
            SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
            SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
          end
          else
          begin
            //if it's the last line, allocate whatever hasn't been pro rata-ed
            if OL.LastNormalItem then
              ILDiscount := FDiscountAmount - SoFarDiscount
            else
            begin              //pro rata discount and accumulate discount pro rata-ed so far
              ILDiscount := OL.AllowedDiscount;
              ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
              SoFarDiscount := SoFarDiscount + ILDiscount;
              SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
            end;
          end;

          if (AppDetails.TaxExclusivePrices) then begin
            LineSub := OL.ToPay;
            LineTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
            LineDisc := ILDiscount;
            LineTotal := LineSub + LineTax - LineDisc;
          end
          else begin
            LineTotal := (OL.ToPay - ILDiscount);
          end;

          if (OL.GLCode <> '') and (not OverrideIGGLCode) then
          begin
            OL.NetToPay := LineTotal;
            if (OL.FORB = 'F') then begin
              GLFoodTotal := GLFoodTotal + LineTotal;
            end
            else begin
              GLBevTotal := GLBevTotal + LineTotal;
            end;
          end
          else
          begin
            if (OL.FORB = 'F') then begin
              FoodTotal := FoodTotal + LineTotal;
            end
            else begin
              BevTotal := BevTotal + LineTotal;
            end;
          end;
        end;

        PhoenixTransaction := nil;

        ListCount := (SurchargeList.Count - 1);
        for I := 0 to (ListCount) do begin
          SurchargeItem := SurchargeList.Items[I];

          if (AppDetails.TaxExclusivePrices) then begin
            LineSub := SurchargeItem.Amount;
            LineTax := GetRoundedUpDown(((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100), AppDetails.DecimalPlaces);
            LineTotal := LineSub + LineTax;
          end
          else begin
            LineTotal := SurchargeItem.Amount;
          end;
          LineTotal := GetRoundedUpDown(LineTotal, AppDetails.DecimalPlaces);

          tmpItemGroup := GlobalMenuList.GetItemsItemGroup(SurchargeItem.ItemID);
          if (Assigned(tmpItemGroup)) then
            SurchargeItem.GLCode := tmpItemGroup.GLCode;

          if (SurchargeItem.GLCode <> '') and (StrToIntDef(SurchargeItem.GLCode, 0) > 0) then
          begin
            if (not Assigned(PhoenixTransaction)) then
              PhoenixTransaction := TPhoenixTransaction.Create;
            PhoenixTransaction.AddPhoenixCategory(StrToInt(SurchargeItem.GLCode), LineTotal, True);
          end
          else
            MiscTotal := MiscTotal + LineTotal;
        end;

        if (Assigned(TL)) then
        begin
          if (not Assigned(PhoenixTransaction)) then
            PhoenixTransaction := TPhoenixTransaction.Create;

          PhoenixTransaction.GuestNumber := GlbPhoenixAccount.GuestNumber;
          PhoenixTransaction.RoomNumber := GlbPhoenixAccount.RoomNumber;
          PhoenixTransaction.GuestName := GlbPhoenixAccount.GuestName;
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
          if (GlbTable.Invoiced > 0) then begin
            PhoenixTransaction.Covers := 0;
          end
          else begin
            PhoenixTransaction.Covers := GlbTable.Guests;
          end;

          PhxTotal := TL.TLPayment;
          TipTotal := TL.TLTip;

          PhoenixTransaction.BreakdownType := 1;
          PhoenixTransaction.Refund := False;

          ListPT.Add(PhoenixTransaction);
        end
        else if (AppDetails.PhoenixExtendedTender) then
        begin
          if (not Assigned(PhoenixTransaction)) then
            PhoenixTransaction := TPhoenixTransaction.Create;

          PhoenixTransaction.GuestNumber := AppDetails.PhoenixTenGuestNumber;
          PhoenixTransaction.RoomNumber := AppDetails.PhoenixTenRoomNumber;
          PhoenixTransaction.GuestName := '';
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
          if (GlbTable.Invoiced > 0) then begin
            PhoenixTransaction.Covers := 0;
          end
          else begin
            PhoenixTransaction.Covers := GlbTable.Guests;
          end;

          PhoenixTransaction.BreakdownType := 1;
          PhoenixTransaction.Refund := False;

          ListPT.Add(PhoenixTransaction);
        end;

        if (Assigned(PhoenixTransaction)) then begin
          AddPhoenixValues(AppDetails.PhoenixExtendedTender);

          if (AppDetails.PhoenixExtendedTender) then begin
            //Get the tips first
            ListCount := (TLList.Count - 1);
            for I := 0 to ListCount do begin
              TL := TLList.Items[I];
              if (TL.TLTypeID <> GetTLTID(TLTPhoenix)) then begin
                TipTotal := TipTotal + TL.TLTip;
              end;
            end;
            if (TipTotal <> 0) then begin
              PhoenixTransaction.AddPhoenixCategory(TipCat, TipTotal);
            end;

            for I := 0 to ListCount do begin
              TL := TLList.Items[I];
              if (TL.TLTypeID <> GetTLTID(TLTPhoenix)) then begin
                TenderLineType := GetTenderLineTypeRecord(TL.TLType);

                PhoenixTransaction.AddPhoenixCategory(StrToIntDef(TenderLineType.LibGLCode, 0), TL.TLPayment + TL.TLTip);
              end;
            end;
          end
          else if (TipTotal <> 0) then begin
            PhoenixTransaction.AddPhoenixCategory(TipCat, TipTotal);
          end;
        end;

        if ((AppDetails.PhoenixAllTransactions) and (ListPT.Count <= 0)) then
        begin
          if (not Assigned(PhoenixTransaction)) then
            PhoenixTransaction := TPhoenixTransaction.Create;

          PhoenixTransaction.GuestNumber := AppDetails.PhoenixTenGuestNumber;
          PhoenixTransaction.RoomNumber := AppDetails.PhoenixTenRoomNumber;
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
          if (GlbTable.Invoiced > 0) then begin
            PhoenixTransaction.Covers := 0;
          end
          else begin
            PhoenixTransaction.Covers := GlbTable.Guests;
          end;

          PhoenixTransaction.BreakdownType := 1;
          PhoenixTransaction.Refund := False;

          ListPT.Add(PhoenixTransaction);
        end;

        if (Assigned(PhoenixTransaction)) then begin
          if (PhoenixTransaction.ListPhoenixCategorys.Count = 0) then begin
            PhoenixTransaction.AddPhoenixCategory(FoodCat, 0);
          end;
        end;
      end;
    end;

    Result := DoPhoenixTransactionList(ListPT);
  finally
    for I := (ListPT.Count - 1) downto 0 do begin
      PhoenixTransaction := ListPT.Items[I];
      PhoenixTransaction.Free;
      ListPT.Delete(I);
    end;
    ListPT.Free;
  end;
end;
{*******************************************************************************
Calculates and send the transactions to the Micros 4700 spec system.

Change History
#00338# 23/08/2012 JBB Breakdown into subtotals with tax and discounts.
*******************************************************************************}

function TformPaymentTable.SendMicros4700Transactions(AOLList: TList): Boolean;
var
  TL: TTL;
  I: Integer;
  MT: TMicros4700Transaction;
  TenderLineType: TTenderLineType;  
  PaymentTotal: Currency;
  GLFoodTotal, GLBevTotal: Currency;
  FoodSub, FoodTax, FoodTaxBefore, FoodDisc, FoodDiscBefore, FoodTotal: Currency;
  BevSub, BevTax, BevTaxBefore, BevDisc, BevDiscBefore, BevTotal: Currency;
  MiscSub, MiscTax, MiscTaxBefore, MiscDisc, MiscDiscBefore, MiscTotal: Currency;
  LineSub, LineTax, LineTaxBefore, LineDisc, LineDiscBefore, LineTotal: Currency;
  Ratio: Double;
  MaxI: Integer;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  OL: TOrderLine;
  SurchargeItem: TSurchargeItem;
  tmpItemGroup: TItemGroup;
  FoodCat, BevCat, MiscCat, TipCat: String;
  tmpFoodCat, tmpBevCat: String;
  OverrideIGGLCode: Boolean;

  procedure AddMicros4700Values(DoEverything: Boolean);
    procedure AllocateAgainstM4700Total(ACategory: String; ATotal: Currency);
    var
      Ratio: Double;
      Value: Currency;
    begin
      if ((MT.PostingValue > ATotal) or (DoEverything)) then begin
        Ratio := 1;
      end
      else if (ATotal > 0) then begin
        Ratio := (MT.PostingValue / ATotal);
      end
      else begin
        Ratio := 0;
      end;

      Value := GetRoundedUpDown(ATotal * Ratio, AppDetails.DecimalPlaces);
      if (Value <> 0) then begin
        MT.AddMicros4700Category(ACategory, Value, True);
      end;

      MT.PostingValue := MT.PostingValue - (ATotal * Ratio);
    end;

    procedure AllocateGLOLsAgainstM4700Total(ForB: string; ATotal: Currency);
    var
      I: Integer;
      aAmtToAlloc: Currency;
    begin
      //if there is a GL Food/Bev total, it is distributed across the order
      //lines below, where there is a GLCode, and the ForB param matches
      if ATotal > 0 then
      begin
        //loop through each order line
        MaxI := (AOLList.Count - 1);
        for I := 0 to MaxI do
        begin
          OL := AOLList.Items[I];

          //if find an order line with a GLCode (from item's itemgroup)
          if (OL.GLCode <> '') and (OL.FORB = ForB) then
          begin
            //allocate the net amount to pay on the orderline (or less, if remaining total is smaller)
            if OL.NetToPay <= ATotal then
              aAmtToAlloc := OL.NetToPay
            else
              aAmtToAlloc := ATotal;
            AllocateAgainstM4700Total(OL.ForB + OL.GLCode, aAmtToAlloc);

            //remove amount just allocated from remaining total
            ATotal := ATotal - aAmtToAlloc;
            if ATotal <= 0 then
              Break;
          end;
        end;
      end;
    end;

  begin
    AllocateGLOLsAgainstM4700Total('F', GLFoodTotal);
    AllocateGLOLsAgainstM4700Total('B', GLBevTotal);
    AllocateAgainstM4700Total(FoodCat, FoodTotal);
    AllocateAgainstM4700Total(BevCat, BevTotal);
    AllocateAgainstM4700Total(MiscCat, MiscTotal);
  end;

begin
  MT := TMicros4700Transaction.Create;
  Result := False;
  try
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTMicros4700Account));
    if (not Assigned(TL)) then
      TL := TLList.GetTLOfTypeID(GetTLTID(TLTMicros4700));
    if ((not Assigned(TL)) and (AppDetails.Micros4700ExtendedTender) and (AppDetails.Micros4700AllTransactions <> 1)) then
    begin
    //This should never happen cos we should only come here if MicrosTransExist
      Exit;
    end;

    MT.RoomNumber := GlbMicros4700Account.RoomNumber;
    MT.SubFolio := GlbMicros4700Account.SubFolio;
    MT.InvoiceID := WBInvoices.RefreshID;

    if Assigned(TL) then
    begin
      MT.PostingValue := TL.TLPayment + TL.TLTip;
      MT.TipValue := TL.TLTip;

      TenderLineType := GetTenderLineTypeRecord(TL.TLType);
      MT.Payment := StrToIntDef(TenderLineType.LibGLCode, 0)
    end;
    MT.FoodValue := 0;
    MT.BevValue := 0;
    MT.MiscValue := 0;

    MT.BreakdownType := AppDetails.Micros4700BreakdownType;
    MT.RevenueCenter := AppDetails.Micros4700RevenueCenter;
    MT.Refund := False;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.sp do begin
        SetStoredProcName('GETSALECATEGORYINFO');

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        Execute;

        if (FieldByName('salecategoryid').IsNull) then begin
          dm.tr.Rollback;   //Jon 14-07-2003
          ShowQuickMsg(sSaleCatNotFound);
          Exit;
        end;

        MT.ServingPeriod := FieldAsInt(FieldByName('fidservingtime'), 0);

        if ((FieldAsInt(FieldByName('tpfidservingtime'))) > 0) then begin
          MT.ServingPeriod := FieldAsInt(FieldByName('tpfidservingtime'));
        end;

        FoodCat := 'F' + FieldAsString(FieldByName('libglf'));
        BevCat := 'B' + FieldAsString(FieldByName('libglb'));
        MiscCat := 'M' + FieldAsString(FieldByName('libglmisc'));
        TipCat := 'T' + FieldAsString(FieldByName('libgltip'));

        tmpFoodCat := 'F' + FieldAsString(FieldByName('tplibglf'));
        tmpBevCat := 'B' + FieldAsString(FieldByName('tplibglb'));
        OverrideIGGLCode := FieldAsBool(FieldByName('overrideigglcode'));
        if ((tmpFoodCat <> 'F') and (tmpBevCat <> 'B')) then begin
          FoodCat := tmpFoodCat;
          BevCat := tmpBevCat;
        end;

      end;
    except
      on e: exception do begin
        dm.tr.Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;

    if (AppDetails.Micros4700BreakdownType = 1) then begin
      FoodSub := 0;
      FoodTax := 0;
      FoodTaxBefore := 0;
      FoodDisc := 0;
      FoodDiscBefore := 0;
      FoodTotal := 0;
      BevSub := 0;
      BevTax := 0;
      BevTaxBefore := 0;
      BevDisc := 0;
      BevDiscBefore := 0;
      BevTotal := 0;
      MiscSub := 0;
      MiscTax := 0;
      MiscTaxBefore := 0;
      MiscDisc := 0;
      MiscDiscBefore := 0;
      MiscTotal := 0;
      SoFarDiscount := 0;
      SoFarComboDiscount := 0;
      MaxI := (AOLList.Count - 1);
      for I := 0 to MaxI do begin
        OL := AOLList.Items[I];

        if (FDiscountAmount <= 0) then begin
          ILDiscount := 0;
        end
        else
        begin
          //if it's the last line, allocate whatever hasn't been pro rata-ed
          if (I = MaxI) then begin
            ILDiscount := FDiscountAmount - SoFarDiscount;
          end
          else begin
            //pro rata discount and accumulate discount pro rata-ed so far
            ILDiscount := OL.AllowedDiscount;
            (*if (not FUseDiscountAmount) then
            begin
              ILDiscount := OL.AllowedDiscount;
            end
            else begin
              ILDiscount := ((OL.ToPay / FToPay) * FDiscountAmount);   //Jon 25-6-2001
            end;*)

            ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
            SoFarDiscount := SoFarDiscount + ILDiscount;
            SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
          end;
        end;

        if (AppDetails.TaxExclusivePrices) then
        begin
          LineSub := OL.ToPay;
          LineTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineTaxBefore := GetRoundedUpDown((OL.ToPay * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineDisc := ILDiscount;
          LineDiscBefore := ILDiscount;
          LineTotal := LineSub + LineTax - LineDisc;
        end
        else
        begin
          LineTax := GetRoundedUpDown((OL.ToPay - ILDiscount) * ((OL.SalesTaxPercent) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
          LineTaxBefore := GetRoundedUpDown((OL.ToPay * OL.SalesTaxPercent) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
          LineDisc := GetRoundedUpDown((ILDiscount * 100) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
          LineDiscBefore := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          LineSub := GetRoundedUpDown(OL.ToPay - LineTax, AppDetails.DecimalPlaces);
          LineTotal := (OL.ToPay - ILDiscount);
        end;

        if (OL.FORB = 'F') then
        begin
          FoodSub := FoodSub + LineSub;
          FoodTax := FoodTax + LineTax;
          FoodTaxBefore := FoodTaxBefore + LineTaxBefore;
          FoodDisc := FoodDisc + LineDisc;
          FoodDiscBefore := FoodDiscBefore + LineDiscBefore;
          FoodTotal := FoodTotal + LineTotal;
        end
        else
        begin
          BevSub := BevSub + LineSub;
          BevTax := BevTax + LineTax;
          BevTaxBefore := BevTaxBefore + LineTaxBefore;
          BevDisc := BevDisc + LineDisc;
          BevDiscBefore := BevDiscBefore + LineDiscBefore;
          BevTotal := BevTotal + LineTotal;
        end;
      end;

      MaxI := (SurchargeList.Count - 1);
      for I := 0 to MaxI do begin
        SurchargeItem := SurchargeList.Items[I];

        if (AppDetails.TaxExclusivePrices) then
        begin
          LineSub := SurchargeItem.Amount;
          LineTax := GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineTaxBefore := LineTax;
          LineDisc := 0;
          LineDiscBefore := 0;
          LineTotal := LineSub + LineTax;
        end
        else
        begin
          LineSub := ((SurchargeItem.Amount * 100) / (100 + SurchargeItem.SalesTaxPercent));
          LineTotal := SurchargeItem.Amount;
          LineTax := GetRoundedUpDown(LineTotal * ((SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent)), AppDetails.DecimalPlaces);
          LineTaxBefore := LineTax;
          LineDisc := 0;
          LineDiscBefore := 0;
        end;

        MiscSub := MiscSub + LineSub;
        MiscTax := MiscTax + LineTax;
        MiscTaxBefore := MiscTaxBefore + LineTaxBefore;
        MiscDisc := MiscDisc + LineDisc;
        MiscDiscBefore := MiscDiscBefore + LineDiscBefore;
        MiscTotal := MiscTotal + LineTotal;
      end;

      PaymentTotal := TL.TLPayment;

      if (PaymentTotal > FoodTotal) then
      begin
        Ratio := 1;
      end
      else if (FoodTotal > 0) then
      begin
        Ratio := (PaymentTotal / FoodTotal);
      end
      else
      begin
        Ratio := 0;
      end;

      MT.FoodValue := GetRoundedUpDown(FoodTotal * Ratio, AppDetails.DecimalPlaces);
      MT.FoodSub := GetRoundedUpDown(FoodSub * Ratio, AppDetails.DecimalPlaces);
      MT.FoodTax := GetRoundedUpDown(FoodTax * Ratio, AppDetails.DecimalPlaces);
      MT.FoodTaxBefore := GetRoundedUpDown(FoodTaxBefore * Ratio, AppDetails.DecimalPlaces);
      MT.FoodDisc := GetRoundedUpDown(MT.FoodSub + MT.FoodTax - MT.FoodValue, AppDetails.DecimalPlaces);
      MT.FoodDiscBefore := GetRoundedUpDown(MT.FoodSub + MT.FoodTaxBefore - MT.FoodValue, AppDetails.DecimalPlaces);
      PaymentTotal := PaymentTotal - MT.FoodValue;

      if (PaymentTotal > BevTotal) then
      begin
        Ratio := 1;
      end
      else if (BevTotal > 0) then
      begin
        Ratio := (PaymentTotal / BevTotal);
      end
      else
      begin
        Ratio := 0;
      end;

      MT.BevValue := GetRoundedUpDown(BevTotal * Ratio, AppDetails.DecimalPlaces);
      MT.BevSub := GetRoundedUpDown(BevSub * Ratio, AppDetails.DecimalPlaces);
      MT.BevTax := GetRoundedUpDown(BevTax * Ratio, AppDetails.DecimalPlaces);
      MT.BevTaxBefore := GetRoundedUpDown(BevTaxBefore * Ratio, AppDetails.DecimalPlaces);
      MT.BevDisc := GetRoundedUpDown(MT.BevSub + MT.BevTax - MT.BevValue, AppDetails.DecimalPlaces);
      MT.BevDiscBefore := GetRoundedUpDown(MT.BevSub + MT.BevTaxBefore - MT.BevValue, AppDetails.DecimalPlaces);
      PaymentTotal := PaymentTotal - MT.BevValue;


      if (PaymentTotal > MiscTotal) then
      begin
        Ratio := 1;
      end
      else if (MiscTotal > 0) then
      begin
        Ratio := (PaymentTotal / MiscTotal);
      end
      else
      begin
        Ratio := 0;
      end;

      MT.MiscValue := GetRoundedUpDown(MiscTotal * Ratio, AppDetails.DecimalPlaces);
      MT.MiscSub := GetRoundedUpDown(MiscSub * Ratio, AppDetails.DecimalPlaces);
      MT.MiscTax := GetRoundedUpDown(MiscTax * Ratio, AppDetails.DecimalPlaces);
      MT.MiscTaxBefore := GetRoundedUpDown(MiscTaxBefore * Ratio, AppDetails.DecimalPlaces);
      MT.MiscDisc := GetRoundedUpDown(MT.MiscSub + MT.MiscTax - MT.MiscValue, AppDetails.DecimalPlaces);
      MT.MiscDiscBefore := GetRoundedUpDown(MT.MiscSub + MT.MiscTaxBefore - MT.MiscValue, AppDetails.DecimalPlaces);
    end
    else if (AppDetails.Micros4700BreakdownType = 2) then
    begin
      MaxI := (AOLList.Count - 1);

      GLFoodTotal := 0;
      GLBevTotal := 0;
      FoodTotal := 0;
      BevTotal := 0;
      MiscTotal := 0;
      SoFarDiscount := 0;
      SoFarComboDiscount := 0.00;      

      for I := 0 to MaxI do begin
        OL := AOLList.Items[I];

        if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
        begin
          ILDiscount := 0;
        end
        else if OL.ComboID > 0 then
        begin
          if OL.LastComboItem then
          begin
            ILDiscount := FComboDiscount - SoFarComboDiscount;
          end
          else
          begin
            ILDiscount := OL.AllowedDiscount;
          end;

          ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
          SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
        end
        else
        begin
          //if it's the last line, allocate whatever hasn't been pro rata-ed
          if OL.LastNormalItem then
            ILDiscount := FDiscountAmount - SoFarDiscount
          else
          begin              //pro rata discount and accumulate discount pro rata-ed so far
            ILDiscount := OL.AllowedDiscount;
            ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
            SoFarDiscount := SoFarDiscount + ILDiscount;
            SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
          end;
        end;


        if (AppDetails.TaxExclusivePrices) then begin
          LineSub := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);
          LineTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineDisc := ILDiscount;
          LineTotal := LineSub + LineTax - LineDisc;
        end
        else begin
          LineTotal := (OL.ToPay - ILDiscount);
        end;

        if (OL.GLCode <> '') and (not OverrideIGGLCode) then begin
          OL.NetToPay := LineTotal;
          if (OL.FORB = 'F') then begin
            GLFoodTotal := GLFoodTotal + LineTotal;
          end
          else begin
            GLBevTotal := GLBevTotal + LineTotal;
          end;
        end
        else begin
          if (OL.FORB = 'F') then begin
            FoodTotal := FoodTotal + LineTotal;
          end
          else begin
            BevTotal := BevTotal + LineTotal;
          end;
        end;
      end;

      MaxI := (SurchargeList.Count - 1);
      for I := 0 to (MaxI) do begin
        SurchargeItem := SurchargeList.Items[I];

        if (AppDetails.TaxExclusivePrices) then
        begin
          LineSub := GetRoundedUpDown(SurchargeItem.Amount, AppDetails.DecimalPlaces);
          LineTax := GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineTotal := LineSub + LineTax;
        end
        else begin
          LineTotal := SurchargeItem.Amount;
        end;

        tmpItemGroup := GlobalMenuList.GetItemsItemGroup(SurchargeItem.ItemID);
        if (Assigned(tmpItemGroup)) then
          SurchargeItem.GLCode := tmpItemGroup.GLCode;

        if (SurchargeItem.GLCode <> '') then
        begin
          MT.AddMicros4700Category('M' + SurchargeItem.GLCode, LineTotal, True);
        end
        else
          MiscTotal := MiscTotal + LineTotal;
      end;


      if (not Assigned(TL)) and ((AppDetails.Micros4700ExtendedTender) or (AppDetails.PhoenixAllTransactions)) then
      begin
        MT.RoomNumber :=  AppDetails.Micros4700FolioID;
        MT.SubFolio := AppDetails.Micros4700SubFolio;
      end;

      AddMicros4700Values(AppDetails.Micros4700ExtendedTender);


      if (AppDetails.Micros4700ExtendedTender) then begin
        //Get the tips first
        MaxI := (TLList.Count - 1);
        for I := 0 to MaxI do begin
          TL := TLList.Items[I];
          if (TL.TLTypeID <> GetTLTID(TLTMicros4700)) and (TL.TLTypeID <> GetTLTID(TLTMicros4700Account)) then begin
            MT.TipValue := MT.TipValue + TL.TLTip;
          end;
        end;
        if (MT.TipValue<> 0) then begin
          MT.AddMicros4700Category(TipCat, MT.TipValue);
        end;

        for I := 0 to MaxI do begin
          TL := TLList.Items[I];
          if (TL.TLTypeID <> GetTLTID(TLTMicros4700)) and (TL.TLTypeID <> GetTLTID(TLTMicros4700Account)) then begin
            TenderLineType := GetTenderLineTypeRecord(TL.TLType);

            MT.AddMicros4700Category('T' + TenderLineType.LibGLCode, -(TL.TLPayment + TL.TLTip), True);
          end;
        end;
      end
      else if (MT.TipValue <> 0) then begin
        MT.AddMicros4700Category(TipCat, MT.TipValue);
      end;
    end;

    if (DoMicros4700Transaction(MT)) then
    begin
      with dm.sp do
      begin
        try
          SetStoredProcName('INSERT_INVOICEMICROS4700');
          ParamByName('invoiceid').AsInteger := MT.InvoiceID;
          BoolAsParam(ParamByName('bulkpost'), False);
          Execute;

        except
          on e: exception do begin
            dm.tr.Rollback;   //Jon 14-07-2003
            ShowMessage(e.message);
            Exit;
          end;
        end;
      end;
      Result := True;
    end;
  finally
    MT.Free;
  end;
end;
{******************************************************************************}
function TformPaymentTable.SendNZATransactions(AOLList: TList): Boolean;
var
  NZAFood, NZABev, NZATip: Currency;
  NZAFoodTax, NZABevTax, NZATipTax: Currency;
  TotalFood, TotalBev, NZAAmt: Currency;
  TotalFoodTax, TotalBevTax: Currency;
  NZAStockF, NZAStockB, NZAStockTip: string;
  ListCount: Integer;
  I: Integer;
  TL: TTL;
  OL: TOrderLine;
  SoFarDiscount, ILDiscount: Currency;
  NZAInvoiceData: TNZAInvoiceData;
  NZAInvoiceLineList: TList;

begin
  I := 0;
  ListCount := TLList.Count;
  while ((I < ListCount) and (TTL(TLList.Items[I]).TLTypeID <> GetTLTID(TLTNZAGold))) do begin
    Inc(I);
  end;
  if (I >= ListCount) then
  begin
    //This should never happen cos we should only come here if nzatransexist
    Result := False;
    Exit;
  end;
  TL := TLList.Items[I];

  NZAAmt := TL.TLPayment;
  NZATip := TL.TLTip;

  ListCount := AOLList.Count;

  TotalFood := 0;
  TotalBev := 0;
  TotalFoodTax := 0;
  TotalBevTax := 0;
  SoFarDiscount := 0;

  for I := 0 to (ListCount - 1) do begin
    OL := AOLList.Items[I];
    if (FDiscountAmount = 0) then begin
      ILDiscount := 0.00;
    end
    else if (I = (ListCount - 1)) then begin
      ILDiscount := FDiscountAmount - SoFarDiscount;
    end
    else begin
      //pro rata discount and accumulate discount pro rata-ed so far
      ILDiscount := ((((OL.ToPay/FToPay) * FDiscountAmount) * 100)) / 100;
      SoFarDiscount := SoFarDiscount + ILDiscount;
    end;
    ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
    SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);

    if (OL.ForB = 'F') then begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalFood := TotalFood + (((OL.ToPay - ILDiscount) * (100 + OL.SalesTaxPercent)) / 100);
        TotalFoodTax := GetRoundedUpDown(TotalFoodTax + (((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / 100), AppDetails.DecimalPlaces);
      end
      else begin
        TotalFood := TotalFood + OL.ToPay - ILDiscount;
        TotalFoodTax := GetRoundedUpDown(TotalFoodTax + (((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
      end;
    end
    else begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalBev := TotalBev + (((OL.ToPay - ILDiscount) * (100 + OL.SalesTaxPercent)) / 100);
        TotalBevTax := GetRoundedUpDown(TotalBevTax + (((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / 100), AppDetails.DecimalPlaces);
      end
      else begin
        TotalBev := TotalBev + OL.ToPay - ILDiscount;
        TotalBevTax := GetRoundedUpDown(TotalBevTax + (((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
      end;
    end;
  end;

  if (NZAAmt > TotalFood) then begin
    NZAFood := TotalFood;
    NZABev := NZAAmt - TotalFood;
  end
  else begin
    NZAFood := NZAAmt;
    NZABev := 0;
  end;

  NZAFoodTax := 0;
  NZABevTax := 0;
  NZATipTax := 0;    //Jon 03-02-2003
  if (TotalFood > 0) then begin
    NZAFoodTax := (NZAFood * TotalFoodTax) / TotalFood;
  end;
  if (TotalBev > 0) then begin
    NZABevTax := (NZABev * TotalBevTax) / TotalBev;
  end;

  try
    //We are already in a Transaction so another one does not need to be started
    with dm.sp do begin
      SetStoredProcName('GETSALECATEGORYINFO');

      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := CurrentDay;
      Execute;

      if (FieldByName('salecategoryid').IsNull) then begin
        ShowQuickMsg(sSaleCatNotFound);
        Result := False;
        Exit;
      end;

      NZAStockF := FieldByName('nzastockf').AsString;
      NZAStockB := FieldByName('nzastockb').AsString;
      NZAStockTip := FieldByName('nzastocktip').AsString;

      if ((FieldByName('tpnzastockf').AsString <> '') and
      (FieldByName('tpnzastockb').AsString <> '')) then begin
        NZAStockF := FieldByName('tpnzastockf').AsString;
        NZAStockB := FieldByName('tpnzastockb').AsString;
      end;
    end;
  except
    on e: exception do begin
      Result := False;
      ShowMessage(e.message);
      Exit;
    end;
  end;

//  NZADesc := AppDetails.TerminalName + ' Ref# ' + IntToStr(WBInvoices.RefreshID);    Jon 03-10-2003

  if (((NZAFood > 0) and (NZAStockF = '')) or
      ((NZABev > 0) and (NZAStockB = '')) or
      ((NZATip > 0) and (NZAStockTip = ''))) then begin
    ShowQuickMsg(sNoStockCodeProvided);
    Result := False;
    Exit;
  end;

  NZAInvoiceData.Account := NZAAccount;
  NZAInvoiceData.Date := Now;
  NZAInvoiceData.GSTInc := True;
  NZAInvoiceData.IntRef := IntToStr(WBInvoices.RefreshID);

  if (AppDetails.NZATaxExclusive) then begin    //Jon 21-02-2003
    NZAFood := NZAFood - NZAFoodTax;
    NZABev := NZABev - NZABevTax;
    NZATip := NZATip - NZATipTax;
  end;

  NZAInvoiceLineList := TList.Create;

  if (AppDetails.NZAOneInvoiceLine) then begin
    NZAInvoiceLineList.Add(NewPNZAInvoiceLineData(NZAStockF, NZAFood + NZABev, NZAFoodTax + NZABevTax, 'I'));
  end
  else begin
    if (NZAFood > 0) then begin
      NZAInvoiceLineList.Add(NewPNZAInvoiceLineData(NZAStockF, NZAFood, NZAFoodTax, 'I'));
    end;
    if (NZABev > 0) then begin
      NZAInvoiceLineList.Add(NewPNZAInvoiceLineData(NZAStockB, NZABev, NZABevTax, 'I'));
    end;
  end;
  if (NZATip > 0) then begin
    NZAInvoiceLineList.Add(NewPNZAInvoiceLineData(NZAStockTip, NZATip, NZATipTax, 'I'));
  end;

  Result := DoNZATransactions(NZAInvoiceData, NZAInvoiceLineList);
  ClearNZAInvoiceLineDataList(NZAInvoiceLineList);
  NZAInvoiceLineList.Free;
end;
{******************************************************************************}
function TformPaymentTable.SendEvolutionTransactions(AOLList: TList): Boolean;
var
  iLineNo: Integer;

  varTL: TTL;
  iTLIDx: Integer;
  eTotTLAmt: Extended; //total tendered
  eEvoTLAmt: Extended; //total tendered to Evo
  eEvoTLTip: Extended; //total tip to Evo
  eEvoTLProp: Extended; //proportion of Evo tender to Total tender

  varOL: TOrderLine;
  iOLIDx: Integer;
  eILQty: Extended;  //proportional line qty
  eILDisc: Extended; //proportional line disc
  eILAmt: Extended;  //proportional line amount
  eILDiscPerc: Extended;

  varSC: TSurchargeItem;
  iSCIDx: Integer;

  eITDisc_Food: Extended;    //disc total for food
  eITAmt_Food: Extended;     //amount total for food
  eITDisc_Bev: Extended;     //disc total for bev
  eITAmt_Bev: Extended;      //amount total for bev
  eITDisc_SC: Extended;      //surcharge disc total
  eITDisc_SC_Food: Extended; //surcharge disc food total
  eITDisc_SC_Bev: Extended;  //surcharge disc bev total
  eITAmt_SC: Extended;       //surcharge amount total
  eITAmt_SC_Food: Extended;  //surcharge amount food total
  eITAmt_SC_Bev: Extended;   //surcharge amount bev total
  eEvoFoodProp: Extended; //proportion of Food total to Food+Bev Total (pre-surcharge)

  NZAStockF, NZAStockB, NZAStockTip: string;

  aInvoiceDescr: string;
  iSaleCategoryPL, iInvoicePL: Integer;
  aCodesNotFound: string;
  varEvoItemF: PGlbEvoSCItem;
  varEvoItemB: PGlbEvoSCItem;
  varEvoItemTip: PGlbEvoSCItem;
begin
  ClearEvoInvoice;

  //get sale category information (sale category, price level, stock codes)
  try
    //We are already in a Transaction so another one does not need to be started
    with dm.sp do begin
      SetStoredProcName('GETSALECATEGORYINFO');    //getsalecategoryinfo
      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := CurrentDay;
      Execute;

      if (FieldByName('salecategoryid').IsNull) then begin
        ShowQuickMsg(sSaleCatNotFound);
        Result := False;
        Exit;
      end;

      NZAStockF := FieldByName('nzastockf').AsString;
      NZAStockB := FieldByName('nzastockb').AsString;
      NZAStockTip := FieldByName('nzastocktip').AsString;

      aInvoiceDescr := FieldByName('salecategory').AsString; //varchar(15)
      iSaleCategoryPL := 0; //not specified

      if ((FieldByName('tpnzastockf').AsString <> '') and
      (FieldByName('tpnzastockb').AsString <> '')) then begin
        NZAStockF := FieldByName('tpnzastockf').AsString;
        NZAStockB := FieldByName('tpnzastockb').AsString;
      end;
    end;
  except
    on e: exception do begin
      Result := False;
      ShowMessage(e.message);
      Exit;
    end;
  end;

  //if in a sale category period, add period's custom price level name to invoice description
  if ((iSaleCategoryPL <> 0) and (AppDetails.PriceLevelName[iSaleCategoryPL] <> '')) then begin
    if (aInvoiceDescr <> '') then begin
      aInvoiceDescr := aInvoiceDescr + ' ';
    end;
    aInvoiceDescr := aInvoiceDescr + AppDetails.PriceLevelName[iSaleCategoryPL]; //varchar(12)
  end;

  aInvoiceDescr := aInvoiceDescr + '  #' + IntToStr(WBInvoices.RefreshID); //varchar(15)

  //calc total Evolution tender amount/tip, loop just in case more than one Evo tender line (should not be)
  eTotTLAmt := 0;
  eEvoTLAmt := 0;
  eEvoTLTip := 0;
  for iTLIDx := 0 to Pred(TLList.Count) do begin
    varTL := TLList.Items[iTLIDx];
    eTotTLAmt := eTotTLAmt + varTL.TLPayment;

    if (varTL.TLTypeID = GetTLTID(TLTEvolution)) then begin
      eEvoTLAmt := eEvoTLAmt + varTL.TLPayment;
      eEvoTLTip := eEvoTLTip + varTL.TLTip;
    end;
  end;
  eEvoTLProp := eEvoTLAmt / eTotTLAmt;

  //check Evo F/B/Tip stock codes specified
  if (((not AppDetails.EvolutionItemisedInvoicing) and ((NZAStockF = '') or (NZAStockB = ''))) or
      ((eEvoTLTip <> 0) and (NZAStockTip = ''))) then begin
    ShowQuickMsg(sNoStockCodeProvided);
    Result := False;
    Exit;
  end;

  aCodesNotFound := '';
  varEvoItemF := nil;
  varEvoItemB := nil;
  varEvoItemTip := nil;

  //get Evo F/B stock item information, if not using itemised invoicing
  if (not AppDetails.EvolutionItemisedInvoicing) then begin
    if (not GlbEvoSCItems.FindEvoSCItem(NZAStockF, varEvoItemF)) then begin
      aCodesNotFound := aCodesNotFound + NZAStockF + ', ';
    end;
    if (not GlbEvoSCItems.FindEvoSCItem(NZAStockB, varEvoItemB)) then begin
      aCodesNotFound := aCodesNotFound + NZAStockB + ', ';
    end;
  end;

  //get Evo Tip stock item information, if need to save a tip invoice line
  if (eEvoTLTip <> 0) then begin
    if (not GlbEvoSCItems.FindEvoSCItem(NZAStockTip, varEvoItemTip)) then begin
      aCodesNotFound := aCodesNotFound + NZAStockTip + ', ';
    end;
  end;

  //if any of the needed stock codes are not found in Evolution, then error and exit
  if (aCodesNotFound <> '') then begin
    Delete(aCodesNotFound, Length(aCodesNotFound) - 1, 2); //remove last comma and space
    ShowQuickMsg(Format(sStockCodeNotFoundInEvol, [aCodesNotFound]));
    Result := False;
    Exit;
  end;

  //initialise invoice disc/amount F/B totals
  iLineNo := 0;
  eITDisc_Food := 0;
  eITAmt_Food := 0;
  eITDisc_Bev := 0;
  eITAmt_Bev := 0;
  eITDisc_SC := 0;
  eITAmt_SC := 0;

  //go through OLList (order lines being invoiced), add itemised Evo invoice lines or increment totals
  for iOLIDx := 0 to Pred(AOLList.Count) do begin
    varOL := TOrderLine(AOLList[iOLIDx]);
    if (varOL.ItemID <> 0) then begin    //don't do modifiers yet
      //work out proportional invoice amounts, based on portion of tender being transfered to Evo
      eILDisc := varOL.ILDiscount * eEvoTLProp;
      eILAmt := varOL.ILAmount * eEvoTLProp;

      //add itemised invoice line
      if (AppDetails.EvolutionItemisedInvoicing) then begin
        eILQty := varOL.Qty * eEvoTLProp;
        eILDiscPerc := GetRoundedUpDown((eILDisc / eILAmt) * 100, 4);

        Inc(iLineNo);
        AddToEvoInv(iLineNo, varOL.ItemString, varOL.UnitPrice, varOL.OLPriceLevel,
          eILDiscPerc, varOL.SalesTaxPercent, eILQty, eILDisc, eILAmt,
          EvoGetStockLink(eitItem, varOL.ItemID), GlbEvoItemDef.iINVTaxTypeID);
      end
      else begin      //increment F/B totals
        if (varOL.ForB = 'F') then begin
          eITDisc_Food := eITDisc_Food + eILDisc;
          eITAmt_Food := eITAmt_Food + eILAmt;
        end
        else begin    //varOL.ForB = 'B'
          eITDisc_Bev := eITDisc_Bev + eILDisc;
          eITAmt_Bev := eITAmt_Bev + eILAmt;
        end;
      end;
    end;
  end;

  //go through each surcharge item, add itemised Evo invoice lines or increment totals
  for iSCIDx := 0 to Pred(SurchargeList.Count) do begin
    varSC := TSurchargeItem(SurchargeList.Items[iSCIDx]);
    if (varSC.Amount <> 0) then begin
      //work out proportional invoice amounts, based on portion of tender being transfered to Evo
      eILDisc := 0 * eEvoTLProp;
      eILAmt := varSC.Amount * eEvoTLProp;

      //add itemised invoice line
      if (AppDetails.EvolutionItemisedInvoicing) then begin
        eILQty := 1 * eEvoTLProp;

        Inc(iLineNo);
        AddToEvoInv(iLineNo, varSC.ItemAbbrev, varSC.Amount, 1,
          0, varSC.SalesTaxPercent, eILQty, eILDisc, eILAmt,
          EvoGetStockLink(eitItem, varSC.ItemID), GlbEvoItemDef.iINVTaxTypeID);
      end
      else begin    //increment surcharge totals
        eITDisc_SC := eITDisc_SC + eILDisc;
        eITAmt_SC := eITAmt_SC + eILAmt;
      end;
    end;
  end;

  //if not itemising invoices, add F/B total invoice lines
  if (not AppDetails.EvolutionItemisedInvoicing) then begin
    //set invoice price level
    if (iSaleCategoryPL <> 0) then begin
      iInvoicePL := iSaleCategoryPL;
    end
    else begin
      iInvoicePL := 1; //default price level
    end;

    //add surcharge total onto F/B totals pro-rata
    if (eITAmt_SC <> 0) then begin
      if ((eITAmt_Food + eITAmt_Bev) <> 0) then begin
        eEvoFoodProp := eITAmt_Food / (eITAmt_Food + eITAmt_Bev);
      end
      else begin
        eEvoFoodProp := 1; //avoid divide by zero, should never happen
      end;

      //split surcharge discount total into F/B amounts
      eITDisc_SC_Food := GetRoundedUpDown(eITDisc_SC * eEvoFoodProp, 4);
      eITDisc_SC_Bev := eITDisc_SC - eITDisc_SC_Food;

      //split surcharge amount total into F/B amounts
      eITAmt_SC_Food := GetRoundedUpDown(eITAmt_SC * eEvoFoodProp, 4);
      eITAmt_SC_Bev := eITAmt_SC - eITAmt_SC_Food;

      //add split surcharge F/B amounts to F/B totals
      eITDisc_Food := eITDisc_Food + eITDisc_SC_Food;
      eITAmt_Food := eITAmt_Food + eITAmt_SC_Food;
      eITDisc_Bev := eITDisc_Bev + eITDisc_SC_Bev;
      eITAmt_Bev := eITAmt_Bev + eITAmt_SC_Bev;
    end;

    //add food/beverage Evo invoice lines
    if (eITAmt_Food <> 0) then begin
      eILQty := 1;
      eILDiscPerc := GetRoundedUpDown((eITDisc_Food / eITAmt_Food) * 100, 4);

      Inc(iLineNo);
      AddToEvoInv(iLineNo, varEvoItemF.Description_1, eITAmt_Food, iInvoicePL,
        eILDiscPerc, varEvoItemF.TaxRate, eILQty, eITDisc_Food, eITAmt_Food,
        varEvoItemF.StockLink, varEvoItemF.idTaxRate);
    end;
    if (eITAmt_Bev <> 0) then begin
      eILQty := 1;
      eILDiscPerc := GetRoundedUpDown((eITDisc_Bev / eITAmt_Bev) * 100, 4);

      Inc(iLineNo);
      AddToEvoInv(iLineNo, varEvoItemB.Description_1, eITAmt_Bev, iInvoicePL,
        eILDiscPerc, varEvoItemB.TaxRate, eILQty, eITDisc_Bev, eITAmt_Bev,
        varEvoItemB.StockLink, varEvoItemB.idTaxRate);
    end;
  end;

  //insert any tip invoice line
  if (eEvoTLTip <> 0) then begin
    eILQty := 1;
    eILDisc := 0;
    eILAmt := eEvoTLTip;

    Inc(iLineNo);
    AddToEvoInv(iLineNo, varEvoItemTip.Description_1, eEvoTLTip, 1,
      0, varEvoItemTip.TaxRate, eILQty, eILDisc, eILAmt,
      varEvoItemTip.StockLink, varEvoItemTip.idTaxRate);
  end;

  SetEvoInvHdr(GlbTable.Guests, WBInvoices.RefreshID, aInvoiceDescr);
  Result := EvoSaveInvoice;
end;
{******************************************************************************}
function TformPaymentTable.SendICRTouchTransactions(AOLList: TList): Boolean;
var
  TL: TTL;
  OL: TOrderLine;
  lICRPosting: TICRTouchPosting;
  lICRItem: TICRTouchSaleItem;
  lOLList: TList;
  I: Integer;
  lTenderAvailable: Currency;
  lRatio: Double;
  lGroup: Integer;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  LineValue, LineSub, LineTax, LineDisc, LineTotal: Currency;

  function SortOLList(Item1, Item2: Pointer): Integer;
  var
    lG1, lG2: Integer;
    OL1, OL2: TOrderLine;

  begin
    OL1 := TOrderLine(Item1);
    OL2 := TOrderLine(Item2);
    lG1 := StrToIntDef(OL1.GLCode, 0);
    lG2 := StrToIntDef(OL2.GLCode, 0);
    if (lG1 > lG2) then
    begin
      Result := 1;
    end
    else if (lG1 < lG2) then
    begin
      Result := -1;
    end
    else if ((OL1.ForB = 'B') and (OL2.ForB = 'F')) then
    begin
      Result := 1;
    end
    else if ((OL1.ForB = 'F') and (OL2.ForB = 'B')) then
    begin
      Result := -1;
    end
    else if (OL1.OrderLineID > OL2.OrderLineID) then
    begin
      Result := 1;
    end
    else if (OL1.OrderLineID < OL2.OrderLineID) then
    begin
      Result := -1;
    end
    else
    begin
      Result := 0;
    end;
  end;

begin
  Result := False;
  lICRPosting := TICRTouchPosting.Create;
  lOLList := TList.Create;
  try
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTRoomCharge));
    if (not Assigned(TL)) then
    begin
      //This should never happen cos we should only come here if there is a room charge
      Result := False;
      Exit;
    end;
    lTenderAvailable := TL.TLPayment;

    for I := 0 to AOLList.Count - 1 do
    begin
      OL := AOLList.Items[I];
      if (OL.ItemID > 0) then
      begin
        lOLList.Add(OL);
      end;
    end;

    SoFarDiscount := 0;
    SoFarComboDiscount := 0.00;

    lOLList.Sort(@SortOLList);

    lICRPosting.TerminalID := AppDetails.TerminalID;
    lICRPosting.TerminalName := AppDetails.TerminalName;
    lICRPosting.RoomNumber := GlbICRTouchFolio.RoomNumber;
    lICRPosting.FolioNumber := GlbICRTouchFolio.FolioNumber;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.sp do
      begin
        SetStoredProcName('GETSALECATEGORYINFO');

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := Currentday;
        Execute;

        if (FieldByName('salecategoryid').IsNull) then
        begin
          dm.tr.Rollback;   //Jon 14-07-2003
          ShowQuickMsg(sSaleCatNotFound);
          Exit;
        end;

        lICRPosting.ShiftNumber := FieldAsInt(FieldByName('fidservingtime'));
        if ((FieldAsInt(FieldByName('tpfidservingtime'))) > 0) then begin
          lICRPosting.ShiftNumber := FieldAsInt(FieldByName('tpfidservingtime'));
        end;
      end;
    except
      on e: exception do
      begin
        dm.tr.Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;

    lICRPosting.ConseqNumber := WBInvoices.RefreshID;
    lICRPosting.CheckNumber := WBInvoices.RefreshID;
    lICRPosting.TabNumber := GlbTable.TableNo;
    lICRPosting.Covers := GlbTable.Guests;        
    lICRPosting.PriceLevel := GlbTable.PriceLevel;
    lICRPosting.StaffID := GlbLogin.StaffID;
    lICRPosting.Tip := TL.TLTip;
    if (GlbAccount.AccountID > 0) then
    begin
      lICRPosting.AccountName := GlbAccount.AccountName;
    end;

    //Add the items until we're out of tender
    for I := 0 to lOLList.Count - 1 do
    begin
      if (lTenderAvailable > 0) then
      begin
        OL := lOLList.Items[I];

        lGroup := StrToIntDef(OL.GLCode, 0);
        if (not(lGroup in [1..CN_ICRTotalsArraySize])) then
        begin
          if (OL.ForB = 'F') then
          begin
            lGroup := 2;
          end
          else
          begin
            lGroup := 1;
          end;
        end;

        if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
        begin
          ILDiscount := 0;
        end
        else if (OL.ComboID > 0) then
        begin
          {if OL.LastComboItem then
          begin
            ILDiscount := FComboDiscount - SoFarComboDiscount
          end
          else
          begin}
            ILDiscount := OL.AllowedDiscount;
          {end;}

          ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
          SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
        end
        else
        begin
          //if it's the last line, allocate whatever hasn't been pro rata-ed
          {if OL.LastNormalItem then
            ILDiscount := FDiscountAmount - SoFarDiscount
          else
          begin              //pro rata discount and accumulate discount pro rata-ed so far}
            ILDiscount := OL.AllowedDiscount;
            ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
            SoFarDiscount := SoFarDiscount + ILDiscount;
            SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
          {end;}
        end;

        if (AppDetails.TaxExclusivePrices) then
        begin
          LineValue := GetRoundedUpDown((OL.ToPay * (100 + OL.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
          LineSub := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);
          LineTax := GetRoundedUpDown((OL.ToPay - ILDiscount) * OL.SalesTaxPercent / 100, AppDetails.DecimalPlaces);
          LineDisc := ((ILDiscount * (100 + OL.SalesTaxPercent)) / 100);
          LineTotal := LineSub + LineTax - LineDisc;
        end
        else
        begin
          LineValue := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);
          //LineSub := GetRoundedUpDown((OL.ToPay * 100) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
          LineTotal := (OL.ToPay - ILDiscount);
          LineTax := GetRoundedUpDown(LineTotal * ((OL.SalesTaxPercent) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
          LineDisc := ILDiscount;
        end;

        if (LineTotal <= lTenderAvailable) then
        begin
          lRatio := 1;
        end
        else
        begin
          lRatio := lTenderAvailable / LineTotal;
        end;


        lICRItem := TICRTouchSaleItem.Create;
        lICRItem.PLUNumber := OL.ItemID;
        lICRItem.Quantity := OL.Qty * lRatio;
        lICRItem.Value := LineValue * lRatio;
        lICRItem.ItemText := OL.ItemString;
        lICRItem.PLUDepartment := OL.ItemGroupID;   //?
        lICRItem.PLUGroup := lGroup;
        //lICRItem.StatusFlag := 0;                   //?
        if (OL.SalesTaxPercent = 0) then
        begin
          lICRItem.TaxRate := 0;
        end
        else if (OL.SalesTaxPercent = 9) then  //Because I'm a lazy bastard. This should probably have sort of lookup on the itemgroups to determine the TaxRate ID.
        begin
          lICRItem.TaxRate := 2;
        end
        else
        begin
          lICRItem.TaxRate := 1;
        end;
        lICRPosting.AddSaleItem(lICRItem);
        lICRPosting.SaleTotal := lICRPosting.SaleTotal + LineTotal * lRatio;
        lICRPosting.Sub[lGroup] := lICRPosting.Sub[lGroup] + LineTotal * lRatio;
        lICRPosting.Tax[lGroup] := lICRPosting.Tax[lGroup] + LineTax * lRatio;
        lICRPosting.DiscountTotal := lICRPosting.DiscountTotal + LineDisc * lRatio;
        lTenderAvailable := lTenderAvailable - LineTotal * lRatio;
      end;
    end;

    Result := lICRPosting.SavePosting(AppDetails.ICRTouchPostingsPath);
    if (AppDetails.ICRTouchPostingsLogPath <> '') then
    begin
      lICRPosting.SavePosting(AppDetails.ICRTouchPostingsLogPath);
    end;
  finally
    lICRPosting.Free;
    lOLList.Free;
  end;
end;
{******************************************************************************}
function TformPaymentTable.SendMewsTransactions(AOLList: TList; SPNumber, TheTenderID: Integer): Boolean;
var
  TL: TTL;
  I: Integer;
  TransactionList: TMewsTransactionList;
  Transaction: TMewsTransaction;
  MewsItem: TMewsItem;
  MewsPayment: TMewsPayment;
  OL: TOrderLine;
  TenderLineType: TTenderLineType;
  TipCat, OutletId, ServiceId: string;
  TimePeriodID: Integer;
  CustomerId, DisplayName, RoomNumber: string;
begin
  Transaction := nil;
  TransactionList :=  TMewsTransactionList.Create;
  Result := False;
  try
  //Build the room transfer first
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTMews));
    if ((not Assigned(TL)) and (not (AppDetails.MewsAddOutletBills))) then
    begin
     //This should never happen cos we should only come here if MicrosTransExist
      Exit;
    end;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.GetSP(SPNumber) do
      begin
        SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        Execute;

        TipCat := FieldAsString(FieldByName('libgltip'));
        TimePeriodID := FieldAsInt(FieldByName('timeperiodid'));

        SetStoredProcName('GETSALECATEGORYMEWS', SPNumber);
        ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
        Execute;

        ServiceId := FieldAsString(FieldByName('serviceguid'));
        OutletId := FieldAsString(FieldByName('outletguid'));

        if (TimePeriodID > 0) then
        begin
          SetStoredProcName('GETTIMEPERIODMEWS', SPNumber);
          ParamByName('timeperiodid').AsInteger := TimePeriodID;
          Execute;

          ServiceId := FieldAsString(FieldByName('serviceguid'), ServiceId);
          OutletId := FieldAsString(FieldByName('outletguid'), OutletId);
        end;
      end;
    except
      on e: exception do begin
        dm.tr.Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;


    if Assigned(TL) then
    begin
      Transaction := TransactionList.NewMewsTransaction;
      Transaction.CustomerId := GlbMewsAccount.CustomerId;
      Transaction.ServiceId := ServiceId;
      Transaction.InvoiceID := WBInvoices.RefreshID;
      CustomerId := GlbMewsAccount.CustomerId;
      DisplayName := GlbMewsAccount.DisplayName;
      RoomNumber := GlbMewsAccount.RoomNumber;
    end
    else if (AppDetails.MewsAddOutletBills) then
    begin
      Transaction := TransactionList.NewMewsTransaction;
      Transaction.ServiceId := ServiceId;
      Transaction.OutletId := OutletId;
      Transaction.InvoiceID := WBInvoices.RefreshID;
      CustomerId := '';
      DisplayName := '';
      RoomNumber := '';
    end;

    if (Assigned(Transaction)) then
    begin
      for I:= 0 to (AOLList.Count -1) do
      begin
        OL := AOLList.Items[I];
        MewsItem := Transaction.Items.NewMewsItem;
        MewsItem.Name := OL.ItemString;
        MewsItem.UnitCount := ceil(OL.Qty);
        MewsItem.GrossValue := (OL.ToPay - OL.ILDiscount);
        MewsItem.TaxCode := OL.GLCode2;
        MewsItem.AccountingCategoryId := OL.GLCode;
      end;

      if Assigned(TL) then
      begin
        if (TL.TLTip > 0) then
        begin
          MewsItem := Transaction.Items.NewMewsItem;
          MewsItem.Name := 'Tip';
          MewsItem.UnitCount := 1;
          MewsItem.GrossValue := TL.TLTip;
          MewsItem.TaxCode := AppDetails.MewsTaxCode;
          MewsItem.AccountingCategoryId := TipCat;
        end;
      end;

      for I:= 0 to (TLList.Count - 1) do
      begin
        TL := TLList.Items[I];
        if (TL.TLTypeID <> GetTLTID(TLTMews)) then
        begin
          if (TL.TLTip > 0) then
          begin
            MewsItem := Transaction.Items.NewMewsItem;
            MewsItem.Name := 'Tip';
            MewsItem.UnitCount := 1;
            MewsItem.GrossValue := TL.TLTip;
            MewsItem.TaxCode := AppDetails.MewsTaxCode;
            MewsItem.AccountingCategoryId := TipCat;
          end;

          TenderLineType := GetTenderLineTypeRecord(TL.TLType);
          MewsPayment := Transaction.Payments.NewMewsPayment;
          MewsPayment.Name := TL.TLTypeName;
          MewsPayment.GrossValue := TL.TLTender;
          MewsPayment.AccountingCategoryId := TenderLineType.LibGLCode;
        end;
      end;
    end;

    Result := DoMewsTransactionList(TransactionList);

    Transaction := TransactionList.GetMewsTransaction(0);
    if Assigned(Transaction) then
    begin
      try
        with dm.GetSP(SPNumber) do
        begin
          SetStoredProcName('EDIT_TENDERMEWS', SPNumber);
          ParamByName('tenderid').AsInteger := TheTenderID;
          StringAsParam(ParamByName('billguid'), Transaction.OrderId);
          StringAsParam(ParamByName('customerguid'), CustomerId);
          StringAsParam(ParamByName('displayname'), DisplayName);
          StringAsParam(ParamByName('roomnumber'), RoomNumber);
          ExecProc;
        end;
      except
        on e: exception do
        begin
          Result := False;
          dm.tr.Rollback;   //Jon 14-07-2003
          ShowMessage(e.message);
          Exit;
        end;
      end;
    end;

  finally
    TransactionList.Free;
  end;
end;
{*******************************************************************************
Creates and send the details of the invoice to the Table Management interface.

Change history
#00445# 10/10/2012 JBB Created procedure.
*******************************************************************************}
procedure TformPaymentTable.SendTableManagementTransactions(aOLList: TList);
var
  ListCount: Integer;
  I: Integer;
  OL: TOrderLine;
  TL: TTL;
  SurchargeItem: TSurchargeItem;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  TabManILList: TTableManagementILList;
  IL: TTableManagementIL;
begin
  TabManILList := TTableManagementILList.Create;
  try
    ListCount := aOLList.Count;
    SoFarDiscount := 0;
    SoFarComboDiscount := 0;
    IL := nil;
    for I := 0 to (ListCount - 1) do begin
      OL := aOLList.Items[I];

      if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
      begin
        ILDiscount := 0;
      end
      else if OL.ComboID > 0 then
      begin
        if OL.LastComboItem then
        begin
          ILDiscount := (FComboDiscount - SoFarComboDiscount);
        end
        else
        begin
          ILDiscount := OL.AllowedDiscount;
        end;

        ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
        SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
        SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
      end
      else
      begin
        //if it's the last line, allocate whatever hasn't been pro rata-ed
        if OL.LastNormalItem then
          ILDiscount := FDiscountAmount - SoFarDiscount
        else
        begin              //pro rata discount and accumulate discount pro rata-ed so far
          ILDiscount := OL.AllowedDiscount;
          ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          SoFarDiscount := SoFarDiscount + ILDiscount;
          SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
        end;
      end;


      ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
      SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);

      IL := TabManILList.AddIL;
      IL.ItemID := OL.ItemID;
      IL.ItemName := OL.ItemString;
      IL.Qty := OL.Qty;
      IL.ForB := OL.ForB;
      IL.UnitPrice := OL.UnitPrice;
      IL.Discount := ILDiscount;

      if (AppDetails.TaxExclusivePrices) then begin
        IL.Value := (((OL.ToPay - ILDiscount) * (100 + OL.SalesTaxPercent)) / 100);
        IL.SalesTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
      end
      else
      begin
        IL.Value := OL.ToPay - ILDiscount;
        IL.SalesTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * (OL.SalesTaxPercent)) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
      end;

      if (OL.UnitPrice <> 0) then
        IL.ItemCount := Trunc(OL.QtyLeft) - Trunc(OL.QtyLeft + 0.0000001 - (OL.ToPay / OL.UnitPrice))
      else
        IL.ItemCount := Round(OL.Qty);
    end;

    ListCount := (SurchargeList.Count - 1);
    for I := 0 to ListCount do begin
      SurchargeItem := SurchargeList.Items[I];

      IL := TabManILList.AddIL;
      IL.ItemID := SurchargeItem.ItemID;
      IL.ItemName := SurchargeItem.ItemAbbrev;
      IL.ForB := 'M';
      IL.Discount := 0;
      IL.ItemCount := 1;
      IL.Qty := 1;
      IL.UnitPrice := SurchargeItem.Amount;
      if (AppDetails.TaxExclusivePrices) then
      begin
        IL.Value := (SurchargeItem.Amount * (100 + SurchargeItem.SalesTaxPercent) / 100);
        IL.SalesTax := ((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100);
      end
      else
      begin
        IL.Value := SurchargeItem.Amount;
        IL.SalesTax := GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent), AppDetails.DecimalPlaces);
      end;
    end;

    ListCount := (TLList.Count - 1);
    for I := 0 to ListCount do
    begin
      TL := TLList.Items[I];
      if (TL.TLTip > 0) then
      begin
        if ((not Assigned(IL)) or (IL.ItemName <> 'Tip')) then
        begin
          IL := TabManILList.AddIL;
          IL.ItemID := 0;
          IL.ItemName := 'Tip';
          IL.ForB := 'T';
          IL.Discount := 0;
          IL.ItemCount := 1;
          IL.Qty := 1;
          IL.UnitPrice := IL.UnitPrice + TL.TLTip;
          IL.SalesTax := 0;
        end;
        IL.Value := IL.Value + TL.TLTip;
      end;
    end;

    STableManagementIntegration.CreateInvoice(TabManILList);
  finally
    TabManILList.Free;
  end;
end;
{*******************************************************************************}
function TformPaymentTable.SendTaxCoreTransactions(AOLList: TList): Boolean;
var
  I: Integer;
  OL: TOrderLine;
  SurchargeItem: TSurchargeItem;
  tmpItemGroup: TItemGroup;
begin
  with TaxCoreTransactionData do
  begin
    if (AppDetails.FiscalTaxCoreTrainingMode) then
      IT := tcitTraining
    else
      IT := tcitNormal;

    TT := tcttSale;
    if (TLList.Count > 0) then
      PaymentType := GetTenderLineTypeRecord(TLList.Items[0].TLType).TaxCorePaymentType
    else
      PaymentType := tcptother;

    Cashier := GlbLogin.StaffName;
    InvoiceNumber := WBInvoices.RefreshID;
    ReferentDocumentNumber := '';

    ItemList.Clear;
    for I := 0 to (aOLList.Count - 1) do
    begin
      OL := aOLList.Items[I];
      with ItemList.NewTaxCoreItem do
      begin
        Name := OL.ItemString;
        GTIN := OL.GTIN;

        Quantity := OL.Qty;
        UnitPrice := OL.UnitPrice;
        TotalAmount := OL.ILAmount - OL.ILDiscount;
        Labels.CommaText := OL.TaxCoreLabel;
      end;
    end;

    for I := 0 to (SurchargeList.Count -1) do
    begin
      SurchargeItem := SurchargeList.Items[I];
      with ItemList.NewTaxCoreItem do
      begin
        Name := SurchargeItem.ItemAbbrev;
        GTIN := '';

        Quantity := 1;
        UnitPrice := SurchargeItem.Amount;
        TotalAmount := SurchargeItem.Amount;
        tmpItemGroup := GlobalMenuList.GetItemsItemGroup(SurchargeItem.ItemID);
        if (Assigned(tmpItemGroup)) then
          Labels.CommaText := tmpItemGroup.TaxCoreLabel;
      end;
    end;
  end;
  Result := DoTaxCoreSignInvoice;
end;
{*******************************************************************************
Callback from when the OK button is pressed
*******************************************************************************}
procedure TformPaymentTable.cmdOkClick(Sender: TObject);
var
  I: Integer;
  ConfirmationResult: TModalResult;
  TL: TTL;
  SaveInvoice: Boolean;
  tmpList: TList;
  fAmount: Currency;
  aCombo: string;
  aComboIndex: Integer;
  tmpOL: TOrderLine;
  tmpPaymentOnSelectedItems: Boolean;
  tmpAmountSplittingStarted: Boolean;
begin
  LogMessage(ltPaymentTrace, '39' + #9);
  tmpPaymentOnSelectedItems := False;
  tmpAmountSplittingStarted := False;

  if (DoingTablePayment) then
  begin
    LogMessage(ltPaymentTrace, '40' + #9);
    Exit;
  end;

  if (GlbEvent.EventID > 0) then
  begin
    SetGlbEvent(GlbEvent.EventID, True);

    if ((GlbEvent.Available - FHostSubsidy + FDiscountAmount) < 0) then
    begin
      VisibleWindow := vwTender;
      ShowMsg(sEventSpendLimitReached);
      txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available - FHostSubsidy + FDiscountAmount);
      Exit;
    end;
  end;

  if not ValidateGoodySale then
    Exit;

  if NFManual and (not NFEMode) then
  begin
    if not GetNFManualExtraDetails then
      Exit;
  end;

  LogMessage(ltPaymentTrace, '41' + #9);
  try
    DoingTablePayment := True;

    //if any negative amounts then ask for pinno
    if ((TLList.Count > 0) and (not (ManagerAuthorise.Authorise and ManagerAuthorise.AuthorisePriv6))) then
    begin
      LogMessage(ltPaymentTrace, '54' + #9);
      for I := 0 to (TLList.Count - 1) do begin
        case TTL(TLList.Items[I]).TLType of
          TLTVoucher, TLTOther, TLTGuestHosp, TLTStaffHosp, TLTManagerHosp: begin
            //check if have privilege
            if (not (VerifyPriv(6))) then begin
              ShowMsg(sCannotCompleteTabPayGetHelp);
              LogMessage(ltPaymentTrace, '42' + #9);
              Exit;
            end;

            //verify pinno
            if (not GlbPrivs[6].AvoidPINCheck) then begin
              if (not VerifyPinNo) then begin
                LogMessage(ltPaymentTrace, '43' + #9);
                Exit;
              end;
            end;

            LogMessage(ltPaymentTrace, '55' + #9);
            Break;
          end;
        end;
      end;
    end;

    LogMessage(ltPaymentTrace, '44' + #9);
    if (GlbLogin.TillID = 0) then begin
      ShowQuickMsg(sLogonToTill);
      Exit;
    end;
    if (FStillDue < -0.01) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sPayOverBy, [FormatCurrencySign(Abs(FStillDue))]));
      Exit;
    end
    else if ((FStillDue > 0.01) and (AppDetails.OneBill) and (not OnAccount)) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sOneBillPayShortBy, [FormatCurrencySign(Abs(FStillDue))]));
      Exit;
    end
    else if ((FStillDue > 0.01) and (not AppDetails.OneBill) and ((not OnAccount))) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sPayShortBy, [FormatCurrencySign(Abs(FStillDue))]));
      Exit;
    end
    else if (AppDetails.OneBill and ((GetRoundedUpDown(GlbTable.Uninvoiced, 2) - RoundNearest(FToPay, 2)) > 0.01 ) ) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sOneBillMakeFullPay, [FormatCurrencySign(Abs(FStillDue))]));
      Exit;
    end;

    LogMessage(ltPaymentTrace, '45' + #9);
    SetItemSelection;
    LogMessage(ltPaymentTrace, '46' + #9);

    if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (LoyaltyReward.AvailableQty > 0)) then
    begin
      LogMessage(ltPaymentTrace, '47' + #9 + 'LoyaltyReward.LoyaltyRewardID=' + IntToStr(LoyaltyReward.LoyaltyRewardID)+ ', LoyaltyReward.AvailableQty=' + FloatToStr(LoyaltyReward.AvailableQty));
      if (ShowConf(sFreeLoyaltyRewardNotUsed) = mrYes) then
      begin
        LogMessage(ltPaymentTrace, '48' + #9);
        ResetLoyaltyReward;
        tmpList := TList.Create;
        try
          LogMessage(ltPaymentTrace, '49' + #9);
          GetAllSelectedOLs(tmpList);
          LogMessage(ltPaymentTrace, '57' + #9);

          for i:=0 to tmpList.Count-1 do
          begin
            tmpOL := TOrderLine(tmpList.Items[i]);
            tmpOL.LoyaltyFreeQty := 0;
            tmpOL.LoyaltyFreeDiscount := 0;
          end;

          LogMessage(ltPaymentTrace, '50' + #9);
          CalculateToPay;
        finally
          tmpList.Clear;
          FreeAndNil(tmpList);
        end;
      end;
      VisibleWindow := vwTender;
      LogMessage(ltPaymentTrace, '51' + #9);
      Exit;
    end;
           
    if AppDetails.PaymentAutoComplete or FSelectedItemsNotPaidFull then
    begin
      LogMessage(ltPaymentTrace, '52' + #9);
      FGetAllItems := True;
    end;

    if (FRecordType = rtTenderedItems) then
    begin
      LogMessage(ltPaymentTrace, '53' + #9);
      ClearAllOrderLines;
      LogMessage(ltPaymentTrace, '58' + #9);
    end;

    tmpList := TList.Create;
    LogMessage(ltPaymentTrace, '56' + #9);
    GetAllSelectedOLs(tmplist);
    LogMessage(ltPaymentTrace, '59' + #9);

    if ((OnAccount) and (GlbAccount.CreditLimitActive)) then begin
      if (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance)) then begin
        ShowMsg(Format(sCannotChargeCust, [FormatCurrencyNoSign(FStillDue), FormatCurrencySign(-GlbAccount.CurrentBalance), FormatCurrencySign(GlbAccount.CreditLimit)]));
        FRecordType := rtBoth;
        Exit;
      end;
    end;

    if ((OLToPayList.Count = 0) and (tmpList.Count = 0)) then
    begin
      ShowQuickMsg(sNoOrdLinesSelectedToPay);
      FRecordType := rtBoth;
      cmdToPayClick(Self);
      Exit;
    end;

    LogMessage(ltPaymentTrace, '60' + #9);
    if (FStillDue < -0.01) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sPayOverBy, [FormatCurrencySign(Abs(FStillDue))]));
      FRecordType := rtBoth;
    end
    else if ((FStillDue > 0.01) and (AppDetails.OneBill) and (not OnAccount)) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sOneBillPayShortBy, [FormatCurrencySign(Abs(FStillDue))]));
      FRecordType := rtBoth;
    end
    else if ((FStillDue > 0.01) and (not AppDetails.OneBill) and ((not OnAccount))) then
    begin
      VisibleWindow := vwTender;
      ShowQuickMsg(Format(sPayShortBy, [FormatCurrencySign(Abs(FStillDue))]));
      FRecordType := rtBoth;
    end
    else
    begin
      LogMessage(ltPaymentTrace, '61' + #9);

      SurplusOnAccount := False;
      if ((AppDetails.PaymentAllowChangeOnAccount) and (FChange > 0) and (GlbAccount.AccountID <> 0) and (GlbAccount.AllowCredit)) then
      begin
        LogMessage(ltPaymentTrace, '91' + #9);
        ConfirmationResult := ShowConfWithCancel(Format(sCreditChangeToAcct, [FormatCurrencySign(FChange)]));
        if (ConfirmationResult = mrCancel) then begin
          LogMessage(ltPaymentTrace, '62' + #9);
          Exit;
        end
        else if (ConfirmationResult = mrYes) then begin
          LogMessage(ltPaymentTrace, '63' + #9);
          SurplusOnAccount := True;
        end;
      end;

      if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformDiscNotAllowed(True,
        FDiscountAmount + FComboDiscount + FLoyaltyFreeItemDiscount) then
        Exit;

      //if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformComboDiscNotAllowed(True,
      //  FComboDiscount) then
      //  Exit;

      if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformOTAcctNotAllowed(True,
        GlbAccount.AccountID > 0) then
        Exit;

      if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformEventNotAllowed(True,
        GlbEvent.EventID > 0) then
        Exit;

      if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformExtAcctNotAllowed(True,
        sExternalAccountIntegration.ProviderCardsPresent > 0) then
        Exit;

      LogMessage(ltPaymentTrace, '64' + #9);
      if ((AppDetails.EnableCombo) and CheckForUnFinishedCombos(tmplist, aCombo, aComboIndex)) then
      begin
        VisibleWindow := vwTender;
        ShowQuickMsg(Format(sFinishPartlyPaidCombos, [aCombo]));
        FRecordType := rtBoth;
        Exit;
      end;

      LogMessage(ltPaymentTrace, '65' + #9);
      SaveInvoice := True;
      if AppDetails.OneBill then
      begin
        LogMessage(ltPaymentTrace, '92' + #9);
        if FToPay < GlbTable.Uninvoiced then
        begin
          SaveInvoice := False;
          LogMessage(ltPaymentTrace, '66' + #9);
        end;
      end;
      if SaveInvoice then
      begin
        LogMessage(ltPaymentTrace, '67' + #9);
        tmpPaymentOnSelectedItems := PaymentOnSelectedItems;
        tmpAmountSplittingStarted := AmountSplitingStarted;
        PaymentOnSelectedItems := False;
        AmountSplitingStarted := False;
        SaveInvoice := TryPaymentTable;
        LogMessage(ltPaymentTrace, '68' + #9 + 'SaveInvoice=' + BoolToStr(SaveInvoice, True));
        if (SaveInvoice) then
          LogMessage(ltTableManagement, 'Table Payment saved invoice')
        else
          LogMessage(ltTableManagement, 'Table Payment saved invoice failed');
      end
      else
      begin
        SaveInvoice := True;
        LogMessage(ltPaymentTrace, '69' + #9);
      end;
      FGetAllItems := False;

      if (SaveInvoice) then
      begin
        LogMessage(ltPaymentTrace, '70' + #9);
        AmountSplitingStarted := False;
        FSelectedItemsNotPaidFull := False;
        ItemsSelectedManually := False;
        FUseItemDiscounts := False;
        PaymentOnSelectedItems := False; //Remember the change calculated for the message
        LastThroughAccount := (GlbAccount.AccountID > 0);

        LastChange := FChange;
        LastTable := GlbTable.TableNo;

        TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding);
        LogMessage(ltPaymentTrace, '71' + #9 + 'FPayment=' + FloatToStr(FPayment) + ', FTip=' + FloatToStr(FTip) +
          ', FChange=' + FloatToStr(FChange) + ', FTender=' + FloatToStr(FTender) + ', FRounding=' + FloatToStr(FRounding));

        LogMessage(ltPaymentTrace, '72' + #9 + 'FToPay=' + FloatToStr(FToPay) + ', FSurcharge=' + FloatToStr(FSurcharge) +
          ', FDiscountAmount=' + FloatToStr(FDiscountAmount) + ', FComboDiscount=' + FloatToStr(FComboDiscount) +
          ', FLoyaltyFreeItemDiscount=' + FloatToStr(FLoyaltyFreeItemDiscount) + ', FSalesTax=' + FloatToStr(FSalesTax));

        if ((GlbEvent.EventID <= 0) or (SubsidyCalc.IsHostTransaction)) then
          FStillDue := GetRoundedUpDown(CalculateStillDue(FToPay + FSurcharge, FDiscountAmount + FComboDiscount + FLoyaltyFreeItemDiscount, FPayment, FSalesTax), AppDetails.DecimalPlaces)
        else
          FStillDue := GetRoundedUpDown(CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FLoyaltyFreeItemDiscount, FPayment, FSalesTax), AppDetails.DecimalPlaces);

        LastPayment := FPayment;
        LastStillDue := FStillDue;
        LastOnAccount := (
                          (OnAccount or ((GlbEvent.EventID > 0) and (not SubsidyCalc.IsHostTransaction) )) and
                          (LastStillDue > 0)
                         );
        LogMessage(ltPaymentTrace, '73' + #9 + 'FStillDue=' + FloatToStr(FStillDue) + ', LastOnAccount=' + BoolToStr(LastOnAccount, True));
        ClearTempItemList;

        //Do Cash Cam Doofrey here
        if (AppDetails.CashCamEnabled) then begin
          PaymentTableCashCam;
        end;

        txtSumToPay.Text := '';
        txtStillDue.Text := '';
        txtPayment.Text := '';
        txtBalDue.Text := '';

        if (LastOnAccount) then
        begin
          ShowQuickMsg(Format(sLastTableTenderedDetails, [LastTable, FormatCurrencySign(LastPayment), FormatCurrencySign(Abs(LastStillDue))]));
        end
        else if ((LastPayment > 0) or LastThroughAccount) then
        begin
          ShowQuickMsg(Format(sLastTableTenderedDetails1, [LastTable, FormatCurrencySign(LastPayment), FormatCurrencySign(LastChange)]));
        end;

        LogMessage(ltPaymentTrace, '74' + #9);
        if (TLList.TenderTypeExists(TLTPrepaid)) then begin
          for I := (TLList.Count - 1) downto 0 do begin
            TL := TLList.Items[I];
            if (TL.TLType = TLTPrepaid) then begin
              GlbTable.PrepaidBalance := (GlbTable.PrepaidBalance - TL.TLTender);
            end;
          end;
        end;

        //Try finalise any wiGroup tenders
        if AppDetails.wiGroupEnabled then
          wiGroupPOSSrv.CheckAdviseFinaliseTLs;

        //if still more to pay on table, stay in screen
        fAmount := GlbTable.Uninvoiced;
        GlbTable.Uninvoiced := GetRoundedUpDown(fAmount - GetRoundedUpDown(FToPay, AppDetails.DecimalPlaces), AppDetails.DecimalPlaces);
        GlbTable.Invoiced := GetRoundedUpDown(GlbTable.Invoiced  + GetRoundedUpDown(FToPay, AppDetails.DecimalPlaces), AppDetails.DecimalPlaces);
        if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
        begin
          GlbTable.Uninvoiced := TruncateTo(fAmount - FToPay, 2);
          GlbTable.Invoiced := TruncateTo(GlbTable.Invoiced  + FToPay, 2);
        end;
        LogMessage(ltPaymentTrace, '75' + #9 + 'fAmount=' + FloatToStr(fAmount) + ', GlbTable.Uninvoiced=' + FloatToStr(GlbTable.Uninvoiced) +
          ', GlbTable.Invoiced=' + FloatToStr(GlbTable.Invoiced));

        if (GlbTable.Uninvoiced >= 0.01) then
        begin
          LogMessage(ltPaymentTrace, '76' + #9);
          ShowPaymentTable;
          LogMessage(ltPaymentTrace, '77' + #9);
        end
        else
        begin
          LogMessage(ltPaymentTrace, '78' + #9);

          if ((AppDetails.OneBill) and (AppDetails.EnableFiscalPrinting) and (SendFiscalInvoice)) then
          begin
            LogMessage(ltPaymentTrace, '79' + #9);
            DM.tr.Commit;
          end;

          sExternalAccountIntegration.ClearPresentedCards;   //do now, so Tenderline are unprotected so can be cleared
          ClearAllTenderLines; //so TLList.Count= 0, so not prompted in cmdHideClick
          LogMessage(ltPaymentTrace, '80' + #9);

          if ((AppDetails.PaymentAllowPrePay) and (GlbTable.PrepaidBalance <> 0)) then
          begin
            LogMessage(ltPaymentTrace, '81' + #9);
            FormAlphaBlend.ShowAlphaBlend;
            if (GlbTable.WhenOpened > 0) then
              DoDialogPrepay(GlbTable.GroupID, True, True);
            FormAlphaBlend.HideAlphaBlend;
          end;

          if ((AppDetails.EFTPOSEnablePreAuth) and (GlbTable.EFTPOSTxnRef <> '')) then
          begin
            LogMessage(ltPaymentTrace, '82' + #9);
            formEFTPOSIntegration.EFTPOSTxnRef := GlbTable.EFTPOSTxnRef;
            if AppDetails.EFTPOSType = ecSektorVault then
              formEFTPOSIntegration.SektorVaultAccountID := GlbTable.ReferenceNo;

            if (formEFTPOSIntegration.DoEftposEdit(GlbTable.SpendLimit, 0, ettPreAuth)) then
            begin
              LogMessage(ltPaymentTrace, '83' + #9);
              dm.tr.StartTransaction;
              with dm.sp do
              begin
                SetStoredProcName('SETTABLEEFTPOSTXNREF');
                ParamByName('groupid').AsInteger := GlbTable.GroupID;
                ParamByName('loginid').AsInteger := GlbLogin.LoginID;
                ParamNull(ParamByName('eftpostxnref'));
                ExecProc;
                if (not FieldByName('errorcode').IsNull) then begin
                  ShowErrorMsg(FieldByName('errorcode').AsInteger);
                  dm.tr.Rollback;
                  Exit;
                end;

                //try to close table
                if ((AppDetails.PaymentAutoCloseTable) and (GlbTable.Booking.BookingType = btNone)) or
                   ((not AppDetails.PaymentAutoCloseTable) and
                   ((AppDetails.PaymentSaleCategoryID <> 0) and (AppDetails.PaymentSaleCategoryID <> GlbTable.SaleCategoryID))) then begin
                  SetStoredProcName('CLOSE_TABLE');

                  ParamByName('groupid').AsInteger := GlbTable.GroupID;
                  ParamByName('loginid').AsInteger := GlbLogin.LoginID;
                  ExecProc;
                end;
              end;
              dm.tr.Commit;
              LogMessage(ltPaymentTrace, '84' + #9);
            end;
          end;

          LogMessage(ltPaymentTrace, '85' + #9);
          STableManagementIntegration.CloseTable;
          LogMessage(ltPaymentTrace, '86' + #9);
          PostMessage(Self.Handle, WM_CLOSE, 0, 0);
          LogMessage(ltPaymentTrace, '87' + #9);
        end;
      end
      else
      begin
        LogMessage(ltPaymentTrace, '88' + #9);
        PaymentOnSelectedItems := tmpPaymentOnSelectedItems;
        AmountSplitingStarted := tmpAmountSplittingStarted;
        if (not sExternalAccountIntegration.CanContinueSale) then
        begin
          LogMessage(ltPaymentTrace, '89' + #9);
          sExternalAccountIntegration.CheckLoadedRedemptions;  //we failed completing transaction, ask to clear redemptions
        end;
      end;
    end;
  finally
    if Assigned(tmpList) then
    begin
      tmpList.Clear;
      tmpList.Free;
    end;
    DoingTablePayment := False;
    LogMessage(ltPaymentTrace, '90' + #9);
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.SetOLCols;
begin
  LogMessage(ltPaymentTrace, '303' + #9);
  grdUnInvoicedList.RowCount := 2;
  grdUnInvoicedList.FixedRows := 1;
  SetUnInvoicedOLCols;

  grdUnInvoicedList.Cells[0, 1] := '';
  grdUnInvoicedList.Cells[1, 1] := '';
  grdUnInvoicedList.Cells[2, 1] := '';
  grdUnInvoicedList.Cells[3, 1] := '';
  grdUnInvoicedList.Cells[4, 1] := '';
  grdUnInvoicedList.Cells[5, 1] := '';
  grdUnInvoicedList.Cells[6, 1] := '';
  grdUnInvoicedList.Cells[7, 1] := '';
  grdUnInvoicedList.Cells[8, 1] := '';
  grdUnInvoicedList.Cells[9, 1] := '';

  with grdToPay do
  begin
    RowCount := 2;
    FixedRows := 1;
    ColCount := 5;

    ColWidths[0] := 27;
    ColWidths[1] := 25;
    ColWidths[2] := 153;
    ColWidths[3] := 45;
    ColWidths[4] := 45;

    Cells[0, 0] := sQryPos;
    Cells[1, 0] := sQryQty;
    Cells[2, 0] := sItem;
    Cells[3, 0] := sDue;
    Cells[4, 0] := sToPay;

    Cells[0, 1] := '';
    Cells[1, 1] := '';
    Cells[2, 1] := '';
    Cells[3, 1] := '';
    Cells[4, 1] := '';
  end;
  LogMessage(ltPaymentTrace, '304' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.CalculateToPay(LoadAllowedDisc: Boolean = False; DisplayDiscount: Boolean = True);
var
  I: Integer;
  SalesTaxPercent: Real;
  RoundedToPay: Currency;
  SurchargeItem: TSurchargeItem;
  aOL: TOrderLine;
  rDiscPercToUse: Real;
  tmpList: TList;
begin
  LogMessage(ltPaymentTrace, '167' + #9 + 'LoadAllowedDisc=' + BoolToStr(LoadAllowedDisc, True) + ', DisplayDiscount=' + BoolToStr(DisplayDiscount, True));
  FToPay := 0;
  FSalesTax := 0;
  FSalesTaxCalculated := False;
  FSurcharge := 0.00;
  FLoyaltyFreeItemDiscount := 0;

  tmpList := TList.Create;
  try
    LogMessage(ltPaymentTrace, '585' + #9);
    GetAllSelectedOLs(tmpList);
    if (tmpList.Count > 0) then
    begin
      for I := 0 to (tmpList.Count - 1) do
      begin
        aOL := tmpList.Items[I];
        LogMessage(ltPaymentTrace, '168' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay) +
          ', aOL.LoyaltyFreeDiscount=' + FloatToStr(aOL.LoyaltyFreeDiscount) + 'aOL.SalesTaxPercent=' + FloatToStr(aOL.SalesTaxPercent));

        FToPay := FToPay + aOL.ToPay;
        FLoyaltyFreeItemDiscount := GetRoundedUpDown(FLoyaltyFreeItemDiscount + aOL.LoyaltyFreeDiscount, AppDetails.DecimalPlaces);

        SalesTaxPercent := aOL.SalesTaxPercent;
        RoundedToPay := GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces);

        if (AppDetails.TaxExclusivePrices) then begin
          FSalesTax := FSalesTax + ((RoundedToPay * SalesTaxPercent) / 100);
        end
        else begin
          FSalesTax := FSalesTax + ((RoundedToPay * SalesTaxPercent) / (100 + SalesTaxPercent));
        end;
      end;
    end;
    FLoyaltyFreeItemDiscount := 0;
    FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);
    LogMessage(ltPaymentTrace, '169' + #9 + 'FToPay=' + FloatToStr(FToPay) + ', FSalesTax=' + FloatToStr(FSalesTax));

    CalculateSubsidy(tmpList, LoadAllowedDisc, DisplayDiscount);

    LogMessage(ltPaymentTrace, '170' + #9);
    CalculateAccTypeItemDiscount(tmpList);

    if DisplayDiscount then
    begin
      LogMessage(ltPaymentTrace, '171' + #9);
      UpdateScreenAfterCalculateDiscount;
    end;

    //when using account type discounting, FDiscountPercent is zero, so we need
    //to temporarily calculate a percentage, in order to calculate the sales tax
    if ((FUseItemDiscounts) or (FComboDiscount > 0)) then
    begin
      if (FToPay <> 0) then
      begin
        rDiscPercToUse := ((FDiscountAmount + FComboDiscount) / FToPay) * 100;
      end
      else begin
        rDiscPercToUse := 0;
      end;
      LogMessage(ltPaymentTrace, '172' + #9 + 'rDiscPercToUse=' + FloatToStr(rDiscPercToUse) + ', FUseItemDiscounts=' + BoolToStr(FUseItemDiscounts, True) +
        ', FComboDiscount=' + FloatToStr(FComboDiscount) + ', FDiscountAmount=' + FloatToStr(FDiscountAmount));
    end
    else begin
      rDiscPercToUse := FDiscountPercent;
      LogMessage(ltPaymentTrace, '173' + #9 + 'rDiscPercToUse=' + FloatToStr(rDiscPercToUse));
    end;

    if AppDetails.EnableFiscalPrinting then
    begin
      if (OLToPayList.Count > 0) then
      begin
        for I := 0 to (OLToPayList.Count - 1) do
        begin
          aOL := OLToPayList.Items[I];
          if (aOL.ItemID > 0) then
            FSalesTax := FSalesTax + (((aOL.ToPay - (aOL.topay * rDiscPercToUse/100)) * aOL.SalesTaxPercent ) / 100);
          FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);
        end;
      end;
    end
    else
      FSalesTax := FSalesTax * ((100 - rDiscPercToUse) / 100);

    FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);
    LogMessage(ltPaymentTrace, '174' + #9 + 'FSalesTax=' + FloatToStr(FSalesTax));

    for I := 0 to (SurchargeList.Count - 1) do
    begin
      SurchargeItem := SurchargeList.Items[I];
      if AppDetails.EnableFiscalPrinting then
      begin
        FSalesTax := FSalesTax + ((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100);
      end
      else
      begin
        if (AppDetails.TaxExclusivePrices) then
        begin
          FSalesTax := FSalesTax + ((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100);
        end
        else
        begin
          FSalesTax := FSalesTax + ((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent));
        end;
      end;
      FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);
      LogMessage(ltPaymentTrace, '586' + #9 + 'SurchargeItem.ItemID=' + IntToStr(SurchargeItem.ItemID) + ', FSalesTax=' + FloatToStr(FSalesTax));
    end;

    LogMessage(ltPaymentTrace, '175' + #9);
    LoadTLs(LANone);
    LogMessage(ltPaymentTrace, '587' + #9 + 'FSurcharge=' + FloatToStr(FSurcharge));

    if FToPay + FSurcharge < 0 then
      txtSumToPay.Text := '0.00'
    else
      txtSumToPay.Text := FormatCurrencyNoSign(RoundNearest(FToPay + FSurcharge, 2) );

    if (AppDetails.TaxExclusivePrices) then begin
      txtSalesTax.Text := FormatCurrencyNoSign(FSalesTax);
    end;

    LogMessage(ltPaymentTrace, '176' + #9 + 'FStillDue=' + FloatToStr(FStillDue) + ', FRequired=' + FloatToStr(FRequired));
    FStillDue := GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces);
    FRequired := GetRoundedUpDown(FRequired, AppDetails.DecimalPlaces);

    if FRequired < 0 then
      txtStillDue.Text := '0.00'
    else
      txtStillDue.Text := FormatCurrencyNoSign(FRequired);

    if (GlbAccount.AllowCredit) then
    begin
      LogMessage(ltPaymentTrace, '177' + #9);
      lblAvailDollarSign.Visible := True;
      txtAvail.Visible := True;
      if (GlbEvent.EventID <= 0) then
      begin
        lblAvail.Caption := DoubleAmpersand(GlbAccount.AccountName);
        txtAvail.Text := FormatCurrencyNoSign(Abs(GlbAccount.CurrentBalance));

        if ((GlbAccount.CurrentBalance) > 0.00) then begin
          txtAvail.Font.Color := clRed;
          txtAvail.Color := clWhite;
        end
        else begin
          txtAvail.Font.Color := clBlack;
          txtAvail.Color := $00FFFF80;
        end;

      end
      else
      begin
        lblAvail.Caption := DoubleAmpersand(sAvailable);
        txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available - FHostSubsidy + FDiscountAmount);
        txtAvail.Font.Color := clBlack;
        txtAvail.Color := $00FFFF80;
      end;
      pnlAvail.Visible := True;
      pnlLoyalty.Visible := (GlbEvent.EventID <= 0);
      lblTitle.Visible := False;
    end
    else if (GlbAccount.AccountID > 0) then
    begin
      LogMessage(ltPaymentTrace, '178' + #9);
      lblAvailDollarSign.Visible := False;
      txtAvail.Visible := False;
      lblAvail.Caption := DoubleAmpersand(GlbAccount.AccountName);
      lblTitle.Visible := False;
      pnlLoyalty.Visible := True;
      pnlAvail.Visible := True;
    end
    else begin
      LogMessage(ltPaymentTrace, '179' + #9);
      lblTitle.Visible := True;
      pnlLoyalty.Visible := False;
      pnlAvail.Visible := False;
    end;
  finally
    BroadcastCDAMessage;
    tmpList.Clear; //Temporary list. Object in this list is part of another list. So no need to clear the objects.
    tmpList.Free;
    LogMessage(ltPaymentTrace, '180' + #9);
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.UpdateScreenAfterCalculateDiscount;
var
  I: Integer;
  Balance: Currency;
begin
  LogMessage(ltPaymentTrace, '443' + #9);
  txtDiscountPercent.Text := FormatPercent(FDiscountPercent);
  txtDiscountAmount.Text := FormatCurrencyNoSign(FDiscountAmount + FLoyaltyFreeItemDiscount);

  //Dont show discount for guest transaction
  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (GlbEvent.SubsidyID > 0) and (not OnAccount)) then
  begin
    txtDiscountPercent.Text := '';
    txtDiscountAmount.Text := '';
    cmdDiscountPercent.Enabled := False;
    cmdDiscountAmount.Enabled := False;
  end
  else if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (GlbEvent.SubsidyID > 0) and (OnAccount)) then
  begin
    cmdDiscountPercent.Enabled := True;
    cmdDiscountAmount.Enabled := True;
  end;

  if (GlbTable.SaleCategorySurchargeItemID <> 0) then begin
    if (SaleCategorySurchargeItem = nil) then begin
      SaleCategorySurchargeItem := TSurchargeItem.Create;
      SaleCategorySurchargeItem.ItemID := GlbTable.SaleCategorySurchargeItemID;
      SaleCategorySurchargeItem.ItemAbbrev := GlbTable.SaleCategorySurchargeItemAbbrev;
      SaleCategorySurchargeItem.Percent := GlbTable.SaleCategorySurchargePercent;
      SaleCategorySurchargeItem.Fixed := GlbTable.SaleCategorySurchargeFixed;
      SaleCategorySurchargeItem.SalesTaxPercent := GlbTable.SaleCategorySurchargeSTRate;
      SaleCategorySurchargeItem.SurchargeType := stSaleCategory;

      SurchargeList.Add(SaleCategorySurchargeItem);
    end;
    SaleCategorySurchargeItem.Amount := SaleCategorySurchargeItem.Fixed + GetRoundedUpDown((FToPay - FLoyaltyFreeItemDiscount - FDiscountAmount- FComboDiscount) * SaleCategorySurchargeItem.Percent / 100, AppDetails.DecimalPlaces);
  end;

  FSurcharge := 0;
  for I := 0 to (SurchargeList.Count - 1) do begin
    FSurcharge := FSurcharge + GetRoundedUpDown(TSurchargeItem(SurchargeList[I]).Amount, AppDetails.DecimalPlaces);
  end;
  FSurcharge := GetRoundedUpDown(FSurcharge, AppDetails.DecimalPlaces);

  //    This one line above is probably crap. It probably needs to be added at a later point.
//    Test and see. Don't forget about SalesTax doofreys where they come up.

  if (AppDetails.TaxExclusivePrices) then
  begin      //Jon 30-5-2001
    if (FSalesTaxCalculated) then
    begin
      if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
        Balance := FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount - FTotalSubsidyAllowed + FSalesTax  //Jon 30-5-2001
      else
        Balance := FToPay + FSurcharge  - FComboDiscount - FLoyaltyFreeItemDiscount - FHostSubsidy + FSalesTax
    end                                   //Jon 30-5-2001
    else begin                            //Jon 30-5-2001
      if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
        Balance := FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount - FTotalSubsidyAllowed + ((FSalesTax * (100 - FDiscountPercent)) / 100)
      else
        Balance := FToPay + FSurcharge - FComboDiscount - FLoyaltyFreeItemDiscount - FHostSubsidy + ((FSalesTax * (100 - FDiscountPercent)) / 100)
    end;                                  //Jon 30-5-2001
  end                                     //Jon 30-5-2001
  else
  begin
    if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
      Balance := FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount - FTotalSubsidyAllowed
    else
      Balance := FToPay + FSurcharge - FComboDiscount - FLoyaltyFreeItemDiscount - FHostSubsidy;
  end;

  if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    Balance := TruncateTo(Balance, 2)
  else
    Balance := GetRoundedUpDown(Balance, AppDetails.DecimalPlaces);

  if Balance < 0 then
    txtBalDue.Text := '0.00'
  else
    txtBalDue.Text := FormatCurrencyNoSign(Balance);

  if (FSalesTaxCalculated) then begin
    if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
      FStillDue := CalculateStillDue(FToPay, FDiscountAmount+ FComboDiscount + FLoyaltyFreeItemDiscount+FTotalSubsidyAllowed, FPayment, FSalesTax)
    else
      FStillDue := CalculateStillDue(FToPay, FComboDiscount + FLoyaltyFreeItemDiscount + FHostSubsidy, FPayment, FSalesTax)
  end
  else begin
    if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
      FStillDue := CalculateStillDue(FToPay, FDiscountAmount + FComboDiscount + FLoyaltyFreeItemDiscount+FTotalSubsidyAllowed, FPayment, ((FSalesTax * (100 - FDiscountPercent)) / 100))
    else
      FStillDue := CalculateStillDue(FToPay, FComboDiscount + FLoyaltyFreeItemDiscount+FHostSubsidy, FPayment, FSalesTax);
  end;

  if ((GlbAccount.AccountID > 0) and (OnAccount)) then begin
    if ((GlbAccount.CreditLimitActive) and (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance))) then begin
      FRequired := (FStillDue - (GlbAccount.CreditLimit - GlbAccount.CurrentBalance));
    end
    else begin
      FRequired := 0;
    end;
  end
  else begin
    FRequired := FStillDue;
  end;
  FStillDue := GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces);
  if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    FRequired := TruncateTo(FRequired, 2)
  else
    FRequired := GetRoundedUpDown(FRequired, Appdetails.DecimalPlaces);
  if FRequired < 0 then
    txtStillDue.Text := '0.00'
  else
    txtStillDue.Text := FormatCurrencyNoSign(FRequired);
end;

{******************************************************************************}
procedure TformPaymentTable.CalculateAccTypeItemDiscount(AList: TList);
begin
  LogMessage(ltPaymentTrace, '445' + #9);
  if not FUseDiscountAmount then
    FDiscountAmount := 0;

  CalculateComboAndAccountDiscounts(AList);

  if FComboDiscount > 0 then
    txtComboDisc.Text := FormatFloat(PRICEFORMAT, FComboDiscount);
  txtComboDisc.Visible := (FComboDiscount > 0);
  lblComboDisc.Visible := (FComboDiscount > 0);
end;
{******************************************************************************}
function TformPaymentTable.AddOrderLine(AList:TList; AOLIndex: Integer):Boolean; //Add to OLToPayList
var
  I: Integer;
  aOL, SOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '539' + #9 + 'AOLIndex=' + IntToStr(AOLIndex));
  Result := False;
  if not Assigned(AList) then
    Exit;

  if AOLIndex <= AList.Count-1 then
    SOL := AList.Items[AOLIndex]
  else
    Exit;

  if SOL.OrderLineID <= 0 then
    Exit;

  if ((SOL.Uninvoiced <= 0) and ((not AppDetails.PaymentAllowZeroPriceInvoices) or (SOL.ILCount > 0))) then begin
    LogMessage(ltPaymentTrace, '540' + #9 + 'SOL.OrderLineID=' + IntToStr(SOL.OrderLineID) + ', SOL.Uninvoiced=' + FloatToStr(SOL.Uninvoiced) + ', SOL.ILCount=' + IntToStr(SOL.ILCount));
    //Don't allow it to 0 paid for twice
    Exit;
  end;

  //go through existing OLToPayList, if find a match, update ToPay amount then exit
  for i := 0 to (OLToPayList.Count - 1) do
  begin
    aOL := OLToPayList.Items[I];
    if ((aOL.OrderLineID = SOL.OrderLineID)) then
    begin
      LogMessage(ltPaymentTrace, '541' + #9 + 'i=' + IntToStr(i) + ', aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.ComboID=' + IntToStr(aOL.ComboID) +
        ', aOL.LoyaltyItemID=' + IntToStr(aOL.LoyaltyItemID) + ', aOL.LoyaltyFreeQty=' + FloatToStr(aOL.LoyaltyFreeQty) + ', SOL.Uninvoiced=' + FloatToStr(SOL.Uninvoiced) +
        ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice));

      if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (aOL.ComboID <= 0) and (aOL.LoyaltyItemID > 0)) then
      begin
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        LogMessage(ltPaymentTrace, '542' + #9 + 'LoyaltyReward.LoyaltyRewardID' + IntToStr(LoyaltyReward.LoyaltyRewardID) + ', LoyaltyReward.AvailableQty=' + FloatToStr(LoyaltyReward.AvailableQty));
      end;

      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
        aOL.ToPay := TruncateTo(SOL.Uninvoiced, 2)
      else
        aOL.ToPay := CRoundTo(SOL.Uninvoiced, 2);

      if (aOL.UnitPrice > 0) then begin
        aOL.Qty := CRoundTo(aOL.ToPay / aOL.UnitPrice, 4);
      end
      else begin
        aOL.Qty := 0;
      end;

      LogMessage(ltPaymentTrace, '543' + #9 + 'aOL.ToPay=' + FloatToStr(aOL.ToPay) + ', aOL.Qty=' + FloatToStr(aOL.Qty) + ', aOL.QtyChanged=' + BoolToStr(aOL.QtyChanged, True));

      if (LoyaltyReward.RewardOffer = roItem)  then
      begin
        LogMessage(ltPaymentTrace, '544' + #9);
		    AllocateItemLoyalty(aOL);
      end;

      grdItemAutoClick := True;
      grdToPay.Row := i + 1;
      LogMessage(ltPaymentTrace, '545' + #9);
      UpdateGrdToPayCells(grdToPay.Row);
      Result := True;
      LogMessage(ltPaymentTrace, '546' + #9);
      Exit;
    end;
  end;

  //not found so create and add to oltopaylist
  aOL := TOrderLine.Create;
  with aOL do
  begin
    ShowZeroPriceItem := SOL.ShowZeroPriceItem;
    OrderID := SOL.OrderID;
    AlreadyCombined := False;
    OrderLineID := SOL.OrderLineID;
    ItemID := SOL.ItemID;
    ItemGroupID := SOL.ItemGroupID;
    TaxSituation := SOL.TaxSituation;
    CSOSN := SOL.CSOSN;
    CSTICMS := SOL.CSTICMS;
    ProductOrigin := 0;
    CFOP := SOL.CFOP;
    BarCode := SOL.Barcode;
    NCMProductCode := SOL.NCMProductCode;
    CST_PIS := SOL.CST_PIS;
    CST_COFFINS := SOL.CST_COFFINS;
    if Length(CSTICMS) >= 3 then
    begin
      ProductOrigin := StrToIntDef(Copy(CSTICMS, 1,1),0);
      CSTICMS := Copy(CSTICMS,2,Length(CSTICMS)-1);
    end;
    OLQty := SOL.Qty;
    ItemString := SOL.ItemString;
    ModsString := SOL.ModsString;
    OLPriceLevel := SOL.OLPriceLevel;
    SalesTaxPercent := SOL.SalesTaxPercent;
    Qty := SOL.Qty;
    UnitPrice := SOL.UnitPrice;
    OriginalUnitPrice := SOL.OriginalUnitPrice;
    AllowedDiscount := SOL.AllowedDiscount;
    GTIN := SOL.GTIN;
    TaxCoreLabel := SOL.TaxCoreLabel;

    if (AppDetails.OneBill) and (FUseDiscountAmount) then
    begin
      if (OLToPayList.Count = 0) then
        FDiscountAmount := 0;

      FDiscountAmount := FDiscountAmount + SOL.AllowedDiscount;
      LogMessage(ltPaymentTrace, '547' + #9 + 'SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount) + ', FDiscountAmount=' + FloatToStr(FDiscountAmount));
    end;

    SubsidyAllowed := SOL.SubsidyAllowed;

    ForB := SOL.ForB;
    ItemGrpType := SOL.ItemGrpType;
    WeighedItem := SOL.WeighedItem;
    DiscountSchemeItemPercentage := SOL.DiscountSchemeItemPercentage;
    DiscountSchemeItemAmount := 0;
    DiscountSchemeItemTested := False;
    DiscountSchemeID := 0;
    TenderSeqID := 0;
    LastNormalItem := False;
    LastComboItem := False;

    if not AppDetails.EnableCombo then
    begin
      ComboID := 0;
      ComboGroupID := 0;
      ComboIndex := 0;
      ComboName := '';
      ComboQty := 0;
      ComboColourIndex := -1;
    end
    else
    begin
      ComboID := SOL.ComboID ;
      ComboGroupID := SOL.ComboGroupID;
      ComboIndex := SOL.ComboIndex;
      ComboName := SOL.ComboName;
      ComboQty := SOL.ComboQty;
      ComboColourIndex := SOL.ComboColourIndex;
    end;
    FreeComboItem := SOL.FreeComboItem;
    ApplyDiscountType := SOL.ApplyDiscountType;
    ILDiscount := 0;
    Positions := SOL.Pos;

    Uninvoiced := CRoundTo(SOL.Uninvoiced, 2);
    ToPay := aOL.Uninvoiced;
    Due := aOL.Uninvoiced;
    Invoiced := 0;

    if (UnitPrice > 0) then
    begin
      LogMessage(ltPaymentTrace, '548' + #9);
      Qty := CRoundTo(ToPay / UnitPrice, 4);
    end;

    if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    begin
      Qty := TruncateTo(Qty, 3);
      UnitPrice := TruncateTo(UnitPrice, 2);
      OriginalUnitPrice := TruncateTo(OriginalUnitPrice, 2);
      UnInvoiced := TruncateTo(UnInvoiced, 2);
    end;

    QtyLeft := Qty;

    if (LoyaltyReward.RewardOffer <> roItem) or (ComboID > 0) then
    begin
      LogMessage(ltPaymentTrace, '549' + #9);
      LoyaltyItemID := -1;
      LoyaltyFreeQty := 0;
      LoyaltyFreeDiscount := 0;
    end;

    QtyChanged := SOL.QtyChanged;

    LogMessage(ltPaymentTrace, '558' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.ComboID=' + IntToStr(aOL.ComboID) +
      ', aOL.Uninvoiced=' + FloatToStr(aOL.Uninvoiced) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay) + ', aOL.Due=' + FloatToStr(aOL.Due) + ', aOL.Invoiced=' + FloatToStr(aOL.Invoiced) +
      ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice) + ', aOL.Qty=' + FloatToStr(aOL.Qty) + ', aOL.QtyLeft=' + FloatToStr(aOL.QtyLeft) + 'aOL.QtyChanged=' + BoolToStr(aOL.QtyChanged, True));

    if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem)) then
    begin
      LoyaltyItemID := SOL.LoyaltyItemID;
      if not FoundFirstLoyaltyFreeItem then
      begin
        LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, ItemID);
      end;

      LogMessage(ltPaymentTrace, '550' + #9 + 'LoyaltyReward.LoyaltyRewardID' + IntToStr(LoyaltyReward.LoyaltyRewardID) + ', LoyaltyReward.AvailableQty=' + FloatToStr(LoyaltyReward.AvailableQty) +
        ', aOL.LoyaltyItemID=' + IntToStr(aOL.LoyaltyItemID));
      AllocateItemLoyalty(aOL);
    end;

    ILAmount := 0;
    GLCode := SOL.GLCode;
    GLCode2 := SOL.GLCode2;
    NetToPay := 0;
  end;

  //Add OL to OLToPayList
  OLToPayList.Add(aOL);
  LogMessage(ltPaymentTrace, '559' + #9);

  //add to grid
  GridUpdating := True;
  with grdToPay do
  begin
    if (OLToPayList.Count > 1) then
    begin
      RowCount := RowCount + 1;
    end;
    Row := RowCount-1;
    if Row = 0 then
    begin
      LogMessage(ltPaymentTrace, '560' + #9);
      RowCount := RowCount + 1;
    end;

    GridUpdating := False;
    LogMessage(ltPaymentTrace, '555' + #9);
    UpdateGrdToPayCells(Row);
    LogMessage(ltPaymentTrace, '561' + #9);
    Result := True;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.AddAllOrderLines;
begin
  LogMessage(ltPaymentTrace, '276' + #9);
  LoadUnInvoicedOlsToOLToPayList(UnInvoicedOlsToPay);
  LogMessage(ltPaymentTrace, '277' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.DeleteOrderLine;
var
  i, j: Integer;
  OL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '502' + #9);
  with grdToPay do
  begin
    if (OLToPayList.Count = 0) then  //exit if nothing to clear
      Exit;

    OL := OLToPayList.Items[Row-1];
    if OL.TenderSeqID > 0 then
    begin
      ShowQuickMsg(sCanNotDeleteTenderedItem);
      Exit;
    end;

    if (OLToPayList.Count = 1) then
    begin
      ClearAllOrderLines;
      FoundFirstLoyaltyFreeItem := False;
      if (CDAServer.Connected) then
      begin
        CDAServer.CDAData.onetapFunction := ofTablePaymentByItem;
        CDAServer.SendCDAMessage;
      end;
    end
    else
    begin
      // delete from the OLList
      // UpdateSourceOLValues(OL.OrderLineID, -1*OL.Qty, -1*OL.Invoiced);
      // DeleteFromFullList(OL.OrderLineID);
      OLToPayList.Delete(Row - 1);
      if ((OL.LoyaltyItemID > 0) and (OL.LoyaltyFreeQty > 0) and (OL.LoyaltyFreeDiscount > 0) and (LoyaltyReward.RewardOffer = roItem)) then
        FoundFirstLoyaltyFreeItem := False;

      //delete from the grid
      for i := Row to RowCount-1 do
      begin
        Cells[0, i] := Cells[0, i+1];
        Cells[1, i] := Cells[1, i+1];
        Cells[2, i] := Cells[2, i+1];
        Cells[3, i] := Cells[3, i+1];
        Cells[4, i] := Cells[4, i+1];
      end;
      RowCount := RowCount - 1;

      FDiscCalculator.OneBillUseDiscountAmount := False;

      if (AppDetails.OneBill) and (FUseDiscountAmount) then
        FDiscountAmount := FDiscountAmount - OL.AllowedDiscount;

      if (FDiscountAmount < 0) then
        FDiscountAmount := 0;

      LogMessage(ltPaymentTrace, '421' + #9);
      CalculateToPay;

      if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (OL.LoyaltyItemID > 0)) then
      begin
        LogMessage(ltPaymentTrace, '499' + #9);
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + OL.LoyaltyFreeQty;
        TestAllOLsLoyaltyItem(OLToPayList, True);
        for j := 0 to OLToPayList.Count-1 do
        begin
          LogMessage(ltPaymentTrace, '500' + #9);
          UpdateGrdToPayCells(j + 1);
          LogMessage(ltPaymentTrace, '566' + #9);
        end;
      end;
      grdItemAutoClick := True;
      LogMessage(ltPaymentTrace, '501' + #9);
      grdToPayClick(nil);
      LogMessage(ltPaymentTrace, '503' + #9);
      OL.Free;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOLDeleteClick(Sender: TObject);
var
  i: Integer;
  OL: TOrderLine;
  lComboID, lComboIndex: Integer;
  OkToDelete: Boolean;

begin
  LogMessage(ltPaymentTrace, '536' + #9);
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if (OLToPayList.Count = 0) then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  if (not FUseDiscountAmount) and (not (LoyaltyReward.RewardOffer in [roDiscount])) then
    RecalculateDiscount := True;

  with grdToPay do
  begin
    OL := OLToPayList.Items[Row-1];
    lComboID := OL.ComboID;
    lComboIndex := OL.ComboIndex;
  end;

  OkToDelete := True;
  if lComboID > 0 then
  begin
    if ShowConf(sComboMustClearTogether) = mrNo then
      OkToDelete := False;
  end;

  if OkToDelete then
    DeleteOrderLine;

  if ((lComboID > 0) and OkToDelete) then
  begin
    // delete all combo items in the selected list
    for i := OLToPayList.Count-1 downto 0 do
    begin
      OL := OLToPayList.Items[i];
      if ((OL.ComboID = lComboID) and (OL.ComboIndex = lComboIndex)) then
      begin
        grdToPay.Row := i + 1;
        DeleteOrderLine;
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdAllClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '535' + #9);
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;
  
  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  FDiscCalculator.OneBillUseDiscountAmount := False;
  AddAllOrderLines;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOLClearClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '537' + #9);
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if (OLToPayList.Count > 0) then
  begin // JEH 15/5/1
    
    if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

    if (ShowConf(sClearAll) = mrYes) then
    begin
      if (not FUseDiscountAmount) and (not (LoyaltyReward.RewardOffer in [roDiscount])) then
        RecalculateDiscount := True;
      ClearUnTenderedOrderLines;
    end;
  end;

  if (CDAServer.Connected) then
  begin
    CDAServer.CDAData.onetapFunction := ofTablePaymentByItem;
    CDAServer.SendCDAMessage;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDetailsClick(Sender: TObject);
var
  Notes: string;
  TL: TTL;
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if TLList.Count > 0 then begin
    TL := TLList.Items[grdTLs.Row - 1];

    Notes := TL.TLNotes;

    if (GetSingleNote(Notes, 40, sTenderLineDetail, sTenderLineDetailCol, tiMoneyBag, True)) then begin
      TL.TLNotes := Notes;
      LoadTLs(LAEdit);
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.AddTenderLine(TLType: TTLT; IntendedTenderTypeProviderID: Integer = 0; pTL: TTL = nil);
var
  fTotalAmount, Payment: Currency;
  TL, OldTL: TTL;
  I: Integer;
  TenderLineType: TTenderLineType;
  UsingRequired: Boolean;
  tmpList: TList;
  StillDue, EventDue: Currency;
  SurchargeItem: TSurchargeItem;
  FixedSurcharge, SurchargePercent: Currency;
  SaleCategory: TSaleCategory;
begin
  LogMessage(ltPaymentTrace, '93' + #9);
  if (GlbEvent.EventID > 0) then
  begin
    SetGlbEvent(GlbEvent.EventID, True);

    if ((GlbEvent.Available - FHostSubsidy + FDiscountAmount) < 0) then
    begin
      ShowMsg(sEventSpendLimitReached);
      txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available - FHostSubsidy + FDiscountAmount);
      Exit;
    end;
  end;

  AddingTender := True;

  tmpList := TList.Create;
  try
    if TLList.Count = 0 then
      SFiscalPrinting.CheckForPendingFiscalOperations;

    LogMessage(ltPaymentTrace, '94' + #9);
    GetAllSelectedOLs(tmpList);
    LogMessage(ltPaymentTrace, '95' + #9);

    StillDue := FStillDue;
    LogMessage(ltPaymentTrace, '96' + #9 + 'FStillDue=' + FloatToStr(FStillDue));
    cmdGetCPF.Enabled := False;
    if (DoingTablePayment) then begin
      LogMessage(ltPaymentTrace, '97' + #9);
      Exit;
    end;

    if ((TLType in [TLTLibica, TLTFidelio, TLTPhoenix, TLTMicros4700, TLTMicros4700Account, TLTRoomCharge, TLTMews]) and TenderTypeExists(TLType)) then begin
      ShowMsg(sRoomTransExists);
      Exit;
    end;
    if ((TLType = TLTEvolution) and TenderTypeExists(TLTEvolution)) then begin
      ShowMsg(sEvolTransExists);
      Exit;
    end;
    if ((TLType = TLTNZAGold) and TenderTypeExists(TLTNZAGold)) then begin
      ShowMsg(sNZAGoldTransExists);
      Exit;
    end;
    if ((TLType = TLTPrepaid) and TenderTypeExists(TLTPrepaid)) then begin
      ShowMsg(sPrepaidTenderExists);
      Exit;
    end;

    LogMessage(ltPaymentTrace, '98' + #9);
    Payment := GetDue;
    if (TLType = TLTPrepaid) then begin
      if (GlbTable.PrepaidBalance = 0) then begin
        ShowMsg(sNoPrepaidBal);
        Exit;
      end;

      if (GlbTable.PrepaidBalance < Payment) then begin
        LogMessage(ltPaymentTrace, '99' + #9);
        Payment := GlbTable.PrepaidBalance;
      end;
    end;
    LogMessage(ltPaymentTrace, '474' + #9 + 'Payment=' + FloatToStr(Payment) + ', UsingRequired=' + BoolToStr(UsingRequired, True) + ', FRequired=' + FloatToStr(FRequired));

    if TLType = TLTwiGroup then
    begin
      if wiGroupPOSSrv.CheckInformDiscNotAllowed(False, FDiscountAmount + FComboDiscount + FLoyaltyFreeItemDiscount) then
        Exit;

      //if wiGroupPOSSrv.CheckInformComboDiscNotAllowed(False, FComboDiscount) then
      //  Exit;

      if wiGroupPOSSrv.CheckInformOTAcctNotAllowed(False, GlbAccount.AccountID > 0) then
        Exit;

      if wiGroupPOSSrv.CheckInformEventNotAllowed(False, GlbEvent.EventID > 0) then
        Exit;

      if wiGroupPOSSrv.CheckInformExtAcctNotAllowed(False, sExternalAccountIntegration.ProviderCardsPresent > 0) then
        Exit;

      wiGroupPOSSrv.SetPayment(TLList, FToPay, SaleCategorySurchargeItem, OLToPayList);

      if not wiGroupPOSSrv.ValidatePayment then
        Exit;
    end;

    LogMessage(ltPaymentTrace, '100' + #9);
    UsingRequired := False;
    if (not Assigned(pTL)) then
    begin
      TL := TTL.Create;
      TL.TLType := TLType;

      if (IntendedTenderTypeProviderID > 0) then
        TL.TLProviderID := IntendedTenderTypeProviderID;

      LogMessage(ltPaymentTrace, '101' + #9 + 'TL.TLType=' + IntToStr(Ord(TL.TLType)) + ', TL.TLProviderID=' + IntToStr(TL.TLProviderID));

      if TL.TLType = TLTwiGroup then
      begin
        LogMessage(ltPaymentTrace, '102' + #9);
        wiGroupPOSSrv.SetTL(TL);
      end
      else if ((GlbAccount.AccountID > 0) and (OnAccount) and
          (FRequired <> 0) and (FRequired <> Payment) and (not AppDetails.CashSaleSwapStillDueForTenders) and
          (ShowConf(sUseRequiredAmt) = mrYes)) then begin
        LogMessage(ltPaymentTrace, '103' + #9);
        TL.TLPayment := FRequired;
        UsingRequired := True;
      end
      else begin
        LogMessage(ltPaymentTrace, '104' + #9);
        TL.TLPayment := Payment;
      end;

      TL.TLTip := 0.00;
      TL.TLChange := 0.00;
      TL.TLTender := TL.TLPayment;
      TL.TLTender := RoundToRounding(TL.TLTender, RoundForTLType(TL.TLType));
      TL.TLRounding := (TL.TLPayment - TL.TLTender);

      SaleCategory := GlobalMenuList.ListSaleCategorys.Find(GlbTable.SaleCategoryID);

      LogMessage(ltPaymentTrace, '105' + #9 + 'TL.TLPayment=' + FloatToStr(TL.TLPayment) + ', TL.TLTip=' + FloatToStr(TL.TLTip) +
        ', TL.TLChange=' + FloatToStr(TL.TLChange) + ', TL.TLTender=' + FloatToStr(TL.TLTender) + ', TL.TLRounding=' + FloatToStr(TL.TLRounding) +
        ', GlbTable.SaleCategoryID=' + IntToStr(GlbTable.SaleCategoryID));

      if Assigned(SaleCategory) and (SaleCategory.AutoTipPercent > 0) then
      begin
        TL.TLTip := TL.TLTender * (SaleCategory.AutoTipPercent/100);
        TL.TLTip := RoundToRounding(TL.TLTip, RoundForTLType(TL.TLType));
        TL.TLTender := TL.TLTender + TL.TLTip;
        LogMessage(ltPaymentTrace, '106' + #9 + 'SaleCategory.AutoTipPercent' + FloatToStr(SaleCategory.AutoTipPercent) +
          ', TL.TLTip=' + FloatToStr(TL.TLTip) + ', TL.TLTender=' + FloatToStr(TL.TLTender));
      end;
    end
    else
    begin
      TL := pTL;

      LogMessage(ltPaymentTrace, '107' + #9 + 'TL.TLType=' + IntToStr(Ord(TL.TLType)) + ', TL.TLProviderID=' + IntToStr(TL.TLProviderID) +
        ', TL.TLPayment=' + FloatToStr(TL.TLPayment) + ', TL.TLTip=' + FloatToStr(TL.TLTip) +
        ', TL.TLChange=' + FloatToStr(TL.TLChange) + ', TL.TLTender=' + FloatToStr(TL.TLTender) + ', TL.TLRounding=' + FloatToStr(TL.TLRounding) +
        ', GlbTable.SaleCategoryID=' + IntToStr(GlbTable.SaleCategoryID));
    end;

    TL.PaymentType := ptNormal;
    if PaymentOnSelectedItems then
      TL.PaymentType := ptItemsSelected
    else if AmountSplitingStarted then
      TL.PaymentType := ptAmountSplit;
    LogMessage(ltPaymentTrace, '108' + #9 + 'TL.PaymentType=' + IntToStr(Ord(TL.PaymentType)) +
      ', PaymentOnSelectedItems=' + BoolToStr(PaymentOnSelectedItems, True) + ', AmountSplitingStarted=' + BoolToStr(AmountSplitingStarted, True));

    fAmtToPay := FToPay - FLoyaltyFreeItemDiscount;
    LogMessage(ltPaymentTrace, '109' + #9 + 'fAmtToPay=' + FloatToStr(fAmtToPay) + ', FToPay=' + FloatToStr(FToPay) +
      ', FLoyaltyFreeItemDiscount=' + FloatToStr(FLoyaltyFreeItemDiscount));

    If PaymentOnSelectedItems or AmountSplitingStarted  then
    begin
      fAmtToPay := fAmtToPay - FDiscountAmount - FComboDiscount + FSurcharge - FTotalSubsidyAllowed;
      LogMessage(ltPaymentTrace, '110' + #9 + 'fAmtToPay=' + FloatToStr(fAmtToPay) + ', FDiscountAmount=' + FloatToStr(FDiscountAmount) +
        ', FComboDiscount=' + FloatToStr(FComboDiscount) + ', FSurcharge=' + FloatToStr(FSurcharge) + ', FTotalSubsidyAllowed=' + FloatToStr(FTotalSubsidyAllowed));
    end;

    if ((AppDetails.EFTPOSEnablePreAuth) and (GlbTable.EFTPOSTxnRef <> '') and (TLType in [TLTEFTPOS, TLTVisa, TLTVisaEFTPOS, TLTAmex, TLTAmexEFTPOS, TLTDiners, TLTDinersEFTPOS, TLTMasterCard, TLTMasterCardEFTPOS])) then begin
      LogMessage(ltPaymentTrace, '111' + #9);
      //Check if no other Pre-Authed Transactions have been done.
      TL.EFTPOSTxnRef := GlbTable.EFTPOSTxnRef;
      for I := 0 to (TLList.Count - 1) do begin
        OldTL := TLList.Items[I];
        if (OldTL.PreAuthPayment) then begin
          TL.EFTPOSTxnRef := '';
          Break;
        end;
      end;
    end;

    if (AppDetails.EnableFiscalPrinting and AppDetails.EFTPOSIntegrated and (AppDetails.EFTPOSType in [ecTEFIPDaruma]) and (not(TLType in [TLTNone, TLTCash, TLTCheque]))) then
    begin
      SFiscalTEF.DoesCardDiscountEnabled('1');
    end;

    EventDue := FStillDue;
    if ((TLType = TLTNone) or (Assigned(pTL)) or
      (GetTL(TTPayment, TLAAdd, Payment, TL, False, True, False, True, False))) then
    begin
      LogMessage(ltPaymentTrace, '112' + #9 + 'TL.TLType=' + IntToStr(Ord(TL.TLType)));

      if ((TLType = TLTPocketVoucher) and (AppDetails.PocketVoucherMerchantID <> '')) then
      begin
        LogMessage(ltPaymentTrace, '113' + #9);
        TL.PocketVoucherCode := AppDetails.PocketVoucherPrePopCode;
        if ((not GetNumPadString(TL.PocketVoucherCode, ' Voucher Code', tiPOCKETvoucher)) or
            (not DoPocketVoucherTransaction(TL.PocketVoucherCode, TL.TLTender, FToPay, True))) then begin
          LogMessage(ltPaymentTrace, '114' + #9);
          TL.Free;
          Exit;
        end;
      end;

      TL.TEFCreditCardDiscount := 0;
      if (AppDetails.EnableFiscalPrinting and AppDetails.EFTPOSIntegrated and (AppDetails.EFTPOSType in [ecTEFIPDaruma]) and  (not(TLType in [TLTNone, TLTCash, TLTCheque]))) then
      begin
        TL.TEFCreditCardDiscount := SFiscalTEF.LogCreditCardDiscount('LOGESTENDIDO');
      end;

      if (TL.TLType = TLTLibica) then begin
        LibAccount := GlbLibicaAccount.Account;
        LibRoom := GlbLibicaAccount.Room;
        LibName := GlbLibicaAccount.Name;
        LibStatus := GlbLibicaAccount.Status;
      end
      else if (TL.TLType = TLTNZAGold) then begin
        NZAAccount := GlbNZAAccount.Account;
        NZAAKey := GlbNZAAccount.AKey;
        NZAName := GlbNZAAccount.Name;
      end
      else if (TL.TLType = TLTEvolution) then begin
        EvolutionAccountNo := GlbEvoAccount.DCLink;
        EvolutionAccount := GlbEvoAccount.Account;
        EvolutionName := GlbEvoAccount.Name;
        EvolutionAccExtra := GlbEvoAccount.AccExtra;
      end;

      TenderLineType := GetTenderLineTypeRecord(TL.TLType);
      if (TenderLineType <> nil) then begin
        TL.SurchargeItemID := TenderLineType.SurchargeItemID;
        TL.SurchargeItemAbbrev := TenderLineType.SurchargeItemAbbrev;
        TL.SurchargePercent := TenderLineType.SurchargePercent;
        TL.SurchargeFixed := TenderLineType.SurchargeFixed;
        TL.SurchargeSalesTaxPercent := TenderLineType.SurchargeSalesTaxPercent;
        LogMessage(ltPaymentTrace, '115' + #9 + 'TL.SurchargeItemID=' + IntToStr(TL.SurchargeItemID));
      end
      else begin
        TL.SurchargeItemID := 0;
        LogMessage(ltPaymentTrace, '116' + #9);
      end;

      if ((AppDetails.EFTPOSEnablePreAuth) and (TL.PreAuthPayment)) then begin
        //We have just used the preauth reference. So should clear it from memory
        GlbTable.EFTPOSTxnRef := '';
      end;
      Inc(FTenderSeqID);

      if ((PaymentOnSelectedItems or AmountSplitingStarted) and (TL.TLPayment < (FToPay + FSurcharge))) then
      begin
        LogMessage(ltPaymentTrace, '117' + #9);
        if (not FSelectedItemsNotPaidFull) then
        begin
          LogMessage(ltPaymentTrace, '118' + #9);
          ClearTempItemList;

          LogMessage(ltPaymentTrace, '139' + #9);
          CopyItemSToTempList;
        end;
      end
      else if ((TL.PaymentType in [ptNormal])) then
      begin
        LogMessage(ltPaymentTrace, '119' + #9);
        ClearTempItemList;
      end;

      fTotalAmount := TL.TLPayment - TL.TLSurcharge;
      if (FDiscountPercent = 100) then
        fTotalAmount := FToPay ;
      LogMessage(ltPaymentTrace, '120' + #9 + 'FTenderSeqID=' + IntToStr(FTenderSeqID) + ', FTenderGrpID=' + IntToStr(FTenderGrpID) +
        ', fTotalAmount=' + FloatToStr(fTotalAmount));

      FixedSurcharge := 0;
      SurchargePercent := 0;
      if ((FSurcharge > 0) and (not (PaymentOnSelectedItems or AmountSplitingStarted)) and (fTotalAmount < FToPay)) then
      begin
        LogMessage(ltPaymentTrace, '121' + #9);
        for I := 0 to (SurchargeList.Count - 1) do
        begin
          SurchargeItem := SurchargeList.Items[I];
          if (SurchargeItem.SurchargeType = stSaleCategory) then //last sale category surcharge values would be used (as no break)
          begin
            LogMessage(ltPaymentTrace, '122' + #9);
            FixedSurcharge := SurchargeItem.Fixed;
            SurchargePercent := SurchargeItem.Percent;
          end;
        end;
        //NB: Commented out the line below as it was reducing the amount to allocate against OLs by the fixed surcharge PER tenderline,
        //resulting in the OLs not being fully allocated when there was more than one tender line. Leaving the amount not reduced is
        //fine, as payments can then be fully allocated against all OLs and any extra (sale category surcharge amount) is simply ignored.
        //fTotalAmount := GetRoundedUpDown((fTotalAmount-FixedSurcharge)/(1+(SurchargePercent/100)), AppDetails.DecimalPlaces);
      end;

      ReloadItems := False;
      if ((PaymentOnSelectedItems or AmountSplitingStarted) and ((TL.TLPayment - TL.TLSurcharge) < (FToPay + FSurcharge))) then
      begin
        LogMessage(ltPaymentTrace, '123' + #9);
        ReloadItemsForThePayment(TempItemList, fTotalAmount);
      end
      else if ((TL.PaymentType in [ptNormal])) then
      begin
        LogMessage(ltPaymentTrace, '124' + #9);
        ReloadItemsForThePayment(UnInvoicedOlsToPay, fTotalAmount);
      end;
      ReloadItems := False;

      LogMessage(ltPaymentTrace, '125' + #9);
      CalculateToPay;

      LogMessage(ltPaymentTrace, '126' + #9);
      UpdateUnInvoicedOLValues(((PaymentOnSelectedItems or AmountSplitingStarted) and ((TL.TLPayment - TL.TLSurcharge) <= fAmtToPay)));

      LogMessage(ltPaymentTrace, '127' + #9);
      UpdateOLTenderID(FTenderSeqID);

      LogMessage(ltPaymentTrace, '128' + #9);
      TL.TenderSeqNo := FTenderSeqID;
      TL.TenderGroupID := FTenderGrpID;
      AssociateOLSToTender(TL, FTenderSeqID);

      LogMessage(ltPaymentTrace, '129' + #9);
      TLList.Add(TL);

      LogMessage(ltPaymentTrace, '130' + #9);
      LoadTLs(LAAdd);

      if AppDetails.EnableFiscalPrinting then
      begin
        SWBEncryption.AuxiliaryData.TEFDone := '0';
        if EFTPOSTransExists then
          SWBEncryption.AuxiliaryData.TEFDone := '1';
        SWBEncryption.GenerateEncryptedAuxFile;
      end;

      if (not AppDetails.OneBill) then
      begin
        LogMessage(ltPaymentTrace, '131' + #9);
        FProcessedAllItems := False;
        if ((PaymentOnSelectedItems or AmountSplitingStarted) and ((TL.TLPayment - TL.TLSurcharge) < fAmtToPay) and ((TL.TLPayment - TL.TLSurcharge) < StillDue) ) then
        begin
          LogMessage(ltPaymentTrace, '132' + #9);
          ProcessTheRemainingItems((TL.TLPayment - TL.TLSurcharge));
          FLoyaltyDiscount := 0;
        end;

        if ((not FProcessedAllItems) and (not (PaymentOnSelectedItems or AmountSplitingStarted))) then
        begin
          LogMessage(ltPaymentTrace, '133' + #9);
          ClearAllOrderLines;

          LogMessage(ltPaymentTrace, '140' + #9);
          LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, (TL.TLPayment - TL.TLSurcharge), fAmtToPay);
          if (LoyaltyReward.RewardOffer <> roItem) then
            FLoyaltyDiscount := 0;

          LogMessage(ltPaymentTrace, '141' + #9);
          AddAllOrderLines;
        end;

        if (not Assigned(pTL)) and
           (((AppDetails.PaymentAutoComplete) or (TLType = TLTNone)) and
            (((FStillDue = 0) and (GlbEvent.EventID <= 0)) or
            ((GlbEvent.EventID > 0) and (not OnAccount) and (EventDue = TL.TLPayment)) or  // guest payment
             ((UsingRequired) and (FRequired <= 0)))) then
        begin
          LogMessage(ltPaymentTrace, '134' + #9);
          FRecordType := rtBoth;
          Application.ProcessMessages;

          LogMessage(ltPaymentTrace, '135' + #9);
          ItemsSelectedManually := False;
          if PaymentOnSelectedItems or AmountSplitingStarted then
          begin
            LogMessage(ltPaymentTrace, '136' + #9);
            ItemsSelectedManually := True;
          end;

          LogMessage(ltPaymentTrace, '137' + #9);
          cmdOkClick(Self);
          LogMessage(ltPaymentTrace, '138' + #9);
          Exit;
        end;

        if not FProcessedAllItems then
        begin
          LogMessage(ltPaymentTrace, '142' + #9);
          FSelectedItemsNotPaidFull := False;
          ClearTempItemList;
          FRecordType := rtBoth;

          LogMessage(ltPaymentTrace, '143' + #9);
          RecalculateTotals;

          ItemsSelectedManually := False;
          if PaymentOnSelectedItems or AmountSplitingStarted then
          begin
            LogMessage(ltPaymentTrace, '144' + #9);
            ItemsSelectedManually := True;
          end;

          if (not (PaymentOnSelectedItems or AmountSplitingStarted)) then
          begin
            LogMessage(ltPaymentTrace, '145' + #9);
            ClearAllOrderLines;

            LogMessage(ltPaymentTrace, '146' + #9);
            LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, TL.TLPayment, fAmtToPay);

            LogMessage(ltPaymentTrace, '147' + #9);
            AddAllOrderLines;
          end;
        end;

        VisibleWindow := vwTender;
        ManagerAuthorise.Authorise := False;
        LastPayment := 0;
        LastThroughAccount := False;
        LastOnAccount := False;
        LogMessage(ltPaymentTrace, '148' + #9);
      end
      else if (AppDetails.OneBill) then
      begin
        LogMessage(ltPaymentTrace, '149' + #9);
        FProcessedAllItems := False;
        if ((PaymentOnSelectedItems or AmountSplitingStarted) and ((TL.TLPayment - TL.TLSurcharge) < fAmtToPay) and ((TL.TLPayment - TL.TLSurcharge) < StillDue)) then
        begin
          LogMessage(ltPaymentTrace, '150' + #9);
          ProcessTheRemainingItems((TL.TLPayment - TL.TLSurcharge));
          FLoyaltyDiscount := 0;
        end;

        if not FProcessedAllItems then
        begin
          LogMessage(ltPaymentTrace, '151' + #9);
          FSelectedItemsNotPaidFull := False;
          ClearTempItemList;
          FRecordType := rtBoth;
          Inc(FTenderGrpID);

          LogMessage(ltPaymentTrace, '152' + #9);
          ClearAllOrderLines;

          LogMessage(ltPaymentTrace, '153' + #9);
          LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, TL.TLPayment, fAmtToPay);
          LoyaltyReward.AvailableQty := 0;
          if (LoyaltyReward.RewardOffer <> roItem) then
          begin
            LogMessage(ltPaymentTrace, '154' + #9);
            FLoyaltyDiscount := 0;
          end;

          LogMessage(ltPaymentTrace, '155' + #9);
          AddAllOrderLines;
          ItemsSelectedManually := False;
          if PaymentOnSelectedItems or AmountSplitingStarted then
          begin
            LogMessage(ltPaymentTrace, '156' + #9);
            ItemsSelectedManually := True;
          end;
          PaymentOnSelectedItems := False;
          AmountSplitingStarted := False;
        end;

        LogMessage(ltPaymentTrace, '157' + #9);
        CalculateToPay;

        if (FUseDiscountAmount) and (GlbTable.Uninvoiced = FToPay) then
        begin
          LogMessage(ltPaymentTrace, '158' + #9);
          FDiscountAmount := FDiscCalculator.OneBillDiscountAmount;
        end;

        LogMessage(ltPaymentTrace, '159' + #9);
        RecalculateTotals;

        LogMessage(ltPaymentTrace, '160' + #9);
        CalculateToPay;

        if (not Assigned(pTL)) and
           (((AppDetails.PaymentAutoComplete)) and
            (((FStillDue = 0) and (GlbEvent.EventID <= 0)) or
            ((GlbEvent.EventID > 0) and (not OnAccount) and (EventDue = TL.TLPayment)) or  // guest payment
             ((UsingRequired) and (FRequired <= 0)))) then
        begin
          LogMessage(ltPaymentTrace, '161' + #9);
          FRecordType := rtBoth;
          Application.ProcessMessages;

          LogMessage(ltPaymentTrace, '162' + #9);
          cmdOkClick(Self);
          LogMessage(ltPaymentTrace, '163' + #9);
          Exit;
        end;
        VisibleWindow := vwTender;
        ManagerAuthorise.Authorise := False;

        LastPayment := 0;
        LastThroughAccount := False;
        LastOnAccount := False;
        LogMessage(ltPaymentTrace, '164' + #9);
      end;
    end
    else
    begin
      LogMessage(ltPaymentTrace, '165' + #9);
      TL.Free;
    end;
  finally
    BroadcastCDAMessage;
    AddingTender := False;
    tmpList.Clear;
    tmpList.Free;
  end;
  LogMessage(ltPaymentTrace, '166' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.EditTenderLine;
var
  Payment: Currency;
  TL: TTL;
  TenderLineType: TTenderLineType;
begin
  LogMessage(ltPaymentTrace, '453' + #9);
  if (TLList.Count = 0) then
  begin
    Exit;
  end;

  TL := TLList.Items[grdTLs.Row - 1];

  //cannot edit a wiGroup tender line
  if TL.TLType = TLTwiGroup then
    Exit;

  Payment := TL.TLPayment;

  if (GetTL(TTPayment, TLAEdit, Payment, TL, False, True, False, True, False)) then begin //akm 04.12.01
    TenderLineType := GetTenderLineTypeRecord(TL.TLType);

    if (Assigned(TenderLineType)) then begin
      TL.SurchargeItemID := TenderLineType.SurchargeItemID;
      TL.SurchargeItemAbbrev := TenderLineType.SurchargeItemAbbrev;
      TL.SurchargePercent := TenderLineType.SurchargePercent;
      TL.SurchargeFixed := TenderLineType.SurchargeFixed;
      TL.SurchargeSalesTaxPercent := TenderLineType.SurchargeSalesTaxPercent;
    end
    else begin
      TL.SurchargeItemID := 0;
    end;

    LoadTLs(LAEdit);

    if ((TL.TLSurcharge <> 0) or (FSurcharge <> 0)) then begin
      LogMessage(ltPaymentTrace, '423' + #9);
      CalculateToPay;
      LogMessage(ltPaymentTrace, '508' + #9);
    end
    else
      BroadcastCDAMessage;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdCashClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '9' + #9);
  AddTenderLine(TLTCash);
  LogMessage(ltPaymentTrace, '10' + #9);
  ShowMessage('Payment done');
end;
{******************************************************************************}
procedure TformPaymentTable.cmdEFTPOSClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '11' + #9);
  AddTenderLine(TLTEFTPOS);
  LogMessage(ltPaymentTrace, '12' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdChequeClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '13' + #9);
  if ((AppDetails.PaymentAllowPrePay) and (GlbTable.PrepaidBalance <> 0)) then begin
    AddTenderLine(TLTPrepaid);
  end
  else if (AppDetails.SmartCardInterface > 0) then begin
    AddTenderLine(TLTSmartCard);
  end
  else if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '')) then
  begin
    AddTenderLine(TLTPocketVoucher);
  end
  else if AppDetails.wiGroupEnabled then
    AddTenderLine(TLTwiGroup)
  else
  begin
    AddTenderLine(TLTCheque);
  end;
  LogMessage(ltPaymentTrace, '14' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOtherClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '15' + #9);
  AddTenderLine(TLTOther);
  LogMessage(ltPaymentTrace, '16' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdVisaClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '17' + #9);
  AddTenderLine(TLTVisa);
  LogMessage(ltPaymentTrace, '18' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDinersClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '19' + #9);
  AddTenderLine(TLTDiners);
  LogMessage(ltPaymentTrace, '20' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdAMEXClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '21' + #9);
  AddTenderLine(TLTAMEX);
  LogMessage(ltPaymentTrace, '22' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdMasterCardClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '23' + #9);
  AddTenderLine(TLTMasterCard);
  LogMessage(ltPaymentTrace, '24' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdPMSClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '25' + #9);
  case AppDetails.PMSType of
    pmsLibica: AddTenderLine(TLTLibica);
    pmsFidelio: AddTenderLine(TLTFidelio);
    pmsPhoenix: AddTenderLine(TLTPhoenix);
    pmsMicros4700: begin
      if ((GlbAccount.AccountID > 0) and (GlbAccount.Folio <> '') and (ShowConf(format(sMicrosTransfer, [GlbAccount.Folio, GlbAccount.SubFolio])) = mrYes)) then
        AddTenderLine(TLTMicros4700Account)
      else
        AddTenderLine(TLTMicros4700);
    end;
    pmsNZAGold: AddTenderLine(TLTNZAGold);
    pmsEvolution: AddTenderLine(TLTEvolution);
    pmsMiniBar, pmsICRTouch: AddTenderLine(TLTRoomCharge);
    pmsMews: AddTenderLine(TLTMews);
  end;
  LogMessage(ltPaymentTrace, '26' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdVoucherClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '27' + #9);
  AddTenderLine(TLTVoucher);
  LogMessage(ltPaymentTrace, '28' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.DeleteTenderLine;
var
  Tender: Currency;
  TL: TTL;
begin
  LogMessage(ltPaymentTrace, '454' + #9);
  if (TLList.Count = 0) then begin
    Exit;
  end;

  TL := TLList.Items[grdTLs.Row - 1];

  if TL.isProtected then
  begin
    if (tl.TLType = TLTExternalProviders) then
      ShowMsg('Please clear this tender via the External Accounts dialog');
    Exit;
  end;

  //if EFTPOS integrated and is EFTPOS line, undo trans, if doesn't work showmsg & exit
  if (AppDetails.EftposIntegrated) then
  begin
    if (TL.TLType in [TLTEFTPOS, TLTVisaEFTPOS, TLTAmexEFTPOS, TLTDinersEFTPOS, TLTMasterCardEFTPOS, TLTBarterCard]) then
    begin
      //undoing a payment, thus a refund is made to them
      //NOTE: Even for a refund, the EFTPOS value must be a positive value
      //formEFTPOSIntegration.EFTPOSTxnRef := TL.EFTPOSTxnRef;
      //if (not formEFTPOSIntegration.DoEftposTrans(Tender, 0.00, ettRefund)) then begin
      formEFTPOSIntegration.EFTPOSTLType := TL.TLType;
      formEFTPOSIntegration.EFTPOSTxnRef := TL.EFTPOSTxnRef;
      formEFTPOSIntegration.EFTPOSTxnAuthCode := TL.EFTPOSTxnAuthCode;
      if AppDetails.EFTPOSType in [ecTEFIPDaruma] then
      begin
        formEFTPOSIntegration.DoEftposTrans(0, 0, ettFinalize, '');
        SWBEncryption.AuxiliaryData.TEFDone := '0';
        SWBEncryption.GenerateEncryptedAuxFile;
      end
      else if not formEFTPOSIntegration.DoEftposTrans(TL.TLTender, 0, ettRefund, TL.EFTPOSTxnRef, TL.TLType) then
      begin
        ShowMsg(sCannotClearEFtPosTender);
        Exit;
      end;
      if AppDetails.EFTPOSType in [ecDps, ecSektor, ecSektorVault, ecSyncroPlus, ecSmartpaySmartLink, ecSmartLinkLiteWS, ecPaymentSenseWS] then
        formEFTPOSIntegration.RemoveTransactionID(TL.EFTPOSTxnRef);
    end;
  end;
  if (AppDetails.SmartCardInterface > 0) then
  begin
    if (TL.TLType = TLTSmartCard) then
    begin
      Tender := TL.TLTender;
      //undoing a payment, thus a refund is made to them
      //or undoing a refund, thus a payment is made from them
      if (not DoSmartCardTransaction(-Tender)) then
      begin
        ShowMsg(sCannotClearSmartCardTender);
        Exit;
      end;
    end;
  end;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.CheckReverseTransaction(TL);
  //if PaymentOnSelectedItems or AmountSplitingStarted then

  DeleteOLsOfTender(TL, TL.TenderSeqNo);

  TLList.DisposeTL(TLList.IndexOf(TL));
  LoadTLs(LADelete);

  if LoyaltyReward.LoyaltyRewardID > 0 then
  begin
    ApplyLoyalty(OLToPayList, (TLList.Count > 0));
    LogMessage(ltPaymentTrace, '424' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '509' + #9);
  end;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.CheckClearToPay(TLList);
end;

{******************************************************************************}
procedure TformPaymentTable.cmdTLDeleteClick(Sender: TObject);
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;
  if (TLList.Count <= 0) then
  begin
    Exit;
  end;

  //if (((not PaymentOnSelectedItems) and (not AmountSplitingStarted)) or (ShowConf(sConfirmClearCurrentTender) = mrYes)) then
  begin
    DeleteTenderLine;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdTLClearClick(Sender: TObject);
var
  TL: TTL;
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if (TLList.Count > 0) then
  begin
    if (SuppressClearTenderMessage or (ShowConf(sClearAll) = mrYes)) then
    begin
      if (EFTPOSTransExists and (AppDetails.EFTPOSIntegrated)) then
      begin
        ShowMsg(sEftPosTenderLineFound);
        Exit;
      end;
      if ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard))) then begin
        ShowMsg(sClearSmartCardTenderLines);
        Exit;
      end;

      if AppDetails.wiGroupEnabled and TLList.TenderTypeExists(TLTwiGroup) then
      begin
        ShowMsg(Format(sClearwiGroupTenderLines, [swiGroupTL, swiGroupTL]));
        Exit;
      end;

      TL := TLList.Items[grdTLs.Row - 1];
      if (AppDetails.EftposIntegrated and AppDetails.EnableFiscalPrinting and EFTPOSTransExists and (AppDetails.EFTPOSType in [ecTEFIPDaruma])) then
      begin
        if (TL.TLType in [TLTEFTPOS, TLTVisaEFTPOS, TLTAmexEFTPOS, TLTDinersEFTPOS, TLTMasterCardEFTPOS, TLTBarterCard]) then
        begin
          formEFTPOSIntegration.EFTPOSTLType := TL.TLType;
          formEFTPOSIntegration.EFTPOSTxnRef := TL.EFTPOSTxnRef;
          formEFTPOSIntegration.EFTPOSTxnAuthCode := TL.EFTPOSTxnAuthCode;
          formEFTPOSIntegration.DoEftposTrans(0, 0, ettFinalize, '')
        end;
        if AppDetails.EnableFiscalPrinting then
        begin
          SWBEncryption.AuxiliaryData.TEFDone := '0';
          SWBEncryption.GenerateEncryptedAuxFile;
        end;
      end;

      ClearAllTenderLines;

      if not SuppressClearTenderMessage then
      begin
        AddAllOrderLines;
        LogMessage(ltPaymentTrace, '425' + #9);
        CalculateToPay;
        LogMessage(ltPaymentTrace, '510' + #9);
      end;
      FSelectedItemsNotPaidFull := False;
      AmountSplitingStarted := False;
      PaymentOnSelectedItems := False;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdTenderClick(Sender: TObject);
var
  NoItemsSelected : Boolean;
begin
  LogMessage(ltPaymentTrace, '511' + #9);
  NoItemsSelected := (OLToPayList.Count = 0);
  if OLToPayList.Count > 0 then
  begin
    LogMessage(ltPaymentTrace, '512' + #9);
    FRecordType := rtBoth;
    PaymentOnSelectedItems := True;
    ItemsSelectedManually := True;
    FPayment := 0;
  end
  else
  begin
    LogMessage(ltPaymentTrace, '513' + #9);
    PaymentOnSelectedItems := False;
    FRecordType := rtBoth;
    RecalculateTotals;
    LogMessage(ltPaymentTrace, '514' + #9);
    LoadTLs(LANone);
  end;

  if (DoingTablePayment) then begin
    LogMessage(ltPaymentTrace, '515' + #9);
    Exit;
  end;

  ReloadItems := True;
  try
    if not CheckForUnTenderedItems then
    begin
      LogMessage(ltPaymentTrace, '516' + #9);
      AddAllOrderLines;
    end;

    VisibleWindow := vwTender;   //set now, before RecalculateTotals
    LogMessage(ltPaymentTrace, '426' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '517' + #9);

    RefreshGoodyAccountDiscountAmount;
  finally
    if (NoItemsSelected) and CDAServer.Connected then
    begin
      CDAServer.CDAData.onetapFunction := ofTablePayment;
      CDAServer.SendCDAMessage;
    end;
    RecalculateDiscount := False;
    ReloadItems := False;
  end;
  LogMessage(ltPaymentTrace, '518' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdToPayClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '572' + #9);
  if FSelectedItemsNotPaidFull and FUseItemDiscounts then
  begin
    ShowMsg(sPayOffCurrentItemSelection);
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if ((AppDetails.PaymentDisableSplitOnTender) and (TLList.Count > 0)) then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  if (DoingTablePayment) then
  begin
    LogMessage(ltPaymentTrace, '573' + #9);
    Exit;
  end;

  LogMessage(ltPaymentTrace, '574' + #9);
  grdUnInvoicedList.RePaint;

  LogMessage(ltPaymentTrace, '575' + #9);
  LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, FToPay);

  if not (PaymentOnSelectedItems or AmountSplitingStarted) then
  begin
    LogMessage(ltPaymentTrace, '576' + #9);
    ClearAllOrderLines;

    LogMessage(ltPaymentTrace, '577' + #9);
    ClearTempItemList;
  end
  else
  begin
    LogMessage(ltPaymentTrace, '578' + #9);
    LoadSelectedToPayList;
  end;

  LogMessage(ltPaymentTrace, '579' + #9);
  RecalculateTotals;

  if (not FUseDiscountAmount) and (not (LoyaltyReward.RewardOffer in [roDiscount])) then
  begin
    LogMessage(ltPaymentTrace, '580' + #9);
    RecalculateDiscount := True;
  end;

  AmountSplitingStarted := False;
  FSelectedItemsNotPaidFull := False;
  FRecordType := rtUnTenderedItems;
  LogMessage(ltPaymentTrace, '581' + #9);
  CalculateAmountForUntenderedItems;
  PaymentOnSelectedItems := True;

  LogMessage(ltPaymentTrace, '582' + #9);
  BuildTenderSurchargeList;

  VisibleWindow := vwToPay;
  LogMessage(ltPaymentTrace, '583' + #9);
  RecalculateTotals;

  if CDAServer.Connected then
  begin
    CDAServer.CDAData.onetapFunction := ofTablePaymentByItem;
    CDAServer.SendCDAMessage;
  end;

  LogMessage(ltPaymentTrace, '584' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdHideClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '607' + #9);

  //if doing Goody sale, confirm clear all Goody details first
  if InGoodySale then
  begin
    if (ShowConf(sConfirmClearGoodyDetails) <> mrYes) then
      Exit
    else
      IniGoodyTrans;
  end;

  if (TLList.Count > 0)  then begin
    if ShowConf(sCloseWithoutCompletingTender) <> mrYes then begin
      LogMessage(ltPaymentTrace, '608' + #9);
      Exit;
    end
    else
    begin
      if EFTPOSTransExists then begin
        LogMessage(ltPaymentTrace, '609' + #9);
        ShowMsg(sCloseWithoutCompletingTender);
        Exit;
      end;

      if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
        Exit;
    end;
  end;

  LogMessage(ltPaymentTrace, '610' + #9);
  sExternalAccountIntegration.ClearPresentedCards;

  LogMessage(ltPaymentTrace, '611' + #9);
  ClearAllOrderLines;

  inherited;
end;
{******************************************************************************}
procedure TformPaymentTable.ClearAllOrderLines;
var
  OL: TOrderLine;
  I: Integer;
begin
  LogMessage(ltPaymentTrace, '272' + #9);
  for I := (OLToPayList.Count - 1) downto 0 do
  begin
    OL := OLToPayList.Items[I];
    if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (OL.LoyaltyItemID > 0) and (OL.ComboID <= 0)) then
    begin
      if (OL.TenderSeqID <= 0) then
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + OL.LoyaltyFreeQty;
    end;

    OL.Free;
    OLToPayList.Delete(I);
  end;

  //OLList.Clear;
  with grdToPay do
  begin
    RowCount := 2;
    Cells[0, 1] := '';
    Cells[1, 1] := '';
    Cells[2, 1] := '';
    Cells[3, 1] := '0.00';
    Cells[4, 1] := '0.00';
    Row := 1;
  end;
  if (not FGetAllItems) then
  begin
    LogMessage(ltPaymentTrace, '273' + #9);
    CalculateToPay();
  end;

  grdItemAutoClick := True;
  LogMessage(ltPaymentTrace, '274' + #9);
  grdToPayClick(nil);
  LogMessage(ltPaymentTrace, '275' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.ClearAllSurcharges;
var
  SurchargeItem: TSurchargeItem;
  I: Integer;

begin
  LogMessage(ltPaymentTrace, '415' + #9);
  for I := (SurchargeList.Count - 1) downto 0 do begin
    SurchargeItem := SurchargeList.Items[I];
    SurchargeItem.Free;
    SurchargeList.Delete(I);
  end;
  SaleCategorySurchargeItem := Nil;
end;
{******************************************************************************}
procedure TformPaymentTable.grdToPayClick(Sender: TObject);
var
  OL: TOrderLine;
  Due: Currency;
  Qty: Double;
begin
  LogMessage(ltPaymentTrace, '252' + #9 + 'DoCalcQty=' + BoolToStr(DoCalcQty, True) + ', grdToPay.Cells[4, grdToPay.Row]=' + grdToPay.Cells[4, grdToPay.Row]);
  if (DoingTablePayment or GridUpdating) then
  begin
    LogMessage(ltPaymentTrace, '253' + #9);
    Exit;
  end;

  //Added a ItemSplitPayment Popup
  if (not grdItemAutoClick) and (not isbtnClick) and (grdItemManualClick) and (OLToPayList.Count > 0) then
  begin
    Due := TOrderLine(OLToPayList[grdToPay.Row-1]).Due;
    Qty := TOrderLine(OLToPayList[grdToPay.Row-1]).OlQty;
    LogMessage(ltPaymentTrace, '617' + #9 + 'ItemPrice=' + FloatToStr(TOrderLine(OLToPayList[grdToPay.Row-1]).UnitPrice) + ', Due=' + FloatToStr(Due) + ', Qty=' + FloatToStr(Qty));
    if GetItemSplitPayment((TOrderLine(OLToPayList[grdToPay.Row-1]).UnitPrice), Due, Due, Qty) then
    begin
      LogMessage(ltPaymentTrace, '621' + #9 + 'ItemPrice=' + FloatToStr(TOrderLine(OLToPayList[grdToPay.Row-1]).UnitPrice) + ', Due=' + FloatToStr(Due) + ', Qty=' + FloatToStr(Qty));
      if 0 < Due then
      begin
        if txtToPay.Text = FormatCurrencyNoSign(Due) then
          txtToPay.Text := '';
        txtToPay.Text := FormatCurrencyNoSign(Due);
        LogMessage(ltPaymentTrace, '622' + #9);
      end;
      LogMessage(ltPaymentTrace, '623' + #9)
    end;
  end;

  RefreshGoodyAccountDiscountAmount;

  if (not DoCalcQty) then
    ToPayString :=  '0.00';

  JustSelected := True;
  ReloadItems := True;

  if (grdToPay.Cells[4, grdToPay.Row] = '') then begin
    grdToPay.Cells[4, grdToPay.Row] := '0.00';
  end;

  if (OLToPayList.Count > 0) then
  begin
    LogMessage(ltPaymentTrace, '254' + #9);
    DoCalcQty := True;

    OL := OLToPayList.Items[grdToPay.Row - 1];
    if not Assigned(OL) then
    begin
      LogMessage(ltPaymentTrace, '255' + #9);
      Exit;
    end;

//    txtToPay.ReadOnly := (OL.ComboID > 0);
//    txtQty.ReadOnly := (OL.ComboID > 0);
//    txtToPay.Enabled := True;
//    txtQty.Enabled := True;

    if (ToPayString = '0.00') then
      ToPayString := FormatCurrencyNoSign(OL.ToPay);
    txtToPay.Text := ToPayString;
    DoCalcQty := False;
  end
  else begin
    LogMessage(ltPaymentTrace, '256' + #9);
    DoCalcToPay := False;
    DoCalcQty := False;
    txtToPay.Text := '0.00';
    txtQty.Text := '';
//    txtToPay.Enabled := False;
//    txtQty.Enabled := False;
  end;

  ReloadItems := False;
  JustSelected := False;
  grdItemAutoClick := False;
  LogMessage(ltPaymentTrace, '257' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOLUpClick(Sender: TObject);
begin
  PostMessage(grdUnInvoicedList.Handle, WM_VSCROLL, SB_PAGEUP, 0);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOLDownClick(Sender: TObject);
begin
  PostMessage(grdUnInvoicedList.Handle, WM_VSCROLL, SB_PAGEDOWN, 0);
end;
{******************************************************************************}
procedure TformPaymentTable.txtSumToPayChange(Sender: TObject);
begin
  txtToPayCopy.Text := txtSumToPay.Text;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdToPayUpClick(Sender: TObject);
begin
  isbtnClick := True;
  PostMessage(grdToPay.Handle, WM_KEYDOWN, VK_PRIOR, 0);
  Application.ProcessMessages;
  isbtnClick := False;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdToPayDownClick(Sender: TObject);
begin
  isbtnClick := True;
  PostMessage(grdToPay.Handle, WM_KEYDOWN, VK_NEXT, 0);
  Application.ProcessMessages;
  isbtnClick := False;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdTLUpClick(Sender: TObject);
begin
  PostMessage(grdTLs.Handle, WM_KEYDOWN, VK_PRIOR, 0);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdTLDownClick(Sender: TObject);
begin
  PostMessage(grdTLs.Handle, WM_KEYDOWN, VK_NEXT, 0);
end;
{******************************************************************************}
procedure TformPaymentTable.DeselectAccount(SuppressConfirmation: Boolean=True; ConfirmationResult: TModalResult=mrNone);
var
  tmpList: TList;
  tmpTLList: TTLList;
  i: Integer;
  TL: TTL;

begin
  LogMessage(ltPaymentTrace, '444' + #9);
  if (GlbAccount.AccountID = 0) then Exit;

  tmpList := TList.Create;
  try
    begin
      if (TLList.Count = 0) or (ConfirmationResult in [mrNo]) then
      begin
        GlbAccount.Initialise;

        txtAccountName.Text := '';

        FDiscountPercent := 0.00;
        FDiscountAmount := 0;
        FUseDiscountAmount := False;
        FUseItemDiscounts := False;
        FLimitMaximum := False;    //Jon 16-12-2002
        FMaxDiscount := 0.00;      //Jon 16-12-2002
        FOrigDiscountPercent := 0.00;  //Jon 16-12-2002
        FCurrentBalance := 0.00;   //Jon 18-10-2003

        LoyaltyReward.LoyaltyRewardID := 0;
        LoyaltyReward.RewardOffer := roNone;
        LoyaltyReward.LoyaltyReward := '';
        LoyaltyReward.LoyaltyPoints1 := 0;
        LoyaltyReward.LoyaltyPoints2 := 0;
        LoyaltyReward.Qty := 0;
        FLoyaltyDiscount := 0;
        FDiscountPercent := 0.00;
        FDiscountAmount := 0;

        RecalculateDiscount := True;

        ClearItemLevelDiscounts(UnInvoicedOlsToPay);
        ClearItemLevelSubsidy(UnInvoicedOlsToPay);
        GetAllSelectedOLs(tmpList);
        ClearItemLevelDiscounts(tmpList);
        ClearItemLevelSubsidy(tmpList);
        UpdateUnInvoicedListComboDetails;
        UpdateScreenAfterCalculateDiscount;
      end
      else
      begin
        if AppDetails.EFTPOSIntegrated and EFTPOSTransExists then
        begin
          ShowMsg(sRefundTenderBeforeAcctChange, True);
          Exit;
        end
        else if ((PaymentOnSelectedItems or AmountSplitingStarted)) and (not SuppressConfirmation) then
        begin
          ConfirmationResult := (ShowConf(sConfirmClearCurrentSelection));
          if (ConfirmationResult = mrNo) then
            Exit;
        end;

        GlbAccount.Initialise;

        txtAccountName.Text := '';

        FDiscountPercent := 0.00;
        FDiscountAmount := 0;
        FUseDiscountAmount := False;
        FUseItemDiscounts := False;
        FLimitMaximum := False;    //Jon 16-12-2002
        FMaxDiscount := 0.00;      //Jon 16-12-2002
        FOrigDiscountPercent := 0.00;  //Jon 16-12-2002
        FCurrentBalance := 0.00;   //Jon 18-10-2003

        LoyaltyReward.LoyaltyRewardID := 0;
        LoyaltyReward.RewardOffer := roNone;
        LoyaltyReward.LoyaltyReward := '';
        LoyaltyReward.LoyaltyPoints1 := 0;
        LoyaltyReward.LoyaltyPoints2 := 0;
        LoyaltyReward.Qty := 0;
        FLoyaltyDiscount := 0;
        FDiscountPercent := 0.00;
        FDiscountAmount := 0;

        RecalculateDiscount := True;

        if (ConfirmationResult = mrYes) then
        begin   
          tmpTLList:= TLList;
          TLList := TTLList.Create;
          ClearAllTenderLines;        
          for i := 0 to tmpTLList.Count - 1 do
          begin
            TL := tmpTLList.Items[i];
            DeleteOLsOfTender(TL, TL.TenderSeqNo); //so OLs become part of UnInvoicedOlsToPay
          end;
          PaymentOnSelectedItems := False;
          AmountSplitingStarted := False;
          FSelectedItemsNotPaidFull := False;

          ClearItemLevelDiscounts(UnInvoicedOlsToPay);
          ClearItemLevelSubsidy(UnInvoicedOlsToPay);
          LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, FToPay);
          AddAllOrderLines;
          GetAllSelectedOLs(tmpList);
          //ClearItemLevelDiscounts(tmpList);
          //ClearItemLevelSubsidy(tmpList);
          UpdateUnInvoicedListComboDetails;   //put combo discounts back on both uninvoiced and topay lists

          //do separately to keep same TL order in TLList
          for i := 0 to tmpTLList.Count - 1 do
          begin
            TL := tmpTLList.Items[i];
            LogMessage(ltPaymentTrace, '31' + #9);
            AddTenderLine(TL.TLType, 0, TL);
            LogMessage(ltPaymentTrace, '32' + #9);
          end;
          for i := tmpTLList.Count - 1 downto 0 do
          begin
            tmpTLList.Delete(i);  //do this, so when freeing tmpTLList we dont free the TL objects just added to TLList
          end;

          UpdateScreenAfterCalculateDiscount;
          tmpTLList.Free;
        end;
      end;

      LogMessage(ltPaymentTrace, '427' + #9);
      lblAllowItemLevelDiscount.Caption := '';
      CalculateToPay;
      LogMessage(ltPaymentTrace, '533' + #9);
      ChangeAllowCreditLabel(False, False);
    end;
  finally
    tmpList.Clear;
    tmpList.Free;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.InitialiseAccountInfo(AShowAcctScanned: Boolean = True; PromptLoyalty: Boolean = True; VerifyNo: Boolean = True);
begin
  LogMessage(ltPaymentTrace, '450' + #9);
  DeselectAccount;
  cmdDiscountPercent.Enabled := False;
  cmdDiscountAmount.Enabled := False;

  if (GlbTable.AccountID > 0) then
  begin
    TryLookupPaymentTableAccount(GlbTable.AccountID, AShowAcctScanned, PromptLoyalty, VerifyNO);
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.BuildAccountButtons;
const
  DEFBUTTON_WIDTH = 76;
  DEFBUTTON_HEIGHT = 48;

var
  cmdAccount: TfcCustomImageBtn;
  ButtonSkin: TButtonSkin;
  I: Integer;
  TheLine: Integer;
  NumberPerRow: Integer;
  ButtonWidth: Integer;
  ButtonHeight: Integer;

begin
  //clear any exixting controls in scrAccounts
  while (scrAccounts.ControlCount > 0) do begin
    scrAccounts.Controls[0].Free;
  end;

  I := 0;
  TheLine := 0;
  ButtonSkin := Skin.GetButtonSkin(bstAccount);
  if ((Assigned(ButtonSkin)) and (ButtonSkin.WidthSet)) then begin
    ButtonWidth := ButtonSkin.Width;
  end
  else begin
    ButtonWidth := DEFBUTTON_WIDTH;
  end;
  if ((Assigned(ButtonSkin)) and (ButtonSkin.HeightSet)) then begin
    ButtonHeight := ButtonSkin.Height;
  end
  else begin
    ButtonHeight := DEFBUTTON_HEIGHT;
  end;

  if (Screen.Width > 800) then begin
    NumberPerRow := ((Screen.Width - pnlTenderLines.Width - 20) div ButtonWidth);
  end
  else begin
    NumberPerRow := 5;
  end;

  with dm.qrAccountButtons do begin
    First;
    while (not EOF) do begin
      if (not FieldAsBool(FieldByName('accinactive'))) then begin
        Skin.CreateNewButton(cmdAccount, TComponent(scrAccounts), TWinControl(scrAccounts), bstAccount);

        cmdAccount.Font.Name := 'Arial';
        cmdAccount.Font.Style := [fsBold];
        cmdAccount.Font.Size := 8;

        cmdAccount.Caption := DoubleAmpersand(FieldByName('buttoncaption').AsString);
        cmdAccount.Tag := FieldByName('accountid').AsInteger;
        cmdAccount.OnClick := AccountClick;
        if ((not Assigned(ButtonSkin)) or (not ButtonSkin.WidthSet)) then begin
          cmdAccount.Width := ButtonWidth;
        end;
        if ((not Assigned(ButtonSkin)) or (not ButtonSkin.HeightSet)) then begin
          cmdAccount.Height := ButtonHeight;
        end;

        cmdAccount.Top := TheLine * ButtonHeight;
        cmdAccount.Left := (I mod NumberPerRow) * ButtonWidth;

        Inc(I);
        if ((I mod NumberPerRow) = 0) then begin
          Inc(TheLine);
        end;
      end;
      Next;
    end;
  end;
end;
{******************************************************************************}
{ if user presses cancel we can now decide not to recalculate discounts}
function TformPaymentTable.CheckLoyaltyReward(Prompt: Boolean): Boolean;
begin
  LogMessage(ltPaymentTrace, '448' + #9);
  Result := True;

  if IsNormalLoyalty then
  begin
    if (Prompt) then
    begin
      LoyaltyReward.LoyaltyRewardID := 0;
      LoyaltyReward.RewardOffer := roNone;
    end;

    if ((not Prompt) or
        (((GlbAccount.AccountID > 0) and (GlbAccount.BestLoyaltyRewardID > 0)) and
         (ShowConf(sReviewLoyaltyRewardOffers) = mrYes))) then
    begin
      if (SelectLoyaltyReward(LoyaltyReward, [roVoucher, roCredit, roDiscount, roItem, roVariableDiscount])) then
      begin
        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roCredit)) then
        begin
          GlbAccount.CurrentBalance := GlbAccount.CurrentBalance - LoyaltyReward.Qty;
          FCurrentBalance := GlbAccount.CurrentBalance;
        end;
      end
      else
        Result := False;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDeselectAccountClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '596' + #9);
  if (DoingTablePayment) then begin
    Exit;
  end;
  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    Exit;
  end;
  
  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    Exit;
  end;

  if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
    if TLList.Count > 0 then
      Exit;

  DeselectAccount(False, mrYes);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdUpAccountsClick(Sender: TObject);
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  try
    if ((scrAccounts.VertScrollBar.Position - scrAccounts.Height) > 0) then begin
      scrAccounts.VertScrollBar.Position := scrAccounts.VertScrollBar.Position - scrAccounts.Height;
    end
    else begin
      scrAccounts.VertScrollBar.Position := 0;
    end;
  except; end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDownAccountsClick(Sender: TObject);
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  try
    if ((scrAccounts.VertScrollBar.Position + scrAccounts.Height) < scrAccounts.VertScrollBar.Range) then begin
      scrAccounts.VertScrollBar.Position := scrAccounts.VertScrollBar.Position + scrAccounts.Height;
    end
    else begin
      scrAccounts.VertScrollBar.Position := scrAccounts.VertScrollBar.Range;
    end;
  except; end;
end;
{******************************************************************************}
function TformPaymentTable.CanSelectAccount: Boolean;
begin
  Result := False;

  if not sExternalAccountIntegration.AllowOnetapAccounts then
    Exit;

  if (AppDetails.EFTPOSIntegrated and EFTPOSTransExists) then
  begin
    ShowMsg(sRefundTenderBeforeAcctChange);
    Exit;
  end;

  //if login pin not already entered for account priviledges
  if ((not ManagerAuthorise.Authorise) or (ManagerAuthorise.AuthoriseAccountRank = 0)) then begin
    //check if have privilege
    if (GlbLogin.RankAccount = 0) then begin
      ShowMsg(sGetHelpToPayThruCustAccts);
      Exit;
    end;
  end;

  {if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
  if TLList.Count > 0 then
    Exit;}

  //if AppDetails.wiGroupEnabled then
  //  wiGroupPOSSrv.CheckWarnAccDiscNotAllowed;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformOTAcctNotAllowed(True, True) then
    Exit;

  Result := True;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdSelectAccountClick(Sender: TObject);
var
  prevAccountID: Integer;
begin
  LogMessage(ltPaymentTrace, '409' + #9);
  if not sExternalAccountIntegration.AllowOnetapAccounts then
    Exit;

  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    Exit;
  end;

  if (CanSelectAccount) then
  begin //Select Account checks if can access account
    prevAccountID := GlbAccount.AccountID;
    if (SelectAccount(True, True)) then
    begin
      if (GlbAccount.AccountID > 0) then
      begin
        If (GlbAccount.AccountInactive = accInactive) then
        begin
          ShowMsg(sAccountInactive);
          if (prevAccountID = 0) then
            DeselectAccount
          else
            GlbAccount.LoadAccount(prevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
          Exit;
        end;

        if not GetSelectedAccount(GlbAccount.AccountID, False, True, prevAccountID, True) then
          Exit;
        BroadcastCDAMessage;
      end
      else begin
        if (prevAccountID = 0) then
          DeselectAccount
        else
          GlbAccount.LoadAccount(prevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.AccountClick(Sender: TObject);
var
  prevAccountID: Integer;
begin
  LogMessage(ltPaymentTrace, '410' + #9);
  if (DoingTablePayment) then begin
    Exit;
  end;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    Exit;
  end;

  try

    //if in a Goody discounted sale, disallow loading a different account
    if InGoodyDiscountedSale then
    begin
      ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
      Exit;
    end;

    if (CanSelectAccount) then
    begin
      with dm.qrAccountButtons do
      begin
        KeyFieldByName('accountid').AsInteger := TfcCustomImageBtn(Sender).Tag;
        if (not LookupKeyForFields) then
        begin
          ShowQuickMsg(sCannotFindCustAcct);
          //DeselectAccount;  //lets leave current selected account as is
          Exit;
        end
        else
        begin
          GlbAccount.LookupAccountID := FieldByName('accountid').AsInteger;
          prevAccountID := GlbAccount.AccountID;

          ResetLoyaltyReward;

          if not GetSelectedAccount(GlbAccount.LookupAccountID, False, True, prevAccountID, True) then
            Exit;    
        end;
      end;
    end;
  finally
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDiscountPercentClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '597' + #9);
  if (DoingTablePayment) then begin
    Exit;
  end;

  //if in a Goody discounted sale, don't allow discount to be changed
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sCannotChangeGoodyDiscAmt);
    Exit;
  end;

  if (GlbAccount.AccountID = 0) then begin
    ShowQuickMsg(sSelectCustAcct);
    Exit;
  end;

  if (AppDetails.OneBill and (TLList.Count > 0)) then
  begin
    Exit;  //for now, don't allow discount to be manually changed after the first tender is made
  end;

  if ((GlbPrivs[13].AvoidPINCheck) or (GlbLogin.LoginPIN <> 0) or (VerifyPinNo)) then
  begin
    if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode) )) then
    if TLList.Count > 0 then
      Exit;

    if (GetDiscountPercent(FDiscountPercent)) then
    begin
      FOrigDiscountPercent := FDiscountPercent;
      FUseItemDiscounts := False;
      FUseDiscountAmount := False;
      RecalculateDiscount := True;
      LogMessage(ltPaymentTrace, '598' + #9);
      CalculateAccTypeItemDiscount(UnInvoicedOlsToPay);

      LogMessage(ltPaymentTrace, '599' + #9);
      UpdateScreenAfterCalculateDiscount;

      LogMessage(ltPaymentTrace, '593' + #9);
      lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;

      LogMessage(ltPaymentTrace, '428' + #9);
      CalculateToPay;
      LogMessage(ltPaymentTrace, '519' + #9);
      AccountChange := True;// this is just to show the max limit mesage
      RecalculateDiscount := False;
    end;
  end
  else begin
    exit;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdDiscountAmountClick(Sender: TObject);
var
  i, j: Integer;
  SOL, OL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '600' + #9);
  if (DoingTablePayment) then begin
    Exit;
  end;

  //if in a Goody discounted sale, don't allow discount to be changed
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sCannotChangeGoodyDiscAmt);
    Exit;
  end;

  if (GlbAccount.AccountID = 0) then begin
    ShowQuickMsg(sSelectCustAcct);
    Exit;
  end;

  if (AppDetails.OneBill and (TLList.Count > 0)) then
  begin
    Exit;  //for now, don't allow discount to be manually changed after the first tender is made
  end;

  if ((GlbPrivs[13].AvoidPINCheck) or (GlbLogin.LoginPIN <> 0) or (VerifyPinNo)) then
  begin
    if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
    if TLList.Count > 0 then
      Exit;

    if GetDiscountAmount(FToPay, FDiscountAmount) then
    begin
      if (FLimitMaximum) then
      begin
        if (FDiscountAmount > FMaxDiscount) then
        begin
          ShowQuickMsg(Format(sAcctLimit, [FormatCurrencySign(FMaxDiscount)]));
          FDiscountAmount := FMaxDiscount;
          FUseDiscountAmount := True;
          LogMessage(ltPaymentTrace, '601' + #9 + 'FMaxDiscount=' + FloatToStr(FMaxDiscount));
        end;
      end;
      FOrigDiscountAmount := FDiscountAmount;
      FUseDiscountAmount := True;
      FUseItemDiscounts := False;
      RecalculateDiscount := True;
      LogMessage(ltPaymentTrace, '594' + #9);
      lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;
      ClearItemLevelDiscounts(UnInvoicedOlsToPay);
      if OLToPayList.Count = 0 then
      begin
        LogMessage(ltPaymentTrace, '602' + #9);
        CalculateAccTypeItemDiscount(UnInvoicedOlsToPay);

        LogMessage(ltPaymentTrace, '603' + #9);
        UpdateScreenAfterCalculateDiscount;
      end;

      FDiscCalculator.OneBillDiscountAmount := FDiscountAmount;
      if (AppDetails.OneBill) then
      begin
        LogMessage(ltPaymentTrace, '604' + #9);
        ApplyItemLevelOneBillDiscountAmount;
      end;

      LogMessage(ltPaymentTrace, '429' + #9);
      CalculateToPay;
      LogMessage(ltPaymentTrace, '520' + #9);
      RecalculateDiscount := False;

      if (not AppDetails.OneBill) then
      begin
        for i:= 0 to OLToPayList.Count-1 do
        begin
          SOL := TOrderLine(OLToPayList.Items[i]);
          for j:= 0 to UnInvoicedOlsToPay.Count-1 do
          begin
            OL := TOrderLine(UnInvoicedOlsToPay.Items[j]);
            if (SOL.OrderLineID = OL.OrderLineID) then
            begin
              OL.AllowedDiscount := SOL.AllowedDiscount;
              LogMessage(ltPaymentTrace, '605' + #9 + 'OL.OrderLineID=' + IntToStr(OL.OrderLineID) + ', OL.AllowedDiscount=' + FloatToStr(OL.AllowedDiscount));
              break;
            end;
          end;
        end;
      end;

    end;
  end
  else begin
    exit;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.ApplyItemLevelOneBillDiscountAmount;
var
  fAllowedDiscount: Currency;
  OL, selectedOL: TOrderLine;
  i, j: Integer;
  DiscountPerOrderline: Currency;
  RemainingDiscountAmount: Currency;
begin
  LogMessage(ltPaymentTrace, '472' + #9);
  //FUseDiscountAmount should always be True, thus no need to work out Discount Scheme or Loyalty schemes.
  
  if ((GlbAccount.LimitMaximum) and (FDiscountAmount > GlbAccount.MaxDiscount)) then
    RemainingDiscountAmount := GlbAccount.MaxDiscount
  else
    RemainingDiscountAmount := FDiscountAmount;

  FDiscountAmount := 0;

    for i := 0 to (UnInvoicedOlsToPay.Count - 1) do
    begin
      DiscountPerOrderline := GetRoundedUpDown(RemainingDiscountAmount / (UnInvoicedOlsToPay.Count-i), AppDetails.DecimalPlaces);
      OL := UnInvoicedOlsToPay.Items[i];
      if ((OL.ComboID > 0) and (OL.ComboGroupID > 0) and (OL.ComboIndex > 0)) then
      begin
        Continue;
      end
      else
      begin
        if (DiscountPerOrderline > OL.ToPay) then
        begin
          fAllowedDiscount := OL.ToPay;
          if AppDetails.EnableFiscalPrinting then
            fAllowedDiscount := OL.ToPay - 0.01; // in order to avoid 100 % discount on fiscal items
        end
        else
          fAllowedDiscount := DiscountPerOrderline;

        RemainingDiscountAmount := RemainingDiscountAmount - fAllowedDiscount;

        OL.AllowedDiscount := fAllowedDiscount;
        for j:=0 to OLToPayList.Count-1 do
        begin
          selectedOL := TOrderLine(OLToPayList.Items[j]);
          if ((Assigned(OL) and Assigned(selectedOL)) and
            (OL.OrderLineID = selectedOL.OrderLineID)) then
          begin
            selectedOL.AllowedDiscount := fAllowedDiscount;
            FDiscountAmount := FDiscountAmount + fAllowedDiscount;
            break;
          end;
        end;
      end;
    end;
end;

{******************************************************************************}
procedure TformPaymentTable.cmdAmountToPayClick(Sender: TObject);
var
  OldQty, AmountPaid, AmountToPay, Allocated: Currency;
  i: Integer;
  aOL: TOrderLine;
  ToPayChanged: Boolean;
  AList: TList;

  function CalculateAmountToPay: Currency;
  var
    j: Integer;
  begin
    LogMessage(ltPaymentTrace, '477' + #9);
    Result := 0;
    if (Assigned(AList)) then
    begin
      for j := 0 to (AList.Count - 1) do
      begin
        aOL := AList.Items[j];
        LogMessage(ltPaymentTrace, '478' + #9 + 'aOL.ToPay=' + FloatToStr(aOL.ToPay));
        Result := Result + aOL.ToPay;
      end;
    end;
  end;
begin
  LogMessage(ltPaymentTrace, '475' + #9);
  if (DoingTablePayment) then begin
    LogMessage(ltPaymentTrace, '476' + #9);
    Exit;
  end;

  if FSelectedItemsNotPaidFull then
  begin
    ShowMsg(sPayOffCurrentItemSelection);
    Exit;
  end;

  if ((AppDetails.PaymentDisableSplitOnTender) and (TLList.Count > 0)) then
    Exit;

  if GlbEvent.EventID > 0 then
  begin
    ShowMsg(sNoAmountSplitForEventTransaction);
    Exit;
  end;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  if not (PaymentOnSelectedItems or AmountSplitingStarted) then
  begin
    LogMessage(ltPaymentTrace, '479' + #9);
    ClearTempItemList;
  end;

  if PaymentOnSelectedItems then
  begin
    LogMessage(ltPaymentTrace, '480' + #9);
    ClearTempItemList;

    LogMessage(ltPaymentTrace, '481' + #9);
    CopyItemSToTempList;
    AList := TempItemList;
  end
  else
  begin
    LogMessage(ltPaymentTrace, '482' + #9);
    AList := UnInvoicedOlsToPay;
  end;

  //here you have to get total amount - already paid amount to split the amount again for the second time. For first split, FTOPAY is correct.
  AmountToPay := CalculateAmountToPay;
  LogMessage(ltPaymentTrace, '483' + #9 + 'AmountToPay=' + FloatToStr(AmountToPay));

  if (GetAmountToPay (AmountToPay, AmountToPay, TLList.Count > 0)) then
  begin
    LogMessage(ltPaymentTrace, '484' + #9 + 'AmountToPay=' + FloatToStr(AmountToPay));
    ItemsSelectedManually := True;

    if AmountToPay = -999 then // clear out the due amount
    begin
      FRecordType := rtTenderedItems;
      LogMessage(ltPaymentTrace, '430' + #9);
      CalculateToPay;
      LogMessage(ltPaymentTrace, '521' + #9);
      FRecordType := rtBoth;
      Exit;
    end;

    AmountSplitingStarted := True;
    LogMessage(ltPaymentTrace, '485' + #9);
    ClearAllOrderLines;
    FPayment := 0;

    i := 0;
    Allocated := 0.00;

    LogMessage(ltPaymentTrace, '486' + #9 + 'AList.Count=' + IntToStr(AList.Count));
    while (i <= AList.Count - 1) and (Allocated < AmountToPay) do
    begin
      aOL := AList.Items[i];
      LogMessage(ltPaymentTrace, '487' + #9 + 'i=' + IntToStr(i) + ', Allocated=' + FloatToStr(Allocated) + ', aOL.Qty=' + FloatToStr(aOL.Qty));

      if aOL.Qty > 0 then
      begin
        if AddOrderLine(AList, i) then
        begin
          aOL := OLToPayList.Items[grdToPay.Row - 1];
          LogMessage(ltPaymentTrace, '488' + #9 + 'aOL.Qty=' + FloatToStr(aOL.Qty) + ', aOL.QtyChanged=' + BoolToStr(aOL.QtyChanged, True) + ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice) +
            ', aOL.ToPay='+ FloatToStr(aOL.ToPay) + ', aOL.SalesTaxPercent='+ FloatToStr(aOL.SalesTaxPercent) + ', aOL.AllowedDiscount=' + FloatToStr(aOL.AllowedDiscount) +
            ', aOL.LoyaltyFreeQty='+ FloatToStr(aOL.LoyaltyFreeQty) + ', aOL.LoyaltyFreeDiscount=' + FloatToStr(aOL.LoyaltyFreeDiscount));

          ToPayChanged := False;
          begin
            if (AppDetails.TaxExclusivePrices) then
            begin
              LogMessage(ltPaymentTrace, '489' + #9);
              if ((Allocated + ((aOL.ToPay * (100 + aOL.SalesTaxPercent)) / 100)) > AmountToPay) then
              begin
                LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
                aOL.ToPay := ((AmountToPay - Allocated) * 100) / (100 + aOL.SalesTaxPercent);
                ToPayChanged := True;
                LogMessage(ltPaymentTrace, '490' + #9 + 'LoyaltyReward.AvailableQty=' + FloatToStr(LoyaltyReward.AvailableQty) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay));
              end;
              Allocated := Allocated + ((aOL.ToPay * (100 + aOL.SalesTaxPercent)) / 100);
              LogMessage(ltPaymentTrace, '491' + #9 + 'Allocated=' + FloatToStr(Allocated));
            end
            else
            begin
              AmountPaid := (Allocated + aOL.ToPay);
              LogMessage(ltPaymentTrace, '492' + #9 + 'AmountPaid=' + FloatToStr(AmountPaid));
              if (AmountPaid > AmountToPay) then begin
                LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
                aOL.ToPay := (AmountToPay - Allocated);
                ToPayChanged := True;
                LogMessage(ltPaymentTrace, '493' + #9 + 'LoyaltyReward.AvailableQty=' + FloatToStr(LoyaltyReward.AvailableQty) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay));
              end;
              Allocated := Allocated + aOL.ToPay;
              LogMessage(ltPaymentTrace, '494' + #9 + 'Allocated=' + FloatToStr(Allocated));
            end;
          end;

          if (ToPayChanged) then
          begin
            aOL.QtyChanged := True;
            OldQty := aOL.Qty;
            if (AppDetails.EnableFiscalPrinting) then
              aOL.ToPay := TruncateTo(aOL.ToPay, 2)
            else
              aOL.ToPay := CRoundTo(aOL.ToPay, 2);

            if (aOL.UnitPrice > 0) then begin
              aOL.Qty := CRoundTo(aOL.ToPay / aOL.UnitPrice, 4);
            end
            else begin
              aOL.Qty := 0;
            end;

            aOL.AllowedDiscount := GetRoundedUpDown(aOL.AllowedDiscount * (aOL.Qty/OldQty), AppDetails.DecimalPlaces) ;
            LogMessage(ltPaymentTrace, '495' + #9 + 'aOL.Qty=' + FloatToStr(aOL.Qty) + ', aOL.QtyChanged=' + BoolToStr(aOL.QtyChanged, True) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay) +
              ', aOL.AllowedDiscount=' + FloatToStr(aOL.AllowedDiscount));

            if (LoyaltyReward.RewardOffer = roDiscount) and (LoyaltyReward.Processed) then
            begin
              aOL.LoyaltyFreeDiscount := GetRoundedUpDown(aOL.LoyaltyFreeDiscount * (aOL.Qty/OldQty), AppDetails.DecimalPlaces) ;
              LogMessage(ltPaymentTrace, '496' + #9 + 'aOL.LoyaltyFreeDiscount=' + FloatToStr(aOL.LoyaltyFreeDiscount));
            end;

            AllocateItemLoyalty(aOL);
            LogMessage(ltPaymentTrace, '497' + #9);
            UpdateGrdToPayCells(grdToPay.Row);
            LogMessage(ltPaymentTrace, '565' + #9);
          end;
        end;
      end;

      inc(i);
    end;

    FRecordType := rtUnTenderedItems;
    FPayment := 0;
    txtPayment.Text := FormatCurrencyNoSign(FPayment);

    LogMessage(ltPaymentTrace, '431' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '498' + #9);
    FRecordType := rtBoth;
    RecalculateDiscount := False;
  end;
  BroadcastCDAMessage;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdAddPosClick(Sender: TObject);
var
  i, j: Integer;
  IncludePos: Boolean;
  ThePos: TSelectedPos;
  aOL: TOrderLine;
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;
  
  if (OLToPayList.Count > 0) then begin // JEH 15/5/1
    if (ShowConf(sClearAllFirst) = mrYes) then begin
      ClearAllOrderLines;
    end;
  end;

  if (GetSelectPos(IncludePos, ThePos) = mrOk) then begin
    Update;

    for i := 0 to UnInvoicedOlsToPay.Count - 1 do
    begin
      aOL := UnInvoicedOlsToPay.Items[i];
      for j := 0 to 19 do
      begin
        if (ThePos[j] <> '') and (aOL.Positions = ThePos[j]) then
        begin
          AddOrderLine(UnInvoicedOlsToPay, i);
          Break;
        end;
      end;
    end;
    grdItemAutoClick := True;
    grdToPay.Row := 1;
    LogMessage(ltPaymentTrace, '432' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '522' + #9);
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdAccountNoLookupClick(Sender: TObject);
var
  tmpList: TList;
  prevAccountID: Integer;
begin
  LogMessage(ltPaymentTrace, '411' + #9);

  if not sExternalAccountIntegration.AllowOnetapAccounts then
    Exit;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    Exit;
  end;

  tmpList := TList.Create;
  ResetLoyaltyReward;
  FDiscountPercent := 0;
  FDiscountAmount := 0;
  txtDiscountPercent.Text := '';
  txtDiscountAmount.Text := '';

  try
    if (DoingTablePayment) then begin   //Jon 25-02-2002
      Exit;
    end;

    if (CanSelectAccount) then
    begin
      if (AccountNoLookup) then
      begin
        prevAccountID := GlbAccount.AccountID;

        if not GetSelectedAccount(GlbAccount.LookupAccountID, False, True, prevAccountID, True) then
          Exit;
      end;
    end;
  finally
    tmpList.Clear;
    tmpList.Free;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.TryLookupPaymentTableAccount(AccountID: Integer; AShowAcctScanned: Boolean = True; PromptLoyalty: Boolean = True; VerifyNO: Boolean = True);
var
  tmpList: TList;
  prevAccountID: Integer;

begin
  LogMessage(ltPaymentTrace, '412' + #9);
  tmpList := TList.Create;
  ResetLoyaltyReward;
  FDiscountPercent := 0;
  FDiscountAmount := 0;
  txtDiscountPercent.Text := '';
  txtDiscountAmount.Text := '';

  try
    if (CanSelectAccount) then
    begin //make sure accounts query set up
      prevAccountID := GlbAccount.AccountID;
      if not GetSelectedAccount(AccountID, AShowAcctScanned, PromptLoyalty, prevAccountID, VerifyNO) then
      begin
        if (AShowAcctScanned and (Trim(GlbScan.ScannedCode) <> '')) then
          ShowQuickMsg(Format(sCustScctLookup, [GlbScan.ScannedCode]));

        GlbAccount.AccountID := GlbTable.AccountID;
      end;
      BroadcastCDAMessage;
    end;
  finally
    tmpList.Clear;
    tmpList.Free;
  end;
end;

{******************************************************************************}
function TformPaymentTable.GetDue(IncludeExternalTenders: Boolean = True): Currency;
var
  i: Integer;
  AlreadyPaying: Currency;
  TL: TTL;
  bAddTender, ItemsSelected: Boolean;
begin
  LogMessage(ltPaymentTrace, '265' + #9 + 'FToPay=' + FloatToStr(FToPay) + ', FSurcharge=' + FloatToStr(FSurcharge) + ', FDiscountAmount=' + FloatToStr(FDiscountAmount) +
    ', FComboDiscount=' + FloatToStr(FComboDiscount) + ', FLoyaltyFreeItemDiscount=' + FloatToStr(FLoyaltyFreeItemDiscount) + ', FSalesTax=' + FloatToStr(FSalesTax) +
    ', FTotalSubsidyAllowed=' + FloatToStr(FTotalSubsidyAllowed) + ', FHostSubsidy=' + FloatToStr(FHostSubsidy) +
    ', GlbEvent.EventID=' + IntToStr(GlbEvent.EventID) + ', SubsidyCalc.IsHostTransaction=' + BoolToStr(SubsidyCalc.IsHostTransaction, True));

  //calculate amount towards invoice(s) already paying
  AlreadyPaying := 0;

  ItemsSelected := PaymentOnSelectedItems or FSelectedItemsNotPaidFull or AmountSplitingStarted;

  if (((FRecordType in [rtBoth, rtTenderedItems]) or (ItemsSelected)) and (TLList.Count > 0)) then
  begin
    LogMessage(ltPaymentTrace, '266' + #9);
    for I := 0 to (TLList.Count - 1) do
    begin
      TL := TLList.Items[I];
      if ((TL.TLType = TLTExternalProviders) and (not IncludeExternalTenders)) then
        Continue;
      bAddTender := (((not ItemsSelected ) or (ItemsSelected and (FTenderGrpID = TL.TenderGroupID))));
      if bAddTender then
        AlreadyPaying := AlreadyPaying + TL.TLPayment;
    end;
  end;
  FGroupPayment := AlreadyPaying;
  LogMessage(ltPaymentTrace, '267' + #9 + 'AlreadyPaying=' + FloatToStr(AlreadyPaying));

  if ((not AppDetails.TaxExclusivePrices) and ((FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount - AlreadyPaying + FTotalSubsidyAllowed) > 0)) then
  begin
    if ((GlbEvent.EventID <= 0) or ( SubsidyCalc.IsHostTransaction)) then
      Result := FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount - AlreadyPaying - FTotalSubsidyAllowed
    else
      Result := FToPay + FSurcharge  - FComboDiscount - FLoyaltyFreeItemDiscount - AlreadyPaying-FHostSubsidy
  end
  else if ((AppDetails.TaxExclusivePrices) and ((FToPay + FSurcharge - FDiscountAmount -  FComboDiscount - FLoyaltyFreeItemDiscount + FTotalSubsidyAllowed + FSalesTax - AlreadyPaying) > 0)) then
  begin
    if ((GlbEvent.EventID <= 0) or ( SubsidyCalc.IsHostTransaction)) then
      Result := FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount + FSalesTax - AlreadyPaying - FTotalSubsidyAllowed
    else
      Result := FToPay + FSurcharge  - FComboDiscount - FLoyaltyFreeItemDiscount + FSalesTax - AlreadyPaying - FHostSubsidy
  end
  else
  begin
    Result := 0.00;
  end;

  Result := GetRoundedUpDown(Result, AppDetails.DecimalPlaces);
  LogMessage(ltPaymentTrace, '268' + #9 + 'Result=' + FloatToStr(Result));
end;

{******************************************************************************}
procedure TformPaymentTable.cmdEditClick(Sender: TObject);
var
  TL: TTL;
begin
  if ((DoingTablePayment) or (TLList.Count = 0)) then begin   //Jon 25-02-2002
    Exit;
  end;

  if AmountSplitingStarted then
  begin
    ShowMsg(sEdtTenderNotAllowedForAMoutSplit);
    Exit;
  end;

  if PaymentOnSelectedItems then
  begin
    ShowMsg(sEdtTenderNotAllowedForItemSelection);
    Exit;
  end;

  //if EFTPOST integrated check that not an EFTPOS line, if is showmsg, then exit
  if AppDetails.EftposIntegrated then
  begin
    TL := TLList.Items[grdTLs.Row-1];
    if (TL.TLType in [TLTEFTPOS, TLTVisaEFTPOS, TLTAmexEFTPOS, TLTDinersEFTPOS, TLTMasterCardEFTPOS, TLTBarterCard]) then begin
      ShowMsg(sCannotEditEftPosTenderLine);
      Exit;
    end;
  end;

  //cannot edit a wiGroup tender line
  if AppDetails.wiGroupEnabled then
  begin
    TL := TLList.Items[grdTLs.Row-1];
    if TL.TLType = TLTwiGroup then
    begin
      ShowMsg(Format(sCannotEditwiGroupTenderLine, [swiGroupTL]));
      Exit;
    end;
  end;

  if (TTL(TLList.Items[grdTLs.Row-1]).TLType = TLTExternalProviders) then
    ShowExternalAccounts(TTL(TLList.Items[grdTLs.Row-1]))
  else
    EditTenderLine;
end;
{******************************************************************************}
procedure TformPaymentTable.LoadTLs(LAction: TLA);
var
  I: Integer;
  TL: TTL;
begin
  LogMessage(ltPaymentTrace, '258' + #9 + 'LAction=' + IntToStr(Ord(LAction)));
  with grdTLs do
  begin
    if (TLList.Count = 0) then
    begin
      LogMessage(ltPaymentTrace, '259' + #9);
      RowCount := 2;
      Cells[0, 1] := '';
      Cells[1, 1] := '';
      Cells[2, 1] := '';
      Cells[3, 1] := '';
      Cells[4, 1] := '';
      Cells[5, 1] := '';
      Row := 1;
    end
    else
    begin
      LogMessage(ltPaymentTrace, '260' + #9);
      RowCount := (TLList.Count + 1);
      for I := 0 to (TLList.Count - 1) do begin
        TL := TLList.Items[I];
        TL.PaymentType := ptNormal;
        if AmountSplitingStarted then
        begin
          TL.PaymentType := ptAmountSplit;
        end
        else if PaymentOnSelectedItems then
        begin
          TL.PaymentType := ptItemsSelected;
        end;

        LogMessage(ltPaymentTrace, '261' + #9 + 'TL.PaymentType=' + IntToStr(Ord(TL.PaymentType)) + ', TL.TLType=' + IntToStr(Ord(TL.TLType)) +
          ', TL.TLPayment=' + FloatToStr(TL.TLPayment) + ', TL.TLTip=' + FloatToStr(TL.TLTip) + ', TL.TLTender=' + FloatToStr(TL.TLTender) +
          ', TL.TLChange=' + FloatToStr(TL.TLChange) + ', TL.TLProvider=' + TL.TLProvider + ', TL.TLNotes=' + TL.TLNotes);

        if (TL.TLType = TLTExternalProviders) and (TL.TLProvider <> '') then
          Cells[0, I+1] := TL.TLProvider
        else
          Cells[0, I+1] := GetTLTType(TL.TLType);
        Cells[1, i+1] := TL.TLNotes;
        Cells[2, i+1] := FormatCurrencyNoSign(TL.TLPayment);
        Cells[3, i+1] := FormatCurrencyNoSign(TL.TLTip);
        Cells[4, i+1] := FormatCurrencyNoSign(TL.TLTender);
        Cells[5, i+1] := FormatCurrencyNoSign(TL.TLChange);
      end;
    end;
    if LAction = LAAdd then Row := RowCount-1;
  end;

  LogMessage(ltPaymentTrace, '262' + #9);
  BuildTenderSurchargeList;

  LogMessage(ltPaymentTrace, '263' + #9);
  RecalculateTotals;
  LogMessage(ltPaymentTrace, '264' + #9);
end;
{******************************************************************************}
function TformPaymentTable.EFTPOSTransExists: Boolean;
var
  i, iCount, iErr: Integer;
  TL: TTL;
begin
  LogMessage(ltPaymentTrace, '613' + #9);
  Result := False;

  //if EFTPOS integrated check that there are NO EFTPOS lines, if is showmsg, then exit
  if (AppDetails.EftposIntegrated) then begin
    Result := TLList.EFTPOSTransExists;
  end;

  if Result and AppDetails.EnableFiscalPrinting then
  if (TLList.Count > 0) then
  begin
    SWBEncryption.AuxiliaryData.TEFReferences := '';
    for I := 0 to (TLList.Count - 1) do
    begin
      TL := TLList.Items[I];
      if (TL.IsEFTPOSTrans) then
      begin
        Val(SWBEncryption.AuxiliaryData.TEFCount, iCount, iErr);
        Inc(iCount);
        SWBEncryption.AuxiliaryData.TEFCount := IntToStr(iCount);
        if Trim(SWBEncryption.AuxiliaryData.TEFReferences) = '' then
          SWBEncryption.AuxiliaryData.TEFReferences := Trim(TL.EFTPOSTxnRef) //+ ':' + Trim(TL.NSUNumber)
        else
          SWBEncryption.AuxiliaryData.TEFReferences := SWBEncryption.AuxiliaryData.TEFReferences + ',' +  Trim(TL.EFTPOSTxnRef);   // + ':' + Trim(TL.NSUNumber);
      end;
    end;
  end;
end;
{******************************************************************************}
function TformPaymentTable.TenderTypeExists(TLType: TTLT): Boolean;
begin
  Result := TLList.TenderTypeExists(TLType);
end;
{******************************************************************************}
procedure TformPaymentTable.ClearAllTenderLines;
var
  i: Integer;
  TL: TTL;
begin
  LogMessage(ltPaymentTrace, '455' + #9);
  for i := TLList.Count - 1 downto 0 do
  begin
    TL := TLList.Items[i];
    TL.PaymentType := ptNormal;  //since external account tenders are protected
    DeleteOLsOfTender(TL, TL.TenderSeqNo);
  end;
  if TLList.Count = 0 then
    AmountSplitingStarted := False;
  TLList.Clear;
  LoadTLs(LAClearAll);
  FTenderSeqID := 0;
  FTenderGrpID := 1;
end;
{******************************************************************************}
procedure TformPaymentTable.SetTLCols;
begin
  LogMessage(ltPaymentTrace, '452' + #9);
  with grdTLs do begin
    ColCount := 6;

    ColWidths[0] := 86;
    ColWidths[1] := 59;
    ColWidths[2] := 57;
    ColWidths[3] := 57;
    ColWidths[4] := 57;
    ColWidths[5] := 57;

    Cells[0, 0] := sType;
    Cells[1, 0] := sDetails;
    Cells[2, 0] := sPayment;
    Cells[3, 0] := sTip;
    Cells[4, 0] := sTender;
    Cells[5, 0] := sChange;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.grdTLsDblClick(Sender: TObject);
begin
  cmdEditClick(nil);
end;
{******************************************************************************}
procedure TformPaymentTable.PaymentTableCashCam;
var
  I: Integer;
  Line: string;
  TL: TTL;

begin
  WipeCashCamTransRecord;
  with CashCamTransRecord do begin
    TransType := 'Payment tab ' + IntToStr(GlbTable.TableNo);
    if WBTerminalLogins.QueryType <> qtLoginsTerminal then begin
      FilterQuery(dm.qrTerminalLogins, qtLoginsTerminal);
    end;
    Staff := dm.qrTerminalLogins.FieldByName('staffname').AsString;
    if (txtAccountName.Text <> '') then begin
      Items.Add(ConcatItemTender('Purchase', FToPay));
      Items.Add(ConcatItemTender('Discount', -(FDiscountAmount+FComboDiscount)));
      Items.Add('  ' + txtAccountName.Text);
    end;
    Items.Add(ConcatItemTender('Total', FToPay - FComboDiscount -FDiscountAmount));

    for I := 0 to (TLList.Count - 1) do begin
      TL := TLList.Items[I];
      Line := ConcatItemTender(GetTLTType(TL.TLType), TL.TLTender);
      TenderLines.Add(Line);
    end;
    if (OnAccount) then begin
      TenderLines.Add(ConcatItemTender(txtAccountName.Text, FStillDue));
    end;
    Change := FChange;
    DoCashCamTrans;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdAuthoriseClick(Sender: TObject);
var
  TheCurrentLoginID: Integer;
  CurrentLoginID: Integer;
  CurrentLoginPIN: Integer;
  CurrentTillID: Integer;
  CurrentTillDriverName: string;
  CurrentTillDrawkickString: string;
  CurrentStaffID: Integer;
  CurrentStaffNo: Integer;
  CurrentStaffName: string;
  DidChangeAuthorisation: Boolean;

begin
  LogMessage(ltPaymentTrace, '534' + #9);
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  inherited;
  TheCurrentLoginID := dm.qrTerminalLogins.FieldByName('loginid').AsInteger;

  CurrentLoginID := GlbLogin.LoginID;
  CurrentLoginPIN := GlbLogin.LoginPIN;
  CurrentStaffID := GlbLogin.StaffID;
  CurrentStaffNo := GlbLogin.StaffNo;
  CurrentStaffName := GlbLogin.StaffName;
  CurrentTillID := GlbLogin.TillID;
  CurrentTillDriverName := GlbLogin.TillDriverName;
  CurrentTillDrawkickString := GlbLogin.TillDrawKickString;

  DidChangeAuthorisation := False;

  CancelLoginSession;
  if (SelectName(True, False, True)) then begin
    ManagerAuthorise.Authorise := True;
    ManagerAuthorise.AuthoriseLoginID := GlbLogin.LoginID;
    ManagerAuthorise.AuthorisePriv6 := VerifyPriv(6);
    ManagerAuthorise.AuthoriseAccountRank := GlbLogin.RankAccount;
    ManagerAuthorise.AuthoriseAccChargeRank := GlbLogin.RankAccount;
    ManagerAuthorise.AuthoriseAccPaymentRank := GlbLogin.RankAccount;
    ManagerAuthorise.AuthorisePriv13 := VerifyPriv(13);
    DidChangeAuthorisation := True;
  end;

  if ((dm.qrTerminalLogins.FieldByName('loginid').AsInteger <> CurrentLoginID) or (DidChangeAuthorisation)) then begin
    //Set everything the way it was before;

    dm.qrTerminalLogins.First;
    while ((not dm.qrTerminalLogins.Eof) and (dm.qrTerminalLogins.FieldByName('loginid').AsInteger <> TheCurrentLoginID)) do begin
      dm.qrTerminalLogins.Next;
    end;
    GlbLogin.LoginID := CurrentLoginID;
    GlbLogin.LoginPIN := CurrentLoginPIN;
    GlbLogin.StaffID := CurrentStaffID;
    GlbLogin.StaffNo := CurrentStaffNo;
    GlbLogin.StaffName := CurrentStaffName;
    GlbLogin.TillID := CurrentTillID;
    GlbLogin.TillDriverName := CurrentTillDriverName;
    GlbLogin.TillDrawKickString := CurrentTillDrawkickString;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.ShowPaymentTable;
begin
  LogMessage(ltPaymentTrace, '334' + #9);
  FForceUnSelectAccount := False;
  FOneBill := AppDetails.OneBill;
  FoundFirstLoyaltyFreeItem := false;
  if Assigned(FDiscCalculator) then
  begin
    LogMessage(ltPaymentTrace, '335' + #9);
    FDiscCalculator.ClearItemLevelDiscounts;
  end;
  PaymentOnSelectedItems := False;
  AmountSplitingStarted := False;
  sExternalAccountIntegration.ClearPresentedCards;   //do now, so Tenderline are unprotected so can be cleared

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.ClearPayment;

  LogMessage(ltPaymentTrace, '336' + #9);
  ClearAllOrderLines;

  LogMessage(ltPaymentTrace, '337' + #9);
  ClearAllTenderLines;

  LogMessage(ltPaymentTrace, '338' + #9);
  ClearAllSurcharges;

  IniGoodyTrans;

  FRecordType := rtBoth;
  if AppDetails.EnableFiscalPrinting then
  begin
    FCPFNumber := GlbTable.CPFNumber;
    if ((Trim(GlbTable.CPFNumber) = '') and (GlbTable.AccountID <= 0 )and AppDetails.FiscalIsCFPRequired) then
    begin
      if CheckPrinterFunctions then
      begin
        if AppDetails.FiscalIsCFPRequired then
          GetCPFNumber(False);
      end;
    end
    else
      CheckPrinterFunctions;
  end;

  Screen.Cursor := crHourGlass;
  try
    LogMessage(ltPaymentTrace, '339' + #9);
    InitialiseAccountInfo(True, False);
    SetCurrentTime;
    with dm.qrOLsToPay do
    begin
      Close;
      ParamByName('thegroupid').AsInteger := GlbTable.GroupID;
      ParamByName('IsFiscal').AsInteger := Integer(AppDetails.EnableFiscalPrinting);
      Open;
    end;

    LogMessage(ltPaymentTrace, '340' + #9);
    LoadAllOlsToPay;

    LogMessage(ltPaymentTrace, '341' + #9);
    SetComboColourIndex(UnInvoicedOlsToPay);

    LogMessage(ltPaymentTrace, '342' + #9);
    AddAllOrderLines;

    if GlbEvent.EventID > 0 then
    begin
      LogMessage(ltPaymentTrace, '343' + #9);
      ApplySubsidyOnItems
    end
    else if GlbAccount.AccountID > 0 then
    begin
      LogMessage(ltPaymentTrace, '344' + #9);
      InitialiseAccountInfo(False, True, False);
    end;

    VisibleWindow := vwTender;
    ManagerAuthorise.Authorise := False;
  finally
    Screen.Cursor := crDefault;
  end;
  LastPayment := 0;
  LastThroughAccount := False;
  LastOnAccount := False;
  IsNormalLoyalty := True; //This is set to false when zapa is overriding discounts!
  LogMessage(ltPaymentTrace, '345' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.txtToPayEnter(Sender: TObject);
begin
  //jeh 12.5.1
  //DoCalcQty := True;
end;
{******************************************************************************}
procedure TformPaymentTable.txtToPayClick(Sender: TObject);
begin
//  txtToPay.SelectAll;
end;
{******************************************************************************}
procedure TformPaymentTable.txtToPayChange(Sender: TObject);
var
  TheTotal: Currency;
  OldQty, ToPayQty: Currency;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '552' + #9);
  ToPayString := txtToPay.Text;
  if (not JustSelected) then begin
    if (txtToPay.Text = '') or (ConvertToCurr(txtToPay.Text) = 0.00) then begin
      if (txtToPay.Text = '') then txtToPay.Text := '0.00';
      exit;
    end
    else begin
      try
        aOL := OLToPayList.Items[grdToPay.Row - 1];
        if ((ConvertToCurr(txtToPay.Text) > (aOL.Due + 0.005)) or
            (ConvertToCurr(txtToPay.Text) > 99999999.99)) then begin
          if DoCalcToPay then begin
            ShowQuickMsg(sEnterValidQuantity);
            txtQty.Text := '';
            txtToPay.Text := '0.00';
          end
          else begin
            ShowQuickMsg(sEnterValidAmtToPay);
          end;
          Exit;
        end;

        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyItemID > 0) and (aOL.ComboID <= 0)) then begin
          LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        end;

        if (GetRoundedUpDown(aOL.Due, AppDetails.DecimalPlaces) = GetRoundedUpDown(ConvertToCurr(txtToPay.Text), AppDetails.DecimalPlaces)) then begin
          aOL.ToPay := GetRoundedUpDown(aOL.Due, AppDetails.DecimalPlaces);
        end
        else begin
          aOL.ToPay := GetRoundedUpDown(ConvertToCurr(txtToPay.Text), AppDetails.DecimalPlaces);   
        end;

        aOL.QtyChanged := True;

        OldQty := aOL.Qty;
        if (aOL.UnitPrice > 0) then
        begin
          aOL.Qty := CRoundTo(aOL.ToPay / aOL.UnitPrice, 4);
        end
        else begin
          aOL.Qty := 0;
        end;

        aOL.AllowedDiscount := GetRoundedUpDown(aOL.AllowedDiscount, AppDetails.DecimalPlaces);
        aOL.SubsidyAllowed := GetRoundedUpDown(aOL.SubsidyAllowed, AppDetails.DecimalPlaces);
        if OldQty <> aOL.Qty then
        begin
          aOL.AllowedDiscount := GetRoundedUpDown(aOL.AllowedDiscount * (aOL.Qty/OldQty), AppDetails.DecimalPlaces);
          aOL.SubsidyAllowed := GetRoundedUpDown(aOL.SubsidyAllowed * (aOL.Qty/OldQty), AppDetails.DecimalPlaces);
        end;

        LogMessage(ltPaymentTrace, '553' + #9);
        AllocateItemLoyalty(aOL);
        LogMessage(ltPaymentTrace, '554' + #9);
        UpdateGrdToPayCells(grdToPay.Row);
        LogMessage(ltPaymentTrace, '567' + #9);
        DoCalcQty := not DoCalcToPay;

      except on e: exception do begin
        if DoCalcToPay then begin
          ShowQuickMsg(sEnterValidQuantity);
          txtQty.Text := '';
        end
        else
          ShowQuickMsg(sEnterValidAmtToPay);
      end; end;
    end;
  end;
  if not ReloadItems then
  begin
    LogMessage(ltPaymentTrace, '433' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '523' + #9);
  end;

  //if not in txtQty, then calc qty as to pay changes
  if (DoCalcQty) and (not QtyChangedManully) then begin
    if (OLToPayList.Count > 0) then begin
      aOL := OLToPayList.Items[grdToPay.Row - 1];
      if (txtToPay.Text <> ToString) and (ToPayString <> '') then
        txtToPay.Text := ToPayString;

      TheTotal := (aOL.OLQty * aOL.UnitPrice);
      if (TheTotal <> 0) then begin
        ToPayQty := CRoundTo((GetRoundedUpDown(ConvertToCurr(txtToPay.Text), 2) / TheTotal) * aOL.OLQty, 3);
      end
      else begin
        ToPayQty := 0;
      end;
      txtQty.Text := FormatFloat(QTYFORMAT, ToPayQty);
    end
    else begin
      txtQty.Text := '';
    end;
  end;
  RecalculateDiscount := False;
end;
{******************************************************************************}
procedure TformPaymentTable.txtToPayExit(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '606' + #9);
//
end;
{******************************************************************************}
procedure TformPaymentTable.txtQtyEnter(Sender: TObject);
begin
  //jeh 12.5.1
  DoCalcToPay := True;
end;
{******************************************************************************}
procedure TformPaymentTable.txtQtyClick(Sender: TObject);
begin
  //jeh 13/5/1
//  txtQty.SelectAll;
end;
{******************************************************************************}
procedure TformPaymentTable.txtQtyChange(Sender: TObject);
var
  //akm 12.5.1
  Err: Integer;
  TheQty: Extended;
  TheUnitPrice: Currency;
  ToPay: Currency;
begin
  LogMessage(ltPaymentTrace, '612' + #9);
  QtyChangedManully := True;
//  akm 12.5.1 - if not in txtToPay, then set change value as qty changes
  if (DoCalcToPay) then
  begin
    if (OLToPayList.Count > 0) then
    begin
      if (txtQty.Text = '') then
      begin
        TheQty := 0.00;
      end
      else
      begin
        Val(txtQty.Text, TheQty, Err);
        if (Err<>0) then
        begin
          TheQty := 0.00;
          txtToPay.Text := '0.00';
        end;
      end;
      if (TheQty <> 0.00) then
      begin
        TheUnitPrice := TOrderLine(OLToPayList[grdToPay.Row-1]).UnitPrice;
        ToPay := (TheQty * TheUnitPrice);
        //txtToPay.Text := FormatFloat('###,###,##0.00', ToPay);
        txtToPay.Text := FormatCurrencyNoSign(ToPay);
      end;
    end;
  end;   
  QtyChangedManully := False;
end;
{******************************************************************************}
procedure TformPaymentTable.txtQtyExit(Sender: TObject);
//var
  //akm 13.5.1
//  Err: Integer;
//  TheQty: Extended;
//  TheUnitPrice: Currency;
//  ToPay: Currency;
begin
  //akm 13.5.1 - no need to validate qty at all
  {if txtQty.Text = '' then
    txtQty.Text := '0'
  else
  try
    if OLList.Count>0 then
    begin
      if txtQty.Text='' then
       TheQty := 0.00
      else
      begin
        Val(txtQty.Text, TheQty, Err);
        if Err<>0 then
          TheQty := 0.00;
      end;
      TheUnitPrice := TOL(OLList[grdToPay.Row-1]^).UnitPrice;
      ToPay := TheQty*TheUnitPrice;
      if ToPay > ConvertToCurr(grdToPay.Cells[3, grdToPay.Row]) then begin
        ShowQuickMsg('Please enter a valid quantity.');
        txtQty.SetFocus;
        txtQty.SelectAll;
        //DoCalcToPay := False; //jeh 12.5.1
        Exit;
      end else if (ConvertToCurr(txtQty.Text) > 99999999.99) then begin
        ShowQuickMsg('Please enter a valid quantity.');
        txtQty.SetFocus;
        txtQty.SelectAll;
        //DoCalcToPay := False; //jeh 12.5.1
        Exit;
      end;
    end;
  except on e: exception do begin
    ShowQuickMsg('Please enter a valid quantity.');
    txtQty.SetFocus;
    txtQty.SelectAll;
  end; end;}

  DoCalcToPay := False; //akm 12.5.1
end;
{******************************************************************************}
procedure TformPaymentTable.SetCurrentTime;
begin
  DecodeTime(Now, CurrentHour, CurrentMin, CurrentSec, CurrentMSec);    //Jon 24-06-2002
  CurrentTime := ((CurrentHour * 100) + CurrentMin);
  CurrentDay := (DayOfTheWeek(Now));
end;
{******************************************************************************}
procedure TformPaymentTable.BuildTenderSurchargeList;
var
  I, J: Integer;
  SurchargeItem: TSurchargeItem;
  TL: TTL;
  bAddSurcharge: Boolean;
begin
  LogMessage(ltPaymentTrace, '456' + #9);
  for I := (SurchargeList.Count - 1) downto 0 do begin
    SurchargeItem := SurchargeList.Items[I];
    if (SurchargeItem.SurchargeType = stTenderType) then begin
      SurchargeItem.Free;
      SurchargeList.Delete(I);
    end;
  end;

  for J := 0 to (TLList.Count-1) do begin
    TL := TLList.Items[J];
    bAddSurcharge := (TL.SurchargeItemID > 0) and
                      ( ((PaymentOnSelectedItems or AmountSplitingStarted) and ( TL.TenderGroupID = FTenderGrpID))
                          or
                         ((not(PaymentOnSelectedItems or AmountSplitingStarted)) )
                      );
    If bAddSurcharge  then
    //if (TL.SurchargeItemID > 0) then
    begin
      SurchargeItem := nil;
      for I := (SurchargeList.Count - 1) downto 0 do begin
        SurchargeItem := SurchargeList.Items[I];
        if (SurchargeItem.ItemID = TL.SurchargeItemID) then begin
          Break;
        end
        else begin
          SurchargeItem := nil;
        end;
      end;
      if (not Assigned(SurchargeItem)) then begin
        SurchargeItem := TSurchargeItem.Create;
        SurchargeItem.ItemID := TL.SurchargeItemID;
        SurchargeItem.ItemAbbrev := TL.SurchargeItemAbbrev;
        SurchargeItem.Percent := TL.SurchargePercent;
        SurchargeItem.Fixed := TL.SurchargeFixed;
        SurchargeItem.Amount := 0.00;
        SurchargeItem.SurchargeType := stTenderType;
        SurchargeItem.SalesTaxPercent := TL.SurchargeSalesTaxPercent;
        SurchargeList.Add(SurchargeItem);
      end;
      SurchargeItem.Amount := SurchargeItem.Amount + TL.TLSurcharge;
    end;
  end;

  FSurcharge := 0.00;
  for I := (SurchargeList.Count-1) downto 0 do begin
    FSurcharge := FSurcharge + TSurchargeItem(SurchargeList[I]).Amount;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.ChangeAllowCreditLabel(AllowCredit: Boolean;VerifyPin:Boolean=True);
begin
  if (AllowCredit) then begin
    lblAllowCredit.Caption := sCreditAllowed;
    lblAllowCredit.Font.Color := clRed;
{ to prevent double pin check we dont verify pin here as:
    outside of fiscal we have already checked pin on TryLookupCashSaleAccount and LoadAccount
, with fiscal we dont seem to check pin on SetAccount so dont check it on charge rank either

  if you can't charge to an account if you can't USE it.
  }
    if ((GlbLogin.RankAccount < GlbAccount.AccountChargeRank) or ((ManagerAuthorise.Authorise) and (ManagerAuthorise.AuthoriseAccChargeRank >= GlbAccount.AccountChargeRank))) then begin
      { if ((AppDetails.AccChargePinCheckRank >= GlbAccount.AccountChargeRank) and (not VerifyPinNo)) then begin   }
      SetOnAccount(False);
      cmdOnAccount.Visible := False;
      lblAllowCredit.Caption := sCreditAllowedNotPriv;
      Exit;
    end
    else begin
      cmdOnAccount.Visible := True;
      SetOnAccount (AppDetails.PaymentAutoSelectPayOnAccount);   //Jon 20-10-2003
    end;
    {
    SetOnAccount(False);
    cmdOnAccount.Visible := False;
    end;  }
  end
  else begin
    lblAllowCredit.Caption := sCreditNotAllowed;
    lblAllowCredit.Font.Color := clBlack;
    SetOnAccount(False);
    cmdOnAccount.Visible := False;
  end;
  LogMessage(ltPaymentTrace, '614' + #9 + 'lblAllowCredit.Caption=' + lblAllowCredit.Caption);
end;
{******************************************************************************}
procedure TformPaymentTable.SetOnAccount(State: Boolean);    //Jon 16-10-2003
begin
  LogMessage(ltPaymentTrace, '457' + #9);
  // fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
  if Trim(AppDetails.FiscalAccountPaymentMethod) = '' then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    Exit;
  end;

  OnAccount := (State and GlbRegistrationInfo.WaiterChargeAccounts and (not FForceUnSelectAccount));
  cmdOnAccount.Down := OnAccount;

  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID >0) and
                                  (GlbEvent.AccountID > 0) and
                                  (OnAccount) and (GlbEvent.SubsidyID > 0);

  LogMessage(ltPaymentTrace, '434' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '524' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.cmdOnAccountClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '538' + #9);
  inherited;

  SetOnAccount(not OnAccount);
  RecalculateTotals;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdLoyaltyDesClick(Sender: TObject);
begin
  LogMessage(ltPaymentTrace, '371' + #9);
  inherited;
  DoLoyalty(False);
  LogMessage(ltPaymentTrace, '372' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.TestAllOLsLoyaltyItem(AList: Tlist;FindNewOnly: Boolean);
var
  i: Integer;
  ListCount: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '458' + #9);
  ListCount := (AList.Count - 1);
  if (not FindNewOnly) then begin
    LoyaltyReward.AvailableQty := LoyaltyReward.Qty;
  end;

  for I := 0 to ListCount do
  begin
    aOL := AList.Items[I];
    if (aOL.ComboID > 0) then
      Continue;

    if (aOL.ItemID > 0) then
    begin
      if (not FindNewOnly) then
      begin
        aOL.LoyaltyItemID := -1;
        aOL.LoyaltyFreeQty := 0;
        aOL.LoyaltyFreeDiscount := 0;
      end;
      if ((LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyFreeQty < aOL.Qty)) then begin
        if ((aOL.LoyaltyItemID <= 0)) then
        begin
          FoundFirstLoyaltyFreeItem := True;
          aOL.LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, aOL.ItemID);
        end
        else begin
          LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        end;
        LogMessage(ltPaymentTrace, '571' + #9);
        AllocateItemLoyalty(aOL);
      end;
    end;
  end;
end;
{******************************************************************************}
function TformPaymentTable.TestLoyaltyItem(LoyaltyRewardID: Integer; ItemID: Integer): Integer;
begin
  LogMessage(ltPaymentTrace, '459' + #9);
  try
    with dm.sp do begin
      SetStoredProcName('TEST_LOYALTYITEM');         //test_loyaltyitem
      ParamByName('loyaltyrewardid').AsInteger := LoyaltyRewardID;
      ParamByName('itemid').AsInteger := ItemID;
      ExecProc;
      Result := FieldAsInt(FieldByName('loyaltyitemid'), -1);
    end;
  except
    Result := -1;
  end;
end;
{******************************************************************************}
function TformPaymentTable.AllocateItemLoyalty(var OL: TOrderLine): Boolean;
var
  OldLoyaltyFreeQty: Currency;
  OldLoyaltyFreeDiscount: Currency;
begin
  LogMessage(ltPaymentTrace, '460' + #9);
  Result := False;
  if (OL.ComboID > 0) then
    Exit;  //Don't apply Free Item to Item belonging to a combo
  if (LoyaltyReward.RewardOffer = roDiscount) and LoyaltyReward.Processed then
    Exit;

  OldLoyaltyFreeQty := OL.LoyaltyFreeQty;
  OldLoyaltyFreeDiscount := OL.LoyaltyFreeDiscount;
  OL.LoyaltyFreeQty := 0;
  OL.LoyaltyFreeDiscount := 0;

  if OL.ComboID > 0 then
    Exit;

  if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and ((LoyaltyReward.AvailableQty) > 0) and (OL.LoyaltyItemID > 0)) then begin
    if (LoyaltyReward.AvailableQty > OL.Qty) then begin
      OL.LoyaltyFreeQty := OL.Qty;
    end
    else begin
      OL.LoyaltyFreeQty := LoyaltyReward.AvailableQty;
    end;
    OL.LoyaltyFreeDiscount := GetRoundedUpDown(OL.LoyaltyFreeQty * ol.UnitPrice, AppDetails.DecimalPlaces);
    LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty - OL.LoyaltyFreeQty;
  end;

  Result := ((OL.LoyaltyFreeQty <> OldLoyaltyFreeQty) or (OL.LoyaltyFreeDiscount <> OldLoyaltyFreeDiscount));
  LogMessage(ltPaymentTrace, '551' + #9 + 'Result=' + BoolToStr(Result, True) + ', OL.OrderLineID=' + IntToStr(OL.OrderLineID) +
    ', OL.LoyaltyFreeQty=' + FloatToStr(OL.LoyaltyFreeQty) + ', OL.LoyaltyFreeDiscount=' + FloatToStr(OL.LoyaltyFreeDiscount));
end;
{******************************************************************************}
procedure TformPaymentTable.UpdateGrdToPayCells(Row: Integer);
var
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '461' + #9);
  if (Row > 0) and (Row < grdToPay.RowCount) then
  begin
    aOL := OLToPayList.Items[Row - 1];
    grdToPay.Row := Row;

    grdToPay.Cells[0, Row] := aOL.Positions;
    grdToPay.Cells[1, Row] := FormatFloat(QTYFORMAT, aOL.Qty);
    if AppDetails.EnableFiscalPrinting then
    begin
      grdToPay.Cells[1, Row] := FormatFloat(QTYFORMAT, WBRoundTo(aOL.Qty, 3));
    end;
    if (aOL.ModsString <> '') then
      grdToPay.Cells[2, Row] := aOL.ItemString+#13+aOL.ModsString
    else
      grdToPay.Cells[2, Row] := aOL.ItemString;
    if ((LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0)) then
    begin
      grdToPay.Cells[3, Row] := '* ' + FormatCurrencyNoSign(aOL.Due);
      grdToPay.Cells[4, Row] := '* ' + FormatCurrencyNoSign(aOL.ToPay - aOL.LoyaltyFreeDiscount);
      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
      begin
        grdToPay.Cells[3, Row] := '* ' + FormatCurrencyNoSign(TruncateTo(aOL.Due, 2));
        grdToPay.Cells[4, Row] := '* ' + FormatCurrencyNoSign(TruncateTo(aOL.ToPay - aOL.LoyaltyFreeDiscount, 2));
      end;
    end
    else begin
      grdToPay.Cells[3, Row] := FormatCurrencyNoSign(aOL.Due);
      grdToPay.Cells[4, Row] := FormatCurrencyNoSign(aOL.ToPay);
      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
      begin
        grdToPay.Cells[3, Row] := FormatCurrencyNoSign(TruncateTo(aOL.Due, 2));
        grdToPay.Cells[4, Row] := FormatCurrencyNoSign(TruncateTo(aOL.ToPay, 2));
      end;
    end;
    grdItemAutoClick := True;
    //save what's in the grid to log file
    LogMessage(ltPaymentTrace, '504' + #9 + 'grdToPay: Row=' + IntToStr(Row) + ', Col0=' + grdToPay.Cells[0, Row] + ', Col1=' + grdToPay.Cells[1, Row] +
      ', Col2=' + grdToPay.Cells[2, Row] + ', Col3=' + grdToPay.Cells[3, Row] + ', Col4=' + grdToPay.Cells[4, Row]);

    grdToPayClick(nil);
    LogMessage(ltPaymentTrace, '505' + #9);
  end;
  LogMessage(ltPaymentTrace, '564' + #9);
end;
{******************************************************************************}
procedure TformPaymentTable.FSetVisibleWindow(Value: TVisibleWindow);
begin
  FVisibleWindow := Value;
  pnlTender.Visible := (FVisibleWindow = vwTender);
  pnlToPay.Visible := (FVisibleWindow = vwToPay);
  if (FVisibleWindow = vwToPay) then begin
    pnlToPay.Align := alClient;
  end
  else begin
    pnlTender.Align := alClient;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdSummaryTableDesClick(Sender: TObject);
begin
  PrintSummary;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdLastDesClick(Sender: TObject);
var
  InvoiceID: Integer;
  InvoiceType: string;
begin
  try
    if (AppDetails.ReceiptsViewLastReceipt) or (AppDetails.FiscalType = fiscalTaxCore) then
    begin
      if (SelectLastCashSale(InvoiceID, InvoiceType)) and (not AppDetails.EnableFiscalPrinting) then
      begin
        if (InvoiceID = -1) then
        begin
          ShowQuickMsg(sNoCashSalesToPrint);
        end
        else if (InvoiceType = 'I') then
        begin
          if (AppDetails.FiscalType = fiscalTaxCore) then
            PrintTaxCoreInvoice(InvoiceID, True)
          else
          begin
            InsertPrintJob('C Invoice', InvoiceID, AppDetails.PrinterID);
            ShowQuickMsg(sInvPrinted);
          end;
        end
        else begin
          if (AppDetails.FiscalType = fiscalTaxCore) then
            PrintTaxCoreInvoice(InvoiceID, True)
          else
          begin
            InsertPrintJob('C Credit Note', InvoiceID, AppDetails.PrinterID);
            ShowQuickMsg(sCreditNotePrinted);
          end;
        end;
      end;
    end
    else begin
    if (not dm.TRStartTest) then begin
      Exit;
    end;
    with dm.sp do begin
      SetStoredProcName('PRINTLASTINVOICE'); //printlastinvoice

      ParamByName('terminalid').AsInteger := AppDetails.TerminalID;
      ParamByName('printerid').AsInteger := AppDetails.PrinterID;
      ExecProc;
      InvoiceID := FieldAsInt(FieldByName('invoiceid'));
      dm.tr.Commit;
      if (InvoiceID = 0) then begin
        ShowQuickMsg('There are no invoices to print.');
      end
      else begin
        ShowQuickMsg('Last invoice printed.');
      end;
    end;
  end;
  except
    dm.tr.Rollback;
  end;
end;
{******************************************************************************}
function TformPaymentTable.GetCPFNumber(UpdateCPF: Boolean): Boolean;
var
  frmCPFNo: TformGetCPFNumber;
begin
  Result := False;
  SFiscalPrinting.FiscalOpTryAgainCommand := False;
  SFiscalPrinting.SupressFiscalErrorMessage := False;

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  lblTitle.Caption := sTabPayment;
  SetStatus(sFiscalPrinterChecking);

  frmCPFNo := TformGetCPFNumber.Create(Nil);
  try
    if UpdateCPF then
      frmCPFNo.AccountID := GlbTable.AccountID;

    frmCPFNo.UpdateCPF := UpdateCPF;
    frmCPFNo.DisplayTaxDetails := True;
    FPerorgID := 0;
    frmCPFNo.ShowManualSelection := ShowManualSelection;
    NFManualSecond := False;
    NFManual := False;
    if frmCPFNo.ShowModal = mrOk then
    begin
      FCPFNumber := frmCPFNo.CPFNumber;
      NFManualSecond := frmCPFNo.NFManualSelected;
      NFManual := ShowManualSelection and frmCPFNo.NFManual;
      ClearStatusPage;
      if NFManual or NFManualSecond then
        SetStatus(sTabPaymentINManualNFMode, False, True)
      else if NFEMode then
        SetStatus(sTabPaymentINNFEMode, False, NFManual)
      else
        SetStatus(sTabPayment, False, NFManual);

      if frmCPFNo.AccountID > 0 then
      begin
        NFEMode := frmCPFNo.SendToNFE;
        GlbAccount.AccountID := frmCPFNo.AccountID;
        GlbTable.AccountID := frmCPFNo.AccountID;
        FPerorgID := GlbPerOrg.PerorgID;
      end
      else
      begin
        if Trim(FCPFNumber) <> '' then
        begin
          RetrieveCPFAccount;
          FPerorgID := GlbPerOrg.PerorgID;
        end
        else
        begin
          GlbAccount.AccountID := 0;
          DeselectAccount;
          FPerorgID := 0;
        end;
      end;
      Result := True;
    end;
  finally
    frmCPFNo.Free;
  end;
  if (not (NFManual or NFEMode)) then
  begin
    if not SFiscalPrinting.CheckAuxFileForManufactureNo then
    begin
      //ShowMsg(sFiscalManufactureNoNotInAuxFile);
      PostMessage(Self.Handle, WM_CLOSE, 0, 0);
      Exit;
    end;
    if not SFiscalPrinting.CheckAuxFileForGrandTotal then
    begin
      //ShowMsg(sInvalidFiscalTotal);
      PostMessage(Self.Handle, WM_CLOSE, 0, 0);
      Exit;
    end;
  end;
end;

procedure TformPaymentTable.RetrieveCPFAccount;
begin
  if (DoingTablePayment) then begin   //Jon 25-02-2002
    Exit;
  end;

  //fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
  if Trim(AppDetails.FiscalAccountPaymentMethod) = '' then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    Exit;
  end;

  if (CanSelectAccount) then
  begin
    with dm.qrCPFAccount do
    begin
      try
        UnPrepare;
        SQL.Clear;

        SQL.Add('select a.accountid, p.TAXNUMBER, p.perorgid from account a, perorg p ');
        SQL.Add('where a.PERORGID = p.PERORGID and p.WHENDELETED is null and a.WHENCLOSED is null');
        SQL.Add('and p.TAXNUMBER = ' + QuotedStr(FCPFNumber));//SQL.Add('select accountid, cpfnumber from account where cpfnumber = ' + QuotedStr(FCPFNumber));

        Prepare;
        Open;
        GlbAccount.AccountID := 0;
        GlbTable.AccountID := 0;
        GlbTable.PerorgID := 0;
        if (not EOF) then
        begin
          GlbAccount.AccountID := FieldByName('AccountId').AsInteger;

          GlbTable.AccountID := FieldByName('AccountId').AsInteger;
          GlbTable.PerorgID := FieldByName('perorgid').AsInteger;
          GlbTable.CPFNumber := FieldByName('TAXNUMBER').AsString;
          Close;
          SQL.Clear;
          SQL.Add('UPDATE AGROUP SET PERORGID = :perorgid, accountid = :accountid, cpfnumber = :cpfnumber where GROUPID = ' + IntToStr(GlbTable.GroupID));
          ParamByName('perorgid').AsInteger := GlbTable.PerorgID;
          ParamByName('accountid').AsInteger := GlbTable.AccountID;
          ParamByName('cpfnumber').AsString := GlbTable.CPFNumber;
          ExecSQL;
        end;
      finally
        Close;
      end;
    end;
  end;
end;

procedure TformPaymentTable.cmdGetCPFDesClick(Sender: TObject);
begin
  if (DoingTablePayment) then
    Exit;

  if (AppDetails.EnableFiscalPrinting and CanSelectAccount) then
  begin
    if GetCPFNumber(False) then
    begin
      InitialiseAccountInfo;
    end;
  end;
end;

procedure TformPaymentTable.ClearVoidLines;
var
  OL: TVoidOL;
  i: Integer;
begin
  LogMessage(ltPaymentTrace, '416' + #9);
  for i := (VoidList.Count - 1) downto 0 do
  begin
    OL := VoidList.Items[i];
    OL.Free;
    VoidList.Delete(i);
  end;
end;

function TformPaymentTable.SendFiscalInvoice: Boolean;
begin
  Result := False;
  if not AppDetails.EnableFiscalPrinting then
  begin
    Result := True;
    Exit;
  end;
  // for Fiscal Printing - Open an Invoice in the Fiscal Printer
end;

procedure TformPaymentTable.LoadAllOlsToPay;
var
  aOL: TOrderLine;
  i: Integer;
begin
  LogMessage(ltPaymentTrace, '462' + #9);
  if not DM.qrOLsToPay.Active then
    Exit;

  if DM.qrOLsToPay.Eof then
    Exit;

  ClearUnInvoicedOls;
  ReloadItems := True;
  i := 1;
  while (not DM.qrOLsToPay.Eof) do
  begin
    aOL := TOrderLine.Create;

    aOL.OrderLineID := DM.qrOLsToPay.FieldByName('orderlineid').AsInteger;
    aOL.OrderID := DM.qrOLsToPay.FieldByName('ORDERID').AsInteger;
    aOL.OrderLineOrder := i;
    Inc(i);
    aOL.WhenOrdered := Now;
    aOL.CourseID := 0;
    aOL.CourseName := '';
    aOL.CourseIsMain := False;
    aOL.ItemID := DM.qrOLsToPay.FieldByName('ITEMID').AsInteger;
    aOL.ItemGroupID := DM.qrOLsToPay.FieldByName('ITEMGROUPID').AsInteger;
    aOL.ForB := DM.qrOLsToPay.FieldByName('FORB').AssTRING;
    aOL.Qty := DM.qrOLsToPay.FieldByName('theQTY').AsDouble;
    aOL.QtyLeft := DM.qrOLsToPay.FieldByName('QTYLEFT').AsDouble;  //RPC Added
    aOL.ILQty := aOL.Qty - aOL.QtyLeft;
    aOL.UnitPrice := DM.qrOLsToPay.FieldByName('UNITPRICE').AsCurrency;
    aOL.OriginalUnitPrice := DM.qrOLsToPay.FieldByName('originalunitprice').AsCurrency;
    aOL.ItemString := DM.qrOLsToPay.FieldByName('ITEMABBREV').AsString;
    aOL.ModsString := FieldAsString(DM.qrOLsToPay.FieldByName('ModText'));
    aOL.SalesTaxPercent := DM.qrOLsToPay.FieldByName('SALESTAXPERCENT').AsCurrency;
    aOL.Invoiced := DM.qrOLsToPay.FieldByName('INVOICED').AsCurrency;
    aOL.CourseName := DM.qrOLsToPay.FieldByName('COURSE').AsString;
    aOL.ILCount := DM.qrOLsToPay.FieldByName('ILCount').AsInteger;
    aOL.ShowZeroPriceItem := (aOL.UnitPrice <= 0) and (aOL.QtyLeft > 0);
    aOL.UnInvoiced := FieldAsCurrency(DM.qrOLsToPay.FieldByName('uninvoiced'));
    aOL.Positions := DM.qrOLsToPay.FieldByName('positions').AsString;
    aOL.OLPriceLevel := DM.qrOLsToPay.FieldByName('pricelevel').AsInteger;
    aOL.TaxSituation := DM.qrOLsToPay.FieldByName('TaxSituation').AsString;
    aOL.CSOSN := DM.qrOLsToPay.FieldByName('CSOSN').AsString;
    aOL.CSTICMS := DM.qrOLsToPay.FieldByName('CST_ICMS').AsString;
    aOL.CFOP := DM.qrOLsToPay.FieldByName('CFOP').AsInteger;
    aOL.BarCode := DM.qrOLsToPay.FieldByName('Barcode').AsString;
    aOL.GTIN := DM.qrOLsToPay.FieldByName('gtin').AsString;
    aOL.NCMProductCode := DM.qrOLsToPay.FieldByName('NCMProductCode').AsString;
    aOL.CST_PIS := DM.qrOLsToPay.FieldByName('cst_pis').AsString;
    aOL.CST_COFFINS := DM.qrOLsToPay.FieldByName('cst_coffins').AsString;
    aOL.CEST := FieldAsInt(DM.qrOLsToPay.FieldByName('cest'));
    aOL.TaxCoreLabel := FieldAsString(DM.qrOLsToPay.FieldByName('taxlabel'));
    aOL.ProductOrigin := 0;
    if Length(aOL.CSTICMS) >= 3 then
    begin
      aOL.ProductOrigin := StrToIntDef(Copy(aOL.CSTICMS, 1, 1), 0);
      aOL.CSTICMS := Copy(aOL.CSTICMS, 2, Length(aOL.CSTICMS) - 1);
    end;
    aOL.GLCode := DM.qrOLsToPay.FieldByName('glcode').AsString;
    aOL.WeighedItem := FieldAsBool(DM.qrOLsToPay.FieldByName('weighteditem'));
    aOL.ToPay := aOL.UnInvoiced;
    aOL.Due := aOL.UnInvoiced;
    aOL.OLQty := aOL.Qty;
    aOL.ItemGrpType := TItemGrpType(DM.qrOLsToPay.FieldByName('ItemGrpType').AsInteger);
    if ((DM.qrOLsToPay.FieldByName('comboid').IsNotNull) and (DM.qrOLsToPay.FieldByName('comboid').AsInteger > 0)) then
    begin
      aOL.ComboID := DM.qrOLsToPay.FieldByName('comboid').AsInteger;
      aOL.ComboGroupID := DM.qrOLsToPay.FieldByName('combogroup').AsInteger;
      aOL.ComboIndex := DM.qrOLsToPay.FieldByName('comboindex').AsInteger;
      aOL.ComboName := DM.qrOLsToPay.FieldByName('comboname').AsString;
      aOL.ComboQty := DM.qrOLsToPay.FieldByName('comboqty').AsDouble;
    end;

    aOL.AllowedDiscount := 0;
    aOL.ILDiscount := 0;
    aOL.HostSubsidy := 0;
    aOL.GuestSubsidy := 0;
    aOL.SubsidyAllowed := 0;

    if ((aOL.ComboID > 0) and  (aOL.ComboGroupID > 0) and (aOL.ComboIndex > 0)) then
    begin
      aOL.AllowedDiscount := DM.qrOLsToPay.FieldByName('OLDiscount').AsCurrency;
      //aOL.ILDiscount := aOL.AllowedDiscount;
      aOL.ComboDiscount := aOL.AllowedDiscount;
    end;
    aOL.LoyaltyItemID := 0;
    aOL.LoyaltyFreeQty := 0;
    aOL.LoyaltyFreeDiscount := 0;
    aOL.DiscountSchemeItemPercentage := 0;
    aOL.DiscountSchemeItemAmount := 0;
    aOL.DiscountSchemeID := 0;

    DM.qrOLsToPay.Next;
    UnInvoicedOlsToPay.Add(aOL)
  end;
  ReloadItems := False;
  SubsidyCalc.CalculateEventSubsidy(UnInvoicedOlsToPay);
  LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, FToPay);
end;

procedure TformPaymentTable.ClearUnInvoicedOls;
var
  i: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '463' + #9);
  for i := UnInvoicedOlsToPay.Count - 1 downto 0 do
  begin
    aOL := UnInvoicedOlsToPay.Items[i];
    aOL.Free;
    UnInvoicedOlsToPay.Delete(I);
  end;
  UnInvoicedOlsToPay.Clear;

  ClearUnInvoicedGrid;
end;

procedure TformPaymentTable.SetUnInvoicedOLCols;
begin
  LogMessage(ltPaymentTrace, '307' + #9);
  with grdUnInvoicedList do
  begin
    ColCount := 8;
    if AppDetails.EnableCombo then
      ColCount := 10;

    ColWidths[0] := 30;
    ColWidths[1] := 50;
    ColWidths[2] := 40;
    ColWidths[3] := 40;
    ColWidths[4] := 200;
    ColWidths[5] := 40;
    ColWidths[6] := 50;
    ColWidths[7] := 50;

    Cells[0, 0] := sQryBF;
    Cells[1, 0] := sCourse;
    Cells[2, 0] := sQryQty;
    Cells[3, 0] := sQryLeft;
    Cells[4, 0] := sItem;
    Cells[5, 0] := sQryPos;
    Cells[6, 0] := sToPay;
    Cells[7, 0] := sDue;
    if AppDetails.EnableCombo then
    begin
      ColWidths[8] := 110;
      ColWidths[9] := 90;
      Cells[8, 0] := sComboName;
      Cells[9, 0] := sComboSeq;
    end;
  end;
  LogMessage(ltPaymentTrace, '308' + #9);
end;

procedure TformPaymentTable.LoadUnInvoicedOlsToGrid(AList: TList; aPaid, aToPay: Currency);
var
  iRow, i: Integer;
  aOL: TOrderLine;
  AddOL: Boolean;
  FoundCombos: Boolean;
begin
  LogMessage(ltPaymentTrace, '295' + #9 + 'aPaid=' + FloatToStr(aPaid) + ', aToPay=' + FloatToStr(aToPay));
  SetOLCols;

  LogMessage(ltPaymentTrace, '296' + #9);
  ClearUnInvoicedGrid;

  if PaymentOnSelectedItems or AmountSplitingStarted then
  begin
    LogMessage(ltPaymentTrace, '297' + #9);
    if (APaid >= AToPay) then
    begin
      LogMessage(ltPaymentTrace, '298' + #9);
      ClearOLsWithoutTenderSeqNo; //means delete those items added simply without a tender . Clear those and let operator select the items again
    end;
  end
  else
  begin
    LogMessage(ltPaymentTrace, '299' + #9);
    ClearOLsWithoutTenderSeqNo; //means delete those items added simply without a tender . Clear those and let operator select the items again
  end;

  if not Assigned(AList) then
    Exit;

  if AList.Count = 0 then
    Exit;

  iRow := 1;
  FoundCombos := False;

  for i := 0 to AList.Count-1 do
  begin
    aOL := AList.Items[i];
    LogMessage(ltPaymentTrace, '300' + #9);

    AddOL := (aOL.UnInvoiced > 0) or ((aOL.UnitPrice <= 0) and (aOL.ShowZeroPriceItem));
    if (AddOL) then
    begin
      LogMessage(ltPaymentTrace, '301' + #9);
      if aOL.ComboID > 0 then
        FoundCombos := True;
      grdUnInvoicedList.Cells[0, iRow] := aOL.ForB;
      grdUnInvoicedList.Objects[0, iRow] := TObject(aOL.OrderLineID);
      grdUnInvoicedList.Cells[1, iRow] := aOL.CourseName;
      if (aOL.ModsString <> '') then
        grdUnInvoicedList.Cells[4, iRow] := aOL.ItemString+#13+aOL.ModsString
      else
        grdUnInvoicedList.Cells[4, iRow] := aOL.ItemString;
      grdUnInvoicedList.Cells[5, iRow] := aOL.Positions;

      grdUnInvoicedList.Cells[2, iRow] := FormatFloat(QTYFORMAT, aOL.Qty);
      if aOL.UnitPrice <> 0 then
        grdUnInvoicedList.Cells[3, iRow] := FormatFloat(QTYFORMAT, aOL.ToPay / aOL.UnitPrice)
      else
        grdUnInvoicedList.Cells[3, iRow] := FormatFloat(QTYFORMAT, aOL.Qty);
      grdUnInvoicedList.Cells[6, iRow] := FormatFloat(CurrencyMask, aOL.Invoiced+aOL.ToPay);

      grdUnInvoicedList.Cells[7, iRow] := FormatFloat(CurrencyMask, aOL.Due);
      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
      begin
        grdUnInvoicedList.Cells[2, iRow] := FormatFloat(QTYFORMAT, TruncateTo(aOL.Qty, 3));
        if aOL.UnitPrice <> 0 then
          grdUnInvoicedList.Cells[3, iRow] := FormatFloat(QTYFORMAT, TruncateTo(aOL.ToPay / aOL.UnitPrice, 3))
        else
          grdUnInvoicedList.Cells[3, iRow] := FormatFloat(QTYFORMAT, aOL.Qty);
        grdUnInvoicedList.Cells[6, iRow] := FormatFloat(CurrencyMask, TruncateTo(aOL.Invoiced+aOL.ToPay, 2));
        grdUnInvoicedList.Cells[7, iRow] := FormatFloat(CurrencyMask, TruncateTo(aOL.Due, 2));
      end;
      grdUnInvoicedList.Cells[8, iRow] := aOL.ComboName;
      grdUnInvoicedList.Cells[9, iRow] := '';
      if (aOL.ComboIndex > 0) then
      begin
        grdUnInvoicedList.Cells[9, iRow] := IntToStr(aOL.ComboIndex);
      end;
      Inc(iRow);
      grdUnInvoicedList.RowCount := grdUnInvoicedList.RowCount + 1;
    end;
  end;

  cmdAmountToPay.Enabled := not FoundCombos;
  if grdUnInvoicedList.RowCount > 2 then
    grdUnInvoicedList.RowCount := grdUnInvoicedList.RowCount - 1;
  LogMessage(ltPaymentTrace, '302' + #9);
end;

procedure TformPaymentTable.ClearUnInvoicedGrid;
begin
  LogMessage(ltPaymentTrace, '305' + #9);
  grdUnInvoicedList.RowCount := 2;
  grdUnInvoicedList.FixedRows := 1;
  SetUnInvoicedOLCols;

  grdUnInvoicedList.Cells[0, 1] := '';
  grdUnInvoicedList.Cells[1, 1] := '';
  grdUnInvoicedList.Cells[2, 1] := '';
  grdUnInvoicedList.Cells[3, 1] := '';
  grdUnInvoicedList.Cells[4, 1] := '';
  grdUnInvoicedList.Cells[5, 1] := '';
  grdUnInvoicedList.Cells[6, 1] := '';
  grdUnInvoicedList.Cells[7, 1] := '';
  grdUnInvoicedList.Cells[8, 1] := '';
  grdUnInvoicedList.Cells[9, 1] := '';
  LogMessage(ltPaymentTrace, '306' + #9);
end;

procedure TformPaymentTable.UpdateUnInvoicedOLValues(Apply: Boolean);
var
  i, j: Integer;
  aOL, SOL: TOrderLine;

begin
  LogMessage(ltPaymentTrace, '323' + #9 + 'Apply=' + BoolToStr(Apply, True) + ', TempItemList.Count=' + IntToStr(TempItemList.Count));
  if not Assigned(UnInvoicedOlsToPay) then
    Exit;

  if UnInvoicedOlsToPay.Count = 0 then
    Exit;

  for i := 0 to OLToPayList.Count - 1 do
  begin
    aOL := OLToPayList.Items[i];
    LogMessage(ltPaymentTrace, '324' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay) + ', aOL.Qty=' + FloatToStr(aOL.Qty) +
      ', aOL.AllowedDiscount=' + FloatToStr(aOL.AllowedDiscount) + ', aOL.SubsidyAllowed=' + FloatToStr(aOL.SubsidyAllowed));

    for j := 0 to UnInvoicedOlsToPay.Count - 1 do
    begin
      SOL := UnInvoicedOlsToPay.Items[j];
      if aOL.OrderLineID = SOL.OrderLineID then
      begin
        LogMessage(ltPaymentTrace, '325' + #9 + 'SOL.OrderLineID=' + IntToStr(SOL.OrderLineID) +
          ', SOL.UnInvoiced=' + FloatToStr(SOL.UnInvoiced) + ', SOL.Invoiced=' + FloatToStr(SOL.Invoiced) +
          ', SOL.QtyLeft=' + FloatToStr(SOL.QtyLeft) + ', SOL.Qty=' + FloatToStr(SOL.Qty) + ', SOL.Due=' + FloatToStr(SOL.Due) + ', SOL.ToPay=' + FloatToStr(SOL.ToPay) +
          ', SOL.UnitPrice=' + FloatToStr(SOL.UnitPrice) + ', SOL.ShowZeroPriceItem=' + BoolToStr(SOL.ShowZeroPriceItem, True) +
          ', SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount) + ', SOL.SubsidyAllowed=' + FloatToStr(SOL.SubsidyAllowed));

        SOL.UnInvoiced := SOL.UnInvoiced - aOL.ToPay ;
        SOL.Invoiced := SOL.Invoiced + aOL.ToPay ;
        SOL.QtyLeft := SOL.Qty - aOL.Qty;
        SOL.Qty := SOL.Qty - aOL.Qty;
        if SOL.QtyLeft < 0 then
          SOL.QtyLeft := 0;
        if SOL.Qty < 0 then
          SOL.Qty := 0;
        SOL.Due := SOL.UnInvoiced;
        SOL.ToPay := SOL.UnInvoiced;
        if SOL.UnitPrice = 0 then
          SOL.ShowZeroPriceItem := False;
        SOL.AllowedDiscount := SOL.AllowedDiscount - aOL.AllowedDiscount;
        SOL.SubsidyAllowed := SOL.SubsidyAllowed - aOL.SubsidyAllowed;

        LogMessage(ltPaymentTrace, '326' + #9 +
          'SOL.UnInvoiced=' + FloatToStr(SOL.UnInvoiced) + ', SOL.Invoiced=' + FloatToStr(SOL.Invoiced) +
          ', SOL.QtyLeft=' + FloatToStr(SOL.QtyLeft) + ', SOL.Qty=' + FloatToStr(SOL.Qty) + ', SOL.Due=' + FloatToStr(SOL.Due) + ', SOL.ToPay=' + FloatToStr(SOL.ToPay) +
          ', SOL.UnitPrice=' + FloatToStr(SOL.UnitPrice) + ', SOL.ShowZeroPriceItem=' + BoolToStr(SOL.ShowZeroPriceItem, True) +
          ', SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount) + ', SOL.SubsidyAllowed=' + FloatToStr(SOL.SubsidyAllowed));
        Break;
      end;
    end;
  end;

  if ((Apply) and (TempItemList.Count > 0)) then
  begin
    for i := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[i];
      for j := 0 to TempItemList.Count - 1 do
      begin
        SOL := TempItemList.Items[j];
        if aOL.OrderLineID = SOL.OrderLineID then
        begin
          LogMessage(ltPaymentTrace, '327' + #9 + 'SOL.OrderLineID=' + IntToStr(SOL.OrderLineID) +
            ', SOL.UnInvoiced=' + FloatToStr(SOL.UnInvoiced) + ', SOL.Invoiced=' + FloatToStr(SOL.Invoiced) +
            ', SOL.QtyLeft=' + FloatToStr(SOL.QtyLeft) + ', SOL.Qty=' + FloatToStr(SOL.Qty) + ', SOL.Due=' + FloatToStr(SOL.Due) + ', SOL.ToPay=' + FloatToStr(SOL.ToPay) +
            ', SOL.UnitPrice=' + FloatToStr(SOL.UnitPrice) +
            ', SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount) + ', SOL.SubsidyAllowed=' + FloatToStr(SOL.SubsidyAllowed));

          SOL.UnInvoiced := SOL.UnInvoiced - aOL.ToPay;
          SOL.Invoiced := SOL.Invoiced + aOL.ToPay;
          SOL.QtyLeft := SOL.Qty - aOL.Qty;
          SOL.Qty := SOL.Qty - aOL.Qty;
          SOL.Due := SOL.UnInvoiced;

          SOL.ToPay := SOL.UnInvoiced;
          SOL.AllowedDiscount := SOL.AllowedDiscount - aOL.AllowedDiscount; // calculating the new allowed discount for that item
          SOL.SubsidyAllowed := SOL.SubsidyAllowed - aOL.SubsidyAllowed;

          LogMessage(ltPaymentTrace, '328' + #9 +
            'SOL.UnInvoiced=' + FloatToStr(SOL.UnInvoiced) + ', SOL.Invoiced=' + FloatToStr(SOL.Invoiced) +
            ', SOL.QtyLeft=' + FloatToStr(SOL.QtyLeft) + ', SOL.Qty=' + FloatToStr(SOL.Qty) + ', SOL.Due=' + FloatToStr(SOL.Due) + ', SOL.ToPay=' + FloatToStr(SOL.ToPay) +
            ', SOL.UnitPrice=' + FloatToStr(SOL.UnitPrice) +
            ', SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount) + ', SOL.SubsidyAllowed=' + FloatToStr(SOL.SubsidyAllowed));
          Break;
        end;
      end;
    end;
  end;
  LogMessage(ltPaymentTrace, '329' + #9);
end;

procedure TformPaymentTable.LoadUnInvoicedOlsToOLToPayList(AList: TList);
var
  i, iCount: Integer;
  aOL, OL: TOrderLine;
  AddOL: Boolean;
begin
  LogMessage(ltPaymentTrace, '309' + #9);
  if not Assigned(AList) then
    Exit;

  if AList.Count = 0 then
    Exit;

  LogMessage(ltPaymentTrace, '310' + #9);
  ClearAllOrderLines;
  for i := 0 to AList.Count - 1 do
  begin
    aOL := AList.Items[i];
    AddOL := (aOL.Due > 0) or (aOL.UnInvoiced > 0) or ((aOL.UnitPrice <= 0) and (aOL.ShowZeroPriceItem));

    LogMessage(ltPaymentTrace, '311' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.Due=' + FloatToStr(aOL.Due) + ', aOL.UnInvoiced=' + FloatToStr(aOL.UnInvoiced) +
      ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice) + ', aOL.ShowZeroPriceItem=' + BoolToStr(aOL.ShowZeroPriceItem, True));

    if ((AddOL) and (DM.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, [])) and
        (not dm.qrOLsToPay.Fields[0].IsNull)) then
    begin
      iCount := OLToPayList.Count;
      LogMessage(ltPaymentTrace, '312' + #9 + 'iCount=' + IntToStr(iCount));
      AddOrderLine(AList, i);
      if ((OLToPayList.Count = (iCount + 1)) and (aOL.ComboID >0) and (aOL.ComboGroupID > 0) and (aOL.ComboIndex > 0)) then
      begin
        LogMessage(ltPaymentTrace, '313' + #9);
        OL := OLToPayList.Items[OLToPayList.Count - 1];
        OL.ComboDiscount := aOL.ComboDiscount;
      end;
    end;
  end;

  if (AppDetails.OneBill) and (FUseDiscountAmount) and (FDiscCalculator.OneBillUseDiscountAmount) then
    FDiscountAmount := FDiscCalculator.OneBillDiscountAmount;

  LogMessage(ltPaymentTrace, '314' + #9);
  grdItemAutoClick := True;
  grdToPay.Row := 1;

  LogMessage(ltPaymentTrace, '315' + #9);
  CalculateToPay(True);
  LogMessage(ltPaymentTrace, '316' + #9);
end;

procedure TformPaymentTable.UpdateOLTenderID(ATenderID: Integer);
var
  i: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '330' + #9 + 'ATenderID=' + IntToStr(ATenderID));
  for i := 0 to OLToPayList.Count - 1 do
  begin
    aOL := OLToPayList.Items[i];
    LogMessage(ltPaymentTrace, '331' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID));
    aOL.TenderSeqID := ATenderID;
  end;
end;


procedure TformPaymentTable.CopyOL(aDestOL: TOrderLine; aSourceOL: TOrderLine);
begin
  aDestOL.CopyOL(aSourceOL);
end;

procedure TformPaymentTable.DeleteOLsOfTender(TL:TTL;ASeqNo: Integer);
var
  i, iCount, j: Integer;
  aOL, SOL, tmpOL: TOrderLine;
  FoundSelected: Boolean;
begin
  LogMessage(ltPaymentTrace, '465' + #9);
  iCount := TL.OLList.Count;
  SOL := nil;
  aOL := nil;

  for i := TL.OLList.Count - 1 downto 0 do
  begin
    tmpOL := TL.OLList.Items[i];

    if Assigned(TempItemList) then
    begin
      for j := 0 to TempItemList.Count - 1 do
      begin
        SOL := TempItemList.Items[j];
        if SOL.OrderLineID = tmpOL.OrderLineID then
        begin
          SOL.Qty := SOL.Qty + tmpOL.Qty;
          SOL.ToPay := SOL.ToPay + tmpOL.ToPay;
          SOL.Due := SOL.ToPay;
          SOL.UnInvoiced := SOL.ToPay;
          SOL.Invoiced := SOL.Invoiced - tmpOL.ToPay;
          SOL.QtyLeft := SOL.QtyLeft + tmpOL.Qty;

          if SOL.UnitPrice = 0 then
            SOL.ShowZeroPriceItem := True;

          SOL.AllowedDiscount := GetRoundedUpDown(SOL.AllowedDiscount + tmpOL.AllowedDiscount, AppDetails.DecimalPlaces);
          SOL.SubsidyAllowed := GetRoundedUpDown(SOL.SubsidyAllowed + tmpOL.SubsidyAllowed, AppDetails.DecimalPlaces);

          SOL.LoyaltyFreeQty := 0;
          SOL.LoyaltyFreeDiscount := 0;
          Break;
        end;
      end;
      SOL := nil;
    end;

    for j := 0 to UnInvoicedOlsToPay.Count - 1 do
    begin
      SOL := UnInvoicedOlsToPay.Items[j];
      if SOL.OrderLineID = tmpOL.OrderLineID then
      begin
        SOL.Qty := SOL.Qty + tmpOL.Qty;
        SOL.ToPay := SOL.ToPay + tmpOL.ToPay;
        SOL.Due := SOL.ToPay;
        SOL.UnInvoiced := SOL.ToPay;
        SOL.Invoiced := SOL.Invoiced - tmpOL.ToPay;
        SOL.QtyLeft := SOL.QtyLeft + tmpOL.Qty;

        if SOL.UnitPrice = 0 then
          SOL.ShowZeroPriceItem := True;

        SOL.AllowedDiscount := GetRoundedUpDown(SOL.AllowedDiscount + tmpOL.AllowedDiscount, AppDetails.DecimalPlaces);
        SOL.SubsidyAllowed := GetRoundedUpDown(SOL.SubsidyAllowed + tmpOL.SubsidyAllowed, AppDetails.DecimalPlaces);

        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + tmpOL.LoyaltyFreeQty;   //do only once
        SOL.LoyaltyFreeQty := 0;
        SOL.LoyaltyFreeDiscount := 0;
        Break;
      end;
    end;

    FoundSelected := False;
    for j := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[j];
      if aOL.OrderLineID = tmpOL.OrderLineID then
      begin
        FoundSelected := True;
        Break;
      end;
      aOL := nil;
    end;

    if (not FoundSelected) and Assigned(SOL) then
    begin
      AddOrderLine(UnInvoicedOlsToPay, UnInvoicedOlsToPay.IndexOf(SOL));
      ReOrderOLtoPayList;
    end
    else if Assigned(aOL) then
    begin
      if (not Assigned(SOL)) or ((Assigned(SOL) and ((aOL.Qty + tmpOL.Qty) < SOL.Qty))) then
      begin
        aOL.Qty := aOL.Qty + tmpOL.Qty;
        aOL.ToPay := aOL.ToPay + tmpOL.ToPay;
        aOL.Due := aOL.ToPay;
        aOL.UnInvoiced := aOL.ToPay;
        aOL.Invoiced := aOL.Invoiced + tmpOL.Invoiced;
        aOL.QtyLeft := aOL.QtyLeft + tmpOL.Qty;

        if aOL.UnitPrice = 0 then
          aOL.ShowZeroPriceItem := True;

        aOL.AllowedDiscount := GetRoundedUpDown(aOL.AllowedDiscount + tmpOL.AllowedDiscount, AppDetails.DecimalPlaces);
        aOL.SubsidyAllowed := GetRoundedUpDown(aOL.SubsidyAllowed + tmpOL.SubsidyAllowed, AppDetails.DecimalPlaces);
      end
      else if ((aOL.Qty + tmpOL.Qty) >= SOL.Qty) then
      begin
        aOL.Qty := sOL.Qty;
        aOL.ToPay := sOL.ToPay;
        aOL.Due := sOL.ToPay;
        aOL.UnInvoiced := sOL.ToPay;
        aOL.Invoiced := sOL.Invoiced;
        aOL.QtyLeft := sOL.QtyLeft;

        if aOL.UnitPrice = 0 then
          aOL.ShowZeroPriceItem := True;

        aOL.AllowedDiscount := sOL.AllowedDiscount;
        aOL.SubsidyAllowed := sOL.SubsidyAllowed;
      end;
      aOL.LoyaltyFreeQty := 0;
      aOL.LoyaltyFreeDiscount := 0;
      aOL.TenderSeqID := 0;
    end;

    tmpOL.Free;
    TL.OLList.Delete(i);
  end;

  if TL.OLList.Count <> iCount then
  begin
    LogMessage(ltPaymentTrace, '435' + #9);
    CalculateToPay;
    LogMessage(ltPaymentTrace, '525' + #9);
  end;
end;

procedure TformPaymentTable.ReOrderOLtoPayList;

var
  i, iCount, j: Integer;
  aOL, SOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '473' + #9);
  iCount := OLToPayList.Count - 1;

  for i := 0 to UnInvoicedOlsToPay.Count - 1 do
  begin
    SOL := UnInvoicedOlsToPay.Items[i];
    for j := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[j];

      if SOL.OrderLineID = aOL.OrderLineID then
      begin
        OLToPayList.Add(aOL);
      end;
    end;
  end;
  
  for i := iCount downto 0 do
    OLToPayList.Delete(i);

end;

procedure TformPaymentTable.ClearOLsWithoutTenderSeqNo;
var
  i, iCount: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '464' + #9);
  iCount := OLToPayList.Count;
  for i := OLToPayList.Count - 1 downto 0 do
  begin
    aOL := OLToPayList.Items[i];
    if aOL.TenderSeqID <= 0 then
    begin
      if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyItemID > 0)) then begin
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
      end;

      OLToPayList.Delete(i);
    end;
  end;

  if iCount = OLToPayList.Count then
    Exit;

  with grdToPay do
  begin
    RowCount := 2;

    Cells[0, 1] := '';
    Cells[1, 1] := '';
    Cells[2, 1] := '';
    Cells[3, 1] := '';
    Cells[4, 1] := '';

    for i := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[i];
      if (DM.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, []) and (not dm.qrOLsToPay.Fields[0].IsNull)) then
        AddOrderLine(UnInvoicedOlsToPay, i);
    end;
  end;
end;

function TformPaymentTable.CheckForUnTenderedItems: Boolean;
var
  i: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '466' + #9);
  Result := False;
  try
    for i := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[i];
      if aOL.TenderSeqID <= 0 then
      begin
        Result := True;
        Exit;
      end;
    end;
  finally
    FDiscCalculator.OneBillUseDiscountAmount := (not Result);
  end;
end;


procedure TformPaymentTable.ClearUnTenderedOrderLines;
var
  OL: TOrderLine;
  i: Integer;
begin
  LogMessage(ltPaymentTrace, '226' + #9);
  if (AppDetails.OneBill) and (FUseDiscountAmount) then
    FDiscountAmount := 0;

  for I := (OLToPayList.Count - 1) downto 0 do
  begin
    OL := OLToPayList.Items[I];
    if OL.TenderSeqID > 0 then
    begin
      if (AppDetails.OneBill) and (FUseDiscountAmount) then
        FDiscountAmount := FDiscountAmount + OL.AllowedDiscount;

      Continue;
    end;

    if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (OL.LoyaltyItemID > 0) and (OL.ComboID <= 0)) then begin
      LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + OL.LoyaltyFreeQty;
    end;
    OL.Free;
    OLToPayList.Delete(I);
  end;

  if (AppDetails.OneBill) and (FUseDiscountAmount) and (OLToPayList.Count = 0) then
  begin
    FDiscountAmount := FDiscCalculator.OneBillDiscountAmount;
    FDiscCalculator.OneBillUseDiscountAmount := True;
  end;

  if OLToPayList.Count = 0 then
  begin
    LogMessage(ltPaymentTrace, '227' + #9);
    with grdToPay do
    begin
      RowCount := 2;
      Cells[0, 1] := '';
      Cells[1, 1] := '';
      Cells[2, 1] := '';
      Cells[3, 1] := '0.00';
      Cells[4, 1] := '0.00';
      Row := 1;
    end;
  end;

  LogMessage(ltPaymentTrace, '228' + #9);
  CalculateToPay();

  LogMessage(ltPaymentTrace, '229' + #9);
  grdItemAutoClick := True;
  grdToPayClick(nil);
  LogMessage(ltPaymentTrace, '230' + #9);
end;

procedure TformPaymentTable.AssociateOLSToTender(var TL: TTL ; ATenderSeqNo: Integer);
var
  i: Integer;
  aOL, tmpOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '231' + #9);
  for i := 0 to OLToPayList.Count - 1 do
  begin
    aOL := OLToPayList.Items[i];
    if ((aOL.TenderSeqID = ATenderSeqNo) and ((aOL.Qty > 0) or ((aOL.UnitPrice <= 0) and (aOL.ShowZeroPriceItem)) )) then
    begin
      LogMessage(ltPaymentTrace, '232' + #9);
      tmpOL := TOrderLine.Create;
      CopyOL(tmpOL, aOL);
      TL.OLList.Add(tmpOL);
    end;
  end;
  LogMessage(ltPaymentTrace, '233' + #9);
end;

procedure TformPaymentTable.GetAllSelectedOLs(var AList: Tlist);
var
  i, j: Integer;
  aOL, ListOL: TOrderLine;
  TL: TTL;
  bAddTender, ItemsSelected, OLAlreadyAdded: Boolean;
begin
  LogMessage(ltPaymentTrace, '234' + #9 + 'FRecordType=' + IntToStr(Ord(FRecordType)) + ', PaymentOnSelectedItems=' + BoolToStr(PaymentOnSelectedItems, True) +
    ', FSelectedItemsNotPaidFull=' + BoolToStr(FSelectedItemsNotPaidFull, True) + ', AmountSplitingStarted=' + BoolToStr(AmountSplitingStarted, True) +
    ', FTenderGrpID=' + IntToStr(FTenderGrpID) + ', FGetAllItems=' + BoolToStr(FGetAllItems, True));

  if not Assigned(AList) then
    Exit;

  //Get all ols already selected.
  AList.Clear;

  ItemsSelected := PaymentOnSelectedItems or FSelectedItemsNotPaidFull or AmountSplitingStarted;

  //Added all OLs already selected and associated to tender.
  if FRecordType in [rtTenderedItems, rtBoth] then
  begin
    for i := 0 to TLList.Count-1 do
    begin
      TL := TLList.Items[i];
      LogMessage(ltPaymentTrace, '235' + #9 + 'TL.TenderGroupID=' + IntToStr(TL.TenderGroupID));

      if AppDetails.OneBill then
        bAddTender := ((not ItemsSelected) or (ItemsSelected and (FTenderGrpID = TL.TenderGroupID)))
      else
        bAddTender := (FGetAllItems) or ((not FGetAllItems) and (((not ItemsSelected) or (ItemsSelected and (FTenderGrpID = TL.TenderGroupID)))));

      if ((bAddTender) and (Assigned(TL.OLList)) and (TL.OLList.Count > 0)) then
      begin
        LogMessage(ltPaymentTrace, '236' + #9);
        for j := 0 to TL.OLList.Count - 1 do
        begin
          AList.Add(TL.OLList.Items[j]);
        end;
      end;
    end;
  end;

  if FRecordType in [rtUnTenderedItems, rtBoth] then
  begin
    for i := 0 to OLToPayList.Count - 1 do
    begin
      aOL := OLToPayList.Items[i];
      LogMessage(ltPaymentTrace, '237' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID) + ', aOL.QtyLeft=' + FloatToStr(aOL.QtyLeft));

      //determine if OL has already been added to the list (in the code above)
      OLAlreadyAdded := False;
      for j := 0 to Pred(AList.Count) do
      begin
        ListOL := AList.Items[j];
        if (aOL.OrderLineID = ListOL.OrderLineID) and (aOL.QtyLeft = 0) then
        begin
          OLAlreadyAdded := True;
          Break;
        end;
      end;

      if not OLAlreadyAdded then
      begin
        LogMessage(ltPaymentTrace, '238' + #9);
        AList.Add(aOL);
      end;
    end;
  end;
  LogMessage(ltPaymentTrace, '239' + #9);
end;

procedure TformPaymentTable.CombineSameOls(var AList: TList);
var
  tmpList: TList;
  i, j: Integer;
  aOL, SOL, tmpNewOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '467' + #9);
  //returns a list of copied OLs, so ensure they are freed as required

  if AList.Count = 0 then
    Exit;

  tmpList := TList.Create;
  try
    for i := 0 to AList.Count - 1 do
    begin
      SOL := AList.Items[i];
      SOL.AlreadyCombined := False;
    end;

    if AList.Count >= 2 then
    begin
      for i := 0 to AList.Count - 2 do
      begin
        SOL := AList.Items[i];
        if not SOL.AlreadyCombined then
        begin
          tmpNewOL := TOrderLine.Create;
          CopyOL(tmpNewOL, SOL);         //copy it first, so dont change original that might be OL assigned to a TL
          tmpList.Add(tmpNewOL);  
          SOL := tmpNewOL;               //change the copy, not the original

          for j := AList.Count - 1 downto i + 1  do
          begin
            aOL := AList.Items[j];

            if not aOL.AlreadyCombined then
            if ((i <> j) and (SOL.OrderLineID = aOL.OrderLineID)) then
            begin
              SOL.ToPay := SOL.ToPay + aOL.ToPay;
              if (SOL.UnitPrice <> 0) then
                SOL.Qty := SOL.ToPay / SOL.UnitPrice
              else
                SOL.Qty := SOL.Qty + aOL.Qty;
              if (AppDetails.EnableFiscalPrinting) then
                SOL.Qty := TruncateTo(SOL.Qty, 3)
              else
                SOL.Qty := CRoundTo(SOL.Qty, 4);
              SOL.Due := SOL.ToPay;
              SOL.UnInvoiced := SOL.ToPay;
              SOL.AllowedDiscount := SOL.AllowedDiscount + aOL.AllowedDiscount;
              SOL.ComboQty := SOL.ComboQty + aOL.ComboQty;
              SOL.SubsidyAllowed := SOL.SubsidyAllowed + aOL.SubsidyAllowed;
              aOL.AlreadyCombined := True;
            end;
          end;
        end;
      end;

      SOL := AList.Items[AList.Count-1];  //last item
      if not SOL.AlreadyCombined then
      begin
        tmpNewOL := TOrderLine.Create;
        CopyOL(tmpNewOL, SOL);
        tmpList.Add(tmpNewOL);
      end;
    end
    else
    begin
      SOL := AList.Items[0];
      tmpNewOL := TOrderLine.Create;
      CopyOL(tmpNewOL, SOL);
      tmpList.Add(tmpNewOL);
    end;

    AList.Clear;
    for i := 0 to tmpList.Count - 1 do
      AList.Add(tmpList.Items[i]);

  finally
    tmpList.Clear;
    tmpList.Free;
  end;
end;

procedure TformPaymentTable.CalculateAmountForUntenderedItems;
begin
  LogMessage(ltPaymentTrace, '468' + #9);
  if PaymentOnSelectedItems or AmountSplitingStarted then
  begin
    FRecordType := rtBoth;

    FPayment := 0;
    txtPayment.Text := FormatCurrencyNoSign(FPayment);
  end;
  LogMessage(ltPaymentTrace, '436' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '526' + #9);
end;

function TformPaymentTable.GetNFManualExtraDetails:Boolean;
var
  frmManual: TformNFEDetail;
begin
  Result := False;
  FManualSeries := '';
  FManualSubSeries := '';
  FManualInvNo := '';

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  frmManual := TformNFEDetail.Create(Nil);
  try
    frmManual.DisplayTaxDetails := False;
    frmManual.NFManual := NFManual;
    frmManual.DisplayNFEDetails := False;
    if frmManual.ShowModal = mrOk then
    begin
      FManualSeries := frmManual.ManualSeries;
      FManualSubSeries := frmManual.ManualSubSeries;
      FManualInvNo := frmManual.ManualInvoiceNo;
      Result := True;
    end;
  finally
    frmManual.Free;
  end;
end;

procedure TformPaymentTable.FormActivate(Sender: TObject);
begin
  inherited;
  FormActivated := True;
end;

function TformPaymentTable.GetNFEDetails:Boolean;
var
  frmNFE : TformNFEDetail;
begin
  Result := False;
  FManualSeries := '';
  FManualSubSeries := '';
  FManualInvNo := '';

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  frmNFE := TformNFEDetail.Create(Nil);
  try
    frmNFE.DisplayTaxDetails := False;
    frmNFE.NFManual := False;
    frmNFE.DisplayNFEDetails := True;
    if frmNFE.ShowModal = mrOk then
      Result := True;
  finally
    frmNFE.Free;
  end;
end;

procedure TformPaymentTable.ReloadItemsForThePayment(AList: TList; aPaid: Currency);
var
  i: Integer;
  OldQty, Allocated, AmountToPay, UnInvoiced, ToPay: Currency;
  aOL, NewOL: TOrderLine;
  ToPayChanged, AddZeroPricedOL: Boolean;
  DiscAmount: Currency;
  TotalAllocate: Currency;
begin
  LogMessage(ltPaymentTrace, '278' + #9 + 'AList.Count=' + IntToStr(AList.Count) + ', aPaid=' + FloatToStr(aPaid) +
    ', FDiscountAmount=' + FloatToStr(FDiscountAmount) + ', FComboDiscount=' + FloatToStr(FComboDiscount) +
    ', FTotalSubsidyAllowed=' + FloatToStr(FTotalSubsidyAllowed) + ', FSalesTax=' + FloatToStr(FSalesTax));

  DiscAmount := FDiscountAmount + FComboDiscount + FTotalSubsidyAllowed;

  ReloadItems := True;
  ClearAllOrderLines;
  ReloadItems := False;

  i := 0;
  Allocated := 0.00;
  ReloadItems := True;

  if (AppDetails.EnableFiscalPrinting and FUseItemDiscounts) then
    APaid := APaid + FSalesTax;

  AmountToPay := GetRoundedUpDown(APaid, AppDetails.DecimalPlaces);
  if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    AmountToPay := TruncateTo(APaid, 2);
  LogMessage(ltPaymentTrace, '279' + #9 + 'AmountToPay=' + FloatToStr(AmountToPay));

  while ((i <= AList.Count - 1) and (Allocated < AmountToPay)) do
  begin
    aOL := AList.Items[i];
    UnInvoiced := aOL.UnInvoiced;
    ToPay := aOL.ToPay;
    AddZeroPricedOL := ((aOL.UnitPrice <= 0) and aOL.ShowZeroPriceItem);
    if ((aOL.Qty > 0) and (DiscAmount > 0)) then
    begin
      UnInvoiced := aOL.UnInvoiced - aOL.AllowedDiscount - aOL.SubsidyAllowed;
      ToPay := aOL.ToPay - aOL.AllowedDiscount - aOL.SubsidyAllowed;
    end;

    if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
      ToPay := TruncateTo(ToPay, 2);

    LogMessage(ltPaymentTrace, '280' + #9 + 'Allocated=' + FloatToStr(Allocated) + ', UnInvoiced=' + FloatToStr(UnInvoiced) + ', ToPay=' + FloatToStr(ToPay) +
      ', aOL.UnInvoiced=' + FloatToStr(aOL.UnInvoiced) + ', aOL.ToPay=' + FloatToStr(aOL.ToPay) + ', aOL.AllowedDiscount=' + FloatToStr(aOL.AllowedDiscount) +
      ', aOL.SubsidyAllowed=' + FloatToStr(aOL.SubsidyAllowed) + ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice) +
      ', aOL.ShowZeroPriceItem=' + BoolToStr(aOL.ShowZeroPriceItem, True) + ', aOL.Qty=' + FloatToStr(aOL.Qty));

    if AddOrderLine(AList, i) then
    begin
      LogMessage(ltPaymentTrace, '281' + #9);
      if ((((aOL.Qty - aOL.LoyaltyFreeQty) > 0) and (UnInvoiced > 0)) or (AddZeroPricedOL) ) then
      begin
        LogMessage(ltPaymentTrace, '282' + #9);

        //NewOL is the OrderLine just added by AddOrderLine
        NewOL := OLToPayList.Items[grdToPay.Row - 1];
        NewOL.ComboID := 0;
        NewOL.ComboGroupID := 0;
        NewOL.ComboIndex := 0;
        NewOL.ComboDiscount := 0;
        NewOL.ComboName := '';
        NewOL.ComboQty := 0;

        if ((AppDetails.EnableCombo) and (aOL.ComboID > 0) and (aOL.ComboGroupID > 0) and (aOL.ComboIndex > 0)) then
        begin
          LogMessage(ltPaymentTrace, '283' + #9);
          NewOL.ComboID := aOL.ComboID;
          NewOL.ComboGroupID := aOL.ComboGroupID;
          NewOL.ComboIndex := aOL.ComboIndex;
          NewOL.ComboName := aOL.ComboName;
          NewOL.ApplyDiscountType := aOL.ApplyDiscountType;
          NewOL.FreeComboItem := aOL.FreeComboItem;
          NewOL.ComboDiscount := 0;
          NewOL.ComboQty := aOL.ComboQty;
          NewOL.AllowedDiscount := aOL.AllowedDiscount;
          NewOL.SubsidyAllowed := aOL.SubsidyAllowed;
          NewOL.ILDiscount := aOL.ILDiscount;
          NewOL.Qty := aOL.Qty;
          NewOL.OrderLineID := aOL.OrderLineID;
        end;

        ToPayChanged := False;
        if (AppDetails.TaxExclusivePrices) then
        begin
          LogMessage(ltPaymentTrace, '284' + #9);
          TotalAllocate := GetRoundedUpDown(Allocated + ((ToPay * (100 + NewOL.SalesTaxPercent)) / 100), AppDetails.DecimalPlaces);
          if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
            TotalAllocate := TruncateTo((Allocated + ((ToPay * (100 + NewOL.SalesTaxPercent)) / 100)), 2);

          if (TotalAllocate > AmountToPay) then
          begin
            LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + NewOL.LoyaltyFreeQty;
            if ((DiscAmount > 0)) then
              NewOL.ToPay := ((AmountToPay - Allocated) * 100) / (100 + NewOL.SalesTaxPercent) / (1 - ((NewOL.AllowedDiscount) / NewOL.ToPay))
            else
              NewOL.ToPay := ((AmountToPay - Allocated) * 100) / (100 + NewOL.SalesTaxPercent);

            if ((DiscAmount > 0)) then
              NewOL.AllowedDiscount := NewOL.ToPay  -  ((AmountToPay - Allocated) * 100) / (100 + NewOL.SalesTaxPercent);
            ToPay := ((AmountToPay - Allocated) * 100) / (100 + NewOL.SalesTaxPercent);
            ToPayChanged := True;
          end;
          Allocated := Allocated + ((ToPay * (100 + NewOL.SalesTaxPercent)) / 100);
        end
        else
        begin
          LogMessage(ltPaymentTrace, '285' + #9);
          TotalAllocate := GetRoundedUpDown(Allocated + ToPay, AppDetails.DecimalPlaces);
          if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
            TotalAllocate := TruncateTo(Allocated + ToPay, 2) ;
          if (TotalAllocate > AmountToPay) then
          begin
            LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + NewOL.LoyaltyFreeQty;
            if ((DiscAmount > 0)) then
              NewOL.ToPay := ((AmountToPay - Allocated) / (1 - ((NewOL.AllowedDiscount+NewOL.SubsidyAllowed)/NewOL.ToPay)))
            else
              NewOL.ToPay := (AmountToPay - Allocated);

            ToPay := (AmountToPay - Allocated);
            ToPayChanged := True;
          end;
          Allocated := Allocated + ToPay;
        end;

        NewOL.QtyChanged := False;
        if (ToPayChanged) then
        begin
          LogMessage(ltPaymentTrace, '286' + #9);
          NewOL.QtyChanged := True;
          OldQty := NewOL.Qty;
          if AppDetails.EnableFiscalPrinting then
            NewOL.ToPay := TruncateTo(NewOL.ToPay, 2)
          else
            NewOL.ToPay := CRoundTo(NewOL.ToPay, 2);

          if (NewOL.UnitPrice > 0) then
          begin
            NewOL.Qty := CRoundTo(NewOL.ToPay / NewOL.UnitPrice, 4);
          end
          else begin
            NewOL.Qty := 0;
          end;

          if ((DiscAmount > 0)) then
          begin
            if OldQty <> NewOL.Qty then
            begin
              LogMessage(ltPaymentTrace, '287' + #9);
              NewOL.AllowedDiscount := GetRoundedUpDown(NewOL.AllowedDiscount * (NewOL.Qty/OldQty), AppDetails.DecimalPlaces);
              NewOL.SubsidyAllowed := GetRoundedUpDown(NewOL.SubsidyAllowed * (NewOL.Qty/OldQty), AppDetails.DecimalPlaces);
            end
            else
            begin
              LogMessage(ltPaymentTrace, '288' + #9);
              NewOL.AllowedDiscount := GetRoundedUpDown(NewOL.AllowedDiscount, AppDetails.DecimalPlaces) ;
              NewOL.SubsidyAllowed := GetRoundedUpDown(NewOL.SubsidyAllowed, AppDetails.DecimalPlaces) ;
            end;
          end;
          NewOL.Due := NewOL.Due - NewOL.ToPay;
          NewOL.UnInvoiced := NewOL.UnInvoiced - NewOL.ToPay;

          LogMessage(ltPaymentTrace, '289' + #9);
          AllocateItemLoyalty(NewOL);

          LogMessage(ltPaymentTrace, '290' + #9);
          UpdateGrdToPayCells(grdToPay.Row);
          LogMessage(ltPaymentTrace, '568' + #9);
        end;
      end;
    end;

    inc(i);
  end;

  while (i <= AList.Count - 1) do
  begin
    LogMessage(ltPaymentTrace, '291' + #9);
    aOL := AList.Items[i];
    AddZeroPricedOL := ((aOL.UnitPrice <= 0) and aOL.ShowZeroPriceItem);
    if (((aOL.Qty > 0) and (DiscAmount > 0) ) or AddZeroPricedOL) then
    begin
      UnInvoiced := aOL.UnInvoiced - aOL.AllowedDiscount - aOL.SubsidyAllowed;

      LogMessage(ltPaymentTrace, '292' + #9 + 'UnInvoiced=' + FloatToStr(UnInvoiced) +
        ', aOL.UnInvoiced=' + FloatToStr(aOL.UnInvoiced) + ', aOL.AllowedDiscount=' + FloatToStr(aOL.AllowedDiscount) +
        ', aOL.SubsidyAllowed=' + FloatToStr(aOL.SubsidyAllowed) + ', aOL.UnitPrice=' + FloatToStr(aOL.UnitPrice) +
        ', aOL.ShowZeroPriceItem=' + BoolToStr(aOL.ShowZeroPriceItem, True) + ', aOL.Qty=' + FloatToStr(aOL.Qty));

      if (UnInvoiced <= 0) then
      begin
        LogMessage(ltPaymentTrace, '293' + #9);
        AddOrderLine(AList, i); // means adding free items at the end of the list if items selected
      end;
    end;
    Inc(i);
  end;

  ReloadItems := False;
  LogMessage(ltPaymentTrace, '294' + #9);
end;

procedure TformPaymentTable.ClearTempItemList;
begin
  LogMessage(ltPaymentTrace, '269' + #9);
  ClearListObjects(TempItemList);
end;

procedure TformPaymentTable.RecalculateTotals;
begin
  LogMessage(ltPaymentTrace, '240' + #9 + 'PaymentOnSelectedItems=' + BoolToStr(PaymentOnSelectedItems, True) + ', FSelectedItemsNotPaidFull=' + BoolToStr(FSelectedItemsNotPaidFull, True) +
    ', AmountSplitingStarted=' + BoolToStr(AmountSplitingStarted, True));
  LogMessage(ltPaymentTrace, '248' + #9 + 'FPayment=' + FloatToStr(FPayment)+ ', FTip=' + FloatToStr(FTip) + ', FChange=' + FloatToStr(FChange) +
    ', FTender=' + FloatToStr(FTender) + ', FRounding=' + FloatToStr(FRounding) + ', FTenderGrpID=' + IntToStr(FTenderGrpID));

  if PaymentOnSelectedItems or FSelectedItemsNotPaidFull or AmountSplitingStarted  then
    TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding, FTenderGrpID)
  else
    TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding);
  LogMessage(ltPaymentTrace, '241' + #9 + 'FPayment=' + FloatToStr(FPayment)+ ', FTip=' + FloatToStr(FTip) + ', FChange=' + FloatToStr(FChange) +
    ', FTender=' + FloatToStr(FTender) + ', FRounding=' + FloatToStr(FRounding));

  if FPayment < 0 then
    txtTPayment.Text := '0.00'
  else
    txtTPayment.Text := FormatCurrencyNoSign(FPayment);

  txtTTip.Text := FormatCurrencyNoSign(FTip);
  txtTChange.Text := FormatCurrencyNoSign(FChange);

  if FTender < 0 then
    txtTTender.Text := '0.00'
  else
    txtTTender.Text := FormatCurrencyNoSign(FTender);

  if FPayment < 0 then
    txtPayment.Text := '0.00'
  else
    txtPayment.Text := FormatCurrencyNoSign(FPayment);

  LogMessage(ltPaymentTrace, '242' + #9 + 'GlbEvent.EventID=' + IntToStr(GlbEvent.EventID) + ', SubsidyCalc.IsHostTransaction=' + BoolToStr(SubsidyCalc.IsHostTransaction, True));
  LogMessage(ltPaymentTrace, '243' + #9 + 'FToPay=' + FloatToStr(FToPay) + ', FSurcharge=' + FloatToStr(FSurcharge) + ', FComboDiscount=' + FloatToStr(FComboDiscount) +
    ', FDiscountAmount=' + FloatToStr(FDiscountAmount) + ', FLoyaltyFreeItemDiscount=' + FloatToStr(FLoyaltyFreeItemDiscount) + ', FTotalSubsidyAllowed=' + FloatToStr(FTotalSubsidyAllowed) +
    ', FPayment=' + FloatToStr(FPayment) + ', FSalesTax=' + FloatToStr(FSalesTax));

  if ((GlbEvent.EventID <=0) or (SubsidyCalc.IsHostTransaction)) then
    FStillDue := GetRoundedUpDown(CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FDiscountAmount + FLoyaltyFreeItemDiscount+FTotalSubsidyAllowed, FPayment, FSalesTax), AppDetails.DecimalPlaces)
  else
    FStillDue := GetRoundedUpDown(CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FLoyaltyFreeItemDiscount+FTotalSubsidyAllowed, FPayment, FSalesTax), AppDetails.DecimalPlaces);
  LogMessage(ltPaymentTrace, '244' + #9 + 'FStillDue=' + FloatToStr(FStillDue));

  if ((GlbAccount.AccountID > 0) and (OnAccount)) then begin
    LogMessage(ltPaymentTrace, '248' + #9 + 'GlbAccount.AccountID=' + IntToStr(GlbAccount.AccountID) + ', OnAccount=' + BoolToStr(OnAccount, True));

    if ((GlbAccount.CreditLimitActive) and (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance))) then begin
      FRequired := (FStillDue - (GlbAccount.CreditLimit - GlbAccount.CurrentBalance));
      LogMessage(ltPaymentTrace, '245' + #9 + 'FRequired=' + FloatToStr(FRequired) + ', GlbAccount.CreditLimitActive=' + BoolToStr(GlbAccount.CreditLimitActive) +
        ', GlbAccount.CreditLimit' + FloatToStr(GlbAccount.CreditLimit) + ', GlbAccount.CurrentBalance=' + FloatToStr(GlbAccount.CurrentBalance));
    end
    else begin
      FRequired := 0;
      LogMessage(ltPaymentTrace, '246' + #9 + 'FRequired=' + FloatToStr(FRequired));
    end;
  end
  else begin
    FRequired := FStillDue;
    LogMessage(ltPaymentTrace, '247' + #9 + 'FRequired=' + FloatToStr(FRequired));
  end;
  if FRequired < 0 then
    txtStillDue.Text := '0.00'
  else
    txtStillDue.Text := FormatCurrencyNoSign(FRequired);
end;

procedure TformPaymentTable.ProcessTheRemainingItems(TLPay: Currency);
var
  i: Integer;
  tmpOl: TOrderLine;
  AddZeroPriceOl: Boolean;
begin
  LogMessage(ltPaymentTrace, '317' + #9);
  FProcessedAllItems := True;
  FSelectedItemsNotPaidFull := True;
  ClearAllOrderLines;
  for i := 0 to TempItemList.Count -1 do // add back the rest of the items which are not paid completely
  begin
    LogMessage(ltPaymentTrace, '322' + #9);
    tmpOl := TempItemList.Items[i];
    AddZeroPriceOl := (tmpOl.UnitPrice <= 0) and tmpOl.ShowZeroPriceItem;
    if (((tmpOl.UnInvoiced > 0) or AddZeroPriceOl) and
        (DM.qrOLsToPay.Locate('orderlineid', tmpOl.OrderLineID, [])) and
        (not dm.qrOLsToPay.Fields[0].IsNull)) then
    begin
      LogMessage(ltPaymentTrace, '318' + #9);
      AddOrderLine(TempItemList, i);
    end;
  end;
  LogMessage(ltPaymentTrace, '319' + #9);
  LoadUnInvoicedOlsToGrid(TempItemList, TLPay, fAmtToPay);
  LogMessage(ltPaymentTrace, '320' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '321' + #9);
  txtPayment.Text := FormatCurrencyNoSign(FPayment);
end;

procedure TformPaymentTable.CopyItemSToTempList;
var
  i: Integer;
  tmpOl, tmpNew: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '270' + #9);
  for i := 0 to OLToPayList.Count - 1 do
  begin
    tmpOl := OLToPayList.Items[i];
    if Assigned(tmpOl) then
    begin
      tmpNew := TOrderLine.Create;
      CopyOL(tmpNew, tmpOl);
      tmpNew.UnInvoiced := tmpOl.ToPay;
      LogMessage(ltPaymentTrace, '271' + #9 + 'tmpNew.OrderLineID=' + IntToStr(tmpNew.OrderLineID) + ', tmpNew.UnInvoiced=' + FloatToStr(tmpNew.UnInvoiced));
      TempItemList.Add(tmpNew);
    end;
  end;
end;

procedure TformPaymentTable.LoadSelectedToPayList;
var
  i: Integer;
  tmpList: TList;
  tmpRecordType: TRecType;

begin
  LogMessage(ltPaymentTrace, '469' + #9);
  with grdToPay do
  begin
    RowCount := 2;
    Cells[0, 1] := '';
    Cells[1, 1] := '';
    Cells[2, 1] := '';
    Cells[3, 1] := '0.00';
    Cells[4, 1] := '0.00';
  end;
  tmpRecordType := FRecordType;
  FRecordType := rtUnTenderedItems;  //only those left to be tendered

  tmpList := TList.Create;
  GetAllSelectedOLs(tmpList);  //only those left to be tendered
  CombineSameOls(tmpList);    //copies of OLs now in tmpOLList so need to free them

  with grdToPay do
  begin
    //set row count, but leave as is if list empty (with 2nd blank line)
    if tmpList.Count > 0 then
      RowCount := tmpList.Count + 1;

    for i := 0 to (tmpList.Count - 1) do
    begin
      LogMessage(ltPaymentTrace, '556' + #9);
      UpdateGrdToPayCells(i+1);
      LogMessage(ltPaymentTrace, '569' + #9);
    end;
  end;

  FRecordType := tmpRecordType;
  ClearListObjects(tmpList, True);
  LogMessage(ltPaymentTrace, '437' + #9);
  CalculateToPay(True);
  LogMessage(ltPaymentTrace, '527' + #9);
end;

procedure TformPaymentTable.ApplyPreviousNonAppliedDiscount;
begin
  LogMessage(ltPaymentTrace, '470' + #9);
  Exit;

  if TLList.Count <= 0 then
    Exit;

  ClearAllOrderLines;

  LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, 0);
  AddAllOrderLines;
  RecalculateTotals;
  LogMessage(ltPaymentTrace, '438' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '528' + #9);
end;

procedure TformPaymentTable.ResetLoyaltyReward;
begin
  LogMessage(ltPaymentTrace, '408' + #9);
  FLoyaltyDiscount := 0;
  with LoyaltyReward do
  begin
    LoyaltyRewardID := -1;
    LoyaltyReward := '';
    LoyaltyPoints1 := 0;
    LoyaltyPoints2 := 0;
    Qty := 0;
    DecrementLoyalty1Points := 0;
    DecrementLoyalty2Points := 0;
    RewardOffer := roNone;
    NoSaleTypeID := 0;
    TenderLineTypeID := 0;
    Processed := False;
    AvailableQty := 0;
  end;
end;

procedure TformPaymentTable.ApplyLoyalty(tmpList: TList;FindNewOnly: Boolean = False);
var
  i: Integer;
begin
  LogMessage(ltPaymentTrace, '449' + #9);
  if ((OldLoyaltyRewardOffer = roItem) or (LoyaltyReward.RewardOffer = roItem)) then
  begin
    TestAllOLsLoyaltyItem(tmpList, FindNewOnly);

    for i := 0 to OLToPayList.Count-1 do       //tmpList might include OL linked to TenderLines
    begin
      LogMessage(ltPaymentTrace, '557' + #9);
      UpdateGrdToPayCells(I + 1);
      LogMessage(ltPaymentTrace, '570' + #9);
    end;
  end;
end;

procedure TformPaymentTable.SetItemSelection;
begin
  LogMessage(ltPaymentTrace, '224' + #9 + 'FRecordType=' + IntToStr(Ord(FRecordType)));
  FRecordType := rtBoth;

  if not AppDetails.OneBill then
  begin
    if ((OnAccount) or (TLList.Count = 0)) then {means 100% discount so no tender lines}
    begin
      if OnAccount then
      begin
        if (GlbAccount.CreditLimitActive) then
        begin
          if ((FStillDue > 0) and (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance)) ) then
            FRecordType := rtTenderedItems;
          if GlbAccount.CreditLimit = 0 then
          begin
            if ((FStillDue > 0) and (GlbAccount.CurrentBalance > 0 ) ) then // means account is charged
              FRecordType := rtTenderedItems;
          end;
        end;
      end
      else
        FRecordType := rtUnTenderedItems;
    end
    else
    begin
      if FComboDiscount > 0 then
      begin
        if (GetRoundedUpDown( FComboDiscount - FSurcharge, AppDetails.DecimalPlaces) <> GetRoundedUpDown(FTopay, AppDetails.DecimalPlaces)) then //means 100% combo discount
          FRecordType := rtTenderedItems;
      end
      else
        FRecordType := rtTenderedItems;
    end;
  end;

  if ItemsSelectedManually and (OnAccount ) then
  begin
    if (TLList.Count > 0) then
    begin
      if (GetRoundedUpDown(FPayment + FLoyaltyFreeItemDiscount + FComboDiscount + FDiscountAmount - FSurcharge, AppDetails.DecimalPlaces) = GetRoundedUpDown(FTopay, AppDetails.DecimalPlaces)) then
        FRecordType := rtTenderedItems
      else
        FRecordType := rtBoth;
    end
    else
      FRecordType := rtUnTenderedItems;
  end;

  if ((GetRoundedUpDown(GlbTable.Uninvoiced, AppDetails.DecimalPlaces) = GetRoundedUpDown(FPayment + FComboDiscount+ FDiscountAmount, AppDetails.DecimalPlaces)) and
    (not ItemsSelectedManually)) then // means last payment
    FRecordType := rtBoth;
  LogMessage(ltPaymentTrace, '225' + #9 + 'FRecordType=' + IntToStr(Ord(FRecordType)));
end;

function TformPaymentTable.GetSelectedAccount(AAccountID: Integer; AShowAcctScanned: Boolean; PromptLoyalty: Boolean; PrevAccountID: Integer;
  VerifyPin: Boolean = False; ForGoodyDiscount: Boolean = False): Boolean;
var
  tmpList: TList;
  PinEntered: Boolean;
  sClearItem: Boolean;
  i: Integer;
  TL: TTL;
  tmpTLList: TTLList;
  ConfResult: TModalResult;
  TimeToExit: Boolean;
begin
  LogMessage(ltPaymentTrace, '373' + #9);
  Result := False;

  if not sExternalAccountIntegration.AllowOnetapAccounts then
  begin
    LogMessage(ltPaymentTrace, '374' + #9);
    Exit;
  end;

  //Check goody discount account is only used if a goody discount has just been scanned
  if (AAccountID = AppDetails.GoodyDiscountAccountID) and (not ForGoodyDiscount) then
  begin
    LogMessage(ltPaymentTrace, '375' + #9);
    //NB: Goody discount account may be partially selected, e.g., cmdSelectAccountClick > SelectAccount sets GlbAccount.AccountID (which don't want)
    //If the AccountID has changed, then revert to previously selected account
    if GlbAccount.AccountID <> PrevAccountID then
    begin
      if (PrevAccountID = 0) then
        DeselectAccount
      else
        GlbAccount.LoadAccount(PrevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
    end;
    ShowMsg(sGoodyDiscAcctExclusiveToGoodyDiscSale);
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    LogMessage(ltPaymentTrace, '376' + #9);
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    Exit;
  end;

  ConfResult := mrNone;
  TimeToExit:= False;

  RecalculateDiscount := True;
  tmpList := TList.Create;
  try
    if (not GlbAccount.LoadAccount(aAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay)) then
    begin
      LogMessage(ltPaymentTrace, '377' + #9);
      ShowQuickMsg(sCannotFindCustAcct);
      Exit;
    end;

    //set the account record details as with USelectAccount
    FoundFirstLoyaltyFreeItem := False;
    FCPFNumber := GlbAccount.PerorgTaxNumber;

    if ((GlbAccount.AccountRank > GlbLogin.RankAccount) or ((ManagerAuthorise.Authorise) and (GlbAccount.AccountRank > ManagerAuthorise.AuthoriseAccountRank))) then
    begin
      LogMessage(ltPaymentTrace, '378' + #9);
      ShowQuickMsg(sAccountRankNotAllowSelAccount);
      if (PrevAccountID = 0) then
        DeselectAccount
      else
        GlbAccount.LoadAccount(PrevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
      TimeToExit := True;
    end;

    PinEntered := (GlbLogin.LoginPIN > 0);

    if (not TimeToExit) and ((not PinEntered) and (VerifyPin)) then
    begin
      if ((AppDetails.AccountPinCheckRank <= GlbAccount.AccountRank) and (GlbLogin.LoginPIN = 0) and (not (VerifyPinNo))) then
      begin
        LogMessage(ltPaymentTrace, '379' + #9);
        if (PrevAccountID = 0) then
          DeselectAccount
        else
          GlbAccount.LoadAccount(PrevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
        TimeToExit := True;
      end;
    end;

    if (not TimeToExit) and (GlbAccount.AccountInactive = accInactive) then
    begin
      LogMessage(ltPaymentTrace, '380' + #9);
      ShowMsg(sAccountInactive);
      if (PrevAccountID = 0) then
        DeselectAccount
      else
        GlbAccount.LoadAccount(PrevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
      TimeToExit := True;
    end;

    sClearItem := True;
    if (not TimeToExit) and (TLList.Count > 0) then
    begin
      if AppDetails.EFTPOSIntegrated and EFTPOSTransExists then
      begin
        sClearItem := False;
        ShowMsg(sRefundTenderBeforeAcctChange, True);
      end
      else if ((PaymentOnSelectedItems or AmountSplitingStarted)) then
      begin
        ConfResult := (ShowConf(sConfirmClearCurrentSelection));
        if (ConfResult = mrNo) then
          sClearItem := False;
      end;

      if not sClearItem then
      begin
        LogMessage(ltPaymentTrace, '381' + #9);
        if (PrevAccountID = 0) then
          DeselectAccount(True, ConfResult)
        else
          GlbAccount.LoadAccount(PrevAccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay);
        TimeToExit := True;
      end;
    end;

    if (not sExternalAccountIntegration.AllowOnetapLoyaltyAccounts) then
    begin
      LogMessage(ltPaymentTrace, '382' + #9);
      DeselectAccount;
      TimeToExit := True;
    end;

    if (not TimeToExit) then
    begin
      if Assigned(FDiscCalculator) then
      begin
        LogMessage(ltPaymentTrace, '383' + #9);
        FDiscCalculator.ClearItemLevelDiscounts;
      end;
      txtDiscountPercent.Text := FormatFloat(PRICEFORMAT, 0);
      txtDiscountAmount.Text := FormatFloat(PRICEFORMAT, 0);
    end;

    txtAccountName.Text := GlbAccount.AccountName;

    //Only if account is active, so dont discount or show loyalty reward options
    if (GlbAccount.AccountInactive = accActive) then begin
      LogMessage(ltPaymentTrace, '384' + #9 + 'GlbAccount.AccountDiscount=' + FloatToStr(GlbAccount.AccountDiscount) +
        ', GlbAccount.UseItemDiscounts=' + BoolToStr(GlbAccount.UseItemDiscounts, True) + ', GlbAccount.UseTimePeriods=' + BoolToStr(GlbAccount.UseTimePeriods));
      FDiscountPercent := GlbAccount.AccountDiscount;
      FDiscountAmount := 0;
      FLimitMaximum := GlbAccount.LimitMaximum;
      FMaxDiscount := GlbAccount.MaxDiscount;
      FOrigDiscountPercent := GlbAccount.AccountDiscount;
      FCurrentBalance := GlbAccount.CurrentBalance;
      FUseDiscountAmount := False;
      FUseItemDiscounts := ((GlbAccount.UseItemDiscounts) or (GlbAccount.UseTimePeriods));
    end
    else begin
      FDiscountPercent := 0;
      FDiscountAmount := 0;
      FOrigDiscountPercent := 0;
      FCurrentBalance := GlbAccount.CurrentBalance;
      FUseDiscountAmount := False;
      FUseItemDiscounts := False;
    end;   // Account Active block ends

    LogMessage(ltPaymentTrace, '595' + #9);
    lblAllowItemLevelDiscount.Caption := '';
    if FUseItemDiscounts then
    begin
      LogMessage(ltPaymentTrace, '591' + #9);
      lblAllowItemLevelDiscount.Caption := sAllowedItemLevelDiscount;
    end;

    if (GlbTable.AccountID = GlbAccount.AccountID) then
    begin
      LogMessage(ltPaymentTrace, '385' + #9);
      if GlbTable.DiscountRate > 0 then
      begin
        FOrigDiscountPercent := GlbTable.DiscountRate;
        FUseItemDiscounts := False;
        FUseDiscountAmount := False;
        FDiscountPercent := GlbTable.DiscountRate;
        txtDiscountPercent.Text := FormatFloat('####0', FDiscountPercent);
        LogMessage(ltPaymentTrace, '386' + #9 + 'FDiscountPercent=' + FloatToStr(FDiscountPercent));
        lblAllowItemLevelDiscount.Caption := sTabDiscountApplied;
      end
      else if GlbTable.TabDiscountAmt > 0 then
      begin
        FUseDiscountAmount := True;
        FUseItemDiscounts := False;
        FDiscountPercent := 0;
        if ((GlbTable.TabDiscountAmt-GlbTable.ILDiscount) > FToPay) then
          FDiscountAmount := FToPay
        else
          FDiscountAmount := GlbTable.TabDiscountAmt-GlbTable.ILDiscount;

        if FDiscountAmount < 0 then
          FDiscountAmount := 0;

        txtDiscountAmount.Text := FormatFloat('####0', FDiscountAmount);
        LogMessage(ltPaymentTrace, '387' + #9 + 'FDiscountAmount=' + FloatToStr(FDiscountAmount));
        lblAllowItemLevelDiscount.Caption := sTabDiscountApplied;
      end;
    end;

    ChangeAllowCreditLabel(GlbAccount.AllowCredit and (GlbAccount.AccountInactive = accActive), VerifyPin);    //do before CalculateDiscounts, so OnAccount is set

    if (TimeToExit) then Exit;

    if (TLList.Count > 0) and sClearItem then
    begin
      LogMessage(ltPaymentTrace, '388' + #9);
      tmpTLList:= TLList;
      TLList := TTLList.Create;
      ClearAllTenderLines;
      for i := 0 to tmpTLList.Count - 1 do
      begin
        TL := tmpTLList.Items[i];
        LogMessage(ltPaymentTrace, '389' + #9);
        DeleteOLsOfTender(TL, TL.TenderSeqNo);  //so OLs become part of UnInvoicedOLsToPay
      end;

      PaymentOnSelectedItems := False;
      AmountSplitingStarted := False;
      FSelectedItemsNotPaidFull := False;

      if (not FUseItemDiscounts) then
      begin
        LogMessage(ltPaymentTrace, '390' + #9);
        ApplyPreviousNonAppliedDiscount;
      end;

      LogMessage(ltPaymentTrace, '391' + #9);
      CalculateAccTypeItemDiscount(UnInvoicedOlsToPay);

      LogMessage(ltPaymentTrace, '392' + #9);
      LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, FToPay);

      LogMessage(ltPaymentTrace, '393' + #9);
      UpdateUnInvoicedListComboDetails;

      LogMessage(ltPaymentTrace, '394' + #9);
      UpdateScreenAfterCalculateDiscount;

      LogMessage(ltPaymentTrace, '395' + #9);
      AddAllOrderLines;

      if PromptLoyalty and (GlbAccount.AccountInactive = accActive)  then
      begin
        LogMessage(ltPaymentTrace, '396' + #9);
        DoLoyalty(True);
        LogMessage(ltPaymentTrace, '397' + #9);
      end;

      LogMessage(ltPaymentTrace, '398' + #9);
      GetAllSelectedOLs(tmpList);

      LogMessage(ltPaymentTrace, '399' + #9);
      CalculateAccTypeItemDiscount(tmpList);

      LogMessage(ltPaymentTrace, '400' + #9);
      UpdateScreenAfterCalculateDiscount;

      //do separately to keep same TL order in TLList
      for i := 0 to tmpTLList.Count - 1 do
      begin
        TL := tmpTLList.Items[i];
        LogMessage(ltPaymentTrace, '33' + #9);
        AddTenderLine(TL.TLType, 0, TL);
        LogMessage(ltPaymentTrace, '34' + #9);
      end;
      for i := tmpTLList.Count - 1 downto 0 do
      begin
        tmpTLList.Delete(i);  //do so when freeing tmpTLList we dont free the TL objects just added to TLList
      end;
      tmpTLList.Free;

      FSelectedItemsNotPaidFull := False;
    end
    else
    begin
      LogMessage(ltPaymentTrace, '401' + #9);
      UpdateUnInvoicedListComboDetails;

      if PromptLoyalty and (GlbAccount.AccountInactive = accActive)  then
      begin
        LogMessage(ltPaymentTrace, '402' + #9);
        DoLoyalty(True);
        LogMessage(ltPaymentTrace, '403' + #9);
      end;
    end;

    AccountChange := False;
    RecalculateDiscount := False;

    //if Goody discount account, don't allow discount to be changed
    if GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID then
    begin
      cmdDiscountPercent.Enabled := False;
      cmdDiscountAmount.Enabled := False;
    end
    //if have priv to change discount then enable
    else if ((VerifyPriv(13) or (ManagerAuthorise.Authorise and ManagerAuthorise.AuthorisePriv13))) then
    begin
      LogMessage(ltPaymentTrace, '404' + #9);
      cmdDiscountPercent.Enabled := True;
      cmdDiscountAmount.Enabled := True;
    end
    else
    begin
      cmdDiscountPercent.Enabled := False;
      cmdDiscountAmount.Enabled := False;
    end;

    if (AShowAcctScanned and (GlbAccount.SwipedAccountNo > 0)) then
    begin
      LogMessage(ltPaymentTrace, '405' + #9);
      if (AppDetails.AccountDetailScan >= 0) then
        ShowAccountScanned(AppDetails.AccountDetailScan)
      else
        ShowQuickMsg(Format(sCustAcctScanned, [GlbAccount.AccountName]));
    end;
    Result := True;
    LogMessage(ltPaymentTrace, '406' + #9);
  finally
    txtDiscountPercent.Text := FormatFloat(PRICEFORMAT, FDiscountPercent);
    txtDiscountAmount.Text := FormatFloat(PRICEFORMAT, FDiscountAmount);

    tmpList.Clear;
    tmpList.Free;
    LogMessage(ltPaymentTrace, '407' + #9);
  end;
end;

procedure TformPaymentTable.DoLoyalty(Prompt: Boolean);
var
  tmpList: TList;
  i, j: Integer;
  aOL, SOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '356' + #9 + 'Prompt=' + BoolToStr(Prompt, True));
  //if AppDetails.wiGroupEnabled then
  //  wiGroupPOSSrv.CheckWarnLoyaltyDiscNotAllowed;

  tmpList := TList.Create;
  try
    FLoyaltyDiscount := 0;

    if IsNormalLoyalty then
    begin
      LogMessage(ltPaymentTrace, '357' + #9);
      OldLoyaltyRewardOffer := LoyaltyReward.RewardOffer;
      if (not CheckLoyaltyReward(Prompt)) then
      begin
        LogMessage(ltPaymentTrace, '358' + #9);
        Exit;     //user pressed cancel in select rewards screen, no need to alter anything
      end;

      AccountChange := True;

      LogMessage(ltPaymentTrace, '359' + #9);
      GetAllSelectedOLs(tmpList);

      LogMessage(ltPaymentTrace, '360' + #9);
      ApplyLoyalty(tmpList);

      LogMessage(ltPaymentTrace, '361' + #9);
      CalculateAccTypeItemDiscount(UnInvoicedOlsToPay);

      LogMessage(ltPaymentTrace, '362' + #9);
      UpdateScreenAfterCalculateDiscount;

      FOrigDiscountPercent := GlbAccount.AccountDiscount; //Reset discount % back to account default
      if (GlbTable.AccountID = GlbAccount.AccountID) then
      begin
        LogMessage(ltPaymentTrace, '363' + #9);
        if GlbTable.DiscountRate > 0 then
        begin
          FDiscountPercent := GlbTable.DiscountRate;
          FOrigDiscountPercent := FDiscountPercent;    //Jon 17-12-2002
          FUseItemDiscounts := False;
          FUseDiscountAmount := False;
          LogMessage(ltPaymentTrace, '364' + #9 + 'FDiscountPercent=' + FloatToStr(FDiscountPercent));
          txtDiscountPercent.Text := FormatFloat('####0', FDiscountPercent);
        end
        else if GlbTable.TabDiscountAmt > 0 then
        begin
          FUseDiscountAmount := True;
          FUseItemDiscounts := False;
          if ((GlbTable.TabDiscountAmt- GlbTable.ILDiscount) > FToPay) then
            FDiscountAmount := FToPay
          else
            FDiscountAmount := GlbTable.TabDiscountAmt-GlbTable.ILDiscount;

          if FDiscountAmount < 0 then
            FDiscountAmount := 0;

          LogMessage(ltPaymentTrace, '365' + #9 + 'FDiscountAmount=' + FloatToStr(FDiscountAmount));
          txtDiscountAmount.Text := FormatFloat(PRICEFORMAT, FDiscountAmount);
        end;
      end;
      RecalculateDiscount := True;

      LogMessage(ltPaymentTrace, '366' + #9);
      CalculateToPay;
      LogMessage(ltPaymentTrace, '367' + #9);

      RecalculateDiscount := False;

      //apply this loyalty discount to source list too
      for I := 0 to tmpList.Count-1 do
      begin
        aOL := tmpList.Items[I];
        LogMessage(ltPaymentTrace, '368' + #9 + 'aOL.OrderLineID=' + IntToStr(aOL.OrderLineID));

        for j := 0 to UnInvoicedOlsToPay.Count - 1 do
        begin
          SOL := UnInvoicedOlsToPay.Items[j];
          if aOL.OrderLineID = SOL.OrderLineID then
          begin
            SOL.LoyaltyItemID := aOL.LoyaltyItemID;
            SOL.LoyaltyFreeQty := aOL.LoyaltyFreeQty;
            SOL.LoyaltyFreeDiscount := aOL.LoyaltyFreeDiscount;
            if SOL.ComboID <= 0 then
              SOL.AllowedDiscount := aOL.AllowedDiscount;
            LogMessage(ltPaymentTrace, '369' + #9 + 'SOL.OrderLineID=' + IntToStr(SOL.OrderLineID) + ', SOL.LoyaltyItemID=' + IntToStr(SOL.LoyaltyItemID) +
              ', SOL.LoyaltyFreeQty=' + FloatToStr(SOL.LoyaltyFreeQty) + ', SOL.LoyaltyFreeDiscount=' + FloatToStr(SOL.LoyaltyFreeDiscount) +
              ', SOL.ComboID=' + IntToStr(SOL.ComboID) + ', SOL.AllowedDiscount=' + FloatToStr(SOL.AllowedDiscount));
            Break;
          end;
        end;
      end;
    end;
  finally
    tmpList.Clear;
    tmpList.Free;
  end;
  LogMessage(ltPaymentTrace, '370' + #9);
end;

procedure TformPaymentTable.ClearItemLevelDiscounts(AList: TList);
var
  i: Integer;
  OL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '471' + #9);
  if Assigned(AList) then
  if AList.Count > 0 then
  for i := 0 to AList.Count - 1 do
  begin
    OL := AList.Items[i];

    OL.ILDiscount := 0;
    if ((OL.ComboID <= 0) and (OL.ComboGroupID <= 0) and (OL.ComboIndex <= 0)) then
    begin
      OL.AllowedDiscount := 0;
      OL.DiscountSchemeItemPercentage := 0;
      OL.DiscountSchemeItemAmount := 0;
      OL.DiscountSchemeID := 0;
      OL.DiscountedAmount := 0;
      OL.LoyaltyItemID := 0;
      OL.LoyaltyFreeQty := 0;
      OL.LoyaltyFreeDiscount := 0;
    end;
  end;
end;

procedure TformPaymentTable.CalculateComboAndAccountDiscounts(AList:TList);
begin
  LogMessage(ltPaymentTrace, '446' + #9);
  FDiscCalculator.DoingTabPayment := True; // so dont build combos on the fly.

  FDiscCalculator.CalculateDiscountAgain := RecalculateDiscount;
  //if ((not FUseDiscountAmount)) then // and (LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer <> roNone)) then
  if (not FUseDiscountAmount) and (not (LoyaltyReward.RewardOffer in [roDiscount])) then
    FDiscCalculator.CalculateDiscountAgain := True;

  FDiscCalculator.DiscountAmount := FDiscountAmount;
  FDiscCalculator.DiscountPercent := FDiscountPercent;
  FDiscCalculator.GroupID := GlbTable.GroupID;

  FDiscCalculator.UseDiscountAmount := FUseDiscountAmount;
  FDiscCalculator.UseItemDiscounts := FUseItemDiscounts;
  FDiscCalculator.UseDiscountPeriods := FUseDiscountPeriods;

  FDiscCalculator.Account := GlbAccount;
  FDiscCalculator.AccountID := GlbAccount.AccountID;
  FDiscCalculator.OnAccount := OnAccount;
  FDiscCalculator.StillDue := 0;
  FDiscCalculator.MaxDiscount := GlbAccount.MaxDiscount;

  FDiscCalculator.LoyaltyReward := LoyaltyReward;
  FDiscCalculator.LoyaltyDiscount := FLoyaltyDiscount;
  FDiscCalculator.OrigDiscountPercent := FOrigDiscountPercent;
  FDiscCalculator.OrigDiscountAmount := FOrigDiscountAmount;

  //FDiscCalculator.CalculateDiscountAgain := True;
  FDiscCalculator.ShowMessageForOneTime := False;
  FDiscCalculator.ChangeDiscRateManually := False;
  FDiscCalculator.ChangeDiscAmtManually := False;
  FDiscCalculator.SplitTabPayment := ItemsSelectedManually or AmountSplitingStarted;

  FDiscCalculator.CalculateAccountDiscountOnly := False;
  if ((FOrigDiscountPercent = 100) and (not FUseDiscountAmount) and (not FUseItemDiscounts) and (not FUseDiscountPeriods)) then
    FDiscCalculator.CalculateAccountDiscountOnly := True;

  ComboFreeItems.Clear;
  FDiscCalculator.AllCombos := FAllCombos;
  ComboFreeItems.Clear;
  txtComboDisc.Text := '';

  FDiscCalculator.CalculateDiscounts(AList, True, False, False, FDiscountPercent, FDiscountAmount, ComboFreeItems);

  LoyaltyReward := FDiscCalculator.LoyaltyReward;
  FComboDiscount := FDiscCalculator.ComboDiscount;

  if FComboDiscount > 0 then
    txtComboDisc.Text := FormatFloat(PRICEFORMAT, FComboDiscount);

  txtComboDisc.Visible := (FComboDiscount > 0);
  lblComboDisc.Visible := (FComboDiscount > 0);

  txtDiscountPercent.Text := FormatFloat(PRICEFORMAT, FDiscountPercent);
  txtDiscountAmount.Text := FormatFloat(PRICEFORMAT, FDiscountAmount);
end;

function TformPaymentTable.CheckForUnFinishedCombos(aList: TList; var aCombo: string; var aComboIndex: Integer): Boolean;
var
  i, j, k: Integer;
  aOL, OL: TOrderLine;
  iPos: Integer;
  Qty: Double;
  tmpCheckList: TStringList;
  lCombo: string;
  lComboID, lComboIndex: Integer;
  PartialComboFound: Boolean;
begin
  LogMessage(ltPaymentTrace, '615' + #9);
  aCombo := '';
  aComboIndex := 0;
  Result := False; // AList has the selected items with selected qty
  tmpCheckList := TStringList.Create;
  tmpCheckList.Sorted := True;
  tmpCheckList.Duplicates := dupIgnore;
  try
    if ItemsSelectedManually or FSelectedItemsNotPaidFull then
    begin
      for i := 0 to AList.Count-1 do
      begin
        OL := AList.Items[i];
        if ((OL.ComboID > 0) and (OL.ComboIndex > 0)) then
          tmpCheckList.Add(IntToStr(OL.ComboID) + '-' + IntToStr(OL.ComboIndex));
      end;
      if tmpCheckList.Count > 0 then
      begin
        for i := 0 to tmpCheckList.Count - 1 do
        begin
          lCombo := Trim(tmpCheckList.Strings[i]);
          iPos := Pos('-', lCombo);
          lComboID := StrToIntDef(Copy(lCombo, 1, iPos-1), 0);
          lComboIndex := StrToIntDef(Copy(lCombo, iPos+1, Length(lCombo)), 0);
          PartialComboFound := False;
          for j := 0 to UnInvoicedOlsToPay.Count - 1 do
          begin
            aOL := UnInvoicedOlsToPay.Items[j];
            Qty := 0;
            if ((DM.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, [])) and (not dm.qrOLsToPay.Fields[0].IsNull) and
                (DM.qrOLsToPay.FieldByName('unitprice').AsCurrency > 0) and (DM.qrOLsToPay.FieldByName('comboid').AsInteger > 0) and
                (DM.qrOLsToPay.FieldByName('comboid').AsInteger = lComboID) and (DM.qrOLsToPay.FieldByName('comboindex').AsInteger = lComboIndex)) then
            begin
              for k := 0 to AList.Count - 1 do
              begin
                OL := AList.Items[k];
                if ((OL.ComboID > 0) and (OL.ComboIndex > 0) and (OL.ComboID = lComboID) and (OL.ComboIndex = lComboIndex) and (DM.qrOLsToPay.FieldByName('orderlineid').AsInteger = OL.OrderLineID)) then
                  Qty := Qty + OL.Qty;
              end;
              if (aOL.OLQty <> Qty) then
              begin
                PartialComboFound := True;
                aCombo := aOL.ComboName;
                aComboIndex := aOL.ComboIndex;
                Break;
              end;
            end;
          end;
          if PartialComboFound then
          begin
            Result := True;
            Exit;
          end;
        end;
      end;
    end;
  finally
    tmpCheckList.Free;
  end;
end;

procedure TformPaymentTable.grdToPayDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
var
  aOL: TOrderLine;
  FontColour: TColor;
  aAlignment: TAlignment;

begin
  inherited;
  FontColour := clBlack;
  if (((OLToPayList.Count) >= ARow) and (ARow > 0)) then
  begin
    aOL := OLToPayList.Items[ARow - 1];

    if (aOL.ComboID > 0) then
    begin
      FontColour := aOL.ComboColour;
    end;
  end;
  aAlignment := taLeftJustify;
  if (aCol in [3, 4]) then
  begin
    aAlignment := taRightJustify;
  end;
  Skin.ApplySkinOnStringGridDraw(TStringGrid(Sender), ARow, ACol, Rect, State, FontColour, False, False, aAlignment);
end;

procedure TformPaymentTable.grdToPayEnter(Sender: TObject);
begin
  inherited;
  grdItemManualClick := True;
end;

procedure TformPaymentTable.grdToPayExit(Sender: TObject);
begin
  inherited;
  grdItemManualClick := False;
end;

procedure TformPaymentTable.grdToPayKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  inherited;
  if (key = VK_UP) or (Key = VK_Down) or (key = VK_LEFT) or (key = VK_RIGHT) or (key = VK_PRIOR) or (key = VK_NEXT)then
   grdItemAutoClick := True;

  if ((grdToPay.Row = 1) and (key = VK_UP)) or ((grdToPay.Row = 1) and (key = VK_LEFT)) or ((grdToPay.Row = 1) and (key = VK_PRIOR)) then
    grdToPayClick(nil);
  if (((grdToPay.RowCount - 1) = grdToPay.Row) and (key = VK_DOWN)) or (((grdToPay.RowCount - 1) = grdToPay.Row) and (key = VK_RIGHT)) or (((grdToPay.RowCount - 1) = grdToPay.Row) and (key = VK_NEXT)) then
    grdToPayClick(nil);

end;

procedure TformPaymentTable.grdToPayMouseWheelDown(Sender: TObject;
  Shift: TShiftState; MousePos: TPoint; var Handled: Boolean);
begin
  inherited;
  grdItemAutoClick := True;
end;

procedure TformPaymentTable.grdToPayMouseWheelUp(Sender: TObject;
  Shift: TShiftState; MousePos: TPoint; var Handled: Boolean);
begin
  inherited;
  grdItemAutoClick := True;
end;

function TformPaymentTable.GetOriginalOL(aOrderLineID: Integer; var aSrcIndex: Integer): TOrderLine;
var
  i: Integer;
  aOL: TOrderLine;
  
begin
  Result := nil;
  for i := 0 to UnInvoicedOlsToPay.Count - 1 do
  begin
    aOL := UnInvoicedOlsToPay.Items[i];
    if aOrderLineID = aOL.OrderLineID then
    begin
      Result := aOL;
      ASrcIndex := i;
      Exit;
    end;
  end;
end;

procedure TformPaymentTable.grdUnInvoicedListDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
var
  aOL: TOrderLine;
  Italic, Bold: Boolean;
  iOrdLineID, iSrcIndex: Integer;
  FontColour: TColor;
  aAlignment: TAlignment;
begin
  inherited;
  if (((UnInvoicedOlsToPay.Count) >= ARow) and (ARow > 0)) then
  begin
    iOrdLineID := Integer(grdUnInvoicedList.Objects[0, ARow]);
    aOL := GetOriginalOL(iOrdLineID, iSrcIndex);

    Italic := False;
    Bold := False;
    FontColour := clBlack;

    if (Assigned(aOL) and (aOL.ComboID > 0)) then
    begin
      FontColour := aOL.ComboColour;
    end;
  end
  else
  begin
    Italic := False;
    Bold := False;
    FontColour := clBlack;
  end;
  aAlignment := taLeftJustify;
  if (aCol in [2, 3, 6, 7]) then
  begin
    aAlignment := taRightJustify;
  end;
  Skin.ApplySkinOnStringGridDraw(TStringGrid(Sender), ARow, ACol, Rect, State, FontColour, Italic, Bold, aAlignment);
end;

procedure TformPaymentTable.SetComboColourIndex(AList:TList);
var
  j, iMax, i: Integer;
  OL, SecOL: TOrderLine;
begin
  iMax := 1;
  cmdComboIndex.Enabled := False;
  if Assigned(AList) then
  for i := 0 to (AList.Count - 1) do
  begin
    OL := AList.Items[i];
    if (Assigned(OL) and (OL.ComboID > 0)) then
    begin
      OL.ComboColourIndex := -1;
    end;
  end;

  for i := 0 to (AList.Count - 1) do
  begin
    OL := AList.Items[i];
    if (Assigned(OL) and (OL.ComboID > 0) and (OL.ComboColourIndex = -1)) then
    begin
      cmdComboIndex.Enabled := True;
      OL.ComboColourIndex := iMax;
      for j := i + 1 to AList.Count -1 do
      begin
        SecOL := AList.Items[j];

        if (Assigned(SecOL) and (SecOL.ComboID = OL.ComboID) and (SecOL.ComboIndex = OL.ComboIndex)) then
          SecOL.ComboColourIndex := iMax;
      end;
      Inc(iMax);
    end;
  end;
end;

function TformPaymentTable.CheckPrinterFunctions: Boolean;
var
  ExitQS, DateNotMoved: Boolean;
  iDate, iErr: Integer;
begin
  Result := True;
  SetStatus(sFiscalPrinterChecking);
  try
    Screen.Cursor := crHourGlass;
    //SupressFiscalErrorMessage := True;
    NFManual := False;
    NFEMode := False;

    //if not FiscalPrinterEnabled then
    begin
      ExitQS := True;
      if  NFManual then
        ExitQS := False;

      DateNotMoved := False;
      Val(SWBEncryption.AuxiliaryData.FiscalDateMovement, iDate, iErr);

      SFiscalPrinting.GetMovementDate;
  //    cmdManualModeSkin.Visible := False;
 //     cmdManualModeSkin.Enabled := False;
      //ShowManualSelection := (AppDetails.FiscalManualMode in [0, 1]);
      if (iDate = 0) then
        DateNotMoved := True ;

      (*if ((AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC]) and (AuxiliaryData.FiscalDateMovement = '000000') and (iDate = 0)) then //no date movement
        DateNotMoved := True
      else if ((AppDetails.FiscalPrinterType = fpDaruma) and (AuxiliaryData.FiscalDateMovement = '01012000') {and (iDate = 0)}) then
        DateNotMoved := True; *)

      if (ExitQS and DateNotMoved {and (ShowConf(FiscalErrorMessage + sManualModeEntryConf) = mrYes)}) then
      begin
        ExitQS := False;
        if ((not GlbPrivs[11].AvoidPINCheck) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then
          ExitQS := True;
        ShowManualSelection := (AppDetails.FiscalManualMode = 0);
        //cmdManualModeSkin.Visible := (AppDetails.FiscalManualMode = 0);
        //cmdManualModeSkin.Enabled := (AppDetails.FiscalManualMode = 0);

        (*if not FiscalPrinterEnabled then
        begin

          ShowQuickMsg(sManualModeEntry);
        end; *)
      end
      else if SFiscalPrinting.FiscalPrinterEnabled then
        ExitQS := False;

      if ExitQS then
      begin
        SFiscalPrinting.SupressFiscalErrorMessage := False;
        ClearStatus;
        ClearStatusPage;
        Result := False;
        if FormActivated then
          cmdHideClick(Self)
        else
          PostMessage(self.handle, WM_CLOSE, 0, 0);
        Exit;
      end
      else
      begin
        ClearStatus;
        ClearStatusPage;
        if NFManual then
          SetStatusPage(sTabPaymentINManualNFMode);
      end;
    end;

    if not NFManual then
    begin
      if ((not SFiscalPrinting.GetFiscalPrinterDetails) or (GlbFiscal.ManufactureNo = '')) then
      begin
        ClearStatus;
        ClearStatusPage;

        PostMessage(self.handle, WM_CLOSE, 0, 0);
        Result := False;
        Exit;
      end;

      if not SFiscalPrinting.GetFiscalPrinterConfigurations then
      begin
        ClearStatus;
        ClearStatusPage;

        //ShowQuickMsg(sPrinterSystemDateMismatch);
        PostMessage(self.handle, WM_CLOSE, 0, 0);
        Result := False;
        Exit;
      end;
    end;
    ClearStatusPage;
    if NFManual then
      SetStatus(sTabPaymentINManualNFMode, False, NFManual)
    else if NFEMode then
      SetStatus(sTabPaymentINNFEMode, False, NFManual)
    else
      SetStatus(sTabPayment, False, NFManual);
  finally
    SFiscalPrinting.SupressFiscalErrorMessage := False;
    Screen.Cursor := crDefault;
  end;
end;

procedure TformPaymentTable.LoadAllComboItems(aComboID: Integer; aComboIndex: Integer);
var
  i, iSrcIndex: Integer;
  aOL: TOrderLine;
begin
  for i := 0 to UnInvoicedOlsToPay.Count - 1 do
  begin
    aOL := UnInvoicedOlsToPay.Items[i];

    if ((aOL.ComboID = aComboID) and (aOL.ComboIndex = aComboIndex)) then
    begin
      iSrcIndex := i;
      if ((dm.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, []) and (not dm.qrOLsToPay.Fields[0].IsNull))) then
      begin
        AddOrderLine(UnInvoicedOlsToPay, iSrcIndex);
      end;
    end;
  end;
end;

procedure TformPaymentTable.LoadFromUnInvoicedList(aOrderLineID: Integer);
var
  aOL: TOrderLine;
  iOrdLineID, iSrcIndex: Integer;
begin
  LogMessage(ltPaymentTrace, '417' + #9);
  iOrdLineID := AOrderLineID;
  aOL := GetOriginalOL(iOrdLineID, iSrcIndex);

  if ((dm.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, [])) and (not dm.qrOLsToPay.Fields[0].IsNull)) then
  begin
    LogMessage(ltPaymentTrace, '418' + #9);
    AddOrderLine(UnInvoicedOlsToPay, iSrcIndex);

    if ((aOL.ComboID > 0) and (aOL.ComboIndex > 0)) then
    begin
      LogMessage(ltPaymentTrace, '419' + #9);
      LoadAllComboItems(aOL.ComboID, aOL.ComboIndex)
    end;
  end;
  LogMessage(ltPaymentTrace, '420' + #9);
  CalculateToPay(True);
  grdItemAutoClick := True;
  LogMessage(ltPaymentTrace, '506' + #9);
  grdToPayClick(nil);
  LogMessage(ltPaymentTrace, '507' + #9);

  BroadcastCDAMessage;
end;

procedure TformPaymentTable.grdUnInvoicedListClick(Sender: TObject);
var
  iOrdLineID: Integer;
begin
  LogMessage(ltPaymentTrace, '588' + #9);
  if (not Assigned(UnInvoicedOlsToPay)) then
    Exit;

  if UnInvoicedOlsToPay.Count = 0 then
    Exit;

  iOrdLineID := Integer(grdUnInvoicedList.Objects[0, grdUnInvoicedList.Row]);
  LogMessage(ltPaymentTrace, '589' + #9 + 'grdUnInvoicedList.Row=' + IntToStr(grdUnInvoicedList.Row) + ', iOrdLineID=' + IntToStr(iOrdLineID));

  LoadFromUnInvoicedList(iOrdLineID);
  LogMessage(ltPaymentTrace, '590' + #9);
end;

procedure TformPaymentTable.cmdComboIndexDesClick(Sender: TObject);
var
  frmComboColours: TFormCombosInSale;
begin
  frmComboColours := TFormCombosInSale.Create(Nil);
  try
    frmComboColours.OLList := UnInvoicedOlsToPay;
    frmComboColours.AllCombos := FAllCombos;
    frmComboColours.LoadComboColours;
    frmComboColours.ShowModal;
  finally
    frmComboColours.Free;
  end;
end;

procedure TformPaymentTable.UpdateUnInvoicedListComboDetails;
var
  i: Integer;
  aOL: TOrderLine;
begin
  LogMessage(ltPaymentTrace, '451' + #9);
  for i := 0 to UnInvoicedOlsToPay.Count - 1 do
  begin
    aOL := UnInvoicedOlsToPay.Items[i];

    if (dm.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, []) and (not dm.qrOLsToPay.Fields[0].IsNull)) then
    begin
      if (aOL.ComboID <= 0) then // reset for account discounts
      begin
        aOL.ComboID := 0;
        aOL.ComboGroupID := 0;
        aOL.ComboIndex := 0;
        aOL.ComboName := '';
        aOL.ComboQty := 0;
        aOL.ComboGroupOrder := 0;
        aOL.ComboDiscount := 0;
        aOL.AllowedDiscount := 0;
        aOL.ILDiscount := 0;
      end;
      if ((DM.qrOLsToPay.FieldByName('comboid').IsNotNull) and (DM.qrOLsToPay.FieldByName('comboid').AsInteger > 0)) then
      begin
        aOL.ComboID := DM.qrOLsToPay.FieldByName('comboid').AsInteger;
        aOL.ComboGroupID := DM.qrOLsToPay.FieldByName('combogroup').AsInteger;
        aOL.ComboIndex := DM.qrOLsToPay.FieldByName('comboindex').AsInteger;
        aOL.ComboName := DM.qrOLsToPay.FieldByName('comboname').AsString;
        aOL.ComboQty := DM.qrOLsToPay.FieldByName('comboqty').AsDouble;
        aOL.AllowedDiscount := DM.qrOLsToPay.FieldByName('oldiscount').AsCurrency;
        aOL.ComboDiscount := aOL.AllowedDiscount;
      end;
    end;
  end;
  for i := 0 to OLToPayList.Count - 1 do
  begin
    aOL := OLToPayList.Items[i];

    if DM.qrOLsToPay.Locate('orderlineid', aOL.OrderLineID, []) then
    if ((not dm.qrOLsToPay.Fields[0].IsNull)) then
    begin
      if aOL.ComboID <= 0 then // reset for account discounts
      begin
        aOL.ComboID := 0;
        aOL.ComboGroupID := 0;
        aOL.ComboIndex := 0;
        aOL.ComboName := '';
        aOL.ComboQty := 0;
        aOL.ComboGroupOrder := 0;
        aOL.ComboDiscount := 0;
        aOL.AllowedDiscount := 0;
        aOL.ILDiscount := 0;
      end;
      if ((DM.qrOLsToPay.FieldByName('comboid').IsNotNull) and (DM.qrOLsToPay.FieldByName('comboid').AsInteger > 0)) then
      begin
        aOL.ComboID := DM.qrOLsToPay.FieldByName('comboid').AsInteger;
        aOL.ComboGroupID := DM.qrOLsToPay.FieldByName('combogroup').AsInteger;
        aOL.ComboIndex := DM.qrOLsToPay.FieldByName('comboindex').AsInteger;
        aOL.ComboName := DM.qrOLsToPay.FieldByName('comboname').AsString;
        aOL.ComboQty := DM.qrOLsToPay.FieldByName('comboqty').AsDouble;
        aOL.AllowedDiscount := DM.qrOLsToPay.FieldByName('oldiscount').AsCurrency;
        aOL.ComboDiscount := aOL.AllowedDiscount;
      end;
    end;
  end;
end;

procedure TformPaymentTable.BroadcastCDAMessage;
var
  fOnetapFunction: ToneTapFunction;
begin
  if CDAServer.Connected then
  begin
    fOnetapFunction := CDAServer.CDAData.onetapFunction;

    if not (fOnetapFunction in [ofTablePaymentByItem, ofShowChange]) then
      fOnetapFunction := ofTablePayment;

    CDAServer.ProvideCDAData(fOnetapFunction, GlbTable.Destination, FDiscountPercent, FDiscountAmount, FComboDiscount, FSalesTax, FToPay, FSurcharge,
                             StrToFloatDef(StripCommas(txtBalDue.Text),0), FStillDue, FChange, 0{FFoodTotal},
                             0{FBevTotal}, FTender,
                             OLToPayList, SurchargeList, TLList);
  end;
end;

procedure TformPaymentTable.ClearCDAMessage;
begin
  if CDAServer.Connected then
  begin
    CDAServer.ClearCDAMessage;
  end;
end;

procedure TformPaymentTable.grdTLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
var
  aAlignment: TAlignment;

begin
  inherited;
  aAlignment := taLeftJustify;
  if (aCol in [2, 3, 4, 5]) then
  begin
    aAlignment := taRightJustify;
  end;
  Skin.ApplySkinOnStringGridDraw(TStringGrid(Sender), ARow, ACol, Rect, State, clBlack, False, False, aAlignment);
end;

procedure TformPaymentTable.cmdEventDesClick(Sender: TObject);
begin
  inherited;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (GlbEvent.EventID = 0) then // if no event is selected currently
  begin
    if InGoodySale then
    begin
      TfcCustomImageBtn(Sender).Down := False;
      ShowMsg(sNoEventForGoodyScan);
      Exit;
    end;

    //CurrentHostSpend is unknown until we select Event, thus any checks against SpendLimit need to be done after Event selection
    if (not SelectEvent) then
    begin
      Exit;
    end;

    if (GlbEvent.AccountID > 0) and (not ApplySubsidyOnItems) then
      DeselectEvent;

    FForceUnSelectAccount := False;
    FOneBill := True;
  end
  else  // an event is selected and button is down
  begin
    DeselectEvent;
  end;
  lblTitle.Caption := sTabPayment;
end;

procedure TformPaymentTable.DeselectEvent;
begin
  GlbEvent.Clear;
  SubsidyCalc.EventID := 0;
  SubsidyCalc.AccountID := 0;
  SubsidyCalc.SubsidyOverrided := False;
  SubsidyCalc.SubsidyID := 0;
  SubsidyCalc.ClearSubsidyDetails;
  ClearItemLevelSubsidy(UnInvoicedOlsToPay);
  ClearItemLevelSubsidy(TempItemList);
  ClearItemLevelSubsidy(OLToPayList);
  DeselectAccount; // to deselect event account
  FForceUnSelectAccount := False;
  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID >0) and
                                  (GlbEvent.AccountID > 0) and
                                  (OnAccount) and (GlbEvent.SubsidyID > 0);

  LogMessage(ltPaymentTrace, '439' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '529' + #9);
  FOneBill := AppDetails.OneBill;
end;

procedure TformPaymentTable.ClearItemLevelSubsidy(var AOLList: TList);
var
  i: Integer;
  OL: TOrderLine;
begin
  if (Assigned(AOLList) and (AOLList.Count > 0)) then
  begin
    for i := 0 to AOLList.Count - 1 do
    begin
      OL := TOrderLine(AOlList.Items[i]);
      OL.SubsidyAllowed := 0.00;
    end;
  end;
end;

procedure TformPaymentTable.CalculateSubsidy(AList : TList;LoadAllowedDisc, DisplayDiscount:Boolean);
var
  i : Integer;
  aOL : TOrderLine;
begin
  LogMessage(ltPaymentTrace, '447' + #9);
  FHostSubsidy := 0.00;
  FGuestSubsidy := 0.00;
  FTotalSubsidyAllowed := 0.00;

  // calculate subsidy
  if Assigned(AList) then
  begin
    if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (GlbEvent.SubsidyID > 0)) then
    begin
      SubsidyCalc.CalculateEventSubsidy(AList);
      CalculateAccTypeItemDiscount(AList);

      FHostSubsidy := 0.00;
      FGuestSubsidy := 0.00;
      FTotalSubsidyAllowed := 0.00;

      for i := 0 to (AList.Count - 1) do
      begin
        aOL := AList.Items[I];
        if (aOL.ItemID > 0) then
        begin
          FHostSubsidy := FHostSubsidy + aOL.HostSubsidy;
          FGuestSubsidy := FGuestSubsidy + aOL.GuestSubsidy;

          FTotalSubsidyAllowed := FTotalSubsidyAllowed + aOL.SubsidyAllowed;
        end;
      end;
      GetRoundedUpDown(FTotalSubsidyAllowed, AppDetails.DecimalPlaces);

    end;
  end;
end;

function TformPaymentTable.ApplySubsidyOnItems: Boolean;
var
  prevAccountID: Integer;
begin
  LogMessage(ltPaymentTrace, '413' + #9);
  FForceUnSelectAccount := True;
  prevAccountID := GlbAccount.AccountID;
  Result := GetSelectedAccount(GlbEvent.AccountID, False, True, prevAccountID, True);

  if (not Result) then
    Exit;

  cmdOnAccount.Down := False;
  OnAccount := False;
  SubsidyCalc.EventID := GlbEvent.EventID;
  SubsidyCalc.AccountID := GlbEvent.AccountID;
  SubsidyCalc.SubsidyOverrided := GlbEvent.SubsidyOverrided;
  SubsidyCalc.SubsidyID := GlbEvent.SubsidyID;

  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID >0) and
                              (GlbEvent.AccountID > 0) and
                              (OnAccount) and (GlbEvent.SubsidyID > 0);
  cmdOnAccount.Down := False;
  OnAccount := False;
  LogMessage(ltPaymentTrace, '440' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '530' + #9);
  RecalculateTotals;
  FForceUnSelectAccount := False;
end;

procedure  TformPaymentTable.SetGuestCount;
var
  NewGuestCount: Integer;
  InTrans: Boolean;
begin
  NewGuestCount := GlbTable.Guests;
  InTrans := dm.tr.InTransaction;
  if (GetNewGuests(NewGuestCount)) then
  begin
    try
      if (not InTrans) then
      begin
        dm.TRStartTest
      end;

      with dm.sp do
      begin
        SetStoredProcName('EDITGUESTCOUNT');  //editguestcount
        ParamByName('groupid').AsInteger := GlbTable.GroupID;
        ParamByName('newguest').AsInteger := NewGuestCount;
        ParamByName('newchildren').AsInteger := GlbTable.Children;
        ExecProc;
      end;

      if (not InTrans) then
      begin
        dm.tr.Commit;
      end;
    except on e: exception do begin
      if (not InTrans) then
      begin
        dm.tr.Rollback;   //Jon 10-07-2003
      end;
      ShowMessage(e.message);
    end; end;

  end;
end;

{******************************************************************************}
procedure TformPaymentTable.ShowExternalAccounts(TL: TTL = nil);
var
  tmpRecordType: TRecType;
  tmpGetAllItems: Boolean;
  tmpList: Tlist;

begin
  tmpGetAllItems := FGetAllItems;
  tmpRecordType := FRecordType;
  tmpList := TList.Create;
  try
    FRecordType := rtBoth;
    FGetAllItems := True;
    GetAllSelectedOLs(tmpList);
    CombineSameOls(tmpList);       //copies of OLs now in tmpList so need to free them

    sExternalAccountIntegration.SetOrderChanged(csCleanSent);
    SelectExternalAccount(GetDue(False), tmpList, False, TL, -1);
  finally
    FRecordType := tmpRecordType;
    FGetAllItems := tmpGetAllItems;
    ClearListObjects(tmpList, True);
  end;

end;

{******************************************************************************}
procedure TformPaymentTable.CloseExternalAccounts;
begin
  if (Assigned(formSelectExternalAccount)) and (formSelectExternalAccount.Showing) then
    formSelectExternalAccount.cmdCancelClick(nil);
end;

{******************************************************************************}
procedure TformPaymentTable.UpdateTLListExternalAccounts;
begin
  DeleteCurrentExternalTenders;
  AddExternalTenders;
end;
          
{******************************************************************************}
procedure TformPaymentTable.DeleteCurrentExternalTenders;
var
  i, iCount: Integer;
  TL: TTL;
  tmpRecordType: TRecType;

begin
  iCount := TLList.Count;

  for i := TLList.Count-1 downto 0 do
  begin
    TL := TTL(TLList.Items[i]);
    if (tl.TLType = TLTExternalProviders) and (not tl.isProtected) then
    begin
      DeleteOLsOfTender(TL, TL.TenderSeqNo);
      TLList.DisposeTL(i);
    end;
  end;
  tmpRecordType := FRecordType;
  FRecordType := rtBoth;
  LogMessage(ltPaymentTrace, '441' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '531' + #9);
  FRecordType := tmpRecordType;
  if (iCount <> TLList.Count) then
  begin
    LoadUnInvoicedOlsToGrid(UnInvoicedOlsToPay, 0, FToPay);
    if PaymentOnSelectedItems or AmountSplitingStarted then
    begin
      ClearTempItemList;
      CopyItemSToTempList;
      LoadSelectedToPayList;
    end
    else
    begin
      ClearTempItemList;
      ClearAllOrderLines;
      AddAllOrderLines;
    end;

    LoadTLs(LADelete);
    CalculateAmountForUntenderedItems;
  end;
end;

{******************************************************************************}
procedure TformPaymentTable.AddExternalTenders;
var
  i, j, k: Integer;
  CurrentProvider: TExternalProvider;

begin
  for i := 0 to sExternalAccountIntegration.FExternalProviders.Count-1 do
  begin
    CurrentProvider := TExternalProvider(sExternalAccountIntegration.FExternalProviders.Items[i]);
    with CurrentProvider do
    begin
      for j := 0 to FSchemeTypes.Count-1 do
      begin
        FCurrentScheme := TSchemeType(FSchemeTypes.Items[j]);
        with FCurrentScheme do
        begin
          for k:=0 to FListAccountDetails.Count-1 do
          begin
            FCurrentAccountDetails := TExternalAccount(FListAccountDetails.Items[k]);
            with FCurrentAccountDetails do
            begin
              if (Assigned(FTenderLineCredit) and (FTenderLineCredit.TLTender > 0) and (TLList.IndexOf(FTenderLineCredit) = -1)) then
              begin
                LogMessage(ltPaymentTrace, '35' + #9);
                AddTenderLine(FTenderLineCredit.TLType, 0, FTenderLineCredit);
                LogMessage(ltPaymentTrace, '36' + #9);
                FTenderLineCredit.AddedToTLList := True;
              end;
              if (Assigned(FTenderLineLoyalty) and (FTenderLineLoyalty.TLTender > 0) and (TLList.IndexOf(FTenderLineLoyalty) = -1)) then
              begin
                LogMessage(ltPaymentTrace, '37' + #9);
                AddTenderLine(FTenderLineLoyalty.TLType, 0, FTenderLineLoyalty);
                LogMessage(ltPaymentTrace, '38' + #9);
                FTenderLineLoyalty.AddedToTLList := True;
              end;
            end;
          end;
        end;
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdExternalAccLookupDesClick(Sender: TObject);
begin
  DoExternalAccountButtonClick;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdExternalAccRIPDesClick(Sender: TObject);
begin
  DoExternalAccountButtonClick(True);
end;
{******************************************************************************}
procedure TformPaymentTable.DoExternalAccountButtonClick(RIPButtonClicked: Boolean = False);
var
  tmpRecordType: TRecType;
  tmpGetAllItems: Boolean;
  tmpList: Tlist;
begin
  if not sExternalAccountIntegration.AllowExternalAccounts(RIPButtonClicked) then
    Exit;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformExtAcctNotAllowed(True, True) then
    Exit;

  if (not RIPButtonClicked) and (sExternalAccountIntegration.ProviderCardsPresent > 0) then
  begin
    ShowExternalAccounts;
    Exit;
  end;

  tmpGetAllItems := FGetAllItems;
  tmpRecordType := FRecordType;
  tmpList := TList.Create;
  try
    FRecordType := rtBoth;
    FGetAllItems := True;
    GetAllSelectedOLs(tmpList);
    CombineSameOls(tmpList);       //copies of OLs now in tmpList so need to free them

    ExternalAccountLookup(tmpList, RIPButtonClicked);
  finally
    FRecordType := tmpRecordType;
    FGetAllItems := tmpGetAllItems;
    ClearListObjects(tmpList, True);   //copies of OLs now in tmpOLList so need to free them
  end;
end;
{******************************************************************************}
procedure TformPaymentTable.cmdTaxCoreBuyerDetailDesClick(Sender: TObject);
begin
  inherited;
  GetTaxCoreBuyerDetail;
end;
{******************************************************************************}
procedure TformPaymentTable.SetGoodyAccountDiscountAmount;
var
  i, j: Integer;
  SOL, OL: TOrderLine;
begin
  FDiscountAmount := GlbGoody.TotalDiscount;

  //Code from TformPaymentTable.cmdDiscountAmountClick, after call to GetDiscountAmount
  FOrigDiscountAmount := FDiscountAmount;
  FUseDiscountAmount := True;
  FUseItemDiscounts := False;
  RecalculateDiscount := True;
  LogMessage(ltPaymentTrace, '592' + #9);
  lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;
  ClearItemLevelDiscounts(UnInvoicedOlsToPay);
  if OLToPayList.Count = 0 then
  begin
    CalculateAccTypeItemDiscount(UnInvoicedOlsToPay);
    UpdateScreenAfterCalculateDiscount;
  end;

  FDiscCalculator.OneBillDiscountAmount := FDiscountAmount;
  if (AppDetails.OneBill) then
    ApplyItemLevelOneBillDiscountAmount;

  LogMessage(ltPaymentTrace, '442' + #9);
  CalculateToPay;
  LogMessage(ltPaymentTrace, '532' + #9);
  RecalculateDiscount := False;

  if (not AppDetails.OneBill) then
  begin
    for i:= 0 to OLToPayList.Count-1 do
    begin
      SOL := TOrderLine(OLToPayList.Items[i]);
      for j:= 0 to UnInvoicedOlsToPay.Count-1 do
      begin
        OL := TOrderLine(UnInvoicedOlsToPay.Items[j]);
        if (SOL.OrderLineID = OL.OrderLineID) then
        begin
          OL.AllowedDiscount := SOL.AllowedDiscount;
          break;
        end;
      end;
    end;
  end;
end;
{****************************************************************************}
procedure TformPaymentTable.RefreshGoodyAccountDiscountAmount;
begin
  //NB: FDiscountAmount auto-changes as items are added / removed, (because
  //discount is allocated to the selected OLs to pay, top down), and it never
  //resets OL to pay discount amounts to match the specified discount amount.
  //In an attempt to always represent the actual Goody scanned discount amount in
  //the account discount amount field, we call this procedure on the grdToPayClick
  //event, which appears to be called whenever something on it changes (+ / - OLs,
  //change qty), and the cmdTenderClick event, to reset the incorrectly set
  //discount amount back to the scanned amount.
  //We also call it during the validation process, to ensure it IS the same.
  if UseGoody and (GlbGoody.DiscountAmounts.Count > 0) then
    SetGoodyAccountDiscountAmount;
end;
{******************************************************************************}
function TformPaymentTable.TryAddGoodyDiscountAmount(ScannedAmount: string): Boolean;
var
  JustLoadedGoodyDiscAcc: Boolean;
  GoodyDiscountAmount, GoodyAccMaxDisc: Currency;
  PrevAccountID: Integer;
begin
  LogMessage(ltPaymentTrace, '414' + #9);
  //NB: Code around loading an account was originally copied from OpenArchButtonClick where qsbaAccount and OpenArchButton.AssociatedItem2 <= 0.

  Result := False;
  JustLoadedGoodyDiscAcc := False;
  GoodyDiscountAmount := 0;
  GoodyAccMaxDisc := 0;

  if DoingTablePayment then
    Exit;

  //Code from cmdDiscountAmountClick
  if (AppDetails.OneBill and (TLList.Count > 0)) then
  begin
    //TO DO: put message to explain
    Exit;  //for now, don't allow discount to be manually changed after the first tender is made
  end;

  if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
  begin
    if TLList.Count > 0 then
      //TO DO: put message to explain
      Exit;
  end;

  //Check don't have an Event selected
  if GlbEvent.EventID <> 0 then
  begin
    ShowMsg(sNoEventForGoodyScan);
    Exit;
  end;

  //check Goody Customer ID has been scanned
  if GlbGoody.CustomerID = EmptyStr then
  begin
    ShowMsg(sScanGoodyCustomerIDBeforeDisc);
    Exit;
  end;

  //check valid format: up to 7 chars, which should be sufficient for very large discounts
  if Length(ScannedAmount) > 7 then
  begin
    ShowMsg(sTooLongGoodyDiscountAmount);
    Exit;
  end;

  //check discount amount is valid, is up to 2 decimal places and is greater than zero
  try
    GoodyDiscountAmount := StrToFloat(ScannedAmount);

    //check just up to 2dp: compare scanned string to amount rounded to 2dp
    if FloatToStr(GoodyDiscountAmount) <> FloatToStr(Trunc(GoodyDiscountAmount*100)/100) then
    begin
      ShowMsg(sGoodyDiscScannedNotValid);
      Exit;
    end;

    //check greater than zero
    if GoodyDiscountAmount <= 0 then
    begin
      ShowMsg(sGoodyDiscScannedNotValid);
      Exit;
    end;
  except
    on Exception: EConvertError do
    begin
      ShowMsg(sGoodyDiscScannedNotValid);
      Exit;
    end;
  end;

  //check if an account that differs from the Goody discount account is loaded, and if so, confirm ok to continue to load Goody discount account
  if (GlbAccount.AccountID > 0) and (GlbAccount.AccountID <> AppDetails.GoodyDiscountAccountID) then
  begin
    if (ShowConf(sGoodyDiscAcctReplaceSelAcct) <> mrYes) then
      Exit;
  end;

  //check if Goody discount account needs to be loaded, and if so try load now
  if (GlbAccount.AccountID <> AppDetails.GoodyDiscountAccountID) then
  begin
    if not CanSelectAccount then
    begin
      ShowMsg(sGoodyDiscNotAppliedAcctNotLoaded);
      Exit;
    end;

    //NB: GetSelectedAccount ensures account is active and user has privilege to Edit Invoice Discount (13)
    PrevAccountID := GlbAccount.AccountID;
    GlbAccount.LookupAccountID := AppDetails.GoodyDiscountAccountID;
    if not GetSelectedAccount(GlbAccount.LookupAccountID, False, False, PrevAccountID, True, True) then
    begin
      //DeselectAccount; //PrevAccountID should be reloaded above (in GetSelectedAccount)
      ShowMsg(sGoodyDiscNotAppliedAcctNotLoaded);
      Exit;
    end;

    //exit if account wasn't successfully loaded for some reason, e.g. DisplayAccount deselected the account as it was inactive
    if GlbAccount.AccountID = 0 then
    begin
      DeselectAccount;
      ShowMsg(sGoodyDiscNotAppliedAcctNotLoaded);
      Exit;
    end;

    //exit if account is active for gaining points only
    if (GlbAccount.AccountInactive = accPointsOnly) then
    begin
      DeselectAccount;
      ShowMsg(sGoodyDiscNotAppliedAcctActivePointsOnly);
      Exit;
    end;

    JustLoadedGoodyDiscAcc := True;
  end;

  //check total discount doesn't exceed maximum amount (if limited)
  if FLimitMaximum and ((FDiscountAmount + GoodyDiscountAmount) > GlbAccount.MaxDiscount) then
  begin
    GoodyAccMaxDisc := GlbAccount.MaxDiscount; //remember amount for message, as account may be deselected first
    if JustLoadedGoodyDiscAcc then
      DeselectAccount;
    ShowMsg(Format(sGoodyDiscWouldExceedAcctLimit, [FormatCurrencySign(GoodyAccMaxDisc)]));
    Exit;
  end;

  GlbGoody.AddDiscount(GoodyDiscountAmount);

  Result := True;
  ShowQuickMsg(Format(sGoodyDiscApplied, [FormatCurrencySign(GoodyDiscountAmount)]));
end;
{******************************************************************************}
procedure TformPaymentTable.DeselectGoodyAccount;
begin
  //if Goody discount account loaded, deselect now
  if GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID then
    InitialiseAccountInfo(False, True, False);
end;
{******************************************************************************}
end.


--------------------------------------------------------------------------------------------------------


unit USelectCashSale;
{******************************************************************************}
interface
{******************************************************************************}
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, ImgList, Grids, IB_Grid, Buttons, ExtCtrls, StdCtrls, fcImgBtn, fcShapeBtn,
  USkin, FFrameReceipt, UIntegerList{,IvDictio, IvMulti};
{******************************************************************************}
function SelectCashSale(var InvoiceID: Integer; var InvoiceType: string): Boolean;
function SelectTabInvoices(var InvoiceID: Integer; var InvoiceType: string; const aLastReceipt: Boolean = False): Boolean;
function SelectLastCashSale(var InvoiceID: Integer; var InvoiceType: string; const aQuickHide: Boolean = False): Boolean;
{******************************************************************************}
type
  (**
    Displays a list of todays Cash Sales for the user to select.
    This is used by the Print Invoice button in the Cash Sales screen.
  *)
  TformSelectCashSale = class(Tform)
    pnlForm: TPanel;
    pnlInner: TPanel;
    pnlTitle: TPanel;
    lblTitle: TLabel;
    pnlTitleIcon: TPanel;
    imgTitle: TImage;
    lstCashSales: TIB_LookupList;
    pnlButtonsRight: TPanel;
    cmdCancelDes: TSpeedButton;
    cmdPrintReceipt: TSpeedButton;
    pnlButtonsFooter: TPanel;
    cmdSelectStaff: TSpeedButton;
    chkStaff: TCheckBox;
    chkTerminal: TCheckBox;
    cmdSelectTerminal: TSpeedButton;
    pnlGrid: TPanel;
    pnlGridInner: TPanel;
    pnlGridControls: TPanel;
    pnlControls: TPanel;
    cmdDownDes: TSpeedButton;
    cmdUpDes: TSpeedButton;
    pnlReceipt: TPanel;
    pnlReceiptInner: TPanel;
    Receipt: TframeReceipt;
    pnlReceiptControls: TPanel;
    pnlReceiptPrint: TPanel;
    pnlReceiptUpDown: TPanel;
    cmdReceiptDownDes: TSpeedButton;
    cmdReceiptUpDes: TSpeedButton;
    pnlReceiptSpacer: TPanel;
    pnlReceiptOuter: TPanel;
    // LanguageTranslator: TIvTranslator;  //Multilizer 12/2022
    cmdInvoiceNoSaleDes: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure lstCashSalesCellClick(Sender: TObject; ACol, ARow: Integer; AButton: TMouseButton; AShift: TShiftState);
    procedure cmdCancelClick(Sender: TObject);
    procedure cmdUpClick(Sender: TObject);
    procedure cmdDownClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure cmdPrintReceiptClick(Sender: TObject);
    procedure cmbStaffChange(Sender: TObject);
    procedure cmbTerminalChange(Sender: TObject);
    procedure cmdReceiptUpClick(Sender: TObject);
    procedure cmdReceiptDownClick(Sender: TObject);
    procedure chkStaffClick(Sender: TObject);
    procedure cmdInvoiceNoSaleDesClick(Sender: TObject);
  private
    FReceiptWidth: Integer;
    FReceiptActive: Boolean;
    FGridWidth: Integer;
    FGridActive: Boolean;
    FInvoiceID: Integer;
    FInvoiceType: string;
    FStaffID: Integer;
    FTerminalID: Integer;
    FTabInvoices: Boolean;
    FLastCashSale: Boolean;
    FLastReceipt: Boolean;
    FQuickHide: Boolean;
    FShowingInvoices: Boolean;

    cmdUp: TfcCustomImageBtn;
    cmdDown: TfcCustomImageBtn;
    cmdCancel: TfcCustomImageBtn;
    cmdPrint: TfcCustomImageBtn;
    cmdStaff: TfcCustomImageBtn;
    cmdTerminal: TfcCustomImageBtn;
    cmdReceiptUp: TfcCustomImageBtn;
    cmdReceiptDown: TfcCustomImageBtn;
    cmdInvoiceNoSale: TfcCustomImageBtn;
    procedure Initialise;
    procedure DisplayReceipt;
    function TryPrintReceipt: Boolean;
    procedure RefreshReceipts;
  public
    property TabInvoices: Boolean read FTabInvoices write FTabInvoices;
    property LastCashSale: Boolean read FLastCashSale write FLastCashSale;
    property LastReceipt: Boolean read FLastReceipt write FLastReceipt;
    property QuickHide: Boolean read FQuickHide write FQuickHide;
    procedure FormMessage(var Msg: TMsg; var Handled: Boolean);
  end;
{******************************************************************************}
var
  formSelectCashSale: TformSelectCashSale;
{******************************************************************************}
implementation
{******************************************************************************}
{$R *.DFM}
{******************************************************************************}
uses
  LServiceQueries, LMain, DM_WBWaiter, LWBQuery, UQuickMessage, USelectStaff,
  USelectTerminal, UResources, UAlphaBlend, UGlobalMenuList, UAppDetails, UTaxCoreTransaction;
{******************************************************************************}
function SelectCashSale(var InvoiceID: Integer; var InvoiceType: String): Boolean;
var
  ShowingAlphaBlend: Boolean;
begin
  ShowingAlphaBlend := FormAlphaBlend.Showing;
  if (not ShowingAlphaBlend) then FormAlphaBlend.ShowAlphaBlend;
  formSelectCashSale := TformSelectCashSale.Create(Application);
  Result := False;
  try
    with formSelectCashSale do begin
      TabInvoices := False;
      Initialise;
      pnlReceiptControls.Visible := True;
      if (lstCashSales.DataSource.Eof) then begin
        Result := True;
        InvoiceID := -1;
        Exit;
      end;
      ShowModal;
      if (ModalResult = mrOk) then begin
        Result := True;
        InvoiceID := FInvoiceID;
        InvoiceType := FInvoiceType;
      end;
    end;
  finally
    FreeAndNil(formSelectCashSale);
    if (not ShowingAlphaBlend) then FormAlphaBlend.HideAlphaBlend;
  end;
end;

{******************************************************************************}
function SelectTabInvoices(var InvoiceID: Integer; var InvoiceType: string; const aLastReceipt: Boolean = False): Boolean;
var
  ShowingAlphaBlend: Boolean;
begin
  ShowingAlphaBlend := FormAlphaBlend.Showing;
  if (not ShowingAlphaBlend) then FormAlphaBlend.ShowAlphaBlend;
  formSelectCashSale := TformSelectCashSale.Create(Application);
  Result := False;
  try
    with formSelectCashSale do begin
      TabInvoices := True;
      LastReceipt := aLastReceipt;
      lstCashSales.DataSource := dm.dsInvoices;
      Initialise;
      pnlReceiptControls.Visible := True;
      cmdInvoiceNoSale.Visible := True;
      if (dm.qrInvoices.Eof) and (dm.qrNoSales.Eof) then
      begin
        Result := True;
        InvoiceID := -1;
        Exit;
      end;
      if (LastReceipt) then
      begin
        if (dm.qrInvoices.Eof) then
          cmdInvoiceNoSale.Click;
        cmdPrintReceiptClick(nil);
      end
      else
        ShowModal;
      if (ModalResult = mrOk) then
      begin
        Result := True;
        InvoiceID := FInvoiceID;
        InvoiceType := FInvoiceType;
      end;
    end;
  finally
    FreeAndNil(formSelectCashSale);
    if (not ShowingAlphaBlend) then FormAlphaBlend.HideAlphaBlend;
  end;
end;
{******************************************************************************}
function SelectLastCashSale(var InvoiceID: Integer; var InvoiceType: string; const aQuickHide: Boolean = False): Boolean;
var
  ShowingAlphaBlend: Boolean;
begin
  ShowingAlphaBlend := FormAlphaBlend.Showing;
  if (not ShowingAlphaBlend) then FormAlphaBlend.ShowAlphaBlend;
  formSelectCashSale := TformSelectCashSale.Create(Application);
  Result := False;
  try
    with formSelectCashSale do
    begin
      LastCashSale := True;
      LastReceipt := True;
      Initialise;
      QuickHide := aQuickHide;
      pnlReceiptControls.Visible := True;
      if (lstCashSales.DataSource.Eof) then begin
        Result := True;
        InvoiceID := -1;
        Exit;
      end;
      ShowModal;
      if (ModalResult = mrOk) then begin
        Result := True;
        InvoiceID := FInvoiceID;
        InvoiceType := FInvoiceType;
      end;
    end;
  finally
    FreeAndNil(formSelectCashSale);
    if (not ShowingAlphaBlend) then FormAlphaBlend.HideAlphaBlend;
  end;
end;
{******************************************************************************}
procedure TformSelectCashSale.FormCreate(Sender: TObject);
var
  StaffName: String;
begin
  lstCashSales.DefaultRowHeight := AppDetails.DefaultRowHeight;    //Jon 28-10-2003
  lstCashSales.DataSource := dm.dsCashSales;
  FShowingInvoices := True;
  inherited;
  FReceiptWidth := 340;   //Exc Boarder
  FGridWidth := 600; //Inc Boarder

  FReceiptActive := True;
  FGridActive := False;

  FTabInvoices := False;
  FLastCashSale := False;
  FLastReceipt := False;
  FQuickHide := False;

  //Here we setup the form as if only showing receipt, Proc Initialise will change if required
  Width := 600; // This if receipt only Includes spacing
  pnlReceiptOuter.Align := alClient;
  pnlGrid.Visible := FGridActive;
  pnlReceiptOuter.Visible := FReceiptActive;
  pnlReceiptSpacer.Visible := ((FGridActive) and (FReceiptActive));
  pnlReceiptControls.Visible := False;

  FStaffID := GlbLogin.StaffID;
  StaffName := GlbLogin.StaffName;

  cmdSelectStaff.Caption := StaffName;
  cmdSelectTerminal.Caption := AppDetails.TerminalName;
  FTerminalid := AppDetails.TerminalID;

  if (Screen.Height > 600) then
    Height := (Screen.Height - 100) else Height := (Screen.Height - 20);

  if (Skin.FormWindowBorder >= 0) then begin
    pnlForm.BorderWidth := Skin.FormWindowBorder;
    pnlForm.BorderStyle := bsNone;
    pnlForm.BevelOuter := bvNone;
    pnlInner.BorderStyle := bsNone;
    pnlInner.BevelOuter := bvNone;
    if (Skin.FormWindowBorder = 0) then begin
      Height := Height - 16;
      Width := Width - 16;
    end
    else begin
      Height := Height - (2 * (8 - Skin.FormWindowBorder));
      Width := Width - (2 * (8 - Skin.FormWindowBorder));
    end;
  end;

  if (Skin.FormWindowBorderColour <> -1) then begin
    Color := Skin.FormWindowBorderColour;
  end;

  if (Skin.FormWindowColour <> -1) then begin
    pnlInner.ParentColor := False;
    pnlInner.ParentBackground := False;
    pnlInner.Color := Skin.FormWindowColour;
  end;

  if (Skin.FormWindowMessageBorderColour <> -1) then begin
    pnlGrid.ParentColor := False;
    pnlGrid.ParentBackground := False;
    pnlGrid.Color := Skin.FormWindowMessageBorderColour;
    pnlReceipt.ParentColor := False;
    pnlReceipt.ParentBackground := False;
    pnlReceipt.Color := Skin.FormWindowMessageBorderColour;
  end;

  Skin.ApplySkinToGrid(lstCashSales);

  Skin.NewButtonFromOldButton(cmdUp, cmdUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdDown, cmdDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdReceiptUp, cmdReceiptUpDes, bstUP);
  Skin.NewButtonFromOldButton(cmdReceiptDown, cmdReceiptDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdCancel, cmdCancelDes, bstToolbar, gstClose);
  Skin.NewButtonFromOldButton(cmdPrint, cmdPrintReceipt, bstGen96, gstPrinter);
  Skin.NewButtonFromOldButton(cmdStaff, cmdSelectStaff, bstGen72, gstStaff);
  Skin.NewButtonFromOldButton(cmdTerminal, cmdSelectTerminal, bstGen112, gstCashDrawer);
  Skin.NewButtonFromOldButton(cmdInvoiceNoSale, cmdInvoiceNoSaleDes, bstGen96, gstNone);
end;
{******************************************************************************}
procedure TformSelectCashSale.Initialise;
begin
  if (not FTabInvoices) then begin
    if ((SelectedStaffID <> FStaffID) or (SelectedTerminalID <> FTerminalID)) then begin
      SelectedStaffID := FStaffID;
      SelectedTerminalID := FTerminalID;
      SetQueryCashSales;
    end;
  end;

  //this will open it if closed it when exiting cash sale screen earlier 7Dec99 AM
  if (not FTabInvoices) then begin
    if ((Now - WBCashSales.StartTime) > 1) then begin
      WBCashSales.StartTime := SetQueryToday;
      SetQueryCashSales;
    end;
   if (not dm.qrCashSales.Active) then begin
      dm.qrCashSales.Open;
    end
    else begin
      RefreshQuery(dm.qrCashSales); //refresh it anyway, else how refresh when in this form and get the event
    end;
    WBCashSales.RefreshNeeded := False;
    dm.qrCashSales.First;
  end
  else begin
    if (not dm.qrInvoices.Active) then begin
      dm.qrInvoices.Open;
    end
    else begin
      RefreshQuery(dm.qrInvoices); //refresh it anyway, else how refresh when in this form and get the event
    end;
    WBInvoices.RefreshNeeded := False;
    dm.qrInvoices.First;

    if (not dm.qrNoSales.Active) then begin
      dm.qrNoSales.Open;
    end
    else begin
      RefreshQuery(dm.qrNoSales); //refresh it anyway, else how refresh when in this form and get the event
    end;
    WBNoSales.RefreshNeeded := False;
    dm.qrNoSales.First;
  end;

  if (not FLastReceipt) then
  begin
    FReceiptActive := ((AppDetails.ReceiptsViewReceipts) and (Screen.Width > 800));
    FGridActive := True;

    pnlReceiptOuter.Align := alRight;
    pnlReceiptOuter.Width := FReceiptWidth + pnlReceiptSpacer.Width;
    pnlGrid.Visible := FGridActive;
    pnlReceiptOuter.Visible := FReceiptActive;
    pnlReceiptSpacer.Visible := ((FGridActive) and (FReceiptActive));

    if ((FReceiptActive) and (FGridActive)) then
    begin
      Width := FGridWidth + FReceiptWidth + pnlReceiptSpacer.Width;
    end;
    if (not FTabInvoices) then
    begin
      if ((SelectedStaffID <> GlbLogin.StaffID) or (SelectedTerminalID <> AppDetails.TerminalID)) then begin
        SelectedStaffID := GlbLogin.StaffID;
        SelectedTerminalID := AppDetails.TerminalID;
        SetQueryCashSales;
      end;
    end;
  end;

  Position := poScreenCenter;
end;
{******************************************************************************}
procedure TformSelectCashSale.FormShow(Sender: TObject);
begin
  Skin.ApplyWindowCurveToPanelAndForm(pnlForm);
  if (Skin.FormWindowMessageCurve <> -1) then
  begin
    Skin.ApplyMessageCurveToInnerAndOuter(pnlGridInner);
    Skin.ApplyMessageCurveToInnerAndOuter(pnlReceiptInner);
  end;

  if (FTabInvoices) then
  begin
    if (dm.qrInvoices.Eof) then
    begin
      cmdInvoiceNoSale.Click;
    end
    else if (FLastReceipt) then
    begin
      lblTitle.Caption := Format(rsLastTabInvoice, [GlbTable.TableNo, GlbTable.TableName]);
    end
    else
    begin
      lblTitle.Caption := Format(sSelectTabInvoice, [GlbTable.TableNo, GlbTable.TableName]);
    end;
    pnlButtonsFooter.Visible := False;
  end
  else if (FLastCashSale) then
  begin
    lblTitle.Caption := rsLastCashSale;
  end;
  pnlControls.Visible := (lstCashSales.Height < (lstCashSales.DataSource.DataSet.EofRowNum + 1) * (lstCashSales.DefaultRowHeight + lstCashSales.BorderRows));
  pnlGridControls.Visible := pnlControls.Visible or pnlButtonsFooter.Visible;

  RefreshReceipts;
end;
{******************************************************************************}
procedure TformSelectCashSale.lstCashSalesCellClick(Sender: TObject; ACol, ARow: Integer; AButton: TMouseButton; AShift: TShiftState);
begin
  if (not FReceiptActive) then
  begin
    if (FShowingInvoices) and (not lstCashSales.DataSource.DataSet.FieldByName('invoiceid').IsNull) then
    begin
      FInvoiceID := lstCashSales.DataSource.DataSet.FieldByName('invoiceid').AsInteger;
      FInvoiceType := lstCashSales.DataSource.DataSet.FieldByName('invoicetype').AsString;
    end
    else if (not FShowingInvoices) and (not lstCashSales.DataSource.DataSet.FieldByName('tenderid').IsNull) then
    begin
      FInvoiceID := lstCashSales.DataSource.DataSet.FieldByName('tenderid').AsInteger;
      FInvoiceType := 'N';
    end;
    ModalResult := mrOk;
  end;
  GlobalMenuList.ListNYHappyHourOL.Clear;
  DisplayReceipt;

end;
{******************************************************************************}
procedure TformSelectCashSale.cmdCancelClick(Sender: TObject);
begin
  ModalResult := mrCancel;
end;
{******************************************************************************}
procedure TformSelectCashSale.cmdUpClick(Sender: TObject);
begin
  SendMessage(lstCashSales.Handle,WM_VSCROLL,SB_PAGEUP,0);
end;
{******************************************************************************}
procedure TformSelectCashSale.cmdDownClick(Sender: TObject);
begin
  SendMessage(lstCashSales.Handle,WM_VSCROLL,SB_PAGEDOWN,0);
  if lstCashSales.DataSource.DataSet.Fields[1].IsNull then
    lstCashSales.DataSource.DataSet.Last;
end;
{******************************************************************************}
procedure TformSelectCashSale.DisplayReceipt;
begin
  Receipt.Clear;
  if (lstCashSales.DataSource.DataSet.EOF) then begin
    Exit;
  end;
  with lstCashSales.DataSource.DataSet do begin   
    Receipt.RemoteLocationName := FieldAsString(FieldByName('remotelocationname'));
    Receipt.LoyaltyReward := FieldAsString(FieldByName('loyaltyreward'));
    Receipt.LoyaltyPoints1 := FieldAsInt(FieldByName('loyaltypoints1'));
    Receipt.LoyaltyPoints2 := FieldAsInt(FieldByName('loyaltypoints2'));
    Receipt.LoyaltyPoints1Rewarded := FieldAsInt(FieldByName('loyaltypoints1rewarded'));
    Receipt.LoyaltyPoints2Rewarded := FieldAsInt(FieldByName('loyaltypoints2rewarded'));
    Receipt.BalanceChanged := FieldAsCurrency(FieldByName('balancechanged'));
    if (FShowingInvoices) then
    begin
      Receipt.InvoiceID := FieldByName('invoiceid').AsInteger;
      Receipt.TenderID := FieldAsInt(FieldByName('tenderid'));
      Receipt.AccountName := FieldAsString(FieldByName('accountname'));
      Receipt.TableNo := FieldAsInt(FieldByName('tableno'));
      Receipt.Location := FieldAsString(FieldByName('location'));
      Receipt.GroupName := FieldAsString(FieldByName('groupname'));

      Receipt.InvoiceNo := FieldAsInt(FieldByName('invoiceno'));
      Receipt.StaffName := FieldAsString(FieldByName('staffname'));
      Receipt.WhenInvoiced := FieldAsDate(FieldByName('wheninvoiced'));
      Receipt.EventID := FieldAsInt(FieldByName('eventid'));
      Receipt.EventName := FieldAsString(FieldByName('eventname'));
      Receipt.HostSubsidy := FieldAsCurrency(FieldByName('HostSubsidy'));
      Receipt.GuestSubsidy := FieldAsCurrency(FieldByName('GuestSubsidy'));
    end
    else
    begin
      Receipt.TenderID := FieldAsInt(FieldByName('tenderid'));
      Receipt.NoSaleTypeName := FieldAsString(FieldByName('nosaletype'));
      Receipt.TableNo := FieldAsInt(FieldByName('tableno'));
      Receipt.RemoteLocationName := FieldAsString(FieldByName('remotelocationname'));
      Receipt.LoyaltyReward := FieldAsString(FieldByName('loyaltyreward'));
      Receipt.LoyaltyPoints1 := FieldAsInt(FieldByName('loyaltypoints1'));
      Receipt.LoyaltyPoints2 := FieldAsInt(FieldByName('loyaltypoints2'));
      Receipt.LoyaltyPoints1Rewarded := FieldAsInt(FieldByName('loyaltypoints1rewarded'));
      Receipt.LoyaltyPoints2Rewarded := FieldAsInt(FieldByName('loyaltypoints2rewarded'));
      Receipt.BalanceChanged := FieldAsCurrency(FieldByName('balancechanged'));

      Receipt.TenderNo := FieldAsInt(FieldByName('tenderno'));
      Receipt.StaffName := FieldAsString(FieldByName('staffname'));
      Receipt.WhenInvoiced := FieldAsDate(FieldByName('whentendered'));
    end;
  end;
  Receipt.DisplayReceipt;   
  cmdReceiptDown.Visible := (Receipt.mmoReceipt.Height < Receipt.Height);
  cmdReceiptUp.Visible := (Receipt.mmoReceipt.Height < Receipt.Height);
end;
{******************************************************************************}
procedure TformSelectCashSale.cmdPrintReceiptClick(Sender: TObject);
begin
  if (FGridActive) then begin
    TryPrintReceipt;
  end
  else
  begin
    if ((AppDetails.SpecialNYHappyHour) and (GlobalMenuList.ListNYHappyHourOL.Count > 0)) then begin
      Receipt.PrintNYReceipt;
      ModalResult := mrCancel;
    end
    else if (FShowingInvoices) and (not lstCashSales.DataSource.DataSet.FieldByName('invoiceid').IsNull) then
    begin
      FInvoiceID := lstCashSales.DataSource.DataSet.FieldByName('invoiceid').AsInteger;
      FInvoiceType := lstCashSales.DataSource.DataSet.FieldByName('invoicetype').AsString;
      ModalResult := mrOk;
    end
    else if (not FShowingInvoices) and (not lstCashSales.DataSource.DataSet.FieldByName('tenderid').IsNull) then
    begin
      FInvoiceID := lstCashSales.DataSource.DataSet.FieldByName('tenderid').AsInteger;
      FInvoiceType := 'N';                                    
      ModalResult := mrOk;
    end
    else
    begin
      ModalResult := mrCancel;
    end;
  end;
end;
{******************************************************************************}
function TformSelectCashSale.TryPrintReceipt : Boolean;
begin
  Result := True;
  try
    If (lstCashSales.DataSource.DataSet.Fields[1].IsNull) then
    begin
      SendtoBack;
      ShowQuickMsg(sNoReceiptSelected);
      BringtoFront;
    end
    else if (FShowingInvoices) then
    begin
      if ((AppDetails.SpecialNYHappyHour) and (GlobalMenuList.ListNYHappyHourOL.Count > 0)) then
      begin
        Receipt.PrintNYReceipt;
      end
      else if (lstCashSales.DataSource.DataSet.FieldByName('invoicetype').AsString = 'I') then
      begin
        if (AppDetails.FiscalType = fiscalTaxCore) then
          PrintTaxCoreInvoice(lstCashSales.DataSource.DataSet.FieldByName('invoiceid').AsInteger, True)
        else
        begin
          InsertPrintJob('C Invoice', lstCashSales.DataSource.DataSet.FieldByName('invoiceid').AsInteger, AppDetails.PrinterID);
          SendtoBack;
          if (not FTabInvoices) then
            ShowQuickMsg(sCashSalePrinted)
          else
            ShowQuickMsg(sInvPrinted);
          BringtoFront;
        end;
      end
      else
      begin
        InsertPrintJob('C Credit Note', lstCashSales.DataSource.DataSet.FieldByName('invoiceid').AsInteger, AppDetails.PrinterID);
        SendtoBack;
        ShowQuickMsg('Credit note printed.', False);
        BringtoFront;
      end;
    end
    else
    begin
      InsertPrintJob('C No Sale', lstCashSales.DataSource.DataSet.FieldByName('tenderid').AsInteger, AppDetails.PrinterID);
      SendtoBack;
      ShowQuickMsg(sNoSalePrinted);
      BringtoFront;
    end;

  except on e : exception do
  begin
    Result := False;
    ShowMessage(e.message);
  end; end;
end;
{******************************************************************************}
procedure TformSelectCashSale.cmbStaffChange(Sender: TObject);
begin
  inherited;
  hide;
  try
    if (SelectStaff) then begin
      FStaffID := GlbStaff.StaffID;
      if (Assigned(cmdSelectStaff)) then begin
        cmdSelectStaff.Caption := GlbStaff.StaffName;
      end
      else begin
        cmdStaff.Caption := GlbStaff.StaffName;
      end;
    end;

    BringToFront;

    SelectedStaffID := FStaffID;
  finally
    Show;
  end;
end;
{******************************************************************************}
procedure TformSelectCashSale.cmbTerminalChange(Sender: TObject);
begin
  inherited;
  Hide;
  try
    if (SelectTerminal) then begin
      FTerminalid := GlbTerminal.TerminalID;
      if (Assigned(cmdSelectStaff)) then begin
        cmdSelectTerminal.Caption := GlbTerminal.Terminal
      end
      else begin
        cmdTerminal.Caption := GlbTerminal.Terminal;
      end;
    end;
    SelectedTerminalid := FTerminalid;
  finally
    Show;
  end;
end;
{******************************************************************************}
procedure TformSelectCashSale.cmdReceiptUpClick(Sender: TObject);
begin
  Receipt.ScrollUp;
end;
{******************************************************************************}
procedure TformSelectCashSale.cmdReceiptDownClick(Sender: TObject);
begin
  Receipt.Scrolldown;
end;
{******************************************************************************}
procedure TformSelectCashSale.chkStaffClick(Sender: TObject);
begin
  if (chkStaff.Checked) then
  begin
    SelectedStaffID := 0;
  end
  else
  begin
    SelectedStaffID := FStaffID;
  end;

  if (chkTerminal.Checked) then
  begin
    SelectedTerminalID := 0;
  end
  else
  begin
    SelectedTerminalID := FTerminalID;
  end;

  if (Assigned(cmdSelectStaff)) then
  begin
    cmdSelectStaff.Enabled := (not chkstaff.Checked);
    cmdSelectTerminal.Enabled := (not chkTerminal.Checked);
  end
  else
  begin
    cmdStaff.Enabled := (not chkstaff.Checked);
    cmdTerminal.Enabled := (not chkTerminal.Checked);
  end;

  RefreshReceipts;
end;
{******************************************************************************}
procedure TformSelectCashSale.RefreshReceipts;
begin
  if (not FTabInvoices) then
    SetQueryCashSales
  else
    SetQueryInvoices(qtTabInvoices);
  lstCashSales.DataSource.DataSet.first;
  pnlControls.Visible := (lstCashSales.Height < (lstCashSales.DataSource.DataSet.EofRowNum + 1) * (lstCashSales.DefaultRowHeight + lstCashSales.BorderRows));
  Receipt.Clear;
  DisplayReceipt;
end;
{******************************************************************************}
procedure TformSelectCashSale.FormMessage(var Msg: TMsg; var Handled: Boolean);
var
  lPoint: TPoint;

begin
  case Msg.message of
    WM_LBUTTONDOWN, WM_MBUTTONDOWN, WM_RBUTTONDOWN, WM_KEYDOWN:
      begin
        if (FQuickHide) then
        begin
          //Don't close if button click was within pnlReceiptControls
          lPoint := pnlReceiptControls.ScreenToClient(Msg.Pt);
          if ((lPoint.X < 0) or (lPoint.Y < 0) or (lPoint.X >= pnlReceiptControls.Width) or (lPoint.Y >= pnlReceiptControls.Height)) then
          begin
            Close;
          end;
        end;
      end;
  end;
end;
{******************************************************************************}

procedure TformSelectCashSale.cmdInvoiceNoSaleDesClick(Sender: TObject);
begin
  if (FShowingInvoices) then
  begin
    lstCashSales.DataSource := dm.dsNoSales;
    cmdInvoiceNoSale.Caption := sInvoices;
    lblTitle.Caption := Format(sSelectTabNoSales, [GlbTable.TableNo, GlbTable.TableName]);
    FShowingInvoices := False;
  end
  else
  begin
    lstCashSales.DataSource := dm.dsInvoices;
    cmdInvoiceNoSale.Caption := sNoSales;   
    lblTitle.Caption := Format(sSelectTabInvoice, [GlbTable.TableNo, GlbTable.TableName]);
    FShowingInvoices := True;
  end;                                     
  DisplayReceipt;
end;

end.


