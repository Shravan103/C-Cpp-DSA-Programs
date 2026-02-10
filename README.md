unit UItemDetail;
{******************************************************************************}
interface
{******************************************************************************}
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  UDetailForm, ExtCtrls, StdCtrls, Buttons, IB_Grid, ComCtrls, Grids,
  ImgList{,IvDictio, IvMulti}, FRemoteOverrideGroup, IB_Components, strutils,
  UItemSharing, System.ImageList;
{******************************************************************************}
type
  (**
    Used for editing a selected item.
  *)
  TformItemDetail = class(TformDetailForm)
    pcDetail: TPageControl;
    tabGeneral: TTabSheet;
    tabControl: TTabSheet;
    lblStaff: TLabel;
    lblLastUpdated: TLabel;
    lblTerminal: TLabel;
    lblDeleted: TLabel;
    txtStaff: TEdit;
    txtLastUpdated: TEdit;
    txtTerminal: TEdit;
    txtDeleted: TEdit;
    lblItem: TLabel;
    lblDescription: TLabel;
    txtAbbrev: TEdit;
    cmbItemGroup: TIB_LookupCombo;
    lblItemGroup: TLabel;
    txtPrice: TEdit;
    lblUnitPrice1Dollar: TLabel;
    mmoName: TMemo;
    chkCourse: TCheckBox;
    cmbCourse: TIB_LookupCombo;
    lblUnitPrice1: TLabel;
    lblUnitPrice2Dollar: TLabel;
    lblUnitPrice2: TLabel;
    txtPrice2: TEdit;
    lblUnitPrice3Dollar: TLabel;
    lblUnitPrice3: TLabel;
    txtPrice3: TEdit;
    lblUnitPrice4Dollar: TLabel;
    lblUnitPrice4: TLabel;
    txtPrice4: TEdit;
    lblUnitPrice5Dollar: TLabel;
    lblUnitPrice5: TLabel;
    txtPrice5: TEdit;
    lblUnitPrice6Dollar: TLabel;
    lblUnitPrice6: TLabel;
    txtPrice6: TEdit;
    chkPriceExcludesTax: TCheckBox;
    chkOpenPrice: TCheckBox;
    lblOpenPriceMin: TLabel;
    lblOpenPriceMinDollar: TLabel;
    txtOpenPriceMin: TEdit;
    txtOpenPriceMax: TEdit;
    lblOpenPriceMaxDollar: TLabel;
    lblOpenPriceMax: TLabel;
    cmdUpdateAllPrices: TSpeedButton;
    chkEnterQty: TCheckBox;
    lblEnterQty: TLabel;
    txtEnterQty: TEdit;
    lblBarcode: TLabel;
    txtBarCode: TEdit;
    tabLoyalty: TTabSheet;
    lblLoyalty1: TLabel;
    lblLoyaltyRatio: TLabel;
    lblLoyaltyQty: TLabel;
    txtLoyaltyPoints1RatioNum: TEdit;
    txtLoyaltyPoints1RatioDen: TEdit;
    lblLoyalty1Div: TLabel;
    txtLoyaltyPoints1Qty: TEdit;
    lblLoyalty2: TLabel;
    txtLoyaltyPoints2RatioNum: TEdit;
    txtLoyaltyPoints2RatioDen: TEdit;
    lblLoyalty2Div: TLabel;
    txtLoyaltyPoints2Qty: TEdit;
    tabNYHappyHour: TTabSheet;
    lblNYPriceMin: TLabel;
    lblNYPriceMinDollar: TLabel;
    lblNYPriceMax: TLabel;
    lblNYPriceMaxDollar: TLabel;
    lblNYIncrease: TLabel;
    lblNYPeriod: TLabel;
    lblNYUnits: TLabel;
    lblNYAmount: TLabel;
    lblNYDecrease: TLabel;
    chkNYPricingEnabled: TCheckBox;
    txtNYPriceMin: TEdit;
    txtNYPriceMax: TEdit;
    txtNYIncPeriod: TEdit;
    txtNYIncUnits: TEdit;
    txtNYIncAmount: TEdit;
    txtNYDecPeriod: TEdit;
    txtNYDecUnits: TEdit;
    txtNYDecAmount: TEdit;
    lblLinkCode: TLabel;
    txtLinkCode: TEdit;
    chkWeighedItem: TCheckBox;
    txtNonStockPrice: TEdit;
    lblNonStockPriceDollar: TLabel;
    lblNonStockPrice: TLabel;
    chkNonStockPrice: TCheckBox;
    edtGTIN: TEdit;
    edtUnit: TEdit;
    lblUnit: TLabel;
    frameRemoteOverrideGroup: TframeRemoteOverrideGroup;
    lblVariation: TLabel;
    cmbSize: TIB_LookupCombo;
    lblParentItem: TLabel;
    cmbParentItem: TIB_LookupCombo;
    dsParentItem: TIB_DataSource;
    chkShowParentInSelection: TCheckBox;
    tabAdvPricing: TTabSheet;
    frameItemSharing: TframeItemSharing;
    cmdNextBarcode: TSpeedButton;
    cmdCopyCourse: TSpeedButton;
    tabFiscal: TTabSheet;
    rgIATIndicator: TRadioGroup;
    lblCEST: TLabel;
    txtCEST: TEdit;
    txtCFOP: TEdit;
    lblCFOP: TLabel;
    rgTaxSituation: TRadioGroup;
    chkIsService: TCheckBox;
    chkManufacturedBy: TCheckBox;
    pnlFiscalCST: TPanel;
    lblCst_Icms: TLabel;
    lblCst_IpI: TLabel;
    lblCST_PIS: TLabel;
    lblCST_COFFINS: TLabel;
    lblCSOSN: TLabel;
    edtCST_ICMS: TEdit;
    edtCST_IPI: TEdit;
    edtCST_PIS: TEdit;
    edtCST_COFFINS: TEdit;
    edtCSOSN: TEdit;
    txtNCMCode: TEdit;
    lblGTIN: TLabel;
    lblNCMCode: TLabel;
    chkItemAutoTender: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure cmdOkClick(Sender: TObject);
    procedure cmdHideClick(Sender: TObject);
    procedure mmoNameChange(Sender: TObject);
    procedure SetItemDetailBarCode(BarCode: String);
    procedure cmdUpdateAllPricesClick(Sender: TObject);
    procedure txtAbbrevEnter(Sender: TObject);
    procedure rgTaxSituationClick(Sender: TObject);
    procedure chkManufacturedByClick(Sender: TObject);
    procedure rgImportedFromClick(Sender: TObject);
    procedure edtCST_ICMSChange(Sender: TObject);
    procedure rgIATIndicatorClick(Sender: TObject);
    procedure txtNCMCodeChange(Sender: TObject);
    procedure cmbSizeChange(Sender: TObject);
    procedure cmbParentItemChange(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure chkShowParentInSelectionClick(Sender: TObject);
    procedure frameRemoteOverrideGroupcmbRemoteOverrideGroupsChange(
      Sender: TObject);
    procedure frameRemoteOverrideGroupcmdManuallySelectClick(
      Sender: TObject);
    procedure txtPriceChange(Sender: TObject);
    procedure frameItemSharinglstItemSharesSetEditText(Sender: TObject;
      ACol, ARow: Integer; const Value: String);
    procedure frameItemSharingtxtPrice1Change(Sender: TObject);
    procedure frameItemSharingtxtOutletP1Change(Sender: TObject);
    procedure pcDetailChange(Sender: TObject);
    procedure frameItemSharingcmdDefaultClick(Sender: TObject);
    procedure frameItemSharingcmdDefaultAllClick(Sender: TObject);
    procedure frameItemSharingcmdDeltaPricesClick(Sender: TObject);
    procedure frameItemSharingcmdDeltaAllPricesClick(Sender: TObject);
    procedure frameItemSharingcmdUpdateAllPricesClick(Sender: TObject);
    procedure tabAdvPricingEnter(Sender: TObject);
    procedure cmdNextBarcodeClick(Sender: TObject);
    procedure txtBarCodeChange(Sender: TObject);
    procedure cmdCopyCourseClick(Sender: TObject);
  private
    FItemWithChildren: Boolean;
    FParentItemChanged: Boolean;
    FAllowUpdateName: Boolean;
  private
    CurrentSelectedTEdit: TEdit;
    CurrentSelectedTEditText: String;
    function TryEditItem : Boolean;
    function ValidateItem : Boolean;
    function ValidateTxtPrice(PriceEdit: TEdit; PriceName: String): Boolean;
    //function RestrictPriceToOneDecimal(Sender: TObject): boolean;
  end;
{******************************************************************************}
var
  formItemDetail: TformItemDetail;
{******************************************************************************}
implementation
{******************************************************************************}
uses
  LItemQueries, LWBQuery, UConfirmation, UMessage, LErrors, LMain, UAppDetails,
  DM_WBWaiter, UTendering,
  UBaseDetailForm, UEvolutionTransaction, UResources, UGlobalVariables, UEncryption,
  IB_Access, LSetupQueries, ULogFile, UThemeApply;
{******************************************************************************}
{$R *.DFM}
{******************************************************************************}
procedure TformItemDetail.FormCreate(Sender: TObject);
begin
  tabNYHappyHour.TabVisible := AppDetails.NewOrderAllowNYHappyHour;
  tabFiscal.TabVisible := AppDetails.EnableFiscalPrinting;
  inherited;
  FAllowUpdateName := False;
  ChangeTitleImage(tiItem);

  lblUnitPrice1.Caption := GetItemPriceName(1);
  lblUnitPrice2.Caption := GetItemPriceName(2);
  lblUnitPrice3.Caption := GetItemPriceName(3);
  lblUnitPrice4.Caption := GetItemPriceName(4);
  lblUnitPrice5.Caption := GetItemPriceName(5);
  lblUnitPrice6.Caption := GetItemPriceName(6);
  cmdUpdateAllPrices.Caption := Format(sCopyName,[GetItemPriceName(1)]);

  lblUnitPrice1Dollar.Caption := AppDetails.CurrencyType;
  lblUnitPrice2Dollar.Caption := AppDetails.CurrencyType;
  lblUnitPrice3Dollar.Caption := AppDetails.CurrencyType;
  lblUnitPrice4Dollar.Caption := AppDetails.CurrencyType;
  lblUnitPrice5Dollar.Caption := AppDetails.CurrencyType;
  lblUnitPrice6Dollar.Caption := AppDetails.CurrencyType;
  lblOpenPriceMinDollar.Caption := AppDetails.CurrencyType;
  lblOpenPriceMaxDollar.Caption := AppDetails.CurrencyType;

  lblLoyaltyRatio.Caption := Format(sPointsPerType,[AppDetails.CurrencyType]);

  lblNYPriceMinDollar.Caption := AppDetails.CurrencyType;
  lblNYPriceMaxDollar.Caption := AppDetails.CurrencyType;

  if (WBItemGroups.QueryType <> qtItemGroupsCurrent) then begin
    FilterQuery(dm.qrItemGroups, qtItemGroupsCurrent);
  end;

  if (WBCourses.QueryType <> qtCoursesCurrent) then begin
    FilterQuery(dm.qrCourses, qtCoursesCurrent);
  end;

  txtAbbrev.Text := GlbItem.Abbrev;
  mmoName.Text := GlbItem.ItemName;

  {if (GlbItem.WeighedItem) then
  begin
    txtPrice.Text := FormatFloat('##,##0.00##',GlbItem.Price  * 1000);
    txtPrice2.Text := FormatFloat('##,##0.00##',GlbItem.Price2 * 1000); //AKM 3.3.1
    txtPrice3.Text := FormatFloat('##,##0.00##',GlbItem.Price3 * 1000); //AKM 3.3.1
    txtPrice4.Text := FormatFloat('##,##0.00##',GlbItem.Price4 * 1000); //AKM 3.3.1
    txtPrice5.Text := FormatFloat('##,##0.00##',GlbItem.Price5 * 1000); //AKM 3.3.1
    txtPrice6.Text := FormatFloat('##,##0.00##',GlbItem.Price6 * 1000); //AKM 3.3.1
  end
  else }
  begin
    txtPrice.Text := FormatCurr('##,##0.00##',GlbItem.Price);
    txtPrice2.Text := FormatCurr('##,##0.00##',GlbItem.Price2); //AKM 3.3.1
    txtPrice3.Text := FormatCurr('##,##0.00##',GlbItem.Price3); //AKM 3.3.1
    txtPrice4.Text := FormatCurr('##,##0.00##',GlbItem.Price4); //AKM 3.3.1
    txtPrice5.Text := FormatCurr('##,##0.00##',GlbItem.Price5); //AKM 3.3.1
    txtPrice6.Text := FormatCurr('##,##0.00##',GlbItem.Price6); //AKM 3.3.1
  end;

  tabAdvPricing.TabVisible := ((frameItemSharing.pnlOutlet.Visible) or frameItemSharing.pnlRemoteLocations.Visible);

  if (GlbItem.WhenDeleted > 0) then begin
    txtDeleted.Text := FormatDateTime(TIMEDATEFORMAT,GlbItem.WhenDeleted);
  end;

  if (GlbItem.WhenUpd > 0) then begin
    txtLastUpdated.Text := FormatDateTime(TIMEDATEFORMAT,GlbItem.WhenUpd);
  end;

  txtStaff.Text := GlbItem.StaffName;
  txtTerminal.Text := GlbItem.TerminalName;

  chkPriceExcludesTax.Checked := GlbItem.PriceExcludesTax;
  chkPriceExcludesTax.Caption := Format(sPriceExcludes,[AppDetails.TaxName]);  //Jon 25-5-2001
  chkPriceExcludesTax.Visible := (Not AppDetails.EnableFiscalPrinting);

  chkOpenPrice.Checked := GlbItem.OpenPrice;
  if (GlbItem.OpenPrice) then begin
    txtOpenPriceMin.Text := FormatCurr('##,##0.00',GlbItem.OpenPriceMin);
    txtOpenPriceMax.Text := FormatCurr('##,##0.00',GlbItem.OpenPriceMax);
  end
  else begin
    txtOpenPriceMin.Text := FormatCurr('##,##0.00',GlbItem.Price);
    txtOpenPriceMax.Text := FormatCurr('##,##0.00',GlbItem.Price);
  end;

  chkNonStockPrice.Checked := (GlbItem.NonStockPrice > 0);
  if (GlbItem.NonStockPrice > 0) then begin
    txtNonStockPrice.Text := FormatCurr('##,##0.00##', GlbItem.NonStockPrice);
  end;

  if (GlbItem.EnterQty > 0) then begin
    txtEnterQty.Text := FormatFloat('#####0.###',GlbItem.EnterQty);
    chkEnterQty.Checked := True;
  end;

  chkWeighedItem.Checked := GlbItem.WeighedItem;
  txtNCMCode.Text := GlbItem.NCMProductCode;
  txtBarcode.Text := GlbItem.Barcode;
  txtLinkCode.Text := GlbItem.LinkCode;

  if (GlbItem.LoyaltyPoints1RatioNum >= 0) then begin
    txtLoyaltyPoints1RatioNum.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints1RatioNum);
  end;
  if (GlbItem.LoyaltyPoints1RatioDen > 0) then begin
    txtLoyaltyPoints1RatioDen.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints1RatioDen);
  end;
  if (GlbItem.LoyaltyPoints1Qty >= 0) then begin
    txtLoyaltyPoints1Qty.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints1Qty);
  end;
  if (GlbItem.LoyaltyPoints2RatioNum >= 0) then begin
    txtLoyaltyPoints2RatioNum.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints2RatioNum);
  end;
  if (GlbItem.LoyaltyPoints2RatioDen > 0) then begin
    txtLoyaltyPoints2RatioDen.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints2RatioDen);
  end;
  if (GlbItem.LoyaltyPoints2Qty >= 0) then begin
    txtLoyaltyPoints2Qty.Text := FormatCurrencyPrecision(GlbItem.LoyaltyPoints2Qty);
  end;

  if ((GlbItem.NYPriceMax > 0) and (GlbItem.NYIncAmount > 0) and (GlbItem.NYIncUnits > 0)) then begin
    chkNYPricingEnabled.Checked := True;
    txtNYPriceMin.Text := FormatCurr('##,##0.00##',GlbItem.NYPriceMin);
    txtNYPriceMax.Text := FormatCurr('##,##0.00##',GlbItem.NYPriceMax);
    txtNYIncPeriod.Text := IntToStr(GlbItem.NYIncPeriod);
    txtNYIncUnits.Text := IntToStr(GlbItem.NYIncUnits);
    txtNYIncAmount.Text := FormatCurr('##,##0.00##',GlbItem.NYIncAmount);
    txtNYDecPeriod.Text := IntToStr(GlbItem.NYDecPeriod);
    txtNYDecUnits.Text := IntToStr(GlbItem.NYDecUnits);
    txtNYDecAmount.Text := FormatCurr('##,##0.00##',GlbItem.NYDecAmount);
  end
  else begin
    chkNYPricingEnabled.Checked := False;
    txtNYPriceMin.Text := '0.00';
    txtNYPriceMax.Text := '0.00';
    txtNYIncPeriod.Text := '1';
    txtNYIncUnits.Text := '0';
    txtNYIncAmount.Text := '0.00';
    txtNYDecPeriod.Text := '1';
    txtNYDecUnits.Text := '0';
    txtNYDecAmount.Text := '0.00';
  end;

  if (AppDetails.PMSType = pmsEvolution) then begin
    txtLinkCode.ReadOnly := True;
    txtLinkCode.Color := clBtnFace;
  end;

  if (GlbItem.happy > 0) then chkItemAutoTender.checked := true;
  if ((AppDetails.CashSaleItemAutoTender) and (height > 600)) then
    chkItemAutoTender.Visible := True
  else
    chkItemAutoTender.Visible := False;

  if Not GlbItem.WeighedItem then
    edtUnit.Text := 'UN'
  else
  begin //0 = in Kilos 1 - in 10gm , 2 - in gm
    if AppDetails.WeightedItem in [0,1] then
      edtUnit.Text := 'KG'
    else
      edtUnit.Text := 'GM';
  end;

  chkOpenPrice.Visible := Not AppDetails.EnableFiscalPrinting;
  chkNonStockPrice.Visible := Not AppDetails.EnableFiscalPrinting;
  chkManufacturedBy.Visible := AppDetails.EnableFiscalPrinting;
  rgTaxSituation.Visible := AppDetails.EnableFiscalPrinting;
  rgIATIndicator.Visible := AppDetails.EnableFiscalPrinting;
  chkIsService.Visible := AppDetails.EnableFiscalPrinting;
  chkIsService.Checked := Boolean(GlbItem.IsService);
  edtGTIN.Text := GlbItem.GTIN;
  chkManufacturedBy.Checked := False;
  pnlFiscalCST.Visible := AppDetails.EnableFiscalPrinting;

  if Trim(GlbItem.ManufacturedBy) = 'P' then
    chkManufacturedBy.Checked := True;
  {In Brazil there are  three  non taxed rate from ICMS: II, NN or FF
  And there are also three non taxed from ISS: SN, SI or SF
  I	Exempt
  N	Not Taxed
  F	Tax Substitution}
  if AppDetails.EnableFiscalPrinting then
  begin
    if GlbItem.TaxSituation = 'T' then
      rgTaxSituation.ItemIndex := 0
    else if GlbItem.TaxSituation = 'S' then
      rgTaxSituation.ItemIndex := 1
    else if GlbItem.TaxSituation = 'II' then
      rgTaxSituation.ItemIndex := 2
    else if GlbItem.TaxSituation = 'NN' then
      rgTaxSituation.ItemIndex := 3
    else if GlbItem.TaxSituation = 'FF' then
      rgTaxSituation.ItemIndex := 4
    else if GlbItem.TaxSituation = 'SI' then
      rgTaxSituation.ItemIndex := 5
    else if GlbItem.TaxSituation = 'SN' then
      rgTaxSituation.ItemIndex := 6
    else if GlbItem.TaxSituation = 'SF' then
      rgTaxSituation.ItemIndex := 7;

    if GlbItem.IATIndicator = 'A' then
      rgIATIndicator.ItemIndex := 0
    else
      rgIATIndicator.ItemIndex := 1;

    //rgImportedFrom.ItemIndex := GlbItem.ImportedFrom;
    edtCST_ICMS.Text := GlbItem.CST_ICMS;
    edtCST_IPI.Text := GlbItem.CST_IPI;
    edtCST_PIS.Text := GlbItem.CST_PIS;
    edtCST_COFFINS.Text := GlbItem.CST_COFFINS;
    edtCSOSN.Text := GlbItem.CSOSN;
    txtCFOP.Text := IntToStr(GlbItem.CFOP);
    if (GlbItem.CEST > 0) then
      txtCEST.Text := IntToStr(GlbItem.CEST)
    else
      txtCEST.Text := '';
  end;

  frameRemoteOverrideGroup.Visible := ((AppDetails.ROGItemExists) and (GlbItemGroup.RemoteOverrideGroupID = 0));

  if (frameRemoteOverrideGroup.Visible) then begin
    frameRemoteOverrideGroup.TaskItem := True;
    frameRemoteOverrideGroup.RemoteGroupID := GlbItem.RemoteOverrideGroupID;
    frameRemoteOverrideGroup.cmdManuallySelect.Visible := True;
    frameRemoteOverrideGroup.SortAlphabetically := True;
    frameRemoteOverrideGroup.LoadCombo;
    frameRemoteOverrideGroup.CurrentRemoteOverrideGroupID := GlbItem.RemoteOverrideGroupID;
    frameRemoteOverrideGroup.OnChange := mmoNameChange;
  end;

  chkShowParentInSelection.Checked := GlbItem.ParentItemInSelection;

//  ApplyTheme(Self, 'C:\Users\WizBang\Desktop\otPOS_Theme\ThemeApplyNew.json');
end;
{******************************************************************************}
procedure TformItemDetail.FormShow(Sender: TObject);
begin
  inherited;
  FItemWithChildren := False;
  DM.qrItemSize.Close;
  DM.qrItemSize.SQL.Clear;
  FilterQuery(dm.qrItemSize,qtitemsizesCurrent);

  (*DM.qrItemSize.SQL.Clear;
  DM.qrItemSize.SQL.Add('select * from itemsize where sizeid not in ( ');

  if FieldByName('parentitemid').IsNull then
    DM.qrItemSize.SQL.Add('select sizeid from item where parentitemid =' + IntToStr(FieldByName('itemid').AsInteger) ) // means parent item , just check for sizeids used for children , common
  else// means it's a children , then take of all other children's siz and the parent 's size too.
  begin
    DM.qrItemSize.SQL.Add(' select sizeid from item where parentitemid =' + IntToStr(FieldByName('parentitemid').AsInteger) + ' and itemid <> ' + IntToStr(FieldByName('itemid').AsInteger) );
    DM.qrItemSize.SQL.Add('  union select sizeid from item where itemid =' + IntToStr(FieldByName('parentitemid').AsInteger) );
  end;

  DM.qrItemSize.SQL.Add(' )');*)

  DM.qrItemSize.Open;
  dm.qrCourses.Open;

  dm.qrItemGroups.OrderingItemNo := 2;
  dm.qrCourses.OrderingItemNo := 2;

  dm.qrItemGroups.KeyFields.ByName('itemgroupid').AsInteger := GlbItem.ItemGroupID;
  dm.qrItemGroups.LookUpKeyForFields;

  if (GlbItem.CourseID > 0) then begin
    dm.qrCourses.KeyFields.ByName('courseid').AsInteger := GlbItem.CourseID;
    dm.qrCourses.LookUpKeyForFields;
    chkCourse.Checked := True;
    cmbCourse.Visible := True;
  end
  else begin
    chkCourse.Checked := False;
    cmbCourse.Visible := False;
  end;

  with DM.qrGeneral1 do
  begin
    Close;
    sql.Clear;
    SQL.Add('select count(i.itemid) Children from item i where i.parentitemid = :itemid');
    ParamByName('itemid').AsInteger := GlbItem.ItemID;
    Active := True;
    if ((not Eof) and (FieldByName('Children').AsInteger <> 0)) then
      FItemWithChildren := True;
  end;

  cmbSize.Enabled := (Not FItemWithChildren) ;//((GlbItem.ItemID > 0) and (Not IsBaseItem));
  chkShowParentInSelection.Visible := FItemWithChildren;

  if GlbItem.SizeId > 0 then
  begin
    if ( not dm.qrItemSize.Active) then
      CheckQuery(dm.qrItemSize, qtItemSizesCurrent);
    dm.qrItemSize.KeyFields.ByName('sizeid').AsInteger := GlbItem.SizeId;
    dm.qrItemSize.LookUpKeyForFields;
  end;

  cmbItemGroup.DataSource := dm.dsItemGroups;
  cmbCourse.DataSource := dm.dsCourses;
  cmbSize.DataSource := dm.dsItemSize;
  cmbParentItem.DataSource := dsParentItem;

  pcDetail.ActivePage := tabGeneral;
  txtPrice.SetFocus;
  cmdOk.Enabled := False;

  txtNCMCode.Visible := AppDetails.EnableFiscalPrinting;
  lblNCMCode.Visible := AppDetails.EnableFiscalPrinting;
  lblCFOP.Visible := AppDetails.EnableFiscalPrinting;
  txtCFOP.Visible := AppDetails.EnableFiscalPrinting;
  txtCEST.Visible := AppDetails.EnableFiscalPrinting;
  lblCEST.Visible := AppDetails.EnableFiscalPrinting;  

  if AppDetails.EnableFiscalPrinting then
  begin
    cmdUpdateAllPrices.Visible := False;
    pnlControlButtons.Width := pnlControlButtons.Width - cmdUpdateAllPrices.Width;
    cmdLayout.Left := 0;
    cmdOK.Left := cmdLayout.Left + cmdLayout.Width + 4;
    cmdHide.Left := cmdOK.Left + cmdOK.Width + 4;
    //cmdFiscalMenu.Left := cmdLayout.Left - 50;

    txtPrice2.Visible := False;
    txtPrice3.Visible := False;
    txtPrice4.Visible := False;
    txtPrice5.Visible := False;
    txtPrice6.Visible := False;

    lblUnitPrice2Dollar.Visible := False;
    lblUnitPrice3Dollar.Visible := False;
    lblUnitPrice4Dollar.Visible := False;
    lblUnitPrice5Dollar.Visible := False;
    lblUnitPrice6Dollar.Visible := False;

    lblUnitPrice2.Visible := False;
    lblUnitPrice3.Visible := False;
    lblUnitPrice4.Visible := False;
    lblUnitPrice5.Visible := False;
    lblUnitPrice6.Visible := False;
  end;

  FAllowUpdateName := True;
  cmdNextBarcode.Visible := txtBarCode.Text = '';
end;
{******************************************************************************}
procedure TformItemDetail.SetItemDetailBarCode(BarCode: String);
begin
  if (CurrentSelectedTEdit.Focused) then begin
    CurrentSelectedTEdit.Text := CurrentSelectedTEditText;
  end;

  if (BarCode <> '#0') then
    txtBarCode.Text := BarCode;
end;
{******************************************************************************}
function TformItemDetail.ValidateItem : Boolean;
var
  ModalResult: TModalResult;
begin
  Result := True;
  if (dm.qrItemGroups.Fields[0].IsNull) then begin
    pcDetail.ActivePage := tabGeneral;
    ShowMsg(sSelectItemGrp);
    Result := False;
    Exit;
  //akm 5.5.1
  end
  else if (txtAbbrev.Text = '') then begin
    pcDetail.ActivePage := tabGeneral;
    txtAbbrev.SetFocus;
    ShowMsg(sEnterItemName);
    Result := False;
    Exit;
  end
  else if (mmoName.Text = '') then begin
    mmoName.Text := txtAbbrev.Text;
  end;

  if (chkEnterQty.Checked and (StrToCurrDef(txtEnterQty.Text, 0) <= 0)) then begin
    txtEnterQty.SetFocus;
    ShowMsg(sEnterValidQty);
    Result := False;
    Exit;
  end;

  if ((chkCourse.Checked) and (dm.qrCourses.Fields[0].IsNull)) then begin
    pcDetail.ActivePage := tabGeneral;
    ShowMsg(sSelectDefaultCourse);
    Result := False;
  end
  else begin
    pcDetail.ActivePage := tabGeneral; //AKM 3.3.1

  if ((DM.qrItemSize.FieldByName('BaseSize').IsNotNull) and ((DM.qrGeneral2.Eof) or (DM.qrGeneral2.FieldByName('ItemId').IsNull)
  or  (DM.qrGeneral2.FieldByName('ItemId').asInteger = -1))) then
  begin
    cmbSize.SetFocus;
    ShowMsg(sCreateParentItemFirst);
    Result := False;
    Exit;
  end;
    //price 1
    try
      if (not ValidateTxtPrice(txtPrice, GetItemPriceName(1))) then begin
        Result := False;
        Exit;
      end;
      if txtPrice.Text = '' then txtPrice.Text := '0.00';
      if (AppDetails.EnableFiscalPrinting and (StrToCurr(StripCommas(txtPrice.Text)) = 0.00)) then begin
        ShowMsg(sEnterNonZeroPriceForFiscalItem);
        txtPrice.SetFocus;
        Result := False;
        Exit;
      end;

      if StrToCurr(StripCommas(txtPrice.Text)) = 0.00 then begin
        if ShowConf(Format(sConfirmAfterZero, [GetItemPriceName(1)])) <> mrYes then begin
          txtPrice.SetFocus;
          Result := False;
          Exit;
        end;
      end;
    except on e : exception do begin
      //pcDetail.ActivePage := tabGeneral; //AKM 3.3.1
      txtPrice.SetFocus;
      ShowMsg(Format(sEnterValidPriceName,[GetItemPriceName(1)]));
      Result := False;
      Exit;
    end; end;

    //price 2 AKM 3.3.1
    if Not AppDetails.EnableFiscalPrinting then
    if ((not ValidateTxtPrice(txtPrice2, GetItemPriceName(2))) or
        (not ValidateTxtPrice(txtPrice3, GetItemPriceName(3))) or
        (not ValidateTxtPrice(txtPrice4, GetItemPriceName(4))) or
        (not ValidateTxtPrice(txtPrice5, GetItemPriceName(5))) or
        (not ValidateTxtPrice(txtPrice6, GetItemPriceName(6)))) then begin
      Result := False;
      Exit;
    end;

    if (chkOpenPrice.Checked) then begin
      if ((not ValidateTxtPrice(txtOpenPriceMin,sMinimumOpenPrice)) or
        (not ValidateTxtPrice(txtOpenPriceMax,sMaximumOpenPrice))) then begin
        Result := False;
        Exit;
      end;
      if (StrToCurr(StripCommas(txtOpenPriceMin.Text)) > StrToCurr(StripCommas(txtOpenPriceMax.Text))) then begin
        txtOpenPriceMin.SetFocus;
        ShowMsg(sOpenPriceRangeErr);
        Result := False;
        Exit;
      end;
    end;

    if (chkNonStockPrice.Checked) then begin
      if (not ValidateTxtPrice(txtNonStockPrice, sUnitCost)) then begin
        Result := False;
        Exit;
      end;
    end;

    pcDetail.ActivePage := tabLoyalty; //AKM 3.3.1

    if ((not CheckTxtBoxFloat(txtLoyaltyPoints1RatioNum, True, True, sLoyalty1RatioInvalid)) or
        (not CheckTxtBoxFloat(txtLoyaltyPoints1RatioDen, True, False, sLoyalty1RatioInvalid)) or
        (not CheckTxtBoxFloat(txtLoyaltyPoints1Qty, True, True, sLoyalty1QtyInvalid)) or
        (not CheckTxtBoxFloat(txtLoyaltyPoints2RatioNum, True, True, sLoyalty2RatioInvalid)) or
        (not CheckTxtBoxFloat(txtLoyaltyPoints2RatioDen, True, False, sLoyalty2RatioInvalid)) or
        (not CheckTxtBoxFloat(txtLoyaltyPoints2Qty, True, True, sLoyalty2QtyInvalid))) then begin
      Result := False;
      Exit;
    end;

    if (chkNYPricingEnabled.Checked) then begin
      pcDetail.ActivePage := tabNYHappyHour;
      if ((not ValidateTxtPrice(txtNYPriceMin, sNewYorkMinPrice)) or
          (not ValidateTxtPrice(txtNYPriceMax, sNewYorkMaxPrice)) or
          (not ValidateTxtPrice(txtNYIncAmount, sNewYorkIncreaseAmount)) or
          (not ValidateTxtPrice(txtNYDecAmount, sNewYorkDecreaseAcount))) then begin
        Result := False;
        Exit;
      end;

      if (StrToIntDef(txtNYIncPeriod.Text, -1) < 0) then begin
        txtNYIncPeriod.SetFocus;
        ShowMsg(sEnterNewYorkIncreasePeriod);
        Result := False;
        Exit;
      end;

      if (StrToIntDef(txtNYIncUnits.Text, -1) < 0) then begin
        txtNYIncUnits.SetFocus;
        ShowMsg(sEnterNewYorkIncreaseUnit);
        Result := False;
        Exit;
      end;

      if (StrToIntDef(txtNYDecPeriod.Text, -1) < 0) then begin
        txtNYDecPeriod.SetFocus;
        ShowMsg(sEnterNewYorkDecreasePeriod);
        Result := False;
        Exit;
      end;

      if (StrToIntDef(txtNYDecUnits.Text, -1) < 0) then begin
        txtNYDecUnits.SetFocus;
        ShowMsg(sEnterNewYorkDecreaseUnit);
        Result := False;
        Exit;
      end;

      if (StrToCurr(StripCommas(txtNYPriceMin.Text)) > StrToCurr(StripCommas(txtNYPriceMax.Text))) then begin
        txtNYPriceMin.SetFocus;
        ShowMsg(sNewYorkPriceRangeErr);
        Result := False;
        Exit;
      end;
    end;

    pcDetail.ActivePage := tabGeneral; //AKM 3.3.1

    //AKM 3.3.1 - if price 1 not zero, and other (2-6) are zero, ask if want to set to price 1

    if Not AppDetails.EnableFiscalPrinting then
    if (StrToCurr(StripCommas(txtPrice.Text))<>0) and
       (StrToCurr(StripCommas(txtPrice2.Text))=0) and
       (StrToCurr(StripCommas(txtPrice3.Text))=0) and
       (StrToCurr(StripCommas(txtPrice4.Text))=0) and
       (StrToCurr(StripCommas(txtPrice5.Text))=0) and
       (StrToCurr(StripCommas(txtPrice6.Text))=0) then
    begin
      ModalResult := ShowConfWithCancel(Format(sSetAllPricesConfirm, [GetItemPriceName(1)]));
      if (ModalResult = mrYes) then begin
        txtPrice2.Text := txtPrice.Text;
        txtPrice3.Text := txtPrice.Text;
        txtPrice4.Text := txtPrice.Text;
        txtPrice5.Text := txtPrice.Text;
        txtPrice6.Text := txtPrice.Text;
      end
      else if (ModalResult = mrCancel) then begin
        Result := False;
        Exit;
      end;
    end;
  end;
end;
{******************************************************************************}
function TformItemDetail.TryEditItem : Boolean;
var
  sHash: string;
begin
  Result := True;
  try
    dm.tr.StartTransaction;   //Jon 10-07-2003
    with dm.sp do begin
      Unprepare;
      StoredProcName := 'edit_item';
      Prepare;

      ParamByName('itemid').AsInteger := GlbItem.ItemID;
      ParamByName('loginid').AsInteger := GlbLogin.LoginID;
      ParamByName('outletid').AsInteger := AppDetails.OutletID;
      ParamByName('itemgroupid').AsInteger := GlbItemGroup.ItemGroupID;
      ParamByName('itemabbrev').AsString := txtAbbrev.Text; //akm 5.5.1
      ParamByName('itemname').AsString := mmoName.Text;

      ParamByName('itemprice').AsCurrency := StrToCurr(StripCommas(txtPrice.Text));
      ParamByName('itemprice2').AsCurrency := StrToCurr(StripCommas(txtPrice2.Text));
      ParamByName('itemprice3').AsCurrency := StrToCurr(StripCommas(txtPrice3.Text));
      ParamByName('itemprice4').AsCurrency := StrToCurr(StripCommas(txtPrice4.Text));
      ParamByName('itemprice5').AsCurrency := StrToCurr(StripCommas(txtPrice5.Text));
      ParamByName('itemprice6').AsCurrency := StrToCurr(StripCommas(txtPrice6.Text));

      //old design, not used, set to no and zeroes
      BoolAsParam(ParamByName('itembutton'), False);
      ParamByName('specialprice').AsCurrency := 0.00;
      ParamByName('special').AsInteger := 0;
      //ParamByName('happy').AsInteger := 0;
      BoolAsParam(ParamByName('happy'), ChkItemAutoTender.Checked);
      IntegerAsParamIfBool(ParamByName('courseid'), GlbCourse.CourseID, chkCourse.Checked);
      BoolAsParam(ParamByName('priceexcludestax'), ChkPriceExcludesTax.Checked);

      if (chkOpenPrice.Checked) then begin      //Jon 19-7-2001
        ParamByName('openprice').AsInteger := 1;
        ParamByName('openpricemin').AsCurrency := StrToCurr(StripCommas(txtOpenPriceMin.Text));
        ParamByName('openpricemax').AsCurrency := StrToCurr(StripCommas(txtOpenPriceMax.Text));
      end
      else begin
        ParamByName('openprice').AsInteger := 0;
        ParamByName('openpricemin').AsInteger := 0;
        ParamByName('openpricemax').AsInteger := 0;
      end;

      if (chkNonStockPrice.Checked) then begin
        ParamByName('nonstockprice').AsCurrency := StrToCurr(StripCommas(txtNonStockPrice.Text));
      end
      else begin
        ParamByName('nonstockprice').AsCurrency := 0;
      end;

      CurrencyAsParamIfBool(ParamByName('enterqty'), StrTofloatDef(txtEnterQty.Text, 0), (chkEnterQty.Checked and (StrToFloatDef(txtEnterQty.Text, 0) > 0)));
      BoolAsParam(ParamByName('weigheditem'), chkWeighedItem.Checked);
      StringAsParam(ParamByName('barcode'), txtBarcode.Text);

      CurrencyAsParamIfBool(ParamByName('loyaltypoints1rationum'), StrToFloatDef(txtLoyaltyPoints1RatioNum.Text, -1), StrToFloatDef(txtLoyaltyPoints1RatioNum.Text, -1) >= 0);
      CurrencyAsParamIfBool(ParamByName('loyaltypoints1ratioden'), StrToFloatDef(txtLoyaltyPoints1RatioDen.Text, -1), StrToFloatDef(txtLoyaltyPoints1RatioNum.Text, -1) > 0);
      CurrencyAsParamIfBool(ParamByName('loyaltypoints1qty'), StrToFloatDef(txtLoyaltyPoints1Qty.Text, -1), StrToFloatDef(txtLoyaltyPoints1Qty.Text, -1) >= 0);

      CurrencyAsParamIfBool(ParamByName('loyaltypoints2rationum'), StrToFloatDef(txtLoyaltyPoints2RatioNum.Text, -1), StrToFloatDef(txtLoyaltyPoints2RatioNum.Text, -1) >= 0);
      CurrencyAsParamIfBool(ParamByName('loyaltypoints2ratioden'), StrToFloatDef(txtLoyaltyPoints2RatioDen.Text, -1), StrToFloatDef(txtLoyaltyPoints2RatioDen.Text, -1) > 0);
      CurrencyAsParamIfBool(ParamByName('loyaltypoints2qty'), StrToFloatDef(txtLoyaltyPoints2Qty.Text, -1), StrToFloatDef(txtLoyaltyPoints2Qty.Text, -1) >= 0);

      CurrencyAsParamIfBool(ParamByName('nypricemin'), StrToCurrDef(StripCommas(txtNYPriceMin.Text), 0), chkNYPricingEnabled.Checked, False);
      CurrencyAsParamIfBool(ParamByName('nypricemax'), StrToCurrDef(StripCommas(txtNYPriceMax.Text), 0), chkNYPricingEnabled.Checked, False);
      IntegerAsParamIfBool(ParamByName('nyincperiod'), StrToIntDef(txtNYIncPeriod.Text, 0), chkNYPricingEnabled.Checked);
      IntegerAsParamIfBool(ParamByName('nyincunits'), StrToIntDef(txtNYIncUnits.Text, 0), chkNYPricingEnabled.Checked);
      CurrencyAsParamIfBool(ParamByName('nyincamount'), StrToCurrDef(StripCommas(txtNYIncAmount.Text), 0), chkNYPricingEnabled.Checked, False);
      IntegerAsParamIfBool(ParamByName('nydecperiod'), StrToIntDef(txtNYDecPeriod.Text, 0), chkNYPricingEnabled.Checked);
      IntegerAsParamIfBool(ParamByName('nydecunits'), StrToIntDef(txtNYDecUnits.Text, 0), chkNYPricingEnabled.Checked);
      CurrencyAsParamIfBool(ParamByName('nydecamount'), StrToCurrDef(StripCommas(txtNYDecAmount.Text), 0), chkNYPricingEnabled.Checked, False);

      StringAsParam(ParamByName('linkcode'), txtLinkCode.Text);

      StringAsParam(ParamByName('GTIN'), edtGTIN.Text);
      ParamNull(ParamByName('ManufacturedBy'));
      ParamNull(ParamByName('TaxSituation'));
      ParamNull(ParamByName('IATIndicator'));
      ParamByName('IsService').AsInteger := Integer(chkIsService.Checked);
      ParamByName('NEEDTRACE').AsInteger := 0;
      ParamNull(ParamByName('CST_ICMS')); //IntegerAsParam(ParamByName('IMPORTEDFROM'), 0);
      ParamNull(ParamByName('CST_IPI'));
      ParamNull(ParamByName('CST_PIS'));
      ParamNull(ParamByName('CST_COFFINS'));
      ParamNull(ParamByName('CSOSN'));
      ParamNull(ParamByName('CHECKSUM'));

      IntegerAsParam(ParamByName('sizeid'), dm.qrItemSize.FieldByName('Sizeid').AsInteger);
      IntegerAsParamIfBool(ParamByName('parentitemid'), dm.qrGeneral2.FieldByName('itemid').AsInteger, dm.qrGeneral2.FieldByName('itemid').AsInteger > 0);
      ParamByName('ParentItemInSelection').AsBoolean := chkShowParentInSelection.Checked;
      ParamNull(ParamByName('NCMPRODUCTCODE'));
      if AppDetails.EnableFiscalPrinting then
      begin
        StringAsParam(ParamByName('NCMPRODUCTCODE'),txtNCMCode.Text);
        ParamByName('NEEDTRACE').AsInteger := 1;
        if chkManufacturedBy.Checked then
          StringAsParam(ParamByName('ManufacturedBy'), 'P')
        else
          StringAsParam(ParamByName('ManufacturedBy'), 'T');

        case rgTaxSituation.ItemIndex of
          0 : StringAsParam(ParamByName('TaxSituation'), 'T');
          1 : StringAsParam(ParamByName('TaxSituation'), 'S');
          2 : StringAsParam(ParamByName('TaxSituation'), 'II');
          3 : StringAsParam(ParamByName('TaxSituation'), 'NN');
          4 : StringAsParam(ParamByName('TaxSituation'), 'FF');
          5 : StringAsParam(ParamByName('TaxSituation'), 'SI');
          6 : StringAsParam(ParamByName('TaxSituation'), 'SN');
          7 : StringAsParam(ParamByName('TaxSituation'), 'SF');
        end;

        case rgIATIndicator.ItemIndex of
          0 : StringAsParam(ParamByName('IATIndicator'), 'A');
          1 : StringAsParam(ParamByName('IATIndicator'), 'T');
        end;

        StringAsParam(ParamByName('CST_ICMS'), edtCST_ICMS.Text);//IntegerAsParam(ParamByName('IMPORTEDFROM'), rgImportedFrom.ItemIndex);
        StringAsParam(ParamByName('CST_IPI'), edtCST_IPI.Text);
        StringAsParam(ParamByName('CST_PIS'), edtCST_PIS.Text);
        StringAsParam(ParamByName('CST_COFFINS'), edtCST_COFFINS.Text);
        StringAsParam(ParamByName('CSOSN'), edtCSOSN.Text);
        ParamByName('cfop').AsInteger := StrToIntDef(txtCFOP.Text,0);
        IntegerAsParam(ParamByName('cest'), StrToIntDef(txtCEST.Text, 0));


        SWBEncryption.Initialise;
        SWBEncryption.AddInt(GlbItem.ItemID);
        SWBEncryption.AddStr(edtGTIN.Text);
        SWBEncryption.AddStr(txtAbbrev.Text);
        SWBEncryption.AddStr(mmoName.Text);
        SWBEncryption.AddStr(ParamByName('TaxSituation').AsString);
        SWBEncryption.AddStr(FormatFloat('##00.00', GlbItemGroup.SalesTaxPercent));
        SWBEncryption.AddCurrency(ParamByName('itemprice').AsCurrency);

        sHash := SWBEncryption.GetHash;
        StringAsParam(ParamByName('CHECKSUM'), sHash);
      end;

      IntegerAsParam(ParamByName('RemoteOverrideGroupID'),frameRemoteOverrideGroup.CurrentRemoteOverrideGroupID);

      ExecProc;

      if (not FieldByName('errorcode').IsNull) then begin
        Result := False;
        ShowErrorMsg(FieldByName('errorcode').AsInteger);
        dm.tr.Rollback;   //Jon 10-07-2003
        Exit;
      end;

      if (frameItemSharing.ItemSharePricesChanged) then begin
        if (not frameItemSharing.ApplyNewItemSharing(frameItemSharing.CurrentOutletID)) then
          //something went wrong, item changes would be rolled back too since done in same transaction
      end
      else begin
       //  commit changes for ItemDetail changes
         dm.tr.Commit;   //Jon 10-07-2003
      end;

    end;
  except on e : exception do begin
    dm.tr.Rollback;   //Jon 10-07-2003
    Result := False;
    ShowMessage(e.message);
  end; end;
end;
{******************************************************************************}
function TformItemDetail.ValidateTxtPrice(PriceEdit: TEdit; PriceName: String): Boolean;
begin
  try
    if PriceEdit.Text = '' then PriceEdit.Text := '0.00';
    if (StrToCurr(StripCommas(PriceEdit.Text)) < 0) then begin
      PriceEdit.SetFocus;
      ShowMsg(Format(sEnterValidPriceName,[PriceName]));
      Result := False;
      Exit;
    end
    else begin
      Result := True;
    end;
  except on e : exception do begin
    PriceEdit.SetFocus;
    ShowMsg(Format(sEnterValidPriceName,[PriceName]));
    Result := False;
    Exit;
  end; end;
end;
{******************************************************************************}
procedure TformItemDetail.cmdOkClick(Sender: TObject);
begin
  inherited;
  if FParentItemChanged then
  begin
    GlbItem.ParentItemId := dm.qrGeneral2.FieldByName('ItemId').AsInteger;
    GlbItem.ParentName := dm.qrGeneral2.FieldByName('ItemAbbrev').AsString;
  end;

  if (ValidateItem) then begin
    if (TryEditItem) then begin
      if ((AppDetails.PMSType = pmsEvolution) and (AppDetails.EvolutionItemisedInvoicing)) then begin
        EvoSyncItems(eitItem, GlbItem.ItemID);
      end;

      Close;
    end;
  end;
end;
{******************************************************************************}
procedure TformItemDetail.cmdHideClick(Sender: TObject);
begin
  if (cmdOk.Enabled) then begin
    if (ShowConf(sConfirmSave) <> mrYes) then begin
      Exit;
    end;
  end;
  inherited;
end;
{******************************************************************************}
procedure TformItemDetail.mmoNameChange(Sender: TObject);
begin
  cmbCourse.Visible := chkCourse.Checked;

  GlbItemGroup.ItemGroupID := dm.qrItemGroups.FieldByName('itemgroupid').AsInteger;
  GlbCourse.CourseID := dm.qrCourses.FieldByName('courseid').AsInteger;

  //  lblNonStockPrice.Visible := chkNonStockPrice.Checked;
  lblNonStockPriceDollar.Visible := chkNonStockPrice.Checked;
  txtNonStockPrice.Visible := chkNonStockPrice.Checked;

  lblOpenPriceMin.Visible := chkOpenPrice.Checked;         //Jon 19-7-2001
  lblOpenPriceMinDollar.Visible := chkOpenPrice.Checked;   //Jon 19-7-2001
  txtOpenPriceMin.Visible := chkOpenPrice.Checked;         //Jon 19-7-2001
  lblOpenPriceMax.Visible := chkOpenPrice.Checked;         //Jon 19-7-2001
  lblOpenPriceMaxDollar.Visible := chkOpenPrice.Checked;   //Jon 19-7-2001
  txtOpenPriceMax.Visible := chkOpenPrice.Checked;         //Jon 19-7-2001

  lblEnterQty.Visible := chkEnterQty.Checked;
  txtEnterQty.Visible := chkEnterQty.Checked;

  if Not chkWeighedItem.Checked then
    edtUnit.Text := 'UN'
  else
  begin //0 = in Kilos 1 - in 10gm , 2 - in gm
    if AppDetails.WeightedItem in [0,1] then
      edtUnit.Text := 'KG'
    else
      edtUnit.Text := 'GM';
  end;

  cmdOk.Enabled := True;
end;
{******************************************************************************}
procedure TformItemDetail.cmdUpdateAllPricesClick(Sender: TObject);
begin
  inherited;
  txtPrice2.Text := txtPrice.Text;
  txtPrice3.Text := txtPrice.Text;
  txtPrice4.Text := txtPrice.Text;
  txtPrice5.Text := txtPrice.Text;
  txtPrice6.Text := txtPrice.Text;
end;
{******************************************************************************}
procedure TformItemDetail.txtAbbrevEnter(Sender: TObject);
begin
  inherited;
  if (Sender is TEdit) then begin
    CurrentSelectedTEdit := TEdit(Sender);
    CurrentSelectedTEditText := TEdit(Sender).Text;
  end;
end;
{******************************************************************************}

procedure TformItemDetail.rgTaxSituationClick(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.chkManufacturedByClick(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.rgImportedFromClick(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.edtCST_ICMSChange(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.rgIATIndicatorClick(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;


(*function TformItemDetail.RestrictPriceToOneDecimal(Sender: TObject): boolean;
var
  sText : string;
  iPos: integer;
begin
  Result := False;
  if Sender is TEdit then
  begin
    sText := Trim(TEdit(Sender).Text);
    if sText = '' then
      Exit;
    iPos := Pos('.',sText);
    if iPos > 0 then
    begin
      if Length(Copy(sText , iPos+1, Length(sText))) > 0 then
        Result := True;
    end;
  end;
end;

function TformItemDetail.IsBaseItem: boolean;
begin
  Result := False;
  dm.qrItemSize.First;
  while (Not dm.qrItemSize.Eof) do
  begin
    if ((dm.qrItemSize.FieldByName('sizeid').AsInteger = GlbItem.SizeId) and (dm.qrItemSize.FieldByName('basesize').IsNull) ) then
    begin
      Result := True;
      Exit;
    end;
    dm.qrItemSize.Next;
  end;
end;
*)


procedure TformItemDetail.txtNCMCodeChange(Sender: TObject);
begin
  inherited;
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.cmbSizeChange(Sender: TObject);
begin
  inherited;
    with dm.qrGeneral2 do
    begin
      Close;
      SQL.Clear;
      SQL.Add('select distinct ''-1'' itemid , ''' + sCmbSelectParent +  ''' itemabbrev from item i');
      SQL.Add('union');
      SQL.Add('select i.itemid , i.itemabbrev from item i');
      SQL.Add('inner join itemsize s on s.SIZEID = i.SIZEID and s.whendeleted is null');
      SQL.Add('where (i.itemgroupid = :itemgroupid) and (((i.itemid not in (select ii.parentitemid ');
      SQL.Add('from item ii where (ii.WHENDELETED is null) and (ii.SIZEID = :sizeid)))');
      if (GlbItem.ParentItemId > 0) then
        SQL.Add('or (i.itemid in (select ii.itemid from item ii where (ii.WHENDELETED is null) and (ii.PARENTITEMID is null) and (s.SIZEID = :basesize)  and (ii.itemid = :parentitemid) ))');
      SQL.Add('or (i.itemid in (select ii.itemid from item ii where (ii.WHENDELETED is null) and (ii.parentitemid = :itemid))) ');
      SQL.Add(')and (s.SIZEID = :basesize))');
      ParamByName('sizeid').AsInteger := DM.qrItemSize.FieldByName('sizeid').AsInteger;
      ParamByName('basesize').AsInteger := DM.qrItemSize.FieldByName('basesize').AsInteger;
      ParamByName('itemgroupid').AsInteger := DM.qrItemGroups.FieldByName('ItemGroupId').AsInteger;
      ParamByName('itemid').AsInteger := GlbItem.ItemID;
      if (GlbItem.ParentItemId > 0) then
        ParamByName('parentitemid').AsInteger := GlbItem.ParentItemId;

      with KeyLinks do begin
        Clear;
        Add('itemid');
      end;

      Active := True;

      if not (dm.qrGeneral2.Locate('itemid',GlbItem.ParentItemId, [])) then
        KeyFieldByName('itemid').AsInteger := -1;

      lblParentItem.Visible := (RecordCount > 1);
      cmbParentItem.Visible := (RecordCount > 1);
    end;

  mmoNameChange(nil);
end;

procedure TformItemDetail.cmbParentItemChange(Sender: TObject);
var
   itemname, sizeabbrev, s: String;
   slength, trimXchars: Integer;
begin
  inherited;
  FParentItemChanged := True;

  if (dm.qrGeneral2.Active and not dm.qrGeneral2.Eof) and
     (dm.qrItemSize.Active and not dm.qrItemSize.Eof ) and
    (dm.qrGeneral2.FieldByName('itemid').AsInteger <> -1) then begin
    if FAllowUpdateName and ((txtAbbrev.Text = '') or (ShowConf(sUpdateItemName) = mrYes)) then begin

      itemname := dm.qrGeneral2.FieldByName('itemabbrev').AsString;
      sizeabbrev := dm.qrItemSize.FieldByName('sizeabbrev').AsString;

      slength := Length(itemname)+Length(sizeabbrev)+1;
      trimXchars := slength-25;

      s := LeftStr(itemname, (25-trimXchars));

      txtAbbrev.Text := s + '-' + sizeabbrev;
    end;
  end;



  mmoNameChange(nil);
end;

procedure TformItemDetail.FormDestroy(Sender: TObject);
begin
  inherited;
  dm.qrGeneral2.KeyLinks.Clear;
end;

procedure TformItemDetail.chkShowParentInSelectionClick(Sender: TObject);
begin
  inherited;
  mmoNameChange(nil);
end;

procedure TformItemDetail.frameRemoteOverrideGroupcmbRemoteOverrideGroupsChange(
  Sender: TObject);
begin
  inherited;
  frameRemoteOverrideGroup.cmbRemoteOverrideGroupsChange(Sender);
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.frameRemoteOverrideGroupcmdManuallySelectClick(Sender: TObject);
begin
  inherited;
  frameRemoteOverrideGroup.cmdManuallySelectClick(Sender);
  cmdOk.Enabled := True;
end;

procedure TformItemDetail.txtPriceChange(Sender: TObject);
begin
  inherited;

  frameItemSharing.txtPrice1.Text := txtPrice.Text;
  frameItemSharing.txtPrice2.Text := txtPrice2.Text;
  frameItemSharing.txtPrice3.Text := txtPrice3.Text;
  frameItemSharing.txtPrice4.Text := txtPrice4.Text;
  frameItemSharing.txtPrice5.Text := txtPrice5.Text;
  frameItemSharing.txtPrice6.Text := txtPrice6.Text;

  mmoNameChange(Sender);
end;

procedure TformItemDetail.frameItemSharinglstItemSharesSetEditText(
  Sender: TObject; ACol, ARow: Integer; const Value: String);
begin
  inherited;

    frameItemSharing.lstItemSharesSetEditText(Sender, ACol, ARow, Value);
    mmoNameChange(Sender);
end;

procedure TformItemDetail.frameItemSharingtxtPrice1Change(Sender: TObject);
begin
  inherited;

  txtPrice.Text := frameItemSharing.txtPrice1.Text;
  txtPrice2.Text := frameItemSharing.txtPrice2.Text;
  txtPrice3.Text := frameItemSharing.txtPrice3.Text;
  txtPrice4.Text := frameItemSharing.txtPrice4.Text;
  txtPrice5.Text := frameItemSharing.txtPrice5.Text;
  txtPrice6.Text := frameItemSharing.txtPrice6.Text;

    frameItemSharing.txtPrice1Change(Sender);
    mmoNameChange(Sender);
end;

procedure TformItemDetail.frameItemSharingtxtOutletP1Change(
  Sender: TObject);
begin
  inherited;
    frameItemSharing.txtOutletP1Change(Sender);
    mmoNameChange(Sender);
end;

procedure TformItemDetail.pcDetailChange(Sender: TObject);
begin
  inherited;
  if (pcDetail.ActivePage.Name = 'tabAdvPricing') then
    frameItemSharing.DisplayingForm := True
  else
    frameItemSharing.DisplayingForm := False;
end;

procedure TformItemDetail.frameItemSharingcmdDefaultClick(Sender: TObject);
begin
  inherited;
  frameItemSharing.cmdDefaultClick(Sender);
  mmoNameChange(Sender);

end;

procedure TformItemDetail.frameItemSharingcmdDefaultAllClick(
  Sender: TObject);
begin
  inherited;
  frameItemSharing.cmdDefaultAllClick(Sender);
  mmoNameChange(Sender);

end;

procedure TformItemDetail.frameItemSharingcmdDeltaPricesClick(
  Sender: TObject);
begin
  inherited;
  frameItemSharing.cmdDeltaPricesClick(Sender);
  mmoNameChange(Sender);
end;

procedure TformItemDetail.frameItemSharingcmdDeltaAllPricesClick(
  Sender: TObject);
begin
  inherited;
  frameItemSharing.cmdDeltaAllPricesClick(Sender);
  mmoNameChange(Sender);
end;

procedure TformItemDetail.frameItemSharingcmdUpdateAllPricesClick(
  Sender: TObject);
begin
  inherited;
  frameItemSharing.cmdUpdateAllPricesClick(Sender);
  mmoNameChange(Sender);

end;

procedure TformItemDetail.tabAdvPricingEnter(Sender: TObject);
begin
  inherited;
  frameItemSharing.PopulateFrame;
end;

procedure TformItemDetail.cmdNextBarcodeClick(Sender: TObject);
begin
  try
    with dm.qrNextBarcodeForScales do
    begin
      if (SQL.Text = '') then
        SetNextBarcodeForScaleItems;
      StringAsParam(ParamByName('barcodePrefix'),AppDetails.BarCodePrefix);
      Open;
      txtBarCode.Text := FieldAsString(FieldByName('barcode'));
      Close
    end
  except
    txtBarCode.Text := '';
  end;
end;

procedure TformItemDetail.txtBarCodeChange(Sender: TObject);
begin
  cmdNextBarcode.Visible := txtBarCode.Text = '';

  mmoNameChange(Sender);
end;

procedure TformItemDetail.cmdCopyCourseClick(Sender: TObject);
var
  tmpQry: TIB_Query;

begin
  try
    if (ShowConf(sCopyCourse) = mrNo) then
      Exit;

    dm.tr.StartTransaction;
    
    tmpQry := TIB_Query.Create(nil);
    with tmpQry do
    begin
      SQL.Clear;
      SQL.Add('update item i');
      SQL.Add('  set i.courseid = :courseid, i.whenupd = :thenow, i.loginid = :loginid');
      SQL.Add('  where i.itemgroupid = :itemgroupid and i.itemid <> :itemid');
      IntegerAsParamIfBool(ParamByName('courseid'), GlbCourse.CourseID, chkCourse.Checked);
      ParamByName('thenow').AsDateTime := Now();
      ParamByName('loginid').AsInteger := GlbLogin.LoginID;
      ParamByName('itemgroupid').AsInteger := GlbItemGroup.ItemGroupID;
      ParamByName('itemid').AsInteger := GlbItem.ItemID;

      ExecSQL;
    end;

    dm.tr.Commit;
  except on e : exception do begin
    dm.tr.Rollback;
    LogException(e);
  end; end;

end;

end.

