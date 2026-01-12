unit UReports;
{******************************************************************************}
interface
{******************************************************************************}
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Grids, DBGrids, Buttons, ExtCtrls, ComCtrls, DBCtrls, DB,
  Menus, IniFiles, {IvDictio, IvAMulti,
  IvResDllDictionary, IvMulti, IvBinDic,} ShellAPI,
  UTypesConstants, UIntegerList, ImgList, System.ImageList, Vcl.Samples.Spin;
{******************************************************************************}
const
  //Set this if this is being worked on for automatic settings
  Developing = False;
{******************************************************************************}
type
  TReportType = (rptypSales, rptypAccountSales, rptypCourseSales, rptypTerminalSales, rptypStaffSales,
    rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypSalesByDate, rptypSalesHistory, rptypVoids,
    rptypOrders, rptypStaffOrders,rptypStaffOpenPriceItems, rptypMenuItems, rptypModifiers,
    rptypPeople, rptypCashTotals, rptypPeriodEndTransactions, rptypPeriodEndStatement,
    rptypClockIns, rptypAccountSummaries, rptypStaffTips, rptypNoSales, rptypTillSales, rptypDiscountSchemeSales,
    rptypItemModSales, rptypModItems, rptypOrderByBumped, rptypItemByBumped, rptypItemCountByBumped,
    rptypAccountTransactions, rptypComboSales, rptypOrdersSalesVoids, rptypAccountTabSales,
    rptypOpenTables, rptypTableSummary, rptypEventSales, rptypTransList, rptypAccTypeMovements,
    rptypStaffDetails, rptypPerDayCalc);

  (**
    Main page of the Reports Program.
    This is the page where we decide what sort of Report we want,
    the time range of the report, which Group to include,
    and what order the information will be.
    In this page we set up the queries for the reports.
  *)
  TformReports = class(TForm)
    SaveDialog: TSaveDialog;
    pnlReports: TPanel;
    cmdDoReport: TSpeedButton;
    frmRange: TGroupBox;
    cmdFrom: TSpeedButton;
    lblDateFrom: TLabel;
    cmdTo: TSpeedButton;
    lblDateTo: TLabel;
    txtFrom: TEdit;
    txtTo: TEdit;
    frmForB: TGroupBox;
    radFAndB: TRadioButton;
    radFOnly: TRadioButton;
    radBOnly: TRadioButton;
    frmZeros: TGroupBox;
    radExcludeZeros: TRadioButton;
    frmItemGroups: TGroupBox;
    DBGridItemGroups: TDBGrid;
    radGCurrent: TRadioButton;            
    radGDeleted: TRadioButton;
    navItemGroups: TDBNavigator;
    frmItemGroupSelection: TGroupBox;
    radItemGroup: TRadioButton;
    radAllItems: TRadioButton;
    frmGroupSubTotals: TGroupBox;
    ckbGroupGroup: TCheckBox;
    ckbGroupForB: TCheckBox;
    ckbGroupStaff: TCheckBox;
    ckbGroupAccount: TCheckBox;
    ckbGroupOutlet: TCheckBox;
    chkExportFile: TCheckBox;
    frmOrderVoidsBy: TGroupBox;
    frmOrderSalesBy: TGroupBox;
    radGroup: TRadioButton;
    radUnits: TRadioButton;
    radPrice: TRadioButton;
    radGross: TRadioButton;
    radDiscount: TRadioButton;
    radNett: TRadioButton;
    frmOrderOrdersBy: TGroupBox;
    frmAccounts: TGroupBox;
    DBGridAccounts: TDBGrid;
    radACurrent: TRadioButton;
    radAClosed: TRadioButton;
    navAccounts: TDBNavigator;
    fmAccountSelection: TGroupBox;
    frmStaff: TGroupBox;
    DBGridStaffMembers: TDBGrid;
    radSCurrent: TRadioButton;
    radSRemoved: TRadioButton;
    navStaff: TDBNavigator;
    fmStaffSelection: TGroupBox;
    frmDetailsGroupsTotals: TGroupBox;
    radDGTDetails: TRadioButton;
    radDGTGroups: TRadioButton;
    radDGTTotals: TRadioButton;
    radStaffAll: TRadioButton;
    radStaffSelected: TRadioButton;
    radAccountAll: TRadioButton;
    radAccountSelected: TRadioButton;
    radOGroup: TRadioButton;
    radOUnits: TRadioButton;
    radOPrice: TRadioButton;
    radOValue: TRadioButton;
    radVStaff: TRadioButton;
    radVWhen: TRadioButton;
    radVGroup: TRadioButton;
    radVVoided: TRadioButton;
    radVWasted: TRadioButton;
    radVPrice: TRadioButton;
    radVValue: TRadioButton;
    ckbGroupTerminal: TCheckBox;
    frmTerminals: TGroupBox;
    DBGridTerminals: TDBGrid;
    navTerminals: TDBNavigator;
    fmTerminalSelection: TGroupBox;
    radTerminalAll: TRadioButton;
    radTerminalSelected: TRadioButton;
    radTCurrent: TRadioButton;
    radTRemoved: TRadioButton;
    frmOrderMenuBy: TGroupBox;
    radMenuOrderGroup: TRadioButton;
    radMenuOrderItem: TRadioButton;
    radMenuOrderPrice: TRadioButton;
    frmPeopleSelection: TGroupBox;
    pnlPeopleBirthday: TPanel;
    lblPeopleBirthday: TLabel;
    lblPeopleDOB: TLabel;
    cmdFromDOB: TSpeedButton;
    lblPeopleDOBTo: TLabel;
    cmdToDOB: TSpeedButton;
    lblPeopleBirthdayTo: TLabel;
    cmdFromBirthday: TSpeedButton;
    cmdToBirthday: TSpeedButton;
    txtFromBirthday: TEdit;
    txtFromDOB: TEdit;
    txtToDOB: TEdit;
    txtToBirthday: TEdit;
    pnlPeopleWedding: TPanel;
    cmdFromWeddingDate: TSpeedButton;
    lblPeopleWedding: TLabel;
    lblPeopleAnniversary: TLabel;
    cmdToWeddingDate: TSpeedButton;
    lblPeopleAnniversaryTo: TLabel;
    lblPeopleWeddingTo: TLabel;
    cmdFromAnniversary: TSpeedButton;
    cmdToAnniversary: TSpeedButton;
    txtToWeddingDate: TEdit;
    txtFromWeddingDate: TEdit;
    txtFromAnniversary: TEdit;
    txtToAnniversary: TEdit;
    pnlPeople: TPanel;
    pnlPeopleAddress: TPanel;
    lblPeopleStreetname: TLabel;
    lblPeopleSuburb: TLabel;
    lblPeopleCity: TLabel;
    txtStreetName: TEdit;
    txtSuburb: TEdit;
    txtCity: TEdit;
    BevelPeopleSelection3: TBevel;
    BevelPeopleSelection2: TBevel;
    BevelPeopleSelection1: TBevel;
    frmOrderPeopleBy: TGroupBox;
    radPeopleOrderSurname: TRadioButton;
    radPeopleOrderFirstname: TRadioButton;
    frmOrderModifiersBy: TGroupBox;
    radModifiersOrderDefault: TRadioButton;
    radIncludeZeros: TRadioButton;
    radOnlyZeros: TRadioButton;
    radModifiersOrderModifier: TRadioButton;
    radModifiersOrderUnits: TRadioButton;
    radModifiersOrderPrice: TRadioButton;
    radModifiersOrderValue: TRadioButton;
    MainMenu: TMainMenu;
    MenuFile: TMenuItem;
    MenuFileExit: TMenuItem;
    MenuSettings: TMenuItem;
    frmVoidsOrderStyle: TGroupBox;
    radVoidsAllDetails: TRadioButton;
    radVoidsTotalsOnly: TRadioButton;
    ckbGroupSaleCategory: TCheckBox;
    frmSaleCategories: TGroupBox;
    navSaleCategories: TDBNavigator;
    frmSaleCategorySelection: TGroupBox;
    radSaleCategoryAll: TRadioButton;
    radSaleCategorySelected: TRadioButton;
    radSCCurrent: TRadioButton;
    radSCRemoved: TRadioButton;
    chkSalesIncludePriceLevels: TCheckBox;
    frmOrderCashTotalsBy: TGroupBox;
    radCashTotalsOrderCashupID: TRadioButton;
    radCashTotalsOrderTill: TRadioButton;
    frmCashTotalsOrderStyle: TGroupBox;
    ckbGroupTill: TCheckBox;
    frmSalesHistory: TGroupBox;
    lblSalesHistroyNoOfPeriods: TLabel;
    txtSalesHistoryTimeSpanQuantity: TEdit;
    UpDownSalesHistoryTimeSpanQuantity: TUpDown;
    frmSalesHistoryPeriodSpacing: TGroupBox;
    radSalesHistoryHour: TRadioButton;
    frmSalesHistoryPeriodLength: TGroupBox;
    radSalesHistoryLengthHour: TRadioButton;
    frmModifiersOrderedState: TGroupBox;
    radModifiersOrderedOnly: TRadioButton;
    radModifiersNonOrderedOnly: TRadioButton;
    radModifiersOrderedBoth: TRadioButton;
    frmSalesHistoryDailyBetweenTimes: TGroupBox;
    txtSalesHistorySliceStartHour: TEdit;
    lblSalesHistorySliceStartTime: TLabel;
    lblSalesHistorySliceEndTime: TLabel;
    udtxtSalesHistorySliceStartHour: TUpDown;
    txtSalesHistorySliceStartMinute: TEdit;
    udtxtSalesHistorySliceStartMinute: TUpDown;
    txtSalesHistorySliceEndMinute: TEdit;
    udtxtSalesHistorySliceEndMinute: TUpDown;
    udtxtSalesHistorySliceEndHour: TUpDown;
    txtSalesHistorySliceEndHour: TEdit;
    lblSalesHistorySliceHour: TLabel;
    lblSalesHistorySliceMinute: TLabel;
    chkSalesHistoryUsePeriodLength: TCheckBox;
    chkSalesHistoryBetweenTimes: TCheckBox;
    lblSalesHistoryShow: TLabel;
    cmboSalesHistoryShownData: TComboBox;
    radSalesHistory2Hour: TRadioButton;
    radSalesHistoryLength2Hour: TRadioButton;
    radSalesHistory3Hour: TRadioButton;
    radSalesHistory4Hour: TRadioButton;
    radSalesHistoryDay: TRadioButton;
    radSalesHistoryWeek: TRadioButton;
    radSalesHistoryFortnight: TRadioButton;
    radSalesHistoryMonth: TRadioButton;
    radSalesHistoryQuarter: TRadioButton;
    radSalesHistoryYear: TRadioButton;
    radSalesHistoryLength3Hour: TRadioButton;
    radSalesHistoryLength4Hour: TRadioButton;
    radSalesHistoryLengthDay: TRadioButton;
    radSalesHistoryLengthWeek: TRadioButton;
    radSalesHistoryLengthFortnight: TRadioButton;
    radSalesHistoryLengthMonth: TRadioButton;
    radSalesHistoryLengthQuarter: TRadioButton;
    radSalesHistoryLengthYear: TRadioButton;
    chkViewTransactions: TCheckBox;
    lblReportType: TLabel;
    cmbReportType: TComboBox;
    frmPeriodEnds: TGroupBox;
    DBGridPeriodEnds: TDBGrid;
    navPeriodEnds: TDBNavigator;
    chkIncludeZeroBalances: TCheckBox;
    radAccountSelectedType: TRadioButton;
    frmOrderClockInsBy: TGroupBox;
    radClockInsOrderStaffName: TRadioButton;
    radClockInsOrderClockInTime: TRadioButton;
    radClockInsOrderClockOutTime: TRadioButton;
    radVoidsVoidsOnly: TRadioButton;
    radVoidsWastageOnly: TRadioButton;
    frmCourses: TGroupBox;
    radCoursesCurrent: TRadioButton;
    radCoursesClosed: TRadioButton;
    navCourses: TDBNavigator;
    fmCourseSelection: TGroupBox;
    radCourseAll: TRadioButton;
    radCourseSelected: TRadioButton;
    DBGridCourses: TDBGrid;
    chkSalesNettExcludeSalesTax: TCheckBox;
    chkShowAllStaff: TCheckBox;
    radCashTotalsAllDetailsAllTenders: TRadioButton;
    radCashTotalsAllDetails: TRadioButton;
    radCashTotalsTotalsOnly: TRadioButton;
    chkShowLinkCodes: TCheckBox;
    radCashTotalsCashOnly: TRadioButton;
    cmbPeopleCustomer: TComboBox;
    lblPeopleCustomer: TLabel;
    lblPeopleVIP: TLabel;
    cmbPeopleVIP: TComboBox;
    chkPeopleRemoveOptOut: TCheckBox;
    lblPeopleFavoriteItem: TLabel;
    txtPeopleFavoriteItem: TEdit;
    lblPeopleGender: TLabel;
    cmbPeopleGender: TComboBox;
    BevelPeopleSelection4: TBevel;
    pnlPeopleLastWhenInvoiced: TPanel;
    lblPeopleLastWhenInvoiced: TLabel;
    lblPeopleLastWhenInvoicedAfter: TLabel;
    txtPeopleLastWhenInvoicedAfter: TEdit;
    cmdPeopleLastWhenInvoicedAfter: TSpeedButton;
    txtPeopleLastWhenInvoicedBefore: TEdit;
    cmdPeopleLastWhenInvoicedBefore: TSpeedButton;
    lblPeopleLastWhenInvoicedBefore: TLabel;
    chkPeopleValidEmail: TCheckBox;
    pnlOutletsLocations: TPanel;
    pnlOutlets: TPanel;
    lblOutlet: TLabel;
    cmbOutlets: TComboBox;
    pnlRemoteLocations: TPanel;
    lblRemoteLocations: TLabel;
    cmbRemoteLocations: TComboBox;
    txtPeopleNationality: TEdit;
    lblPeopleNationality: TLabel;
    txtPeopleOccupation: TEdit;
    lblPeopleOccupation: TLabel;
    cmbPeopleAccountTypes: TComboBox;
    lblPeopleAccountType: TLabel;
    frmVoidsOrderTypes: TGroupBox;
    radVoidsRefunds: TRadioButton;
    radDriveThrough: TRadioButton;
    radVoidClearitem: TRadioButton;
    radVoidAllTypes: TRadioButton;
    radVoidCancelledTrans: TRadioButton;
    chkOutletBreakdown: TCheckBox;
    chkRemoteLocationBreakdown: TCheckBox;
    ckbGroupRemoteLocation: TCheckBox;
    frmTills: TGroupBox;
    DBGridTills: TDBGrid;
    navTills: TDBNavigator;
    fmTillSelection: TGroupBox;
    radTillAll: TRadioButton;
    radTillSelected: TRadioButton;
    radTillsCurrent: TRadioButton;
    radTillsRemoved: TRadioButton;
    frmDiscountSchemes: TGroupBox;
    DBGridDiscountSchemes: TDBGrid;
    radDiscountSchemesCurrent: TRadioButton;
    radDiscountSchemesRemoved: TRadioButton;
    navDiscountSchemes: TDBNavigator;
    fmDiscountSchemeSelection: TGroupBox;
    radDiscountSchemeAll: TRadioButton;
    radDiscountSchemeSelected: TRadioButton;
    frmModGroups: TGroupBox;
    DBGridModGroups: TDBGrid;
    radModGroupsCurrent: TRadioButton;
    radModGroupsRemoved: TRadioButton;
    navModGroups: TDBNavigator;
    fmModGroupsSelection: TGroupBox;
    radModGroupAll: TRadioButton;
    radModGroupSelected: TRadioButton;
    frmShowModGroups: TGroupBox;
    radShowModGroupsMods: TRadioButton;
    radShowModGroupsAll: TRadioButton;
    DBGridSaleCategories: TDBGrid;
    chkPeopleRemoveInactive: TCheckBox;
    lblPeoplePostCode: TLabel;
    txtPeoplePostCode: TEdit;
    lblPeopleCountry: TLabel;
    txtPeopleCountry: TEdit;
    frmSalePeriods: TGroupBox;
    DBGridSalePeriods: TDBGrid;
    navSalePeriods: TDBNavigator;
    GroupBox2: TGroupBox;
    radSalePeriodAll: TRadioButton;
    radSalePeriodSelected: TRadioButton;
    cmbPeopleRemoteSite: TComboBox;
    lblRemoteSite: TLabel;
    menuHelpUpdates: TMenuItem;
    menuHelp: TMenuItem;
    frmSBOptions: TRadioGroup;
    frmSBDetails: TRadioGroup;
    chkSBTransType: TCheckBox;
    chkSBExtendedDetail: TCheckBox;
    chkRemNumbers: TCheckBox;
    frmCombos: TGroupBox;
    DBGridCombos: TDBGrid;
    navCombos: TDBNavigator;
    GroupBox3: TGroupBox;
    radComboAll: TRadioButton;
    radComboSelected: TRadioButton;
    radComboCurrent: TRadioButton;
    radComboDeleted: TRadioButton;
    frmOrderCombos: TGroupBox;
    radComboOrderCombos: TRadioButton;
    radComboOrderItem: TRadioButton;
    chkShowExtendedCashup: TCheckBox;
    ckbGroupRole: TCheckBox;
    ckbGroupSection: TCheckBox;
    cmdDatesLock: TSpeedButton;
    frmTableOptions: TGroupBox;
    radTableOptionDateRange: TRadioButton;
    radTableOptionIncludeOpen: TRadioButton;
    radTableOptionOpenOnly: TRadioButton;
    frmOrderOpenPriceBy: TGroupBox;
    radOpenPriceStaff: TRadioButton;
    radOpenPriceDate: TRadioButton;
    MenuAbout: TMenuItem;
    TitleImageList: TImageList;
    frmEvents: TGroupBox;
    DBGridEvents: TDBGrid;
    radECurrent: TRadioButton;
    radEClosed: TRadioButton;
    navEvents: TDBNavigator;
    fmEventSelection: TGroupBox;
    radEventAll: TRadioButton;
    radEventSelected: TRadioButton;
    frmTransListOptions: TGroupBox;
    radTransListOptionsCashups: TRadioButton;
    radTransListOptionsClockins: TRadioButton;
    radTransListOptionsStaff: TRadioButton;
    frmOrderTransListBy: TGroupBox;
    radTransListSortByTender: TRadioButton;
    radTransListSortByTrans: TRadioButton;
    radTransListSortByDate: TRadioButton;
    radTransListOptionsAll: TRadioButton;
    frmAccountTypes: TGroupBox;
    DBGridAccountTypes: TDBGrid;
    radATCurrent: TRadioButton;
    radATDeleted: TRadioButton;
    navAccountTypes: TDBNavigator;
    fmAccountTypeSelection: TGroupBox;
    radAccountTypeAll: TRadioButton;
    radAccountTypeSelected: TRadioButton;
    ckbGroupAccountType: TCheckBox;
    radMenuOrderWhenUpd: TRadioButton;
    chkShowNoSale: TCheckBox;
    chkShowLoyaltyReward: TCheckBox;
    chkIncludeStaffOrdersKPI: TCheckBox;
    cmdStaffOrdersKPI: TSpeedButton;
    pnlStaffOrdersKPISetup: TPanel;
    pnlStaffOrdersKPITitle: TPanel;
    lblStaffOrdersKPITitle: TLabel;
    cmdStaffOrdersKPICancel: TSpeedButton;
    pnlStaffOrdersKPI: TPanel;
    sgStaffOrdersKPI: TStringGrid;
    pnlStaffOrdersKPIConfig: TPanel;
    pnlStaffOrdersKPIControls: TPanel;
    DBGridStaffOrdersKPI: TDBGrid;
    txtStaffOrdersKPIName: TEdit;
    lblStaffOrdersKPIName: TLabel;
    rgStaffOrdersKPITotalsN: TRadioGroup;
    rgStaffOrdersKPITotalsD: TRadioGroup;
    lblStaffOrdersKPIConversionRate: TLabel;
    txtStaffOrdersKPIConversionRate: TEdit;
    rgStaffORdersKPIDisplayResultAs: TRadioGroup;
    cmdStaffOrdersKPIAddIG_N: TButton;
    cmdStaffOrdersKPIRemoveIG_N: TButton;
    cmdStaffOrdersKPIRemoveIG_D: TButton;
    cmdStaffOrdersKPIAddIG_D: TButton;
    sgStaffOrdersKPIItemGroupsN: TStringGrid;
    sgStaffOrdersKPIItemGroupsD: TStringGrid;
    cmdStaffOrdersKPINew: TSpeedButton;
    cmdStaffOrdersKPIDelete: TSpeedButton;
    lblStaffOrdersKPIItemGroupsNName: TLabel;
    txtStaffOrdersKPIItemGroupsNName: TEdit;
    lblStaffOrdersKPIItemGroupsDName: TLabel;
    txtStaffOrdersKPIItemGroupsDName: TEdit;
    cmdStaffOrdersKPIOK: TSpeedButton;
    rgStaffOrdersKPIItemGroup: TRadioGroup;
    radSalesHistory15Min: TRadioButton;
    radSalesHistory30Min: TRadioButton;
    radSalesHistoryLength15Min: TRadioButton;
    radSalesHistoryLength30Min: TRadioButton;
    radShowModGroupsOnlyCustom: TRadioButton;
    radShowModGroupsExludeCustom: TRadioButton;
    frmCashupDetailsDisplay: TGroupBox;
    chkCashupReference: TCheckBox;
    chkCashupNotes: TCheckBox;
    frmSections: TGroupBox;
    DBGridSections: TDBGrid;
    navSections: TDBNavigator;
    frmSectionSelection: TGroupBox;
    radSectionAll: TRadioButton;
    radSectionSelected: TRadioButton;
    radSecCurrent: TRadioButton;
    radSecRemoved: TRadioButton;
    frmStaffDetails: TGroupBox;
    Panel1: TPanel;
    radStaffPastStaff: TRadioButton;
    radStaffAllStaff: TRadioButton;
    radStaffCurrentStaff: TRadioButton;
    Panel2: TPanel;
    lblStaffSelectStaff: TLabel;
    cmboBoxStaffLocation: TComboBox;
    Panel3: TPanel;
    lblStaffStartedYearsAgo: TLabel;
    SpinEdtStaffYrsAgo: TSpinEdit;
    lblStaffLocation: TLabel;
    Bevel1: TBevel;
    Bevel2: TBevel;
    frmStaffSalesOptions: TGroupBox;
    radSalesByInvoicingStaff: TRadioButton;
    radSalesByOrderingStaff: TRadioButton;
    procedure FormShow(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure cmdFromClick(Sender: TObject);
    procedure cmdToClick(Sender: TObject);
    procedure radACurrentClick(Sender: TObject);
    procedure radSCurrentClick(Sender: TObject);
    procedure radGCurrentClick(Sender: TObject);
    procedure SaveDialogClose(Sender: TObject);
    procedure DBGridItemGroupsEnter(Sender: TObject);
    procedure navItemGroupsClick(Sender: TObject; Button: TNavigateBtn);
    procedure DBGridStaffMembersCellClick(Column: TColumn);
    procedure cmdDoReportClick(Sender: TObject);
    procedure DBGridAccountsCellClick(Column: TColumn);
    procedure radStaffSelectedClick(Sender: TObject);
    procedure radOGroupClick(Sender: TObject);
    procedure radGroupClick(Sender: TObject);
    procedure radVStaffClick(Sender: TObject);
    procedure radDGTGroupsClick(Sender: TObject);
    procedure radTCurrentClick(Sender: TObject);
    procedure radTillCurrentClick(Sender: TObject);
    procedure DBGridTerminalsCellClick(Column: TColumn);
    procedure DBGridTillsCellClick(Column: TColumn);
    procedure radAllItemsClick(Sender: TObject);
    procedure radItemGroupClick(Sender: TObject);
    procedure cmdFromDOBClick(Sender: TObject);
    procedure cmdToDOBClick(Sender: TObject);
    procedure cmdFromWeddingDateClick(Sender: TObject);
    procedure cmdToWeddingDateClick(Sender: TObject);
    procedure cmdFromBirthdayClick(Sender: TObject);
    procedure cmdToBirthdayClick(Sender: TObject);
    procedure cmdFromAnniversaryClick(Sender: TObject);
    procedure cmdToAnniversaryClick(Sender: TObject);
    procedure radMenuOrderGroupClick(Sender: TObject);
    procedure radModifiersOrderDefaultClick(Sender: TObject);
    procedure MenuFileExitClick(Sender: TObject);
    procedure radVoidsTotalsOnlyClick(Sender: TObject);
    procedure radSCCurrentClick(Sender: TObject);
    procedure DBGridSaleCategoriesCellClick(Column: TColumn);
    procedure MenuSettingsClick(Sender: TObject);
    procedure radCashTotalsOrderCashupIDClick(Sender: TObject);
    procedure chkSalesHistoryUsePeriodLengthClick(Sender: TObject);
    procedure radSalesHistoryHourClick(Sender: TObject);
    procedure radSalesHistoryLengthHourClick(Sender: TObject);
    procedure chkSalesHistoryBetweenTimesClick(Sender: TObject);
    procedure cmbReportTypeChange(Sender: TObject);
    procedure radAccountSelectedTypeClick(Sender: TObject);
    procedure radAccountSelectedClick(Sender: TObject);
    procedure radClockInsOrderStaffNameClick(Sender: TObject);
    procedure radCoursesCurrentClick(Sender: TObject);
    procedure radDiscountSchemesCurrentClick(Sender: TObject);
    procedure radCourseAllClick(Sender: TObject);
    procedure radDiscountSchemeAllClick(Sender: TObject);
    procedure DBGridCoursesCellClick(Column: TColumn);
    procedure DBGridDiscountSchemesCellClick(Column: TColumn);
    procedure navCoursesClick(Sender: TObject; Button: TNavigateBtn);
    procedure navDiscountSchemesClick(Sender: TObject; Button: TNavigateBtn);
    procedure navAccountsBeforeAction(Sender: TObject; Button: TNavigateBtn);
    procedure navSaleCategoriesClick(Sender: TObject; Button: TNavigateBtn);
    procedure navTerminalsClick(Sender: TObject; Button: TNavigateBtn);
    procedure navTillsClick(Sender: TObject; Button: TNavigateBtn);
    procedure navStaffClick(Sender: TObject; Button: TNavigateBtn);
    procedure cmdPeopleLastWhenInvoicedAfterClick(Sender: TObject);
    procedure cmdPeopleLastWhenInvoicedBeforeClick(Sender: TObject);
    procedure cmbOutletsChange(Sender: TObject);
    procedure cmbRemoteLocationsChange(Sender: TObject);
    procedure chkOutletBreakdownClick(Sender: TObject);
    procedure chkRemoteLocationBreakdownClick(Sender: TObject);
    procedure DBGridModGroupsCellClick(Column: TColumn);
    procedure radModGroupsCurrentClick(Sender: TObject);
    procedure radModGroupAllClick(Sender: TObject);
    procedure radShowModGroupsAllClick(Sender: TObject);

    procedure radSalePeriodSelectedClick(Sender: TObject);
    procedure DBGridSalePeriodsCellClick(Column: TColumn);
    procedure menuHelpUpdatesClick(Sender: TObject);
    procedure chkSBExtendedDetailClick(Sender: TObject);
    procedure frmSBOptionsClick(Sender: TObject);
    procedure fmAccountSelectionClick(Sender: TObject);
    procedure radComboCurrentClick(Sender: TObject);
    procedure radComboSelectedClick(Sender: TObject);
    procedure radComboOrderItemClick(Sender: TObject);
    procedure cmdDatesLockClick(Sender: TObject);
    procedure MenuAboutClick(Sender: TObject);
    procedure radECurrentClick(Sender: TObject);
    procedure DBGridEventsCellClick(Column: TColumn);
    procedure navEventsBeforeAction(Sender: TObject; Button: TNavigateBtn);
    procedure radEventAllClick(Sender: TObject);
    procedure DBGridCombosCellClick(Column: TColumn);
    procedure radTransListSortByTenderClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure navAccountTypesBeforeAction(Sender: TObject;
      Button: TNavigateBtn);
    procedure DBGridAccountTypesCellClick(Column: TColumn);
    procedure cmdStaffOrdersKPIClick(Sender: TObject);
    procedure cmdStaffOrdersKPICancelClick(Sender: TObject);
    procedure cmdStaffOrdersKPIAddIG_NClick(Sender: TObject);
    procedure cmdStaffOrdersKPIRemoveIG_NClick(Sender: TObject);
    procedure cmdStaffOrdersKPIAddIG_DClick(Sender: TObject);
    procedure cmdStaffOrdersKPIRemoveIG_DClick(Sender: TObject);
    procedure sgStaffOrdersKPIClick(Sender: TObject);
    procedure txtStaffOrdersKPINameChange(Sender: TObject);
    procedure cmdStaffOrdersKPINewClick(Sender: TObject);
    procedure cmdStaffOrdersKPIDeleteClick(Sender: TObject);
    procedure cmdStaffOrdersKPIOKClick(Sender: TObject);
    procedure chkIncludeStaffOrdersKPIClick(Sender: TObject);
    procedure rgStaffOrdersKPIItemGroupClick(Sender: TObject);
    procedure DBGridSectionsCellClick(Column: TColumn);
    procedure radSecCurrentClick(Sender: TObject);
    procedure cmbPeopleCustomerChange(Sender: TObject);
    procedure SpinEdtStaffYrsAgoChange(Sender: TObject);
  private
    FFromTime, FToTime : TDateTime;
    FFromDOB, FToDOB : TDateTime;
    FFromBirthDay, FFromBirthMonth, FToBirthDay, FToBirthMonth : Integer;
    FFromWeddingDate, FToWeddingDate : TDateTime;
    FLastWhenInvoicedAfter, FLastWhenInvoicedBefore: TDateTime;
    FFromAnniversaryDay, FFromAnniversaryMonth, FToAnniversaryDay, FToAnniversaryMonth : Integer;

    ReportType: TReportType;

    FOutletList, FRemLocsList, FPeopleAcctTypes, FPeopleRemSiteList: TIntegerList;

    LoggedInAlready: Boolean;

    PopulatingFields: Boolean;

    procedure SetDefaultTime;
    function CheckDate : Boolean;

    procedure ShowSalesReport;
    procedure ShowOrdersReport;
    procedure ShowVoidsReport;
    procedure ShowAllStaffOrders;
    procedure ShowAccountSalesReport;
    procedure ShowCourseSalesReport;
    procedure ShowDiscountSchemeSalesReport;
    procedure ShowTerminalSalesReport;
    procedure ShowTillSalesReport;
    procedure ShowMenuItemsReport;
    procedure ShowPeopleReport;
    procedure ShowModifiersReport;
    procedure ShowSalesHistoryReport;
    procedure ShowStaffSalesReport;
    procedure ShowSaleCategorySalesReport;
    procedure ShowSectionSalesReport;
    procedure ShowSalePeriodSalesReport; // 12.03
    procedure ShowCashTotalsReport;
    procedure ShowSalesByDateReport;
    procedure ShowAccountPETransReport;
    procedure ShowAccountPEStatementReport;
    procedure ShowClockInsReport;
    procedure ShowAccountSummariesReport;
    procedure ShowStaffTipsReport;
    procedure ShowNoSalesReport;
    procedure ShowItemModSalesReport;
    procedure ShowModItemsReport;
    procedure ShowOrderByBumpedReport;
    procedure ShowItemByBumpedReport;
    procedure ShowItemCountByBumpedReport;
    procedure ShowAccountTransactionReport;
    procedure ShowOrdersSalesVoidsReport;
    procedure ShowAccountTabSalesReport;
    procedure ShowOpenTablesReport;
    procedure ShowTableSummaryReport;
    procedure ShowComboSalesReport;
    procedure ShowEventSalesReport;
    procedure ShowAllStaffOpenPriceOrders;
    procedure ShowTransListReport;
    procedure ShowAccTypeMovementsReport;
    procedure ShowStaffDetailsReport;
    procedure ShowPerDayCalcReport;

    procedure ChangeReportType;
    procedure EnableGroupSubTotalCheckBoxes(B: Boolean);
    procedure CreateHistoryTimes;

    procedure SetComboCaptions;
    procedure SetDBGridCaptions;

    function GetSelectedIDString(aDBGrid: TDBGrid; aIDFieldName: string): string;
    procedure LoadStaffOrderKPI;

    function GetSelectedSalesHistoryPeriodRadioButton: TRadioButton;
    function GetSelectedSalesHistoryLengthRadioButton: TRadioButton;
    procedure EnableHistoryBetweenTimesCheckBox;
  public
    property FromTime: TDateTime read FFromTime;
    property ToTime: TDateTime read FToTime;

  end;

procedure ApplicationInitialization;

var
  formReports: TformReports;
  bLanguageChanged: Boolean;

  Title1, Title2, Title3, OrdersHeader, VoidsHeader: string;
  Title3Array: array [0..3] of string;
  Title3ArrayCount: Integer;
{******************************************************************************}
implementation
{$R *.DFM}
{******************************************************************************}
uses
  UAppDetails, UDM, LMain, USelectDateTime, USelectDayAndMonth, USettings,
  UQRSales, UQROrders, UQRVoids, UQRAccountSales, UQRCourseSales, UQRAllStaffOrders,
  UQRTerminalSales, UQRMenuItems, UQRPeople, UQRModifiers, UQRSalesHistory, UQRStaffSales,
  UQRSaleCategorySales, UQRCashTotals, UQRSalesByDate, UQRAccountPETrans,
  UQRAccountPEMailout, UQRClockIns, UQRAccountSummary, UQRStaffTips, UQRNoSales,
  UQRTillSales, UQRDiscountSchemeSales, UQRItemModSales, UQRModItems, uResources,
  UQROrdByBumped, UQRItemByBumped, UQRItemCountByBumped, UQRSalePeriodSales,
  UQRAccountTransaction, UQRComboSales, UQRComboItemSales, UQROrdersSalesVoids,
  UQRAccountTabSales, UQROpenTables, UQRTableSummary, UQRStaffOpenPrice,
  UAboutBox, UQREventSales, UQRTransactionsListing, ULogonStaff,
  UQRAccTypeMovements, DateUtils, UQRSectionSales, Math, UQRStaffDetails, UQRPerDayCalc;
{******************************************************************************}
procedure TformReports.FormShow(Sender: TObject);
begin
  txtFrom.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime);
  txtTo.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);
  pnlStaffOrdersKPISetup.Visible := False;
end;
{******************************************************************************}
procedure TformReports.FormCreate(Sender: TObject);
begin
  FOutletList := TIntegerList.Create;
  FRemLocsList := TIntegerList.Create;
  FPeopleAcctTypes := TIntegerList.Create;
  FPeopleRemSiteList := TIntegerList.Create;

  if (Screen.Width > 800) then begin //RPC screen res is 800 by 600 or 1024 by 768
    ClientWidth := ((792 * Screen.PixelsPerInch) div 96);
    ClientHeight := ((524 * Screen.PixelsPerInch) div 96);
  end
  else
  begin
    Width := ((799 * Screen.PixelsPerInch) div 96);
    Height := ((568 * Screen.PixelsPerInch) div 96);
  end;

  //Set the grid datasources to their respective datasources. This is in case it is accidentally cleared in the dfm by the stupid datamodule delphi bug.
  DBGridSaleCategories.DataSource := dm.dsSaleCategories;
  navSaleCategories.DataSource := dm.dsSaleCategories;
  DBGridSections.DataSource := dm.dsSections;
  navSections.DataSource := dm.dsSections;
  DBGridItemGroups.DataSource := dm.dsItemGroups;
  DBGridStaffOrdersKPI.DataSource := dm.dsItemGroups;
  navItemGroups.DataSource := dm.dsItemGroups;
  DBGridStaffMembers.DataSource := dm.dsStaff;
  navStaff.DataSource := dm.dsStaff;
  DBGridTerminals.DataSource := dm.dsTerminals;
  navTerminals.DataSource := dm.dsTerminals;
  DBGridPeriodEnds.DataSource := dm.dsAccountPEs;
  navPeriodEnds.DataSource := dm.dsAccountPEs;
  DBGridCourses.DataSource := dm.dsCourses;
  navCourses.DataSource := dm.dsCourses;
  DBGridTills.DataSource := dm.dsTills;
  navTills.DataSource := dm.dsTills;
  DBGridDiscountSchemes.DataSource := dm.dsDiscountSchemes;
  navDiscountSchemes.DataSource := dm.dsDiscountSchemes;
  DBGridModGroups.DataSource := dm.dsModGroups;
  navModGroups.DataSource := dm.dsModGroups;
  DBGridSalePeriods.DataSource := dm.dsSalePeriods;
  navSalePeriods.DataSource := dm.dsSalePeriods;
  DBGridAccounts.DataSource := dm.dsAccountTypes;
  fmAccountSelectionClick(sender);
  navAccounts.DataSource := dm.dsAccounts;
  DBGridCombos.DataSource := dm.dsCombos;
  navCombos.DataSource := dm.dsCombos;
  DBGridEvents.DataSource := dm.dsEvents;
  DBGridAccountTypes.DataSource := dm.dsAccountTypes;

  cmbOutlets.Clear;
  FOutletList.Clear;

  menuHelpUpdates.Enabled := AppDetails.UpdatePath <> '';

  cmbOutlets.Items.Add(sAllOutlets);
  FOutletList.Add(-1);//sinu added for localisation

  with dm.qrOutlets do begin
    First;
    while (not EOF) do
    begin
      cmbOutlets.Items.Add(FieldByName('outletname').AsString);
      FOutletList.Add(FieldByName('outletid').AsInteger);  //sinu
      Next;
    end;
  end;

  if (cmbOutlets.Items.Count > 2) then begin
    cmbOutlets.ItemIndex := 0;
  end
  else begin
    cmbOutlets.ItemIndex := 1;
  end;

  if cmbOutlets.ItemIndex > 0 then
  begin
    dm.CurrentOutletID := FOutletList.Get(cmbOutlets.ItemIndex);
  end
  else
  begin
    dm.CurrentOutletID := -1;
  end;

  cmbRemoteLocations.Clear;
  FRemLocsList.Clear; //sinu

  cmbRemoteLocations.Items.Add(sAllLoc);//, TObject(-1));
  FRemLocsList.Add(-1);
  cmbRemoteLocations.Items.Add(sOnlyThisLoc);//, TObject(0));
  FRemLocsList.Add(0);

  with dm.qrRemoteLocations do begin
    Open;
    First;
    while (not EOF) do begin
      cmbRemoteLocations.Items.Add(FieldByName('remotelocationname').AsString); //, TObject(FieldByName('remotelocationid').AsInteger));
      FRemLocsList.Add(FieldByName('remotelocationid').AsInteger);//sinu
      Next;
    end;
    Close;
  end;
  with dm.qrSys do begin
    Open;
    First;
    AppDetails.CurrencyType := FieldByName('currencytype').AsString;
    AppDetails.DefaultStartHour := FieldByName('starthour').AsInteger;
    LoggedInAlready := FieldAsBool(FieldByName('avoidpincheck'));
    Close;
  end;
  if (cmbRemoteLocations.Items.Count >= 2) then begin
    cmbRemoteLocations.ItemIndex := 0;
  end
  else begin
    cmbRemoteLocations.ItemIndex := 1;
  end;
  cmbRemoteLocationsChange(nil);

  cmbPeopleAccountTypes.Clear;
  FPeopleAcctTypes.Clear;//sinu

  cmbPeopleAccountTypes.Items.Add(sAllAcctTypes); //, TObject(-1));
  FPeopleAcctTypes.Add(-1);//sinu

  with dm.qrAccountTypes do begin
//    Open;
    First;
    while (not EOF) do begin
      cmbPeopleAccountTypes.Items.Add(FieldByName('accounttype').AsString); //, TObject(FieldByName('accounttypeid').AsInteger));
      FPeopleAcctTypes.Add(FieldByName('accounttypeid').AsInteger); //sinu
      Next;
    end;
//    Close;
  end;
  cmbPeopleAccountTypes.ItemIndex := 0;


// GIH Remote Location filter
  cmbPeopleRemoteSite.Clear;
  FPeopleRemSiteList.Clear;
  with dm.qrRemoteLocations do
  begin
    Open;   
    First;
    if (RecordCount > 1) then
    begin
      cmbPeopleRemoteSite.Items.Add(sAllPeople);
      FPeopleRemSiteList.Add(-1);
    end;
    while (not EOF) do
    begin
      cmbPeopleRemoteSite.Items.Add(FieldByName('remotelocationname').AsString);
      FPeopleRemSiteList.Add(FieldByName('remotelocationid').AsInteger);
      Next;
    end;  
    Close;
  end;
  if (cmbPeopleRemoteSite.Items.Count > 0) then begin
    cmbPeopleRemoteSite.ItemIndex := 0;
  end
  else begin
    cmbPeopleRemoteSite.Visible := False;
    lblRemoteSite.Visible := False;
  end;


  {sinu start}
  SetComboCaptions;
  {sinu end}

  SetDefaultTime;

  FFromDOB := 0;
  FToDOB := 0;
  FFromBirthDay := 0;
  FFromBirthMonth := 0;
  FToBirthDay := 0;
  FToBirthMonth := 0;
  FFromWeddingDate := 0;
  FToWeddingDate := 0;
  FFromAnniversaryDay := 0;
  FFromAnniversaryMonth := 0;
  FToAnniversaryDay := 0;
  FToAnniversaryMonth := 0;
  FLastWhenInvoicedAfter := 0;
  FLastWhenInvoicedBefore := 0;

  cmbReportType.ItemIndex := 18;     //Sales
  chkSalesNettExcludeSalesTax.Checked := AppDetails.NettExcludesSalesTaxDefault;

  cmbPeopleCustomer.ItemIndex := 0;
  cmbPeopleVIP.ItemIndex := 0;
  cmbPeopleGender.ItemIndex := 0;

  PopulatingFields := False;
  chkIncludeStaffOrdersKPI.Checked := AppDetails.IncludeStaffOrdersKPI;
  sgStaffOrdersKPI.Cells[0,0] := sStaffOrderKPIs;
  LoadStaffOrderKPI;

  sgStaffOrdersKPIItemGroupsN.ColWidths[0] := 0;
  sgStaffOrdersKPIItemGroupsN.ColWidths[1] := sgStaffOrdersKPIItemGroupsN.Width-30;
  sgStaffOrdersKPIItemGroupsD.ColWidths[0] := 0;
  sgStaffOrdersKPIItemGroupsD.ColWidths[1] := sgStaffOrdersKPIItemGroupsD.Width-30;

  cmbReportTypeChange(Self);

  UpDownSalesHistoryTimeSpanQuantity.Max := SALEHIST_MAXPD;
  txtSalesHistoryTimeSpanQuantity.Hint := Format(sSalesHistRepNoOfPeriodHint, [SALEHIST_MAXPDPRINT, SALEHIST_MAXPD]);
  UpDownSalesHistoryTimeSpanQuantity.Hint := txtSalesHistoryTimeSpanQuantity.Hint;

  // STAFF DETAILS
  cmboBoxStaffLocation.Items.Clear;
  cmboBoxStaffLocation.Items.Add('ALL');

  with dm.qrStaffDetailsTemp do
  begin
    Close;
    UnPrepare;
    SQL.Clear;
	  SQL.Add('select distinct l.remotelocationname');
	  SQL.Add('from staff s');
	  SQL.Add('left join crol r on s.crolid = r.crolid');
	  SQL.Add('left join remoteoverridegroup g on s.remoteoverridegroupid = g.remoteoverridegroupid and g.overridetaskstaff = 1');
	  SQL.Add('left join remoteoverridegrouploc gl on g.remoteoverridegroupid = gl.remoteoverridegroupid and gl.whendeleted is null');
	  SQL.Add('left join remotelocation l on gl.remotelocationid = l.remotelocationid');
	  SQL.Add('where s.whenend is null and l.remotelocationname is not NULL');
    SQL.Add('order by l.remotelocationname');
    Open;

    while not EOF do
    begin
      cmboBoxStaffLocation.Items.Add(Trim(FieldByName('remotelocationname').AsString));
      Next;
    end;
    Close;
  end;
  cmboBoxStaffLocation.ItemIndex := 0;
  SpinEdtStaffYrsAgo.MinValue := 0;
end;
{******************************************************************************}
procedure TformReports.MenuFileExitClick(Sender: TObject);
begin
  Close;
end;
{******************************************************************************}
procedure TformReports.MenuSettingsClick(Sender: TObject);
begin
  if (ChangeSettings) then begin
    AppDetails.SaveDetails;

  //ResDllDictionary.Language := AppDetails.Language;
    SetDefaultTime;
    txtFrom.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime);
    txtTo.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    dm.GetItemGroups;
  end;
end;
{******************************************************************************}
procedure TformReports.SetDefaultTime;
var
  I: Integer;
  TheDateTime : TDateTime;

  TheTime : TTime;
  WeekDayWanted : string;

  TheDay : Word;
  TheMonth : Word;
  TheYear : Word;

  FromDay : Word;
  FromMonth : Word;
  FromYear : Word;

  ToDay : Word;
  ToMonth : Word;
  ToYear : Word;


begin
  TheDateTime := Now;
  if (Developing) then begin
    FFromTime := (36526-365);
    FToTime := 36526;
  end
  else begin
    if (AppDetails.DefaultTimePeriod = 1) then begin
      FFromTime := TheDateTime - 1;
      FToTime := TheDateTime;
    end
    else begin
      DecodeDate(TheDateTime,TheYear,TheMonth,TheDay);
      TheTime := EncodeTime(AppDetails.DefaultTimeHour, AppDetails.DefaultTimeMinute,0,0);
      FToTime := EncodeDate(TheYear,TheMonth,TheDay) + TheTime;
      I := 0;
      while ((FToTime > TheDateTime ) and (I < 7)) do begin
        FToTime := FToTime - 1;
        Inc(I);
      end;

      if (AppDetails.DefaultTimePeriod = 2) then begin
        FFromTime := FToTime - 1;
      end
      else if (AppDetails.DefaultTimePeriod = 3) then begin
        case AppDetails.DefaultTimeWeekStarts of
          1: WeekDayWanted := 'Sun';
          2: WeekDayWanted := 'Mon';
          3: WeekDayWanted := 'Tue';
          4: WeekDayWanted := 'Wed';
          5: WeekDayWanted := 'Thu';
          6: WeekDayWanted := 'Fri';
          7: WeekDayWanted := 'Sat';
        end;
        I := 0;
        while ((FormatDateTime('ddd',FToTime) <> WeekDayWanted) and (I < 10)) do begin
          FToTime := FToTime - 1;
          Inc(I);
        end;
        FFromTime := FToTime - 7;
      end
      else if (AppDetails.DefaultTimePeriod = 4) then begin
        DecodeDate(FToTime,ToYear,ToMonth,ToDay);
        FToTime := EncodeDate(ToYear,ToMonth,1) + TheTime;
        FFromTime := FToTime - 15;  //To be in last month
        DecodeDate(FFromTime,FromYear,FromMonth,FromDay);
        FFromTime := EncodeDate(FromYear,FromMonth,1) + TheTime;
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdFromClick(Sender: TObject);
var
  aBeforeFrom: TDateTime;

begin
  aBeforeFrom := FFromTime;
  if SelectDateTime(FFromTime) then begin
    txtFrom.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FFromTime);
    if (cmdDatesLock.Down) then
    begin
      FToTime := FToTime - aBeforeFrom + FFromTime;
      txtTo.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FToTime);
    end;

    if (ReportType in [rptypPeriodEndTransactions, rptypPeriodEndStatement]) then begin
      dm.GetPeriodEnds(FFromTime - 60, FToTime + 60);
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdToClick(Sender: TObject);
begin
  if SelectDateTime(FToTime) then begin
    txtTo.Text := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    if (ReportType in [rptypPeriodEndTransactions, rptypPeriodEndStatement]) then begin
      dm.GetPeriodEnds(FFromTime - 60, FToTime + 60);
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.radGCurrentClick(Sender: TObject);
begin
  dm.ItemGroupsCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radACurrentClick(Sender: TObject);
begin
  dm.AccountsCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radSCurrentClick(Sender: TObject);
begin
  dm.StaffCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.SaveDialogClose(Sender: TObject);
begin
  case SaveDialog.FilterIndex of
    1: SaveDialog.DefaultExt := 'txt';
  else
    SaveDialog.DefaultExt := '';
  end;
end;
{******************************************************************************}
procedure TformReports.DBGridItemGroupsEnter(Sender: TObject);
begin
  radItemGroup.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navItemGroupsClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radItemGroup.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridAccountsCellClick(Column: TColumn);
begin
  if (radAccountAll.Checked) then begin
    radAccountSelected.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.navAccountsBeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
  if (radAccountAll.Checked) then begin
    radAccountSelected.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.DBGridCoursesCellClick(Column: TColumn);
begin
  radCourseSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridDiscountSchemesCellClick(Column: TColumn);
begin
  radDiscountSchemeSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navCoursesClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radCourseSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navDiscountSchemesClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radDiscountSchemeSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridTerminalsCellClick(Column: TColumn);
begin
  radTerminalSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridTillsCellClick(Column: TColumn);
begin
  radTillSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navTerminalsClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radTerminalSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navTillsClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radTillSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridSaleCategoriesCellClick(Column: TColumn);
begin
  radSaleCategorySelected.Checked := True;
  radComboSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navSaleCategoriesClick(Sender: TObject; Button: TNavigateBtn);
begin
  radSalePeriodSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridStaffMembersCellClick(Column: TColumn);
begin
  radStaffSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.navStaffClick(Sender: TObject;
  Button: TNavigateBtn);
begin
  radStaffSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.cmdDoReportClick(Sender: TObject);
begin
  cmdDoReport.Cursor := crHourGlass;
  Application.ProcessMessages;

  try
    case ReportType of
      rptypSales: ShowSalesReport;
      rptypOrders: ShowOrdersReport;
      rptypComboSales : ShowComboSalesReport;
      rptypVoids: ShowVoidsReport;
      rptypStaffOrders: ShowAllStaffOrders;
      rptypStaffOpenPriceItems : ShowAllSTaffOpenPriceOrders;
      rptypAccountSales: ShowAccountSalesReport;
      rptypCourseSales: ShowCourseSalesReport;
      rptypTerminalSales: ShowTerminalSalesReport;
      rptypMenuItems: ShowMenuItemsReport;
      rptypPeople: ShowPeopleReport;
      rptypModifiers: ShowModifiersReport;
      rptypSalesHistory: ShowSalesHistoryReport;
      rptypStaffSales: ShowStaffSalesReport;
      rptypSaleCategorySales: ShowSaleCategorySalesReport;
      rptypSectionSales: ShowSectionSalesReport;     
      rptypSalePeriodSales : ShowSalePeriodSalesReport;
      rptypCashTotals: ShowCashTotalsReport;
      rptypSalesByDate: ShowSalesByDateReport;
      rptypPeriodEndTransactions: ShowAccountPETransReport;
      rptypPeriodEndStatement: ShowAccountPEStatementReport;
      rptypClockIns: ShowClockInsReport;
      rptypAccountSummaries: ShowAccountSummariesReport;
      rptypStaffTips: ShowStaffTipsReport;
      rptypNoSales: ShowNoSalesReport;
      rptypTillSales: ShowTillSalesReport;
      rptypDiscountSchemeSales: ShowDiscountSchemeSalesReport;
      rptypItemModSales: ShowItemModSalesReport;
      rptypModItems: ShowModItemsReport;
      rptypOrderByBumped: ShowOrderByBumpedReport;
      rptypItemByBumped : ShowItemByBumpedReport;
      rptypItemCountByBumped : ShowItemCountByBumpedReport;
      rptypAccountTransactions : ShowAccountTransactionReport;
      rptypOrdersSalesVoids : ShowOrdersSalesVoidsReport;
      rptypAccountTabSales : ShowAccountTabSalesReport;
      rptypOpenTables : ShowOpenTablesReport;
      rptypTableSummary : ShowTableSummaryReport;
      rptypEventSales : ShowEventSalesReport;
      rptypTransList : ShowTransListReport;
      rptypAccTypeMovements : ShowAccTypeMovementsReport;
      rptypStaffDetails : ShowStaffDetailsReport;
      rptypPerDayCalc : ShowPerDayCalcReport;
    end;
  finally
    cmdDoReport.Cursor := crDefault;
    Application.ProcessMessages;
  end;
end;
{******************************************************************************}
procedure TformReports.cmbReportTypeChange(Sender: TObject);
begin
  case (cmbReportType.ItemIndex) of
    0: ReportType := rptypAccountSummaries;
    1: ReportType := rptypAccountTabSales;
    2: ReportType := rptypAccountTransactions;
    3: ReportType := rptypAccTypeMovements;
    4: ReportType := rptypCashTotals;
    5: ReportType := rptypClockIns;
    6: ReportType := rptypMenuItems;
    7: ReportType := rptypModifiers;
    8: ReportType := rptypModItems;
    9: ReportType := rptypNoSales;
    10: ReportType := rptypOrders;
    11: ReportType := rptypStaffOrders;
    12: ReportType := rptypStaffOpenPriceItems;
    13: ReportType := rptypOrdersSalesVoids;
    14: ReportType := rptypPeople;
    15: ReportType := rptypPerDayCalc;
    16: ReportType := rptypPeriodEndStatement;
    17: ReportType := rptypPeriodEndTransactions;
    18: ReportType := rptypSales;
    19: ReportType := rptypAccountSales;
    20: ReportType := rptypComboSales;
    21: ReportType := rptypCourseSales;
    22: ReportType := rptypDiscountSchemeSales;
    23: ReportType := rptypEventSales;
    24: ReportType := rptypItemModSales;
    25: ReportType := rptypSaleCategorySales;
    26: ReportType := rptypSectionSales;
    27: ReportType := rptypSalePeriodSales;
    28: ReportType := rptypStaffSales;
    29: ReportType := rptypTerminalSales;
    30: ReportType := rptypTillSales;
    31: ReportType := rptypSalesByDate;
    32: ReportType := rptypSalesHistory;
    33: ReportType := rptypStaffTips;
    34: ReportType := rptypStaffDetails;
    35: ReportType := rptypOpenTables;
    36: ReportType := rptypTableSummary;
    37: ReportType := rptypTransList;
    38: ReportType := rptypVoids;
//    35: ReportType := rptypOrderByBumped;
//    36: ReportType := rptypItemByBumped;
//    37: ReportType := rptypItemCountByBumped;
  end;

  ChangeReportType;
end;
{******************************************************************************}
procedure TformReports.ChangeReportType;
var
  SubTotalsCheckBoxPos: Integer;

  procedure PositionGroupBox(var GroupBox: TGroupBox; Left, Top: Integer);
  begin
    GroupBox.Visible := GroupBox.Enabled;
    GroupBox.Left := ((Left * Screen.PixelsPerInch) div 96);
    GroupBox.Top := ((Top * Screen.PixelsPerInch) div 96);
  end;

  procedure PositionRadioGroup(var RadioGroup: TRadioGroup; Left, Top: Integer);
  begin
    RadioGroup.Visible := RadioGroup.Enabled;
    RadioGroup.Left := ((Left * Screen.PixelsPerInch) div 96);
    RadioGroup.Top := ((Top * Screen.PixelsPerInch) div 96);
  end;

  procedure PositionCheckBox(var CheckBox: TCheckBox; Left, Top: Integer);
  begin
    CheckBox.Visible := CheckBox.Enabled;
    CheckBox.Left := ((Left * Screen.PixelsPerInch) div 96);
    CheckBox.Top := ((Top * Screen.PixelsPerInch) div 96);
  end;

  procedure PositionOrderByGroupBox(var GroupBox: TGroupBox);
  begin
    PositionGroupBox(GroupBox, 588, 8);
  end;

  procedure ShowGroupSubTotalsItems(CheckBox: TCheckBox);
  begin
    Inc(SubTotalsCheckBoxPos);
    CheckBox.Visible := True;
    CheckBox.Left := 8;
    CheckBox.Top := ((SubTotalsCheckBoxPos * 16 * Screen.PixelsPerInch) div 96);
    CheckBox.TabOrder := SubTotalsCheckBoxPos - 1;
  end;

begin
  frmStaffSalesOptions.Enabled := (ReportType in [rptypStaffSales]);
  frmStaffSalesOptions.Visible := frmStaffSalesOptions.Enabled;

  frmStaffDetails.Enabled := (ReportType in [rptypStaffDetails]);
  frmStaffDetails.Visible := frmStaffDetails.Enabled;
  frmStaffDetails.BringToFront;

  frmRange.Enabled := (ReportType in [rptypSales, rptypAccountSales,rptypEventSales, rptypCourseSales, rptypTerminalSales,
      rptypOrders, rptypStaffOrders, rptypVoids, rptypModifiers, rptypSalesHistory,
      rptypStaffSales, rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypCashTotals, rptypSalesByDate,rptypOrderByBumped,
      rptypItemByBumped, rptypItemCountByBumped, rptypComboSales,
      rptypPeriodEndTransactions, rptypPeriodEndStatement, rptypClockIns, rptypAccountSummaries,
      rptypStaffTips, rptypNoSales, rptypTillSales, rptypDiscountSchemeSales, rptypItemModSales, rptypModItems,
      rptypAccountTransactions, rptypOrdersSalesVoids, rptypAccountTabSales, rptypTableSummary, rptypStaffOpenPriceItems,
      rptypTransList, rptypAccTypeMovements, rptypPerDayCalc]);
  frmRange.Visible := frmRange.Enabled;

  txtTo.Enabled := ((ReportType <> rptypSalesHistory) and (not cmdDatesLock.Down));
  cmdTo.Enabled := ((ReportType <> rptypSalesHistory) and (not cmdDatesLock.Down));

  frmForB.Enabled := (ReportType in [rptypSales, rptypAccountSales, rptypEventSales, rptypCourseSales, rptypTerminalSales,
      rptypOrders, rptypStaffOrders, rptypVoids, rptypMenuItems, rptypModifiers, rptypSalesHistory,
      rptypStaffSales, rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypSalesByDate, rptypTerminalSales, rptypDiscountSchemeSales,
      rptypItemModSales, rptypModItems, rptypOrderByBumped, rptypItemByBumped, rptypItemCountByBumped,
      rptypComboSales, rptypOrdersSalesVoids,rptypStaffOpenPriceItems]);

  frmForB.Visible := frmForB.Enabled;

  pnlOutlets.Visible := False;
  pnlRemoteLocations.Visible := False;

  pnlRemoteLocations.Enabled := ((ReportType in [rptypSales, rptypAccountSales, rptypCourseSales, rptypTerminalSales,
      rptypOrders, rptypStaffOrders, rptypVoids, rptypModifiers,
      rptypSalesHistory, rptypStaffSales, rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales,rptypCashTotals, rptypClockIns,
      rptypOrderByBumped,rptypItemByBumped,rptypItemCountByBumped,
      rptypSalesByDate, rptypStaffTips, rptypNoSales, rptypTillSales, rptypDiscountSchemeSales,
      rptypItemModSales, rptypModItems, rptypAccountTransactions,
      rptypOrdersSalesVoids, rptypAccountTabSales, rptypOpenTables, rptypComboSales, rptypTableSummary, rptypStaffOpenPriceItems,
      rptypTransList, rptypAccTypeMovements])
      and (cmbRemoteLocations.Items.Count > 2));
  pnlRemoteLocations.Visible := pnlRemoteLocations.Enabled;

  dm.qrStaffREMOTEOVERRIDEGROUPNAME.Visible := (pnlRemoteLocations.Visible);
  dm.qrAccountsREMOTEOVERRIDEGROUPNAME.Visible := (pnlRemoteLocations.Visible);
  dm.qrAccountTypesREMOTEOVERRIDEGROUPNAME.Visible := (pnlRemoteLocations.Visible);

  chkRemoteLocationBreakdown.Visible := ((ReportType in [rptypSales, rptypAccountSales, rptypTerminalSales,
      rptypTillSales, rptypDiscountSchemeSales, rptypCourseSales, rptypSaleCategorySales, rptypSectionSales, rptypStaffSales,
      rptypSalesByDate, rptypSalesHistory, rptypCashTotals, rptypClockins, rptypItemModSales, rptypModItems,
      rptypModifiers, rptypNoSales, rptypStaffTips, rptypVoids, rptypOrders, rptypStaffOrders, rptypAccountTransactions,
      rptypOrdersSalesVoids, rptypAccountTabSales, rptypOpenTables, rptypComboSales, rptypTableSummary, rptypAccTypeMovements])
    and (cmbRemoteLocations.ItemIndex = 0));
  chkRemoteLocationBreakdown.Checked := True;

  pnlOutlets.Enabled := ((ReportType in [rptypSales, rptypAccountSales, rptypEventSales, rptypCourseSales, rptypTerminalSales,
      rptypOrders, rptypStaffOrders, rptypVoids, rptypMenuItems, rptypModifiers,rptypOrderByBumped,
      rptypSalesHistory, rptypStaffSales, rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypCashTotals,
      rptypItemByBumped,rptypItemByBumped,rptypItemCountByBumped, rptypSalesByDate, rptypStaffTips,
      rptypNoSales, rptypTillSales, rptypDiscountSchemeSales, rptypItemModSales, rptypModItems, rptypAccountTransactions,
      rptypOrdersSalesVoids, rptypAccountTabSales, rptypOpenTables, rptypComboSales, rptypTableSummary, rptypStaffOpenPriceItems,
      rptypTransList, rptypAccTypeMovements])
    and (cmbOutlets.Items.Count > 2));
  pnlOutlets.Visible := pnlOutlets.Enabled;

  chkOutletBreakdown.Visible := ((ReportType in [rptypSales, rptypAccountSales, rptypEventSales, rptypTerminalSales,
      rptypTillSales, rptypDiscountSchemeSales, rptypCourseSales, rptypSaleCategorySales, rptypSectionSales, rptypStaffSales,
      rptypSalesByDate, rptypSalesHistory, rptypCashTotals, rptypItemModSales, rptypModItems,
      rptypModifiers, rptypNoSales, rptypStaffTips, rptypVoids, rptypOrders, rptypStaffOrders, rptypAccountTransactions,
      rptypOrdersSalesVoids, rptypAccountTabSales, rptypOpenTables, rptypComboSales, rptypTableSummary,rptypAccTypeMovements])
     and (cmbOutlets.ItemIndex = 0));
  chkOutletBreakdown.Checked := True;

  frmItemGroups.Enabled := (ReportType in [rptypSales, rptypAccountSales, rptypEventSales, rptypCourseSales, rptypTerminalSales,
      rptypOrders, rptypStaffOrders, rptypVoids, rptypMenuItems, rptypSalesHistory, rptypStaffSales,
      rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypSalesByDate, rptypTillSales, rptypDiscountSchemeSales, rptypItemModSales, rptypModItems,
      rptypOrderByBumped, rptypItemByBumped, rptypItemCountByBumped, rptypOrdersSalesVoids]);

  frmItemGroups.Visible := frmItemGroups.Enabled;

  frmOrderOrdersBy.Visible := False;
  frmOrderVoidsBy.Visible := False;
  frmOrderSalesBy.Visible := False;
  frmOrderMenuBy.Visible := False;
  frmOrderPeopleBy.Visible := False;
  frmOrderModifiersBy.Visible := False;
  frmOrderCashTotalsBy.Visible := False;
  frmOrderClockInsBy.Visible := False;
  frmOrderOpenPriceBy.Visible := False;
  frmOrderTransListBy.Visible := False;

  if (ReportType in [rptypSales, rptypAccountSales,rptypEventSales, rptypCourseSales, rptypTerminalSales, rptypSalesHistory,
      rptypStaffSales, rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypTillSales,
      rptypDiscountSchemeSales, rptypItemModSales]) then begin
    PositionOrderByGroupBox(frmOrderSalesBy);
    EnableGroupSubTotalCheckBoxes(radGroup.Checked);
  end
  else if (ReportType in [rptypOrders, rptypStaffOrders, rptypOrdersSalesVoids]) then begin
    PositionOrderByGroupBox(frmOrderOrdersBy);
    EnableGroupSubTotalCheckBoxes(radOGroup.Checked);
    if (ReportType = rptypOrdersSalesVoids) then
    begin
      //Remove order by unit price option.
      radOPrice.Enabled := False;
      radOValue.Top := 48;
      frmOrderOrdersBy.Height := 72;
    end
    else
    begin
      radOPrice.Enabled := True;
      radOValue.Top := 64;
      frmOrderOrdersBy.Height := 88;
    end;
  end
  else if (ReportType = rptypVoids) then begin
    PositionOrderByGroupBox(frmOrderVoidsBy);
    EnableGroupSubTotalCheckBoxes(radVStaff.Checked);
  end
  else if (ReportType = rptypMenuItems) then begin
    PositionOrderByGroupBox(frmOrderMenuBy);
    EnableGroupSubTotalCheckBoxes(radMenuOrderGroup.Checked);
  end
  else if (ReportType = rptypPeople) then begin
    PositionOrderByGroupBox(frmOrderPeopleBy);
    EnableGroupSubTotalCheckBoxes(False);
  end
  else if (ReportType in [rptypModifiers, rptypModItems]) then begin
    PositionOrderByGroupBox(frmOrderModifiersBy);
    EnableGroupSubTotalCheckBoxes(True);
    CkbGroupForB.Enabled := (radModifiersOrderDefault.Checked or radModifiersOrderModifier.Checked);
  end
  else if (ReportType = rptypCashTotals) then begin
    PositionOrderByGroupBox(frmOrderCashTotalsBy);
    EnableGroupSubTotalCheckBoxes(True);
    ckbGroupTill.Enabled := radCashTotalsOrderTill.Checked;
  end
  else if (ReportType = rptypClockIns) then begin
    PositionOrderByGroupBox(frmOrderClockInsBy);
    EnableGroupSubTotalCheckBoxes(True);
    ckbGroupStaff.Enabled := radClockInsOrderStaffName.Checked;
  end
  else if (ReportType = rptypStaffOpenPriceItems) then
  begin
    PositionOrderByGroupBox(frmOrderOpenPriceBy);
    ckbGroupStaff.Enabled := radClockInsOrderStaffName.Checked;
  end
  else if (ReportType = rptypTransList) then
  begin
    PositionOrderByGroupBox(frmOrderTransListBy);
    EnableGroupSubTotalCheckBoxes(True);
    ckbGroupStaff.Checked := False;
    ckbGroupTill.Checked := False;
    radTransListSortByTenderClick(self);
  end;

  if (ReportType in [rptypSales, rptypModifiers, rptypSalesHistory, rptypModItems]) then begin
    if (ReportType = rptypSales) then
    begin
      PositionGroupBox(frmZeros, 400, 148);
    end
    else if (ReportType = rptypModItems) then
    begin
      PositionGroupBox(frmZeros, 588, 116);
    end
    else begin
      PositionGroupBox(frmZeros, 400, 72);
    end;
  end
  else begin
    frmZeros.Visible := False;
  end;

  if (ReportType = rptypAccountTransactions) then
  begin
    frmSBOptionsClick(self);
    PositionRadioGroup(frmSBOptions, 400, 72);
    PositionRadioGroup(frmSBDetails, 515, 72);
    frmSBDetails.Visible := True;
    PositionCheckBox(chkSBTransType, 645, 72);
    PositionCheckBox(chkSBExtendedDetail, 645, 92);
    PositionCheckBox(chkRemNumbers, 645, 112);
    chkRemNumbers.Visible := pnlRemoteLocations.Visible;
  end
  else
  begin
    frmSBOptions.Visible := False;
    frmSBDetails.Visible := False;
    chkSBTransType.Visible := False;
    chkSBExtendedDetail.Visible := False;
    chkRemNumbers.Visible := False;
  end;

  if (ReportType in [rptypSales, rptypStaffOrders, rptypTerminalSales, rptypStaffSales, rptypComboSales,
      rptypSaleCategorySales, rptypSectionSales, rptypSalePeriodSales, rptypAccountSales, rptypEventSales, rptypCourseSales, rptypStaffTips,
      rptypNoSales, rptypTillSales, rptypDiscountSchemeSales, rptypOrderByBumped, rptypAccountTabSales]) then
  begin
    PositionGroupBox(frmDetailsGroupsTotals, 400, 72);
  end
  else begin
    frmDetailsGroupsTotals.Visible := False;
  end;

  if (ReportType in [rptypItemModSales, rptypModItems]) then
  begin
    PositionGroupBox(frmShowModGroups, 400, 72);
  end
  else begin
    frmShowModGroups.Visible := False;
  end;

  if (ReportType = rptypVoids) then begin
    PositionGroupBox(frmVoidsOrderStyle, 400, 72);
    PositionGroupBox(frmVoidsOrderTypes, 400, 164);
  end
  else begin
    frmVoidsOrderStyle.Visible := False;
    frmVoidsOrderTypes.Visible := False;
  end;

  if (ReportType in [rptypCashTotals, rptypTransList]) then begin
    if (ReportType = rptypCashTotals) then begin
      PositionGroupBox(frmCashupDetailsDisplay, 588, 72);
      chkCashupReference.Enabled := True;
      chkCashupNotes.Enabled := True;
    end
    else begin
      PositionGroupBox(frmCashupDetailsDisplay, 588, 82);
    end;
  end
  else begin
    frmCashupDetailsDisplay.Visible := False;
  end;

  if (ReportType = rptypCashTotals) then begin
    PositionGroupBox(frmCashTotalsOrderStyle, 400, 72);
  end
  else begin
    frmCashTotalsOrderStyle.Visible := False;
  end;

  if (ReportType = rptypModifiers) then begin
    PositionGroupBox(frmModifiersOrderedState, 400, 148);
  end
  else begin
    frmModifiersOrderedState.Visible := False;
  end;

  if (ReportType = rptypTableSummary) then
  begin
    PositionGroupBox(frmTableOptions, 400, 72);
  end
  else
  begin
    frmTableOptions.Visible := False;
  end;

  if (ReportType = rptypTransList) then
  begin
    PositionGroupBox(frmTransListOptions, 400, 72);
  end
  else
  begin
    frmTransListOptions.Visible := False;
  end;

  if (ReportType in [rptypAccountSales, rptypAccountSummaries, rptypAccountTransactions]) then begin
    if (ReportType in [rptypAccountSales, rptypAccountTransactions]) then begin
      PositionControl(TControl(frmAccounts), 400, 164, -1, 356);
      PositionControl(TControl(DBGridAccounts), -1, -1, -1, 260);
    end
    else begin
      PositionControl(TControl(frmAccounts), 400, 148, -1, 372);
      PositionControl(TControl(DBGridAccounts), -1, -1, -1, 276);
//      PositionGroupBox(frmAccounts, 400, 148);
    end;
    frmAccounts.Visible := True;
  end
  else begin
    frmAccounts.Visible := False;
  end;

  if (ReportType in [rptypAccTypeMovements]) then
  begin
    PositionControl(TControl(frmAccountTypes), 400, 148, -1, 372);
    PositionControl(TControl(DBGridAccountTypes), -1, -1, -1, 292);
    frmAccountTypes.Visible := True;
  end
  else begin
    frmAccountTypes.Visible := False;
  end;

  if ReportType = rptypEventSales then
  begin
    PositionControl(TControl(frmEvents), 400, 148, -1, 372);
    PositionControl(TControl(DBGridEvents), -1, -1, -1, 276);
  end;
  frmEvents.Visible := (ReportType = rptypEventSales);

  if (ReportType in [rptypStaffOrders, rptypVoids, rptypStaffSales, rptypClockIns, rptypStaffTips, rptypNoSales, rptypStaffOpenPriceItems, rptypTransList]) then begin
    if (ReportType = rptypStaffSales) or (ReportType = rptypTransList) then
    begin
      PositionControl(TControl(frmStaff), 400, 164, -1, 356);
      PositionControl(TControl(DBGridStaffMembers), -1, -1, -1, 276);
    end
    else if (ReportType = rptypStaffOrders)  then begin
      PositionControl(TControl(frmStaff), 400, 148, -1, 182);
      PositionControl(TControl(DBGridStaffMembers), -1, -1, -1, 102);
    end
    else if (ReportType = rptypVoids) then begin
      PositionControl(TControl(frmStaff), 400, 272, -1, 248);
      PositionControl(TControl(DBGridStaffMembers), -1, -1, -1, 168);
    end
    else begin
      PositionControl(TControl(frmStaff), 400, 148, -1, 372);
      PositionControl(TControl(DBGridStaffMembers), -1, -1, -1, 292);
    end;
    frmStaff.Visible := True;
  end
  else begin
    frmStaff.Visible := False;
  end;

  if (ReportType = rptypTerminalSales) then begin
    PositionGroupBox(frmTerminals, 400, 164);
  end
  else begin
    frmTerminals.Visible := False;
  end;

  if (ReportType in [rptypTillSales, rptypTransList]) then begin
    if (ReportType = rptypTransList) then
    begin
      PositionControl(TControl(frmTills), 4, 210, -1, 310);
      PositionControl(TControl(DBGridTills), -1, -1, -1, 230);
    end
    else
    begin
      PositionControl(TControl(frmTills), 400, 164, -1, 356);
      PositionControl(TControl(DBGridTills), -1, -1, -1, 276);
    end;
    frmTills.Visible := True;
  end
  else begin
    frmTills.Visible := False;
  end;

  if (ReportType = rptypDiscountSchemeSales) then begin
    PositionGroupBox(frmDiscountSchemes, 400, 164);
  end
  else begin
    frmDiscountSchemes.Visible := False;
  end;

  if (ReportType in [rptypItemModSales, rptypModItems]) then begin
    radShowModGroupsAllClick(self);
    if (ReportType = rptypItemModSales) then begin
      PositionGroupBox(frmItemGroups, 4, 210);
      frmModGroups.Height := 356;
      DBGridModGroups.Height := 276;
      frmItemGroups.Height := 310;
      DBGridItemGroups.Height := 233;
    end
    else if (ReportType = rptypModItems) then begin
      PositionGroupBox(frmItemGroups, 400, 194);
      frmModGroups.Height := 310;
      DBGridModGroups.Height := 233;
      frmItemGroups.Height := 326;
      DBGridItemGroups.Height := 246;
    end;
  end
  else begin
    PositionGroupBox(frmItemGroups, 4, 210);
    frmItemGroups.Height := 310;
    DBGridItemGroups.Height := 233;
    frmModGroups.Visible := False;
  end;

  if (ReportType = rptypPeople) then begin
    PositionGroupBox(frmPeopleSelection, 400, 100);
  end
  else begin
    frmPeopleSelection.Visible := False;
  end;

  if (ReportType in [rptypSaleCategorySales, rptypStaffOrders]) then begin
    if (ReportType = rptypStaffOrders) then begin
      PositionControl(TControl(frmSaleCategories), 400, 338, -1, 182);
      PositionControl(TControl(DBGridSaleCategories), -1, -1, -1, 102);
    end
    else begin
      PositionControl(TControl(frmSaleCategories), 400, 164, -1, 356);
      PositionControl(TControl(DBGridSaleCategories), -1, -1, -1, 276);
      //PositionGroupBox(frmSaleCategories, 400, 148);
    end;
    frmSaleCategories.Visible := True;
  end
  else begin
    frmSaleCategories.Visible := False;
  end;

  if (ReportType = rptypSectionSales) then begin
    PositionControl(TControl(frmSections), 400, 164, -1, 356);
    PositionControl(TControl(DBGridSections), -1, -1, -1, 276);

    frmSections.Visible := True;
  end
  else begin
    frmSections.Visible := False;
  end;

  if (ReportType = rptypPerDayCalc) then
  begin
    PositionControl(TControl(frmSections), 400, 164, -1, 356);
    PositionControl(TControl(DBGridSections), -1, -1, -1, 276);
    frmSections.Visible := True;
  end
  else if (ReportType <> rptypSectionSales) then
  begin
    frmSections.Visible := False;
  end;

  frmOrderCombos.Visible := False;
  frmCombos.Visible := False;
  if ReportType in [rptypComboSales] then
  begin
    frmOrderCombos.Visible := True;
    frmCombos.Visible := True;
    PositionControl(TControl(frmCombos), 400, 72, -1, 448);
    PositionControl(TControl(DBGridCombos), -1, -1, -1, 368);
    PositionOrderByGroupBox(frmOrderCombos);
  end;

  frmSalePeriods.Visible := False;
  if (ReportType in [rptypSalePeriodSales]) then
  begin
    PositionControl(TControl(frmSalePeriods),  400, 164, -1, 356);
    PositionControl(TControl(DBGridSalePeriods), -1, -1, -1, 276);
    frmSalePeriods.Visible := True;
  end;

  if (ReportType in [rptypPeriodEndTransactions, rptypPeriodEndStatement]) then begin
    PositionGroupBox(frmPeriodEnds, 400, 148);
    dm.GetPeriodEnds(FFromTime - 60, FToTime + 60);
  end
  else begin
    frmPeriodEnds.Visible := False;
  end;

  if (ReportType = rptypSalesHistory) then begin
    PositionGroupBox(frmSalesHistory, 400, 148);
    chkSalesHistoryUsePeriodLengthClick(nil);
    chkSalesHistoryBetweenTimesClick(nil);
    cmboSalesHistoryShownData.ItemIndex := 0;
  end
  else begin
    frmSalesHistory.Visible := False;
  end;

  if (ReportType = rptypCourseSales) then begin
    PositionGroupBox(frmCourses, 400, 164);
  end
  else begin
    frmCourses.Visible := False;
  end;

  if (ReportType in [rptypSales, rptypSalesByDate]) then begin
    chkSalesIncludePriceLevels.Visible := True;
    chkSalesNettExcludeSalesTax.Visible := True;
    if (ReportType = rptypSales) then begin
      PositionControl(TControl(chkSalesIncludePriceLevels), 400, 224, -1, -1);
      PositionControl(TControl(chkSalesNettExcludeSalesTax), 400, 240, -1, -1);
    end
    else if (ReportType = rptypSalesByDate) then begin
      PositionControl(TControl(chkSalesIncludePriceLevels), 400, 72, -1, -1);
      PositionControl(TControl(chkSalesNettExcludeSalesTax), 400, 88, -1, -1);
    end;
  end
  else if (ReportType in [rptypAccountSales, rptypCourseSales, rptypTerminalSales, rptypStaffSales,
  rptypSaleCategorySales, rptypTillSales, rptypDiscountSchemeSales, rptypItemModSales]) then
  begin
    chkSalesIncludePriceLevels.Visible := False;
    chkSalesNettExcludeSalesTax.Visible := True;
    if (ReportType = rptypItemModSales) then
      PositionControl(TControl(chkSalesNettExcludeSalesTax), 588, 132, -1, -1)
    else
      PositionControl(TControl(chkSalesNettExcludeSalesTax), 400, 148, -1, -1);
  end
  else begin
    chkSalesIncludePriceLevels.Visible := False;
    chkSalesNettExcludeSalesTax.Visible := False;
  end;

  if (ReportType in [rptypPeriodEndTransactions]) then begin
    chkViewTransactions.Visible := True;
    PositionControl(TControl(chkViewTransactions), 400, 72, -1, -1);
  end
  else begin
    chkViewTransactions.Visible := False;
  end;

  if (ReportType in [rptypPeriodEndTransactions, rptypPeriodEndStatement]) then begin
    chkIncludeZeroBalances.Visible := True;
    PositionControl(TControl(chkIncludeZeroBalances), 400, 88, -1, -1);
  end
  else begin
    chkIncludeZeroBalances.Visible := False;
  end;

  if (ReportType in [rptypClockIns]) then begin
    PositionControl(TControl(chkShowAllStaff), 400, 72, -1, -1);
    chkShowAllStaff.Visible := True;
  end
  else begin
    chkShowAllStaff.Visible := False;
  end;

  if (ReportType in [rptypMenuItems, rptypModifiers]) then begin
    if (ReportType = rptypMenuItems) then begin
      PositionControl(TControl(chkShowLinkCodes), 400, 72, -1, -1);
    end
    else begin
      PositionControl(TControl(chkShowLinkCodes), 400, 224, -1, -1);
    end;
    chkShowLinkCodes.Visible := True;
  end
  else begin
    chkShowLinkCodes.Visible := False;
  end;

  if (ReportType in [rptypCashTotals]) then begin
    PositionControl(TControl(chkShowExtendedCashup), 400, 164, -1, -1);
    chkShowExtendedCashup.Visible := True;
  end
  else begin
    chkShowExtendedCashup.Visible := False;
  end;

  if (ReportType in [rptypAccTypeMovements]) then
  begin
    chkShowLoyaltyReward.Visible := True;
    chkShowNoSale.Visible := True;
    PositionControl(TControl(chkShowLoyaltyReward), 400, 72, -1, -1);
    PositionControl(TControl(chkShowNoSale), 400, 88, -1, -1);
  end
  else begin
    chkShowLoyaltyReward.Visible := False;
    chkShowNoSale.Visible := False;
  end;

  frmGroupSubTotals.Visible := True;
  ckbGroupGroup.Visible := False;
  ckbGroupForB.Visible := False;
  ckbGroupStaff.Visible := False;
  ckbGroupRole.Visible := False;
  ckbGroupAccount.Visible := False;
  ckbGroupSaleCategory.Visible := False;
  ckbGroupSection.Visible := False;
  ckbGroupOutlet.Visible := False;
  ckbGroupRemoteLocation.Visible := False;
  ckbGroupTerminal.Visible := False;
  ckbGroupTill.Visible := False;
  ckbGroupAccountType.Visible := False;

  SubTotalsCheckBoxPos := 0;
  if (ReportType in [rptypSales, rptypSalesHistory, rptypComboSales]) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType in [rptypOrders, rptypOrdersSalesVoids]) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypVoids) then begin
    ShowGroupSubTotalsItems(ckbGroupForB);
    ShowGroupSubTotalsItems(ckbGroupStaff);
  end
  else if (ReportType in [rptypAccountSales, rptypEventSales]) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypCourseSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypDiscountSchemeSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypItemModSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
    else if (ReportType = rptypModItems) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypStaffOrders) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypTerminalSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypTillSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypMenuItems) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
  end
  else if (ReportType = rptypModifiers) then begin
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypStaffSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType in [rptypSaleCategorySales, rptypSectionSales]) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypSalePeriodSales) then begin
    ShowGroupSubTotalsItems(ckbGroupGroup);
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypCashTotals) then begin
    ShowGroupSubTotalsItems(ckbGroupTill);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypSalesByDate) then begin
    ShowGroupSubTotalsItems(ckbGroupForB);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypClockIns) then begin
    ShowGroupSubTotalsItems(ckbGroupStaff);
    ShowGroupSubTotalsItems(ckbGroupRole);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType in [rptypStaffTips, rptypNoSales]) then begin
    ShowGroupSubTotalsItems(ckbGroupStaff);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypAccountTransactions) then begin
    ShowGroupSubTotalsItems(ckbGroupAccount);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypOpenTables) then begin
    ShowGroupSubTotalsItems(ckbGroupSection);
    ShowGroupSubTotalsItems(ckbGroupSaleCategory);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypTableSummary) then
  begin
    ShowGroupSubTotalsItems(ckbGroupSection);
    ShowGroupSubTotalsItems(ckbGroupSaleCategory);
    if (pnlOutlets.Visible) then
    begin
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    end;
    if (pnlRemoteLocations.Visible) then
    begin
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
    end;
  end
  else if (ReportType = rptypTransList) then
  begin
    ShowGroupSubTotalsItems(ckbGroupTill);
    ShowGroupSubTotalsItems(ckbGroupStaff);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else if (ReportType = rptypAccTypeMovements) then
  begin
    ShowGroupSubTotalsItems(ckbGroupAccountType);
    if (pnlOutlets.Visible) then
      ShowGroupSubTotalsItems(ckbGroupOutlet);
    if (pnlRemoteLocations.Visible) then
      ShowGroupSubTotalsItems(ckbGroupRemoteLocation);
  end
  else begin
    frmGroupSubTotals.Visible := False;
  end;

  if (frmOrderSalesBy.Visible) then begin
    radGroupClick(nil);
  end;

  PositionCheckBox(chkIncludeStaffOrdersKPI, frmOrderOrdersBy.Left, frmOrderOrdersBy.Top + frmOrderOrdersBy.Height + 4);
  cmdStaffOrdersKPI.Left := frmOrderOrdersBy.Left;
  cmdStaffOrdersKPI.Top := chkIncludeStaffOrdersKPI.Top + chkIncludeStaffOrdersKPI.Height + 4;
  cmdStaffOrdersKPI.Visible := (ReportType = rptypStaffOrders);
  chkIncludeStaffOrdersKPI.Visible := (ReportType = rptypStaffOrders);

  chkExportFile.Visible := (not(ReportType in [rptypPeriodEndStatement, rptypAccountSummaries]));   //Jon 12-12-2003
end;
{******************************************************************************}
function TformReports.CheckDate: Boolean;
begin
  Result := False;
  if (txtFrom.Text = '') then begin
    ShowMessage(sEnterFromDate);
  end
  else if (txtTo.Text = '') then begin
    ShowMessage(sEnterToDate);
  end
  else if (FToTime <= FFromTime) then begin
    ShowMessage(sEnterProperDate);
  end
  else begin
    Result := True;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowSalesReport;
begin
  try
    formQRSales := TformQRSales.Create(Application);

    formQRSales.OutletID := FOutletList.Get(cmbOutlets.ItemIndex);
    formQRSales.RemLocID := FRemLocsList.Get(cmbRemoteLocations.ItemIndex);
    Title1 := sTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select p.*, r.lastrosstransactionsync ');
      SQL.Add('from getsales (:poutletid, :premotelocationid, :pforb, :pitemgroupid, :fromtime, :totime, :returnpricelevels, 0, ''0:00:00'', ''0:00:00'') p');
      SQL.Add('left join remotelocation r on r.remotelocationid = p.remotelocationid and r.whendeleted is null ');

      if (radExcludeZeros.Checked) then begin
        SQL.Add('where ((grosssold <> 0.00) or (amountsold <> 0))');
        Title2 := ', ' + sZeroExcluded;
      end
      else if (radIncludeZeros.Checked) then begin
        //placing a test condition
        SQL.Add('where ((p.whendeleted is null ) or ((p.whendeleted is not null ) and ((Select Distinct itemid from invline where itemid = p.itemid)is not null)) ) ');
        Title2 := ', ' + sZeroIncluded;
      end
      else if (radOnlyZeros.Checked) then begin
        SQL.Add('where ((grosssold >= -0.01) and (grosssold <= 0.01) and (amountsold = 0))');
        Title2 := ', ' + sZeroOnly;
      end;

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := ', ' + sSelectedItemGroups + Title2;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := ', ' + Glbs.ItemGroup + Title2;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      BoolAsParam(ParamByName('returnpricelevels'), chkSalesIncludePriceLevels.Checked);
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sSalesRepDlgTitle;
      SaveDialog.FileName := 'Sales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSales.WBCSV.FileName := SaveDialog.FileName;
        formQRSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRSales.QRSales.ReportTitle := sSalesRepTitle;
    if chkSalesIncludePriceLevels.Checked then
    formQRSales.QRSales.Page.Orientation := TPrinterOrientation(1)    //poLandscape
    else
    formQRSales.QRSales.Page.Orientation := TPrinterOrientation(0);   //poPortrait
    formQRSales.QRSales.Preview;
  finally
    formQRSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowOrdersReport;
begin
  try
    formQROrders := TformQROrders.Create(Application);
    Title1 := sOrdersRepTitle;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrOrders do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select gio.outletid, gio.outletname, gio.remotelocationid, gio.remotelocationname,');
      SQL.Add('  gio.forb, gio.itemgroupid, gio.itemgroupabbrev,');
      SQL.Add('  gio.itemid, gio.itemabbrev, gio.itemgrouporder, gio.itemorder, i.linkcode,');
      SQL.Add('  gio.qtystock, gio.qtywasted, gio.nettqty, gio.avprice, gio.nett,');
      SQL.Add('  gio.superitemgroupid, gio.superitemgroup, gio.superitemgroupabbrev, gio.superitemgrouporder, gio.hideforb');
      SQL.Add('  from getitemorders (:poutletid, :premotelocationid, :pforb, :pitemgroupid, :fromtime, :totime) gio');
      SQL.Add('  inner join item i on (gio.itemid = i.itemid)');

      if (radOGroup.Checked) then begin
        SQL.Add('order by gio.outletname, gio.remotelocationname, gio.superitemgrouporder, gio.forb desc, gio.itemgrouporder, gio.itemorder');
        OrdersHeader := 'Orders (By Group / Item Order)';
      end
      else if (radOUnits.Checked) then begin
        SQL.Add('order by gio.nettqty desc, gio.outletname, gio.remotelocationname, gio.superitemgrouporder, gio.forb desc, gio.itemgrouporder, gio.itemorder');
        OrdersHeader := 'Orders (By Units)';
      end
      else if (radOPrice.Checked) then begin
        SQL.Add('order by gio.avprice desc, gio.outletname, gio.remotelocationname, gio.superitemgrouporder, gio.forb desc, gio.itemgrouporder, gio.itemorder');
        OrdersHeader := 'Orders (By Average Price)';
      end
      else if (radOValue.Checked) then begin
        SQL.Add('order by gio.nett desc, gio.outletname, gio.remotelocationname, gio.superitemgrouporder, gio.forb desc, gio.itemgrouporder, gio.itemorder');
        OrdersHeader := 'Orders (By Value)';
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
          formQROrders.MultipleItemGroups := True;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := Title2 + ', ' + sFoodBev;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := Title2 + ', ' + sFood;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := Title2 + ', ' + sBev;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sOrdRepDlgTitle;
      SaveDialog.FileName := 'Orders' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQROrders.WBCSV.FileName := SaveDialog.FileName;
        formQROrders.WBCSV.OpenToWrite;
      end;
    end;

    formQROrders.QROrders.ReportTitle := sOrdRepTitle;
    formQROrders.QROrders.Preview;
  finally
    formQROrders.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowVoidsReport;
begin
  try
    formQRVoids := TformQRVoids.Create(Application);
    Title1 := sVoidsTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrVoidLines do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getitemvoids (:poutletid, :premotelocationid, :pforb, :pstaffid, :pitemgroupid, :reporttype, :fromtime, :totime)');

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radVoidsRefunds.Checked) then begin
        SQL.Add('where fixedvoidreasonid = 1 or cashsalerefund = 1');
        Title2 := ', Refunds Only';
      end
      else if (radDriveThrough.Checked) then begin
        SQL.Add('where fixedvoidreasonid = 2 ');
        Title2 := ', Drive Through';
      end
      else if (radVoidClearitem.Checked) then begin
        SQL.Add('where fixedvoidreasonid = 3 ');
        Title2 := ', Cleared Items';
      end
      else if (radVoidCancelledTrans.Checked) then begin
        SQL.Add('where fixedvoidreasonid = 4 ');
        Title2 := ', Cancelled Transactions';
      end;

      if (radStaffAll.Checked) then begin
        ParamByName('pstaffid').AsInteger := -1;
      end
      else
      begin
        if (DBGridStaffMembers.SelectedRows.Count > 1) then
        begin
          ParamByName('pstaffid').AsString := GetSelectedIDString(DBGridStaffMembers, 'staffid');
        end
        else
        begin
          ParamByName('pstaffid').AsInteger := Glbs.StaffID;
        end;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := Title2 + ', ' + sFoodBev;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := Title2 + ', ' + sFood;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := Title2 + ', ' + sBev;
        end;
      end;

      if (radVStaff.Checked) then begin
        SQL.Add('order by whovoid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, whenvoid, outletname, remotelocationname');
        Title2 := Title2 + ', ' + sByStaff;
      end
      else if (radVWhen.Checked) then begin
        SQL.Add('order by whenvoid, whovoid, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sByDateTime;
      end
      else if (radVGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, whovoid, whenvoid');
        Title2 := Title2 + ', ' + sByGroupOrder;
      end
      else if (radVVoided.Checked) then begin
        SQL.Add('order by qtyvoided desc, whovoid, whenvoid, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', '+ sByUnitsVoided;
      end
      else if (radVWasted.Checked) then begin
        SQL.Add('order by qtywasted desc, whovoid, whenvoid, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sByUnitsWasted;
      end
      else if (radVPrice.Checked) then begin
        SQL.Add('order by unitprice desc, whovoid, whenvoid, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sByUnitsPrice;
      end
      else if (radVValue.Checked) then begin
        SQL.Add('order by nett desc, whovoid, whenvoid, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sByVoidValue;
      end;

      if (radVoidsVoidsOnly.Checked) then begin
        ParamByName('reporttype').AsInteger := 1;
      end
      else if (radVoidsWastageOnly.Checked) then begin
        ParamByName('reporttype').AsInteger := 2;
      end
      else begin
        ParamByName('reporttype').AsInteger := 0;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sVoidDlgTitle;
      SaveDialog.FileName := 'Voids' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRVoids.WBCSV.FileName := SaveDialog.FileName;
        formQRVoids.WBCSV.OpenToWrite;
      end;
    end;

    formQRVoids.QRVoids.ReportTitle := sVoidRepTitle;
    formQRVoids.QRVoids.Preview;
  finally
    formQRVoids.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.SpinEdtStaffYrsAgoChange(Sender: TObject);
begin
  if SpinEdtStaffYrsAgo.Value < 0 then
    SpinEdtStaffYrsAgo.Value := 0;
end;
{******************************************************************************}
procedure TformReports.ShowAllStaffOrders;
begin
  try
    formQRAllStaffOrders := TformQRAllStaffOrders.Create(Application);

    if (radStaffAll.Checked) then begin
      Title1 := sAllStaffOrdTitle1;
    end
    else if (DBGridStaffMembers.SelectedRows.Count > 1) then
    begin
      Title1 := sSelectedStaff + sOrdRepTitle1;
    end
    else
    begin
      Title1 := Glbs.FirstName + ' ' + Glbs.Surname + sOrdRepTitle1;
    end;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAllStaffOrders do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getitemorderswithstaff (:poutletid, :premotelocationid, :pforb, :pstaffid, :psalecategoryid, :pitemgroupid, :fromtime, :totime)');

      if (radOGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        OrdersHeader := 'Orders (By Group / Item Order)';
      end
      else if (radOUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, nettqty desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        OrdersHeader := 'Orders (By Units)';
      end
      else if (radOPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, avprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        OrdersHeader := 'Orders (By Average Price)';
      end
      else if (radOValue.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, nett desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        OrdersHeader := 'Orders (By Value)';
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radStaffAll.Checked) then begin
        ParamByName('pstaffid').AsInteger := -1;
      end
      else begin
        if (DBGridStaffMembers.SelectedRows.Count > 1) then
        begin
          ParamByName('pstaffid').AsString := GetSelectedIDString(DBGridStaffMembers, 'staffid');
          formQRAllStaffOrders.MultipleStaff := True;
        end
        else
        begin
          ParamByName('pstaffid').AsInteger := Glbs.StaffID;
        end;
      end;

      if (radSaleCategoryAll.Checked) then begin
        ParamByName('psalecategoryid').AsInteger := -1;
      end
      else begin
        if (DBGridSaleCategories.SelectedRows.Count > 1) then
        begin
          ParamByName('psalecategoryid').AsString := GetSelectedIDString(DBGridSaleCategories, 'salecategoryid');
          Title2 := Title2 + ', ' + sSelectedSaleCat;
        end
        else
        begin
          ParamByName('psalecategoryid').AsInteger := Glbs.SaleCategoriesSaleCategoryID;
          Title2 := Title2 + ', ' + Glbs.SaleCategoriesSaleCategory;
        end;
      end;

      if radItemGroup.Checked then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := Title2 + ', ' + sFoodBev;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := Title2 + ', ' + sFood;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := Title2 + ', ' + sBev;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffOrdDlgTitle;
      SaveDialog.FileName := 'StaffOrders' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAllStaffOrders.WBCSV.FileName := SaveDialog.FileName;
        formQRAllStaffOrders.WBCSV.OpenToWrite;
      end;
    end;

    formQRAllStaffOrders.QRAllStaffOrders.ReportTitle := sStaffOrdRepTitle;
    formQRAllStaffOrders.QRAllStaffOrders.Preview;
  finally
    formQRAllStaffOrders.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountSalesReport;
begin
  try
    formQRAccountSales := TformQRAccountSales.Create(Application);
    Title1 := sAcctSalesRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAccountSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getaccountsales (:poutletid, :premotelocationid, :pforb, :paccountid, :paccounttypeid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, accountname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sByGroupOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by accountname, amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by accountname, averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by accountname, grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by accountname, discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by accountname, nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radAccountAll.Checked) then begin
        ParamByName('paccountid').AsInteger := -1;
        ParamByName('paccounttypeid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllAccts;
      end
      else if (radAccountSelected.Checked) then begin
        ParamByName('paccountid').AsInteger := Glbs.AccountID;
        ParamByName('paccounttypeid').AsInteger := -1;
        Title1 := Title1 + ', ' + Glbs.AccountName;
      end
      else begin
        ParamByName('paccountid').AsInteger := -1;
        ParamByName('paccounttypeid').AsInteger := Glbs.AccountTypeID;
        Title1 := Title1 + ', ' + Glbs.AccountType;
      end;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sAcctRepDlgTitle;
      SaveDialog.FileName := 'Account Sales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAccountSales.WBCSV.FileName := SaveDialog.FileName;
        formQRAccountSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRAccountSales.QRAccountSales.ReportTitle := sAcctSalesRepTitle;
    formQRAccountSales.QRAccountSales.Preview;
  finally
    formQRAccountSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowCourseSalesReport;
begin
  try
    formQRCourseSales := TformQRCourseSales.Create(Application);
    Title1 := sCourseSalesRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrCourseSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getcoursesales (:poutletid, :premotelocationid, :pforb, :pcourseid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, course, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by course, amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' +  sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by course, averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by course, grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by course, discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by course, nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radCourseAll.Checked) then begin
        ParamByName('pcourseid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllCourses;
      end
      else begin
        ParamByName('pcourseid').AsInteger := Glbs.CourseID;
        Title1 := Title1 + ', ' + Glbs.Course;
      end;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sCourseSalesDlgTitle;
      SaveDialog.FileName := 'CourseSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRCourseSales.WBCSV.FileName := SaveDialog.FileName;
        formQRCourseSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRCourseSales.QRCourseSales.ReportTitle := sCourseSalesRepTitle;
    formQRCourseSales.QRCourseSales.Preview;
  finally
    formQRCourseSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowDiscountSchemeSalesReport;
begin
  try
    formQRDiscountSchemeSales := TformQRDiscountSchemeSales.Create(Application);
    Title1 := 'Discount Scheme Sales Report';
    Title2 := '';
    Title3 := 'From ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime) +
     ' To ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrDiscountSchemeSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getdiscountschemesales (:poutletid, :premotelocationid, :pforb, :pdiscountschemeid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Group / Item Order';
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by amountsold desc, outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Units Sold';
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by averageunitprice desc, outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Average Unit Price';
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by grosssold desc, outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Gross Value Sold';
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by discount desc, outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Discount Given';
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by nettsold desc, outletname, remotelocationname, discountschemename, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Nett Value Sold';
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', Food & Beverage' + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', Food' + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', Beverage' + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := 'All Outlets' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radDiscountSchemeAll.Checked) then begin
        ParamByName('pdiscountschemeid').AsInteger := -1;
        Title1 := Title1 + ', All Discount Schemes';
      end
      else begin
        ParamByName('pdiscountschemeid').AsInteger := Glbs.DiscountSchemeID;
        Title1 := Title1 + ', ' + Glbs.DiscountSchemeName;
      end;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := 'Discount Scheme Sales Report file export';
      SaveDialog.FileName := 'DiscountSchemeSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRDiscountSchemeSales.WBCSV.FileName := SaveDialog.FileName;
        formQRDiscountSchemeSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRDiscountSchemeSales.QRDiscountSchemeSales.ReportTitle := 'WizBang Waiter Discount Scheme Sales Report';
    formQRDiscountSchemeSales.QRDiscountSchemeSales.Preview;
  finally
    formQRDiscountSchemeSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowTerminalSalesReport;
begin
  try
    formQRTerminalSales := TformQRTerminalSales.Create(Application);

    Title1 := sTerminalSalesTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrTerminalSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getterminalsales (:poutletid, :premotelocationid, :pforb, :pterminalid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, terminalid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radTerminalAll.Checked) then begin
        ParamByName('pterminalid').AsInteger := -1;
        Title1 := Title1 + ', ' +  sAllTerminals;
      end
      else begin
        if (DBGridTerminals.SelectedRows.Count > 1) then
        begin
          ParamByName('pterminalid').AsString := GetSelectedIDString(DBGridTerminals, 'terminalid');
          Title1 := Title1 + ', ' + sSelectedTerminals;
          formQRTerminalSales.MultipleTerminals := True;
        end
        else
        begin
          ParamByName('pterminalid').AsInteger := Glbs.TerminalID;
          Title1 := Title1 + ', ' + Glbs.TerminalName;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sTerminalSalesDlgTitle;
      SaveDialog.FileName := 'TerminalSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRTerminalSales.WBCSV.FileName := SaveDialog.FileName;
        formQRTerminalSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRTerminalSales.QRTerminalSales.ReportTitle := sTerminalSalesRepTitle;
    formQRTerminalSales.QRTerminalSales.Preview;
  finally
    formQRTerminalSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowTillSalesReport;
begin
  try
    formQRTillSales := TformQRTillSales.Create(Application);

    Title1 := 'Till Sales Report';
    Title2 := '';
    Title3 := 'From ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime) +
     ' To ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrTillSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from gettillsales (:poutletid, :premotelocationid, :pforb, :ptillid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Group / Item Order';
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Units Sold';
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Average Unit Price';
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Gross Value Sold';
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Discount Given';
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationid, tillid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', By Nett Value Sold';
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', Food & Beverage' + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', Food' + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', Beverage' + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := 'All Outlets' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radTillAll.Checked) then begin
        ParamByName('ptillid').AsInteger := -1;
        Title1 := Title1 + ', All Tills';
      end
      else begin
        ParamByName('ptillid').AsInteger := Glbs.TillID;
        Title1 := Title1 + ', ' + Glbs.TillName;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := 'Till Sales Report file export';
      SaveDialog.FileName := 'TillSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRTillSales.WBCSV.FileName := SaveDialog.FileName;
        formQRTillSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRTillSales.QRTillSales.ReportTitle := 'WizBang Waiter Till Sales Report';
    formQRTillSales.QRTillSales.Preview;
  finally
    formQRTillSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowMenuItemsReport;
begin
  try
    formQRMenuItems := TformQRMenuItems.Create(Application);

    Title1 := sMenuItemsTitle1;
    Title2 := '';
    Title3 := '';

    if (not CheckDate) then Exit;

    with dm.qrMenuItems do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getmenuitems(:outletidin, :aforbin, :itemgroupidin, :showsharing)');

      if (radMenuOrderGroup.Checked) then begin
        SQL.Add('order by outletname, superitemgrouporder, forb desc, itemgroupabbrev, itemabbrev');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radMenuOrderItem.Checked) then begin
        SQL.Add('order by outletname, itemabbrev');
        Title2 := ', ' + sByItem;
      end
      else if (radMenuOrderPrice.Checked) then begin
        SQL.Add('order by outletname, itemprice');
        Title2 := ', ' + sByUnitsPrice;
      end
      else if (radMenuOrderWhenUpd.Checked) then
      begin
        SQL.Add('order by outletname, whenupd desc, itemabbrev');
        Title2 := ', ' + sByWhenUpd;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('itemgroupidin').AsInteger := Glbs.ItemGroupID;
        ParamByName('aforbin').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('itemgroupidin').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('aforbin').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('aforbin').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('aforbin').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        ParamByName('outletidin').AsInteger := -1;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('outletidin').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      BoolAsParam(ParamByName('showsharing'), AppDetails.ShowSharing);
      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sMenuItemsDlgTitle;
      SaveDialog.FileName := 'Menu' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRMenuItems.WBCSV.FileName := SaveDialog.FileName;
        formQRMenuItems.WBCSV.OpenToWrite;
      end;
    end;

    formQRMenuItems.QRMenuItems.ReportTitle := sMenuItemsRepTitle;
    formQRMenuItems.QRMenuItems.Preview;
  finally
    formQRMenuItems.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowPeopleReport;
begin
  try
    formQRPeople := TformQRPeople.Create(Application);
    Title1 := sPeopleRepTitle1;
    Title2 := '';
    Title3 := '';
    Title3ArrayCount := 0;

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrPeople do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getpeopleforexport(');
      SQL.Add('  :fromdob, :todob, :frombirthday, :frombirthmonth, :tobirthday, :tobirthmonth,');
      SQL.Add('  :fromanniversarydate, :toanniversarydate, :fromanniversaryday, :fromanniversarymonth, :toanniversaryday, :toanniversarymonth,');
      SQL.Add('  :pcustomer, :pvip,');
      SQL.Add('  :paddress1, :paddress2, :paddress3, :ppostcode, :pcountry,');
      SQL.Add('  :pgender, :pfavoriteitem,');
      SQL.Add('  :plastwheninvoicedafter, :plastwheninvoicedbefore, :poptout, :pvalidemail, :paccounttypeid,');
      SQL.Add('  :orderingby, :pperorginactive, :premotelocationid)');

      if (radPeopleOrderSurname.Checked) then begin
        ParamByName('orderingby').AsInteger := 1;
   //     SQL.Add('order by surname, firstname, middlename, title');
        Title2 := sBySurname;
      end
      else if (radPeopleOrderFirstname.Checked) then begin
        ParamByName('orderingby').AsInteger := 2;
   //     SQL.Add('order by firstname, surname, middlename, title');
        Title2 := sByFirstName;
      end;

      case cmbPeopleCustomer.ItemIndex of
        0: ParamByName('pcustomer').AsInteger := -1;    //All People
        1: begin
          ParamByName('pcustomer').AsInteger := 1;      //Customers Only
          Title2 := sCustomers + ', ' + Title2;
        end;
        2: begin
          ParamByName('pcustomer').AsInteger := 0;      //Non Customers
          Title2 := sNonCustomers + ', ' + Title2;
        end;
      else
        ParamByName('pcustomer').AsInteger := -1;
      end;
      case cmbPeopleVIP.ItemIndex of
        0: ParamByName('pvip').AsInteger := -1;         //All People
        1: begin
          ParamByName('pvip').AsInteger := 1;           //VIPs only
          Title2 := sVips + ', ' + Title2;
        end;
        2: begin
          ParamByName('pvip').AsInteger := 0;           //Non VIPs
          Title2 := sNonVips + ', ' + Title2;
        end;
      else
        ParamByName('pvip').AsInteger := -1;
      end;

      if (cmbPeopleRemoteSite.Items.Count > 1) then
      Title2 := cmbPeopleRemoteSite.Text + ', ' + Title2;


      if ((FFromDOB <> 0) and (FToDOB <> 0)) then begin
        ParamByName('fromdob').AsDateTime := FFromDOB;
        ParamByName('todob').AsDateTime := FToDOB;
        Title3Array[Title3ArrayCount] := Format(sDOBFromTo , [txtFromDOB.Text,txtToDOB.Text]);
        Inc(Title3ArrayCount);
      end
      else begin
        ParamByName('fromdob').AsDateTime := 0;
        ParamByName('todob').AsDateTime := 0;
      end;
      if ((FFromBirthDay <> 0) and (FFromBirthMonth <> 0) and (FToBirthDay <> 0) and (FToBirthMonth <> 0)) then begin
        ParamByName('frombirthday').AsInteger := FFromBirthDay;
        ParamByName('frombirthmonth').AsInteger := FFromBirthMonth;
        ParamByName('tobirthday').AsInteger := FToBirthDay;
        ParamByName('tobirthmonth').AsInteger := FToBirthMonth;
        Title3Array[Title3ArrayCount] := Format(sBirthFromTo , [txtFromBirthday.Text,txtToBirthday.Text]); 
        Inc(Title3ArrayCount);
      end
      else begin
        ParamByName('frombirthday').AsInteger := 0;
        ParamByName('frombirthmonth').AsInteger := 0;
        ParamByName('tobirthday').AsInteger := 0;
        ParamByName('tobirthmonth').AsInteger := 0;
      end;

      if ((FFromWeddingDate <> 0) and (FToWeddingDate <> 0)) then begin
        ParamByName('fromanniversarydate').AsDateTime := FFromWeddingDate;
        ParamByName('toanniversarydate').AsDateTime := FToWeddingDate;
        Title3Array[Title3ArrayCount] := Format(sWeddFromTo , [txtFromWeddingDate.Text,txtToWeddingDate.Text]); 
        Inc(Title3ArrayCount);
      end
      else begin
        ParamByName('fromanniversarydate').AsDateTime := 0;
        ParamByName('toanniversarydate').AsDateTime := 0;
      end;
      if ((FFromAnniversaryDay <> 0) and (FFromAnniversaryMonth <> 0) and (FToAnniversaryDay <> 0) and (FToAnniversaryMonth <> 0)) then begin
        ParamByName('fromanniversaryday').AsInteger := FFromAnniversaryDay;
        ParamByName('fromanniversarymonth').AsInteger := FFromAnniversaryMonth;
        ParamByName('toanniversaryday').AsInteger := FToAnniversaryDay;
        ParamByName('toanniversarymonth').AsInteger := FToAnniversaryMonth;
        Title3Array[Title3ArrayCount] := Format(sAnnivFromTo , [txtFromAnniversary.Text,txtToAnniversary.Text]);  
        Inc(Title3ArrayCount);
      end
      else begin
        ParamByName('fromanniversaryday').AsInteger := 0;
        ParamByName('fromanniversarymonth').AsInteger := 0;
        ParamByName('toanniversaryday').AsInteger := 0;
        ParamByName('toanniversarymonth').AsInteger := 0;
      end;

      ParamByName('paddress1').AsString := txtStreetName.Text;
      ParamByName('paddress2').AsString := txtSuburb.Text;
      ParamByName('paddress3').AsString := txtCity.Text;
      ParamByName('ppostcode').AsString := txtPeoplePostCode.Text;
      ParamByName('pcountry').AsString := txtPeopleCountry.Text;

      case cmbPeopleGender.ItemIndex of
        0: ParamByName('pgender').AsString := '';
        1: ParamByName('pgender').AsString := 'M';
        2: ParamByName('pgender').AsString := 'F';
      end;
      ParamByName('pfavoriteitem').AsString := txtPeopleFavoriteItem.Text;
      BoolAsParam(ParamByName('poptout'), chkPeopleRemoveOptOut.Checked);
      BoolAsParam(ParamByName('pvalidemail'), chkPeopleValidEmail.Checked);
      ParamByName('plastwheninvoicedafter').AsDateTime := FLastWhenInvoicedAfter;
      ParamByName('plastwheninvoicedbefore').AsDateTime := FLastWhenInvoicedBefore;
      if cmbPeopleAccountTypes.ItemIndex > 0 then//sinu
        ParamByName('paccounttypeid').AsInteger := FPeopleAcctTypes.Get(cmbPeopleAccountTypes.ItemIndex) //Integer(cmbPeopleAccountTypes.Items.Objects[cmbPeopleAccountTypes.ItemIndex])
      else//sinu
        ParamByName('paccounttypeid').AsInteger := -1;//sinu

      if cmbPeopleRemoteSite.ItemIndex >= 0 then
        ParamByName('premotelocationid').AsInteger := FPeopleRemSiteList.Get(cmbPeopleRemoteSite.ItemIndex)
      else//sinu
        ParamByName('premotelocationid').AsInteger := -1;
        
      if (chkPeopleRemoveInactive.Checked) then
        ParamByName('pperorginactive').AsInteger := 0
      else
        ParamByName('pperorginactive').AsInteger := -1;
      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sPeopleRepDlgTitle;
      SaveDialog.FileName := 'People' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRPeople.WBCSV.FileName := SaveDialog.FileName;
        formQRPeople.WBCSV.OpenToWrite;
      end;
    end;

    formQRPeople.QRPeople.ReportTitle := sPeopleRepTitle;
    formQRPeople.QRPeople.Preview;
  finally
    formQRPeople.Free;
  end;
end;

{******************************************************************************}

procedure TformReports.ShowPerDayCalcReport;
begin
  try
    formQRPerDayCalc := TformQRPerDayCalc.Create(Application);
    Title1 := sPerDayCalcTitle;
    Title2 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);


with dm.qrPerDayCalc do
begin
  Close;
  UnPrepare;
  SQL.Clear;

  SQL.Add('SELECT');
  SQL.Add('    CAST(a.whenclosed AS DATE) AS report_date,');
  SQL.Add('    s.section AS section_name,');

  // Table Count & Total Guest as Integer
  SQL.Add('    CAST(COUNT(a.groupid) AS INTEGER) AS tables_count,');
  SQL.Add('    CAST(SUM(a.guests) AS INTEGER) AS total_guests,');

  // Total Amount as 0.00
  SQL.Add('    CAST(ROUND(CAST(SUM(a.invoiced) AS DECIMAL(18,4)), 2) AS DECIMAL(18,2)) AS total_amount,');

  // Avg Guests / Table as 0.00 (null-safe)
  SQL.Add('    CAST(');
  SQL.Add('      ROUND(');
  SQL.Add('        COALESCE(');
  SQL.Add('          CAST(SUM(a.guests) AS DECIMAL(18,4)) / NULLIF(COUNT(a.groupid), 0),');
  SQL.Add('          0');
  SQL.Add('        ), 2');
  SQL.Add('      ) AS DECIMAL(18,2)');
  SQL.Add('    ) AS avg_guests_per_table,');

  // Avg Spend / Guest as 0.00 (null-safe)
  SQL.Add('    CAST(');
  SQL.Add('      ROUND(');
  SQL.Add('        COALESCE(');
  SQL.Add('          CAST(SUM(a.invoiced) AS DECIMAL(18,4)) / NULLIF(SUM(a.guests), 0),');
  SQL.Add('          0');
  SQL.Add('        ), 2');
  SQL.Add('      ) AS DECIMAL(18,2)');
  SQL.Add('    ) AS avg_spend_per_guest');

  SQL.Add('FROM agroup a');
  SQL.Add('INNER JOIN outlet o ON o.outletid = a.outletid');
  SQL.Add('LEFT JOIN section s');
  SQL.Add('    ON s.outletid = a.outletid');
  SQL.Add('   AND a.tableno BETWEEN s.fromtableno AND s.totableno');
  SQL.Add('   AND s.whendeleted IS NULL');
  SQL.Add('WHERE');
  SQL.Add('    a.guests >= 1');
  SQL.Add('    AND a.whenclosed IS NOT NULL');
  SQL.Add('    AND a.whenclosed >= :fromtime ');
  SQL.Add('    AND a.whenclosed <  :totime ');

  // ---- Dynamic section filter (inline, no helper function) ----
  // If "All sections" is checked, no section filter.
  // Else, build s.sectionid IN (:sid0, :sid1, ...)
  var idsStr: string;
  var ids: TArray<string>;
  var i: Integer;
  var inList: string;

  if radSectionAll.Checked then
  begin
    // No section filter (include all, including NULL section mappings)
    Title1 := Title1 + ', ' + sAllSections;
  end
  else
  begin
    // Example: idsStr = '1,3,7'
    idsStr := GetSelectedIDString(DBGridSections, 'sectionid');

    // Split CSV inline (Delphi XE8+)
    ids := idsStr.Split([',']);

    // Guard against empty / whitespace-only input
    // Build IN (:sid0, :sid1, ...)
    inList := '';
    for i := 0 to High(ids) do
    begin
      ids[i] := Trim(ids[i]);
      if ids[i] <> '' then
      begin
        if inList <> '' then inList := inList + ',';
        inList := inList + ':sid' + i.ToString;
      end;
    end;

    // Only add the predicate if we actually have IDs
    if inList <> '' then
      SQL.Add('    AND s.sectionid IN (' + inList + ')');

    // Update title (if you want to show names instead, you can build that separately)
    Title1 := Title1 + ', ' + Glbs.SectionsSection;
  end;
  // ---- End dynamic section filter ----

  SQL.Add('GROUP BY');
  SQL.Add('    CAST(a.whenclosed AS DATE),');
  SQL.Add('    s.section');
  SQL.Add('ORDER BY');
  SQL.Add('    CAST(a.whenclosed AS DATE),');
  SQL.Add('    s.section');

  // Bind date range
  ParamByName('fromtime').AsDateTime := FFromTime;
  ParamByName('totime').AsDateTime   := FToTime;

  // Bind section IDs (only if not "All")
  if not radSectionAll.Checked then
  begin
    for i := 0 to High(ids) do
    begin
      if Trim(ids[i]) <> '' then
        ParamByName('sid' + i.ToString).AsInteger := StrToInt(Trim(ids[i]));
    end;
  end;

  Open;
end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sPerDayDlgTitle;
      SaveDialog.FileName := 'PerDayCalculations' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSectionSales.WBCSV.FileName := SaveDialog.FileName;
        formQRSectionSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRPerDayCalc.QRPerDayCalc.ReportTitle := sPerDayCalcTitle;
    formQRPerDayCalc.QRPerDayCalc.Preview;
  finally
    formQRPerDayCalc.Free;
  end;
end;

{******************************************************************************}
procedure TformReports.ShowModifiersReport;
begin
  try
    formQRModifiers := TformQRModifiers.Create(Application);

    Title1 := sModifiersRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrModifiers do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getmodifiers(:poutletid, :premotelocationid, :pforb, :pricetype, :orderedstate, :fromtime, :totime, :showsharing)');

      if (radModifiersOrderDefault.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, forb desc, modifierorder, modifier');
        Title2 := ', ' + sByDefOrder;
      end
      else if (radModifiersOrderModifier.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, forb desc, modifier');
        Title2 := ', ' + sByItem;
      end
      else if (radModifiersOrderUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, nettqty desc, forb desc, modifier');
        Title2 := ', ' +  sByUnits;
      end
      else if (radModifiersOrderPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, modprice desc, forb desc, modifier');
        Title2 := ', ' + sAvgPrice;
      end
      else if (radModifiersOrderValue.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, nett desc, forb desc, modifier');
        Title2 := ', ' + sByValue;
      end;

      if (radFAndB.Checked) then begin
        ParamByName('pforb').AsString := '';
        Title2 := ', ' + sFoodBev + Title2;
      end
      else if (radFOnly.Checked) then begin
        ParamByName('pforb').AsString := 'F';
        Title2 := ', ' + sFood + Title2;
      end
      else begin
        ParamByName('pforb').AsString := 'B';
        Title2 := ', ' + sBev + Title2;
      end;

      if (radModifiersOrderedOnly.Checked) then begin
        ParamByName('orderedstate').AsInteger := 1;
        Title2 := ', ' +  sModOrdered;
      end
      else if (radModifiersNonOrderedOnly.Checked) then begin
        ParamByName('orderedstate').AsInteger := 0;
        Title2 := ', ' +  sModNotOrdered;
      end
      else if (radModifiersOrderedBoth.Checked) then begin
        ParamByName('orderedstate').AsInteger := -1;
        Title2 := ', ' + sAllModifiers;
      end;

      if (radExcludeZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := 1;
        Title2 := Title2 + ', ' + sZeroExcluded;
      end
      else if (radIncludeZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := -1;
        Title2 := Title2 + ', ' + sZeroIncluded;
      end
      else if (radOnlyZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := 0;
        Title2 := Title2 + ', ' + sZeroOnly;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      BoolAsParam(ParamByName('showsharing'), AppDetails.ShowSharing);

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sModifierDlgTitle;
      SaveDialog.FileName := 'Modifiers' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRModifiers.WBCSV.FileName := SaveDialog.FileName;
        formQRModifiers.WBCSV.OpenToWrite;
      end;
    end;

    formQRModifiers.QRModifiers.ReportTitle := sModifiersRepTitle;
    formQRModifiers.QRModifiers.Preview;
  finally
    formQRModifiers.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowSalesHistoryReport;
var
  NoOfSubSelects: Integer; //Number of sub-select statements required for number of periods selected
  NoOfSQLPeriods: Integer; //Number of periods in SQL

  function GetPeriodLengthStr: string;
  begin
    Result := EmptyStr;
    if not chkSalesHistoryUsePeriodLength.Checked then
      Exit;

    if radSalesHistoryLength15Min.Checked then
      Result := s15MinPeriod
    else if radSalesHistoryLength30Min.Checked then
      Result := s30MinPeriod
    else if radSalesHistoryLengthHour.Checked then
      Result := sHourPeriod
    else if radSalesHistoryLength2Hour.Checked then
      Result := s2HourPeriod
    else if radSalesHistoryLength3Hour.Checked then
      Result := s3HourPeriod
    else if radSalesHistoryLength4Hour.Checked then
      Result := s4HourPeriod
    else if radSalesHistoryLengthDay.Checked then
      Result := sDayPeriod
    else if radSalesHistoryLengthWeek.Checked then
      Result := sWeekPeriod
    else if radSalesHistoryLengthFortnight.Checked then
      Result := sFortPeriod
    else if radSalesHistoryLengthMonth.Checked then
      Result := sMonthPeriod
    else if radSalesHistoryLengthQuarter.Checked then
      Result := sQuatPeriod
    else if radSalesHistoryLengthYear.Checked then
      Result := sYearPeriod;

    Result := Result + ' ';
    if chkSalesHistoryBetweenTimes.Checked then
      Result := Result + sPeriodAbbrev
    else
      Result := Result + sPeriod;
    Result := Result + ', ';
  end;

  function GetSalesHistorySelectSQL: string;
  var
    S: string; //SQL length limit is 144 periods, else get 'unexpected end of command' error.
    i, j, k: Integer;
  begin
    //group select clause
    S := 'select itemid, itemabbrev, whendeleted, itemorder, forb, itemgroupid' +
      ', itemgroupabbrev, itemgrouporder, outletid, outletname, superitemgroupid, superitemgroup' +
      ', superitemgroupabbrev, superitemgrouporder, hideforb, remotelocationid, remotelocationname' +
      ', sum(amountsold) amountsold, sum(grosssold) grosssold, sum(salestax) salestax, sum(discount) discount, sum(nettsold) nettsold' +
      ', case coalesce(sum(amountsold), 0) when 0 then 0 else coalesce(sum(grosssold), 0) / coalesce(sum(amountsold), 0) end averageunitprice';

    for i := 1 to NoOfSQLPeriods do
    begin
      S := S + ', sum(as' + IntToStr(i) + ') amountsold' + IntToStr(i);
      S := S + ', sum(ns' + IntToStr(i) + ') nettsold' + IntToStr(i);
      S := S + ', sum(st' + IntToStr(i) + ') salestax' + IntToStr(i);
    end;

    S := S +' from (';

    //sub-select clauses
    for i := 1 to NoOfSubSelects do
    begin
      j := (i - 1) * SALEHIST_PDPERSELECT;

      if i > 1 then
      S := S + ' union all ';

      S := S + 'select itemid, itemabbrev, whendeleted, itemorder, forb, itemgroupid' +
        ', itemgroupabbrev, itemgrouporder, outletid, outletname, superitemgroupid, superitemgroup' +
        ', superitemgroupabbrev, superitemgrouporder, hideforb, remotelocationid, remotelocationname' +
        ', amountsold, grosssold, salestax, discount, nettsold';

      for k := 1 to NoOfSQLPeriods do
      begin
        if (k >= (j + 1)) and (k <= (j + SALEHIST_PDPERSELECT)) then
        begin
          S := S + ', amountsold' + IntToStr(k - j) + ' as' + IntToStr(k);
          S := S + ', nettsold' + IntToStr(k - j) + ' ns' + IntToStr(k);
          S := S + ', salestax' + IntToStr(k - j) + ' st' + IntToStr(k);
        end
        else
        begin
          S := S + ', 0' + ' as' + IntToStr(k); //amountsold
          S := S + ', 0' + ' ns' + IntToStr(k); //nettsold
          S := S + ', 0' + ' st' + IntToStr(k); //salestax
        end;
      end;

      S := S + ' from getsaleshistory (' +
        ':poutletid'  + IntToStr(i) + ', :premotelocationid'  + IntToStr(i) +
        ', :pforb'  + IntToStr(i) + ', :pitemgroupid'  + IntToStr(i);

      for k := 1 to SALEHIST_PDPERSELECT do
      begin
        S := S + ', :fromtime' + IntToStr(j + k) + ', :totime' + IntToStr(j + k);
      end;

      S := S + ', :dailybetweentimes'  + IntToStr(i) + ', :dailystart'  + IntToStr(i) +
        ', :dailyend'  + IntToStr(i) + ', :includezerodate'  + IntToStr(i) + ')';
    end;

    S := S + ')';
    Result := S;
  end;

var
  I: Integer;
  AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond: Word;
begin
  //Todo: set Preview boolean based on No of Periods and only do preview code if true (maybe then don't need GlbSalesHistory.NoOfPeriodsToPrint?)
  //If No of Periods exceeds max periods to print, confirm will only be exported (not previewed)
  if UpDownSalesHistoryTimeSpanQuantity.Position > SALEHIST_MAXPDPRINT then
  begin
    if MessageDlg(Format(sSalesHistRepOnlyExportExceedPeriodPrint, [SALEHIST_MAXPDPRINT]), mtConfirmation, [mbOK, mbCancel], 0) <> mrOk then
      Exit;

    //Ensure the export option is selected
    if not chkExportFile.Checked then
      chkExportFile.Checked := True;
  end;

  try
    formQRSalesHistory := TformQRSalesHistory.Create(Application);

    CreateHistoryTimes;

    NoOfSubSelects := Ceil(GlbSalesHistory.NumberOfTimes / SALEHIST_PDPERSELECT); //get next whole number
    NoOfSQLPeriods := NoOfSubSelects * SALEHIST_PDPERSELECT;

    Title1 := sSalesHistoryTitle1;
    Title2 := '';

    if (chkSalesHistoryBetweenTimes.Checked) then begin
      Title3 := 'Daily ' + FormatDateTime('hh:mm AM/PM', GlbSalesHistory.FromTime[1]) + ' - ' + FormatDateTime('hh:mm AM/PM', GlbSalesHistory.LastTime) +
               ', ' + FormatDateTime('ddd dd mmm yyyy',GlbSalesHistory.FromTime[1]) +
               ' To ' + FormatDateTime('ddd dd mmm yyyy',GlbSalesHistory.LastTime);
    end
    else if ((radSalesHistory15Min.Checked) or (radSalesHistory30Min.Checked) or
      (radSalesHistoryHour.Checked) or (radSalesHistory2Hour.Checked) or
      (radSalesHistory3Hour.Checked) or (radSalesHistory4Hour.Checked)) then begin
      Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',GlbSalesHistory.FromTime[1]),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',GlbSalesHistory.LastTime)]);
    end
    else begin
      Title3 := Format(sDateRange,[FormatDateTime('hh:mm AM/PM ddd dd mmm yyyy',GlbSalesHistory.FromTime[1]),FormatDateTime('ddd dd mmm yyyy',GlbSalesHistory.LastTime)]);
    end;

    Title3 := GetPeriodLengthStr + Title3;

    with dm.qrSalesHistory do begin
      Close;
      UnPrepare;
      SQL.Clear;

      //select clause
      SQL.Text := GetSalesHistorySelectSQL;

      //where clause
      if (radExcludeZeros.Checked) then begin
        SQL.Add('where ((grosssold <> 0.00) or (amountsold <> 0))');
        Title2 := ', ' + sZeroExcluded;
      end
      else if (radIncludeZeros.Checked) then begin
        Title2 := ', ' + sZeroIncluded;
      end
      else if (radOnlyZeros.Checked) then begin
        SQL.Add('where ((grosssold >= -0.01) and (grosssold <= 0.01) and (amountsold = 0))');
        Title2 := ', ' + sZeroOnly;
      end;

      //group by clause
      SQL.Add('group by itemid, itemabbrev, whendeleted, itemorder, forb, itemgroupid' +
        ', itemgroupabbrev, itemgrouporder, outletid, outletname, superitemgroupid, superitemgroup' +
        ', superitemgroupabbrev, superitemgrouporder, hideforb, remotelocationid, remotelocationname');

      //order by clause
      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, itemabbrev');
        Title2 := Title2 + ', ' + sNetSold;
      end;

      //set sub-select param values (NB: from/totime param values set afterwards)
      for i := 1 to NoOfSubSelects do
      begin
        if (radItemGroup.Checked) then begin
          ParamByName('pitemgroupid' + IntToStr(i)).AsInteger := Glbs.ItemGroupID;
          ParamByName('pforb' + IntToStr(i)).AsString := '';
          if i = 1 then
            Title2 := ', ' + Glbs.ItemGroup + Title2;
        end
        else begin
          ParamByName('pitemgroupid' + IntToStr(i)).AsInteger := -1;
          if (radFAndB.Checked) then begin
            ParamByName('pforb' + IntToStr(i)).AsString := '';
            if i = 1 then
              Title2 := ', ' + sFoodBev + Title2;
          end
          else if (radFOnly.Checked) then begin
            ParamByName('pforb' + IntToStr(i)).AsString := 'F';
            if i = 1 then
              Title2 := ', ' + sFood + Title2;
          end
          else begin
            ParamByName('pforb' + IntToStr(i)).AsString := 'B';
            if i = 1 then
              Title2 := ', ' + sBev + Title2;
          end;
        end;

        if (cmbOutlets.ItemIndex = 0) then begin
          if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
            ParamByName('poutletid' + IntToStr(i)).AsInteger := -1;
          end
          else begin
            ParamByName('poutletid' + IntToStr(i)).AsInteger := -2;
          end;
          if i = 1 then
            Title2 := sAllOutlets + Title2;
        end
        else begin
          ParamByName('poutletid' + IntToStr(i)).AsInteger := Glbs.OutletID;
          if i = 1 then
            Title2 := Glbs.OutletName + Title2;
        end;

        if (cmbRemoteLocations.ItemIndex = 0) then begin
          if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
            ParamByName('premotelocationid' + IntToStr(i)).AsInteger := -1;
          end
          else begin
            ParamByName('premotelocationid' + IntToStr(i)).AsInteger := -2;
          end;
        end
        else begin
          ParamByName('premotelocationid' + IntToStr(i)).AsInteger := Glbs.RemoteLocationID;
        end;

        BoolAsParam(ParamByName('dailybetweentimes' + IntToStr(i)), chkSalesHistoryBetweenTimes.Checked);
        ParamByName('dailystart' + IntToStr(i)).AsDateTime := GlbSalesHistory.DailyStart;
        ParamByName('dailyend' + IntToStr(i)).AsDateTime := GlbSalesHistory.DailyEnd;
        BoolAsParam(ParamByName('includezerodate' + IntToStr(i)), GlbSalesHistory.IncludeZeroDate);
      end;

      //set from/totime param values
      for I := 1 to NoOfSQLPeriods do begin
        //NB: The from/totimes are being decoded/encoded to get around a strange bug,
        //where midnight values (i.e. with no time value) that have been set via time slice
        //calculation don't work as expected (even though values appear correct on inspection)
        DecodeDateTime(GlbSalesHistory.FromTime[I], AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond);
        ParamByName('fromtime' + IntToStr(I)).AsDateTime := EncodeDateTime(AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond);

        DecodeDateTime(GlbSalesHistory.ToTime[I], AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond);
        ParamByName('totime' + IntToStr(I)).AsDateTime := EncodeDateTime(AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond);
      end;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sHisRepDlgTitle;
      SaveDialog.FileName := 'SalesHistory' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSalesHistory.WBCSV.FileName := SaveDialog.FileName;
        formQRSalesHistory.WBCSV.OpenToWrite;
      end
      else
      begin
        //If No of Periods exceeds max periods to print, will only be exported (not previewed)
        if GlbSalesHistory.NumberOfTimes > SALEHIST_MAXPDPRINT then
          Exit; //don't run report if won't be exported
      end;
    end;

    formQRSalesHistory.QRSalesHistory.ReportTitle := sHisRepTitle;

    //If No of Periods exceeds max periods to print, will only be exported (not previewed)
    if GlbSalesHistory.NumberOfTimes > SALEHIST_MAXPDPRINT then
      formQRSalesHistory.QRSalesHistory.Prepare //run report but don't show preview screen
    else
      formQRSalesHistory.QRSalesHistory.Preview;
  finally
    formQRSalesHistory.Free;
  end;
end;

{******************************************************************************}

procedure TformReports.ShowStaffDetailsReport;
var
  FromDate: TDateTime;
begin
  try
    { RESET GLOBAL TITLES }
    Title1 := sStaffDetailsTitle1;
    Title2 := '';
    Title3 := '';

    formQRStaffDetails := TformQRStaffDetails.Create(Application);

    with dm.qrStaffDetails do
    begin
	  Close;
      UnPrepare;
      SQL.Clear;
	    SQL.Add('select s.staffid, s.staffname, s.staffno, s.staffpin, s.whenbegin, s.staffinactive,');
	    SQL.Add('s.crolid, r.crol,');
	    SQL.Add('s.remoteoverridegroupid, g.remoteoverridegroupname, ');
	    SQL.Add('gl.remotelocationid, l.remotelocationname');
	    SQL.Add('from staff s');
	    SQL.Add('left join crol r on s.crolid = r.crolid');
	    SQL.Add('left join remoteoverridegroup g on s.remoteoverridegroupid = g.remoteoverridegroupid and g.overridetaskstaff = 1');
	    SQL.Add('left join remoteoverridegrouploc gl on g.remoteoverridegroupid = gl.remoteoverridegroupid and gl.whendeleted is null ');
	    SQL.Add('left join remotelocation l on gl.remotelocationid = l.remotelocationid');
	    SQL.Add('where s.whenend is null ');

      if radStaffAllStaff.Checked then
        Title2 := sAllStaff
      else if radStaffCurrentStaff.Checked then
      begin
        SQL.Add('and STAFFINACTIVE = 0');
        Title2 := sCurrentStaff;
      end
      else if radStaffPastStaff.Checked then
      begin
        SQL.Add('and STAFFINACTIVE = 1');
        Title2 := sPastStaff;
      end;

      if SpinEdtStaffYrsAgo.Value > 0 then
      begin
        FromDate := IncYear(Now, -SpinEdtStaffYrsAgo.Value);
        SQL.Add('and WHENBEGIN <= :pfromdate');
        ParamByName('pfromdate').AsDateTime := FromDate;

        Title2 := Title2 + ', ' + sWorkedForYears + IntToStr(SpinEdtStaffYrsAgo.Value);
      end;

      if cmboBoxStaffLocation.ItemIndex > 0 then
      begin
        SQL.Add('and l.remotelocationname = :plocation');
        ParamByName('plocation').AsString := cmboBoxStaffLocation.Text;

        Title2 := Title2 + ', ' + cmboBoxStaffLocation.Text;
      end;
      Open;
    end;

    { FINAL COMPOSED TITLE }
    Title2 := Glbs.OutletName + ' - ' + Title2;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffDetailsDlgTitle;
      SaveDialog.FileName := 'StaffDetails' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRStaffDetails.WBCSV.FileName := SaveDialog.FileName;
        formQRStaffDetails.WBCSV.OpenToWrite;
      end;
    end;

    formQRStaffDetails.QRStaffDetails.ReportTitle := sStaffDetailsRepTitle;
    formQRStaffDetails.QRStaffDetails.Preview;
  finally
    formQRStaffDetails.Free;
  end;
end;


{******************************************************************************}

procedure TformReports.ShowStaffSalesReport;
begin
  try
    formQRStaffSales := TformQRStaffSales.Create(Application);

    Title1 := sStaffTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrStaffSales do begin
      Close;
      UnPrepare;
      SQL.Clear;

      if radSalesByOrderingStaff.Checked then
      begin
        SQL.Add('select * from getstaffsales_order' +
        ' (:poutletid, :premotelocationid, :pforb, :pstaffid, :pitemgroupid, :fromtime, :totime)');
        Title2 := ', Ordering Staff ';
      end
      else
      begin
        SQL.Add('select * from getstaffsales' +
        ' (:poutletid, :premotelocationid, :pforb, :pstaffid, :pitemgroupid, :fromtime, :totime)');
        Title2 := ', Invoicing Staff ';
      end;

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := Title2 + ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radStaffAll.Checked) then begin
        ParamByName('pstaffid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllStaffTitle2;
      end
      else begin
        if (DBGridStaffMembers.SelectedRows.Count > 1) then
        begin
          ParamByName('pstaffid').AsString := GetSelectedIDString(DBGridStaffMembers, 'staffid');
          Title2 := Title1 + ', ' + sSelectedStaff;
          formQRStaffSales.MultipleStaff := True;
        end
        else
        begin
          ParamByName('pstaffid').AsInteger := Glbs.StaffID;
          Title1 := Title1 + ', ' + Glbs.StaffName;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffDlgTitle;
      SaveDialog.FileName := 'StaffSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRStaffSales.WBCSV.FileName := SaveDialog.FileName;
        formQRStaffSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRStaffSales.QRStaffSales.ReportTitle := sStaffRepTitle;
    formQRStaffSales.QRStaffSales.Preview;
  finally
    formQRStaffSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowSaleCategorySalesReport;
begin
  try
    formQRSaleCategorySales := TformQRSaleCategorySales.Create(Application);

    Title1 := sSalesCatTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrSaleCategorySales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getsalecategorysales (:poutletid, :premotelocationid, :pforb, :psalecategoryid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, salecategoryid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radSaleCategoryAll.Checked) then begin
        ParamByName('psalecategoryid').AsInteger := -1;
        Title1 := Title1 + ', ' +  sAllSaleCat;
      end
      else begin
        ParamByName('psalecategoryid').AsInteger := Glbs.SaleCategoriesSaleCategoryID;
        Title1 := Title1 + ', ' + Glbs.SaleCategoriesSaleCategory;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sSalesCatDlgTitle;
      SaveDialog.FileName := 'SaleCategorySales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSaleCategorySales.WBCSV.FileName := SaveDialog.FileName;
        formQRSaleCategorySales.WBCSV.OpenToWrite;
      end;
    end;

    formQRSaleCategorySales.QRSaleCategorySales.ReportTitle := sSalesCatRepTitle;
    formQRSaleCategorySales.QRSaleCategorySales.Preview;
  finally
    formQRSaleCategorySales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowCashTotalsReport;
begin
  try
    formQRCashTotals := TformQRCashTotals.Create(Application);

    Title1 := sCashRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrCashTotals do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getcashuptotals (:poutletid, :premotelocationid, :fromtime, :totime, :showemptytenders)');

      if (radCashTotalsCashOnly.Checked) then begin
        SQL.Add('  where (tenderlinetypeid = 4)');
      end;

      if (radCashTotalsOrderCashupID.Checked) then begin
        SQL.Add('order by cashupid, tenderlinetypeorder');
        Title2 := ', ' +  sByCashup;
      end
      else if (radCashTotalsOrderTill.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, tillname, cashupid, tenderlinetypeorder');
        Title2 := ', ' + sByTill;
      end;

      if (radCashTotalsCashOnly.Checked) then begin
        Title2 := ', ' +  sCashOnly + Title2;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      BoolAsParam(ParamByName('showemptytenders'), radCashTotalsAllDetailsAllTenders.Checked);

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sCashTotDlgTitle;
      SaveDialog.FileName := 'CashTotals' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRCashTotals.WBCSV.FileName := SaveDialog.FileName;
        formQRCashTotals.WBCSV.OpenToWrite;
      end;
    end;

    formQRCashTotals.QRCashTotals.ReportTitle := sCashTotRepTitle;
    formQRCashTotals.QRCashTotals.Preview;
  finally
    formQRCashTotals.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowSalesByDateReport;
begin
  try
    formQRSalesByDate := TformQRSalesByDate.Create(Application);

    Title1 := sSalesByDateTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrSalesByDate do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getsalesbydate (:poutletid, :premotelocationid, :pforb, :pitemgroupid, :fromtime, :totime, :periodlength, :returnpricelevels)');

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := ', ' + sSelectedItemGroups + Title2;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := ', ' + Glbs.ItemGroup + Title2;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      BoolAsParam(ParamByName('returnpricelevels'), chkSalesIncludePriceLevels.Checked);
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      ParamByName('periodlength').AsFloat := 1;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sSalesByDateDlgTitle;
      SaveDialog.FileName := 'SalesByDate' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSalesByDate.WBCSV.FileName := SaveDialog.FileName;
        formQRSalesByDate.WBCSV.OpenToWrite;
      end;
    end;

    formQRSalesByDate.QRSalesByDate.ReportTitle := sSalesByDateRepTitle;
    if chkSalesIncludePriceLevels.Checked then
    formQRSalesByDate.QRSalesByDate.Page.Orientation := TPrinterOrientation(1)    //poLandscape
    else
    formQRSalesByDate.QRSalesByDate.Page.Orientation := TPrinterOrientation(0);   //poPortrait
    formQRSalesByDate.QRSalesByDate.Preview;
  finally
    formQRSalesByDate.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountPETransReport;
begin
  try
    formQRAccountPETrans := TformQRAccountPETrans.Create(Application);

    Title1 := sAccPeriodTitle1;
    Title2 := '';
    {if (radPeriodEndAll.Checked) then begin
      Title3 := 'From ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime) +
       ' To ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);
    end
    else begin
      Title3 := Glbs.PeriodEndAccountType + ' at ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',Glbs.PeriodEndWhenEnded);
    end;}
    Title3 := Format(sSpecDate,[Glbs.PeriodEndAccountType,FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',Glbs.PeriodEndWhenEnded)]);


    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAccountPETrans do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getperiodendtransactions(:pperiodendid, :fromtime, :totime, :viewtransactions, :mailout)');

      if (chkViewTransactions.Checked) then begin
        Title2 := Title2 + sTrans;
      end
      else begin
        Title2 := Title2 + sAcctBal;
      end;

      if (chkIncludeZeroBalances.Checked) then begin
        Title2 := Title2 + ', ' + sZeroIncluded;
      end
      else begin
        SQL.Add('  where ((openingbalance <> 0) or (closingbalance <> 0) or (invoiced <> 0) or (paid <> 0))');
        Title2 := Title2 + ', ' + sZeroExcluded;
      end;
 
      SQL.Add('  order by  accountid, transactiondate');

{      if (radPeriodEndAll.Checked) then begin
        ParamByName('pperiodendid').AsInteger := -1;
        ParamByName('fromtime').AsDateTime := FFromTime;
        ParamByName('totime').AsDateTime := FToTime;
      end
      else begin
        ParamByName('pperiodendid').AsInteger := Glbs.PeriodEndID;
        ParamByName('fromtime').AsDateTime := 0;
        ParamByName('totime').AsDateTime := 0;
      end;}

      ParamByName('pperiodendid').AsInteger := Glbs.PeriodEndID;
      ParamByName('fromtime').AsDateTime := 0;
      ParamByName('totime').AsDateTime := 0;

      BoolAsParam(ParamByName('viewtransactions'), chkViewTransactions.Checked);
      BoolAsParam(ParamByName('mailout'), False);

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sAccPeriodDlgTitle;
      SaveDialog.FileName := 'AccountPETrans' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAccountPETrans.WBCSV.FileName := SaveDialog.FileName;
        formQRAccountPETrans.WBCSV.OpenToWrite;
      end;
    end;

    formQRAccountPETrans.qrAccountPETrans.ReportTitle := sAccPeriodRepTitle;
    formQRAccountPETrans.qrAccountPETrans.Preview;
  finally
    formQRAccountPETrans.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountPEStatementReport;
begin
  try
    formQRAccountPEMailout := TformQRAccountPEMailout.Create(Application);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAccountPETrans do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getperiodendtransactions(:pperiodendid, :fromtime, :totime, :viewtransactions, :mailout)');

      if (not chkIncludeZeroBalances.Checked) then begin
        SQL.Add('  where ((openingbalance <> 0) or (closingbalance <> 0) or (invoiced <> 0) or (paid <> 0))');
      end;

      SQL.Add('  order by accountid, transactiondate');
{      if (radPeriodEndAll.Checked) then begin
        ParamByName('pperiodendid').AsInteger := -1;
        ParamByName('fromtime').AsDateTime := FFromTime;
        ParamByName('totime').AsDateTime := FToTime;
      end
      else begin
        ParamByName('pperiodendid').AsInteger := Glbs.PeriodEndID;
        ParamByName('fromtime').AsDateTime := 0;
        ParamByName('totime').AsDateTime := 0;
      end;}

      ParamByName('pperiodendid').AsInteger := Glbs.PeriodEndID;
      ParamByName('fromtime').AsDateTime := 0;
      ParamByName('totime').AsDateTime := 0;

      BoolAsParam(ParamByName('viewtransactions'), True);
      BoolAsParam(ParamByName('mailout'), True);

      Open;
    end;

    formQRAccountPEMailout.qrAccountPEMailout.ReportTitle := sAccPeriodMailoutRepTitle;
    formQRAccountPEMailout.qrAccountPEMailout.Preview;
  finally
    formQRAccountPEMailout.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowClockInsReport;
begin
  try
    formQRClockIns := TformQRClockIns.Create(Application);

    Title1 := sClockInRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrClockIns do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getclockins(:premotelocationid, :clockintype, :fromtime, :totime, :showallstaff, :needoutletid)');

      if (radStaffSelected.Checked) then begin
        SQL.Add('where (staffid = ' + IntToStr(Glbs.StaffID) + ')');
        SQL.Add('order by remotelocationname, clockintime');
        Title2 := Glbs.StaffName;
      end
      else begin
        Title2 := sAllStaffTitle2;
        if (radClockInsOrderStaffName.Checked) then begin
          if (ckbGroupRole.Checked) then
          begin
            SQL.Add('order by remotelocationname, staffrolename, staffname, clockintime');
          end
          else
          begin
            SQL.Add('order by remotelocationname, staffname, clockintime');
          end;
          Title2 := Title2 + ', ' +  sByStaffName;
        end
        else if (radClockInsOrderClockInTime.Checked) then begin
          SQL.Add('order by clockintime, staffname');
          Title2 := Title2 + ', ' + sByClockInTime;
        end
        else if (radClockInsOrderClockOutTime.Checked) then begin
          SQL.Add('order by clockouttime, staffname');
          Title2 := Title2 + ', ' +  sByClockOutTime;
        end;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
          formQRClockIns.ReportRemoteLocationID := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
          formQRClockIns.ReportRemoteLocationID := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
        formQRClockIns.ReportRemoteLocationID := Glbs.RemoteLocationID;
      end;

      ParamByName('clockintype').AsInteger := 2;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      BoolAsParam(ParamByName('showallstaff'), chkShowAllStaff.Checked);
      ParamByName('needoutletid').AsInteger := 0;

      formQRClockIns.ReportFromTime := FFromTime;
      formQRClockIns.ReportToTime := FToTime;
      formQRClockIns.ReportShowAllStaff := chkShowAllStaff.Checked;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sClockInDlgTitle;
      SaveDialog.FileName := 'Clock Ins' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRClockIns.WBCSV.FileName := SaveDialog.FileName;
        formQRClockIns.WBCSV.OpenToWrite;
      end;
    end;

    formQRClockIns.StartTime := FFromTime;
    formQRClockIns.qrClockIns.ReportTitle := sClockInRepTitle;
    formQRClockIns.qrClockIns.Preview;
  finally
    formQRClockIns.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountSummariesReport;
var
   AccountID, AccountTypeID: Integer;
begin
  if CheckDate then begin
    AccountID := -1;
    AccountTypeID := -1;
    if (radAccountSelected.Checked) then begin
      AccountID := Glbs.AccountID;
    end;
    if (radAccountSelectedType.Checked) then begin
      AccountTypeID := Glbs.AccountTypeID;
    end;
    if DM.GetAccountSummary(FFromTime,FToTime,AccountID,AccountTypeID)> 0 then begin
      formQRAccountSummary := TformQRAccountSummary.Create(Application);
      try
        formQRAccountSummary.StartPeriod := FFromTime;
        formQRAccountSummary.EndPeriod := FToTime;
        formQRAccountSummary.QRAccountSummary.Preview;
      finally
        formQRAccountSummary.Free;
      end;
    end
    else begin
      MessageDlg(sNoSummryAvailMsg,mtInformation,[mbok],0);
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowStaffTipsReport;
begin
  try
    formQRStaffTips := TformQRStaffTips.Create(Application);

    Title1 := sStaffTipsTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrStaffTips do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getstafftips(:poutletid, :premotelocationid, :pstaffid, :ptenderlinetypeid, :fromtime, :totime)');
      SQL.Add('  order by outletname, remotelocationname, staffname, whentendered');

      if (radStaffSelected.Checked) then begin
        ParamByName('pstaffid').AsInteger := Glbs.StaffID;
        Title2 := Glbs.StaffName;
      end
      else begin
        Title2 := sAllStaffTitle2;
        ParamByName('pstaffid').AsInteger := -1;
{        if (radClockInsOrderStaffName.Checked) then begin
          SQL.Add('order by staffname, clockintime');
          Title2 := Title2 + ', By Staffname';
        end
        else if (radClockInsOrderClockInTime.Checked) then begin
          SQL.Add('order by clockintime, staffname');
          Title2 := Title2 + ', By Clock In Time';
        end
        else if (radClockInsOrderClockOutTime.Checked) then begin
          SQL.Add('order by clockouttime, staffname');
          Title2 := Title2 + ', By Clock Out Time';
        end;}
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + ', ' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('ptenderlinetypeid').AsInteger := -1;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffTipsDlgTitle;
      SaveDialog.FileName := 'Staff Tips' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRStaffTips.WBCSV.FileName := SaveDialog.FileName;
        formQRStaffTips.WBCSV.OpenToWrite;
      end;
    end;

    formQRStaffTips.StartTime := FFromTime;
    formQRStaffTips.QRStaffTips.ReportTitle := sStaffTipsRepTitle;
    formQRStaffTips.QRStaffTips.Preview;
  finally
    formQRStaffTips.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowNoSalesReport;
begin
  try
    formQRNoSales := TformQRNoSales.Create(Application);

    Title1 := sNoSalesTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrNoSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getnosalesreport(:poutletid, :premotelocationid, :pstaffid, :fromtime, :totime)');
      SQL.Add('  order by outletname, remotelocationname, staffname, whentendered');

      if (radStaffSelected.Checked) then begin
        ParamByName('pstaffid').AsInteger := Glbs.StaffID;
        Title2 := Glbs.StaffName;
      end
      else begin
        Title2 := sAllStaffTitle2;
        ParamByName('pstaffid').AsInteger := -1;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sNoExportDlgTitle;
      SaveDialog.FileName := 'No Sales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRNoSales.WBCSV.FileName := SaveDialog.FileName;
        formQRNoSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRNoSales.QRNoSales.ReportTitle := sWaitorNoSalesRepTitle;
    formQRNoSales.QRNoSales.Preview;
  finally
    formQRStaffTips.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowItemModSalesReport;
begin
  try
    formQRItemModSales := TformQRItemModSales.Create(Application);
    Title1 := 'Item/Mod Sales Report';
    Title2 := '';
    Title3 := 'From ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime) +
     ' To ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrItemModSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getitemmodsales (:poutletid, :premotelocationid, :pforb, :pmodgroupid, :pitemgroupid, :fromtime, :totime)');

      if (radShowModGroupsExludeCustom.Checked) then
      begin
        SQL.Add('where modifierid <> 0 or modifierid is null');
      end
      else if (radShowModGroupsOnlyCustom.Checked) then
      begin
        SQL.Add('where modifierid = 0 or modifierid is null');
      end;

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Group / Item Order';
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Units Sold';
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Average Unit Price';
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Gross Value Sold';
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Discount Given';
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder, modgrouporder, modorder, modifierid');
        Title2 := ', By Nett Value Sold';
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', Food & Beverage' + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', Food' + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', Beverage' + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := 'All Outlets' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radShowModGroupsAll.Checked) or (radShowModGroupsExludeCustom.Checked) or (radShowModGroupsOnlyCustom.Checked) then
      begin
        ParamByName('pmodgroupid').AsInteger := -1;
        if (radShowModGroupsAll.Checked) then
          Title1 := Title1 + ', All Modifiers'
        else if (radShowModGroupsExludeCustom.Checked) then
          Title1 := Title1 + ', All Defined Modifiers'
        else
          Title1 := Title1 + ', Only Custom Modifiers';
      end
      else begin
        if (radModGroupAll.Checked) then begin
          ParamByName('pmodgroupid').AsInteger := 0;
          Title1 := Title1 + ', Only Grouped Modifier';
        end
        else begin
          ParamByName('pmodgroupid').AsInteger := Glbs.ModGroupID;
          Title1 := Title1 + ', ' + Glbs.ModGroup;
        end;
      end;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := 'Item/Mod Sales Report file export';
      SaveDialog.FileName := 'ItemModSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRItemModSales.WBCSV.FileName := SaveDialog.FileName;
        formQRItemModSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRItemModSales.QRItemModSales.ReportTitle := 'WizBang Waiter Item/Mods Sales Report';
    formQRItemModSales.QRItemModSales.Preview;
  finally
    formQRItemModSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowModItemsReport;
begin
  try
    formQRModItems := TformQRModItems.Create(Application);
    Title1 := 'Modifier with Items Report';
    Title2 := '';
    Title3 := 'From ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime) +
     ' To ' + FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrModItems do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getmodswithitems(:poutletid, :premotelocationid, :pforb, :pitemgroupid, :pmodgroupid, :pricetype,  :fromtime, :totime, :showsharing)');

      if (radShowModGroupsExludeCustom.Checked) then
      begin
        SQL.Add('where modifierid <> 0');
      end
      else if (radShowModGroupsOnlyCustom.Checked) then
      begin
        SQL.Add('where modifierid = 0');
      end;

      if (radModifiersOrderDefault.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, forb desc, modifierorder, modifier');
        Title2 := ', By Default Order';
      end
      else if (radModifiersOrderModifier.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, forb desc, modifier');
        Title2 := ', By Item';
      end
      else if (radModifiersOrderUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, nettqty desc, forb desc, modifier');
        Title2 := ', By Units';
      end
      else if (radModifiersOrderPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, modprice desc, forb desc, modifier');
        Title2 := ', By Average Price';
      end
      else if (radModifiersOrderValue.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, nett desc, forb desc, modifier');
        Title2 := ', By Value';
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', Food & Beverage' + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', Food' + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', Beverage' + Title2;
        end;
      end;

      if (radExcludeZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := 1;
        Title2 := Title2 + ', Zero Values Excluded';
      end
      else if (radIncludeZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := -1;
        Title2 := Title2 + ', Zero Values Included';
      end
      else if (radOnlyZeros.Checked) then begin
        ParamByName('pricetype').AsInteger := 0;
        Title2 := Title2 + ', Zero Values ONLY';
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := 'All Outlets' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radShowModGroupsAll.Checked) or (radShowModGroupsExludeCustom.Checked) or (radShowModGroupsOnlyCustom.Checked) then
      begin
        ParamByName('pmodgroupid').AsInteger := -1;
        if (radShowModGroupsAll.Checked) then
          Title1 := Title1 + ', All Modifiers'
        else if (radShowModGroupsExludeCustom.Checked) then
          Title1 := Title1 + ', All Defined Modifiers'
        else
          Title1 := Title1 + ', Only Custom Modifiers';
      end
      else begin
        if (radModGroupAll.Checked) then begin
          ParamByName('pmodgroupid').AsInteger := 0;
          Title1 := Title1 + ', Only Grouped Modifier';
        end
        else begin
          ParamByName('pmodgroupid').AsInteger := Glbs.ModGroupID;
          Title1 := Title1 + ', ' + Glbs.ModGroup;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      BoolAsParam(ParamByName('showsharing'), AppDetails.ShowSharing);

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := 'Modifier With Items Report file export';
      SaveDialog.FileName := 'ModifierItems' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRModItems.WBCSV.FileName := SaveDialog.FileName;
        formQRModItems.WBCSV.OpenToWrite;
      end;
    end;

    formQRModItems.QRModItems.ReportTitle := 'WizBang Waiter Modifier With Items Report';
    formQRModItems.QRModItems.Preview;
  finally
    formQRModItems.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowOrderByBumpedReport;
begin
  try
    formQROrdersByBumped := TformQROrdersByBumped.Create(Application);

    Title1 := sAvgBumpTitle;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then
    begin
      Exit;
    end;

    with DM.qrOrdersbumped do
    begin
      SQL.Clear;
      SQL.Add('select * from getorderlinebumpinfo (:poutletid ,:premotelocationid, :pforb, :pitemgroupid,:fromtime, :totime, :NEEDEXACTSTARTTIME)');
      if (radItemGroup.Checked) then
      begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := ', ' + Glbs.ItemGroup + Title2;
      end
      else
      begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;
      ParamByName('NEEDEXACTSTARTTIME').AsInteger := AppDetails.NeedExactStartTimeForBumped;

      if (cmbOutlets.ItemIndex = 0) then begin
        ParamByName('poutletid').AsInteger := -1;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + ', ' + Title2;
      end;
      ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then
    begin
      SaveDialog.Title := sNoExportDlgTitle;
      SaveDialog.FileName := 'OrderBumped' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQROrdersByBumped.WBCSV.FileName := SaveDialog.FileName;
        formQROrdersByBumped.WBCSV.OpenToWrite;
      end;
    end;

    formQROrdersByBumped.QROrders.ReportTitle := sAvgBumpTitle;
    formQROrdersByBumped.QROrders.Preview;
  finally
    formQROrdersByBumped.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowItemByBumpedReport;
begin
  try
    formQRItemsByBumped := TformQRItemsByBumped.Create(Application);

    Title1 := sAvgItemBumpTitle;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then
    begin
      Exit;
    end;

    with DM.qrOrdersbumped do
    begin
      SQL.Clear;
      SQL.Add('select * from getorderlinebumpinfo (:poutletid ,:premotelocationid, :pforb, :pitemgroupid,:fromtime, :totime, :NEEDEXACTSTARTTIME) order by itemid');
      if (radItemGroup.Checked) then
      begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := ', ' + Glbs.ItemGroup + Title2;
      end
      else
      begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;
      ParamByName('NEEDEXACTSTARTTIME').AsInteger := AppDetails.NeedExactStartTimeForBumped;

      if (cmbOutlets.ItemIndex = 0) then begin
        ParamByName('poutletid').AsInteger := -1;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + ', ' + Title2;
      end;
      ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then
    begin
      SaveDialog.Title := sNoExportDlgTitle;
      SaveDialog.FileName := 'ItemBumped' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRItemsByBumped.WBCSV.FileName := SaveDialog.FileName;
        formQRItemsByBumped.WBCSV.OpenToWrite;
      end;
    end;

    formQRItemsByBumped.QROrders.ReportTitle := sAvgItemBumpTitle;
    formQRItemsByBumped.QROrders.Preview;
  finally
    formQRItemsByBumped.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowItemCountByBumpedReport;
begin
  try
    formQRItemCountByBumped := TformQRItemCountByBumped.Create(Application);

    Title1 := sAvgItemBumpTitle;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then
    begin
      Exit;
    end;

    with DM.qrOrdersbumped do
    begin
      SQL.Clear;
      SQL.Add('select * from getorderlinebumpinfo (:poutletid ,:premotelocationid, :pforb, :pitemgroupid,:fromtime, :totime, :NEEDEXACTSTARTTIME) order by itemid');
      if (radItemGroup.Checked) then
      begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := ', ' + Glbs.ItemGroup + Title2;
      end
      else
      begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;
      ParamByName('NEEDEXACTSTARTTIME').AsInteger := AppDetails.NeedExactStartTimeForBumped;

      if (cmbOutlets.ItemIndex = 0) then begin
        ParamByName('poutletid').AsInteger := -1;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + ', ' + Title2;
      end;
      ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then
    begin
      SaveDialog.Title := sNoExportDlgTitle;
      SaveDialog.FileName := 'ItemCountBumped' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRItemCountByBumped.WBCSV.FileName := SaveDialog.FileName;
        formQRItemCountByBumped.WBCSV.OpenToWrite;
      end;
    end;

    formQRItemCountByBumped.QROrders.ReportTitle := sAvgItemBumpTitle;
    formQRItemCountByBumped.QROrders.Preview;
  finally
    formQRItemCountByBumped.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowSalePeriodSalesReport;
begin
  try
    formQRSalePeriodSales := TformQRSalePeriodSales.Create(Application);

    Title1 := sSalePeriodTitle;
    Title2 := '';
    Title3 := Format(sDateRange, [FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FFromTime), FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrSalePeriodSales do
    begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from GETITEMSALECATEGORYPERIODSALES (:poutletid, :premotelocationid, :pforb, :psaleperiodid, :pitemgroupid, :fromtime, :totime,:deleteditems)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, TIMEPERIODNAME, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radSalePeriodAll.Checked) then begin
        ParamByName('psaleperiodid').AsInteger := -1;
        Title1 := Title1 + ', ' +  sAllSalePeriod;
      end
      else begin
        ParamByName('psaleperiodid').AsInteger := Glbs.SalePeriodID;
        Title1 := Title1 + ', ' + Glbs.SalePeriodName;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      ParamByName('deleteditems').AsInteger := Integer(not dm.ItemGroupsCurrent);
      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sSalePeriodDlgTitle;
      SaveDialog.FileName := 'SalePeriodSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSalePeriodSales.WBCSV.FileName := SaveDialog.FileName;
        formQRSalePeriodSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRSalePeriodSales.QRSalePeriodSales.ReportTitle := sSalePeriodRepTitle;
    formQRSalePeriodSales.QRSalePeriodSales.Preview;
  finally
    formQRSalePeriodSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountTransactionReport;
begin
  try
    formQRAccountTransaction := TformQRAccountTransaction.Create(Application);
    Title1 := sAcctTransRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    if(chkSBExtendedDetail.Checked) then begin
      formQRAccountTransaction.lblWhoTransaction.Caption := sWhoTransacton;
      formQRAccountTransaction.lblWhenTransaction.Caption := sWhenTransaction;
      if (not chkRemNumbers.Checked) then begin
        formQRAccountTransaction.lblInvoiceNo.Caption := sInvoiceNo;
        formQRAccountTransaction.lblTenderNo.Caption := sTenderNo;
        formQRAccountTransaction.txtInvoiceNo.DataField := 'INVOICENO';
        formQRAccountTransaction.txtTenderNo.DataField := 'TENDERNO';
      end
      else begin
        formQRAccountTransaction.lblInvoiceNo.Caption := sRemInvoiceNo;
        formQRAccountTransaction.lblTenderNo.Caption := sRemTenderNo;
        formQRAccountTransaction.txtInvoiceNo.DataField := 'REMINVOICENO';
        formQRAccountTransaction.txtTenderNo.DataField := 'REMTENDERNO';
      end;
    end
    else begin
        if (not chkSBTransType.Checked and not radAccountAll.Checked) and ckbGroupAccount.Checked then begin
          formQRAccountTransaction.QRExpr2.Enabled := True;
          formQRAccountTransaction.QRExpr2.Expression := '''Acct #: '' + qrAccountTransaction.ACCOUNTNO + '' - '' + qrAccountTransaction.AccountName';
        end
        else if (chkSBTransType.Checked) then begin
          formQRAccountTransaction.QRExpr2.Enabled := True;
          formQRAccountTransaction.QRExpr2.Expression := 'IF(qrAccountTransaction.loyaltyreward = '''', qrAccountTransaction.TransactionType,qrAccountTransaction.TransactionType + '' - Reward Redeemed: '' + qrAccountTransaction.loyaltyreward)';
        end;

        formQRAccountTransaction.lblWhoTransaction.Caption := '';
        formQRAccountTransaction.lblWhenTransaction.Caption := '';
        formQRAccountTransaction.lblInvoiceNo.Caption := '';
        formQRAccountTransaction.lblTenderNo.Caption := '';
        formQRAccountTransaction.txtInvoiceNo.DataField := '';
        formQRAccountTransaction.txtTenderNo.DataField := '';
    end;

    // set caption empty, in QRAccountTransactionBeforePrint clear datafields and subtotals from displaying if caption = ''
    case (frmSBOptions.ItemIndex) of
      0: begin
        formQRAccountTransaction.lblLoyalty1Change.Caption := '';
        formQRAccountTransaction.lblLoyalty2Change.Caption := '';
      end;
      1: begin
        //leave as is....
      end;
      2: begin                                                     
        formQRAccountTransaction.lblAcctBalChange.Caption := '';
        formQRAccountTransaction.lblLoyalty2Change.Caption := '';
      end;
      3: begin
        formQRAccountTransaction.lblAcctBalChange.Caption := '';
        formQRAccountTransaction.lblLoyalty1Change.Caption := '';
      end;
    end;



    with dm.qrAccountTransaction do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select P.OUTLETID, P.OUTLETNAME, P.REMOTELOCATIONID, P.REMOTELOCATIONNAME, P.LASTTRANSRECEIVED, P.ACCOUNTTYPEID, P.ACCOUNTTYPE, P.ACCOUNTID, P.ACCOUNTNO, P.ACCOUNTNAME, P.LASTTRANSACTION,');
      SQL.Add('SUM(P.CURRENTBALANCECHANGE) CURRENTBALANCECHANGE, cast(SUM(P.LOYALTYPOINTS1CHANGE) as Integer) LOYALTYPOINTS1CHANGE, cast(SUM(P.LOYALTYPOINTS2CHANGE)as Integer) LOYALTYPOINTS2CHANGE,');
      SQL.Add('P.TRANSACTIONTYPE, P.LOYALTYREWARD,');
      SQL.Add('P.INVOICENO, P.REMINVOICENO,');
      SQL.Add('P.TENDERNO, P.REMTENDERNO,');
      SQL.Add('P.WHOTRANSACTION, P.WHENTRANSACTION');

      SQL.Add('from GETACCOUNTTRANSACTIONDETAIL (:fromtime,:totime,:poutletid,:premotelocationid,:paccounttypeid,:paccountid,:pschemeorbalance,:pschemeorbalancedetail,:ptransactiontype,:ptransactiondetail) p');

      SQL.Add('GROUP BY P.OUTLETID, P.OUTLETNAME, P.REMOTELOCATIONID, P.REMOTELOCATIONNAME, P.LASTTRANSRECEIVED, P.ACCOUNTTYPEID, P.ACCOUNTTYPE, P.ACCOUNTID, P.ACCOUNTNO, P.ACCOUNTNAME, P.LASTTRANSACTION,');
      SQL.Add('P.TRANSACTIONTYPE, P.LOYALTYREWARD,');
      SQL.Add('P.INVOICENO, P.WHOTRANSACTION, P.WHENTRANSACTION, P.REMINVOICENO,');
      SQL.Add('P.TENDERNO, P.REMTENDERNO, ');
      SQL.Add('P.WHOTRANSACTION, P.WHENTRANSACTION');
      //if (radGroup.Checked) then begin
        SQL.Add('order by p.outletname, p.remotelocationname, p.accounttype, p.accountname, p.transactiontype, p.loyaltyreward, P.WHENTRANSACTION DESC');
      //  Title2 := ', ';
      //end;


      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Visible)) and
             ((ckbGroupRemoteLocation.Checked) and (ckbGroupRemoteLocation.Visible))  then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end; 
        Title2 := Title2 + ', ' + sAllLoc ;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
        if (Glbs.RemoteLocationID = 0) then begin
          if pnlRemoteLocations.Visible then
            Title2 := Title2 + ', ' + Glbs.RemoteLocationName;
        end
        else
          Title2 := Title2 + ', ' + Glbs.RemoteLocationName;
      end;

      if (radAccountAll.Checked) then begin
        ParamByName('paccountid').AsInteger := 0;
        ParamByName('paccounttypeid').AsInteger := 0;
        Title1 := Title1 + ', ' + sAllAccts;
      end
      else if (radAccountSelected.Checked) then begin
        ParamByName('paccountid').AsInteger := Glbs.AccountID;
        ParamByName('paccounttypeid').AsInteger := 0;
        Title1 := Title1 + ', ' + Glbs.AccountName;
      end
      else begin
        ParamByName('paccountid').AsInteger := 0;
        ParamByName('paccounttypeid').AsInteger := Glbs.AccountTypeID;
        Title1 := Title1 + ', ' + Glbs.AccountType;
      end;

      if (not (ckbGroupAccount.Checked) and ckbGroupAccount.Enabled) then
        ParamByName('paccountid').AsInteger := -2;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      
      ParamByName('PSCHEMEORBALANCE').AsInteger := (frmSBOptions.ItemIndex-1);
      ParamByName('PSCHEMEORBALANCEDETAIL').AsInteger := frmSBDetails.ItemIndex;
      BoolAsParam(ParamByName('PTRANSACTIONTYPE'),chkSBTransType.Checked);   
      BoolAsParam(ParamByName('PTRANSACTIONDETAIL'),chkSBExtendedDetail.Checked);

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sAcctRepDlgTitle;
      SaveDialog.FileName := 'Account Transactions' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAccountTransaction.WBCSV.FileName := SaveDialog.FileName;
        formQRAccountTransaction.WBCSV.OpenToWrite;
      end;
    end;

    formQRAccountTransaction.QRAccountTransaction.ReportTitle := sAcctTransRepTitle1;
    formQRAccountTransaction.QRAccountTransaction.Preview;
  finally
    formQRAccountTransaction.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowOrdersSalesVoidsReport;
begin
  try
    formQROrdersSalesVoids := TformQROrdersSalesVoids.Create(Application);

    Title1 := sOrdersSalesVoidsTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrOrdersSalesVoids do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getorderssalesvoids (:poutletid, :premotelocationid, :pforb, :pitemgroupid, :fromtime, :totime)');

      if (radOGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemorder');
        OrdersHeader := 'Orders (By Group / Item Order)';
      end
      else if (radOUnits.Checked) then begin
        SQL.Add('order by ordersqty desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemorder');
        OrdersHeader := 'Orders (By Units)';
      end
      else if (radOValue.Checked) then begin
        SQL.Add('order by ordersvalue desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemorder');
        OrdersHeader := 'Orders (By Value)';
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pforb').AsString := '';
        if (DBGridItemGroups.SelectedRows.Count > 1) then
        begin
          ParamByName('pitemgroupid').AsString := GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := ', ' + sSelectedItemGroups + Title2;
          formQROrdersSalesVoids.MultipleItemGroups := True;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := ', ' + Glbs.ItemGroup + Title2;
        end;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sOrdersSalesVoidsDlgTitle;
      SaveDialog.FileName := 'OrdersSalesVoids' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQROrdersSalesVoids.WBCSV.FileName := SaveDialog.FileName;
        formQROrdersSalesVoids.WBCSV.OpenToWrite;
      end;
    end;

    formQROrdersSalesVoids.QROrdersSalesVoids.ReportTitle := sOrdersSalesVoidsRepTitle;
    formQROrdersSalesVoids.QROrdersSalesVoids.Preview;
  finally
    formQROrdersSalesVoids.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccountTabSalesReport;
begin
  try
    formQRAccountTabSales := TformQRAccountTabSales.Create(Application);

    Title1 := sAccountTabSalesTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAccountTabSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select outletid, outletname, remotelocationid, remotelocationname,');
      SQL.Add('    accountid, accountname, accounttypeid, accounttype,');
      SQL.Add('    count(distinct(groupid)) as tabcount,');
      SQL.Add('    sum(fooddisc) as fooddisc, sum(foodonaccount) as foodonaccount, sum(foodtendered) as foodtendered,');
      SQL.Add('    sum(foodprorata) as foodprorata, sum(foodnotaccount) as foodnotaccount, sum(foodtotal) as foodtotal,');
      SQL.Add('    sum(bevdisc) as bevdisc, sum(bevonaccount) as bevonaccount, sum(bevtendered) as bevtendered,');
      SQL.Add('    sum(bevprorata) as bevprorata, sum(bevnotaccount) as bevnotaccount, sum(bevtotal) as bevtotal,');
      SQL.Add('    sum(alldisc) as alldisc, sum(allonaccount) as allonaccount, sum(alltendered) as alltendered,');
      SQL.Add('    sum(allprorata) as allprorata, sum(allnotaccount) as allnotaccount, sum(alltotal) as alltotal');
      SQL.Add('  from getaccounttabsales (:poutletid, :premotelocationid, :fromtime, :totime)');
      SQL.Add(' group by outletid, outletname, remotelocationid, remotelocationname,');
      SQL.Add('    accountid, accountname, accounttypeid, accounttype');
      SQL.Add(' order by outletname, remotelocationname, accounttype, accountname');

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sAccountTabSalesDlgTitle;
      SaveDialog.FileName := 'AccountTabSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAccountTabSales.WBCSV.FileName := SaveDialog.FileName;
        formQRAccountTabSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRAccountTabSales.QRAccountTabSales.ReportTitle := sAccountTabSalesRepTitle;
    formQRAccountTabSales.QRAccountTabSales.Preview;
  finally
    formQROrdersSalesVoids.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowOpenTablesReport;
begin
  try
    formQROpenTables := TformQROpenTables.Create(Application);

    Title1 := sOpenTablesTitle1;
    Title2 := '';
    Title3 := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', Now);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrOpenTablesOLsToPay do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getopentablesolstopay(:poutletid, :premotelocationid)');
      SQL.Add(' order by outletid, outletname, remotelocationid, remotelocationname, salecategoryid, sectionid, tableno, forb, orderlineid');

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sOpenTablesDlgTitle;
      SaveDialog.FileName := 'OpenTabs' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQROpenTables.WBCSV.FileName := SaveDialog.FileName;
        formQROpenTables.WBCSV.OpenToWrite;
      end;
    end;

    formQROpenTables.QROpenTables.ReportTitle := sOpenTablesRepTitle;
    formQROpenTables.QROpenTables.Preview;
  finally
    formQROpenTables.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowTableSummaryReport;
begin
  try
    formQRTableSummary := TformQRTableSummary.Create(Application);

    Title1 := sTablesSummaryTitle1;
    Title2 := '';
    if (radTableOptionOpenOnly.Checked) then
    begin
      Title3 := FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', Now);
    end
    else
    begin
      Title3 := Format(sDateRange, [FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FFromTime), FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FToTime)]);
    end;

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrTabs do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from gettablesreports(:poutletid, :premotelocationid, :pinvoicedthroughaccount, :popentabs, :fromtime, :totime)');
      SQL.Add(' order by outletid, outletname, remotelocationid, remotelocationname, salecategoryid, sectionid, tableno, whenopened');

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      ParamByName('pinvoicedthroughaccount').AsInteger := -1;

      if (radTableOptionIncludeOpen.Checked) then
      begin
        ParamByName('popentabs').AsInteger := -1;
      end
      else if (radTableOptionOpenOnly.Checked) then
      begin
        ParamByName('popentabs').AsInteger := 1;
      end
      else
      begin
        ParamByName('popentabs').AsInteger := 0;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;
      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sTablesSummaryDlgTitle;
      SaveDialog.FileName := 'TabsSummary' + FormatDateTime('yyyymmddhhnn', Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRTableSummary.WBCSV.FileName := SaveDialog.FileName;
        formQRTableSummary.WBCSV.OpenToWrite;
      end;
    end;

    formQRTableSummary.QRTableSummary.ReportTitle := sTablesSummaryRepTitle;
    formQRTableSummary.QRTableSummary.Preview;
  finally
    formQRTableSummary.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowComboSalesReport;
begin
  try
    if radComboOrderCombos.Checked then
    begin
      formQRComboSales := TformQRComboSales.Create(Application);
      formQRComboItemSales := nil;
    end
    else
    begin
      formQRComboItemSales := TformQRComboItemSales.Create(Application);
      formQRComboSales := nil;
    end;

    Title1 := sTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then
    begin
      Exit;
    end;

    with dm.qrCombosSales do
    begin
      Close;
      UnPrepare;
      SQL.Clear;
      if radComboOrderCombos.Checked then
      begin // combo wise
        SQL.Add('select p.itemabbrev, p.comboname, p.comboorder, p.comboid, p.combocount, p.combogroupid, p.combogroupname, p.allowcombineinreports, p.remotelocationid, p.remotelocationname,');
        SQL.Add('    p.itemid, p.itemorder, p.forb, p.itemgroupid, p.itemgroupabbrev, p.itemgrouporder, p.outletid, p.outletname,');
        SQL.Add('    p.superitemgroupid, p.superitemgroup, p.superitemgroupabbrev, p.superitemgrouporder, p.hideforb, p.comboid, p.averageunitprice,');
        SQL.Add('    p.amountsold, p.grosssold, p.salestax, p.discount, p.nettsold, p.whendeleted');
        SQL.Add('  from getcombosales (:poutletid, :premotelocationid, :pforb, :pcomboid, :pitemgroupid, :fromtime, :totime) p');
        SQL.Add(' order by p.outletname,p.remotelocationname,p.comboorder,p.comboid,p.combogroupid,p.superitemgrouporder,p.forb desc,p.itemgrouporder, p.itemgroupabbrev, p.itemorder');
        Title2 := ', ' + sComboOrder;
      end
      else //item wise
      begin
        SQL.Add('select p.itemabbrev, p.comboname, p.comboorder, p.allowcombineinreports, p.remotelocationid, p.remotelocationname,');
        SQL.Add('    p.itemid, p.itemorder, p.forb, p.itemgroupid, p.itemgroupabbrev, p.itemgrouporder, p.outletid, p.outletname,');
        SQL.Add('    p.superitemgroupid, p.superitemgroup, p.superitemgroupabbrev, p.superitemgrouporder, p.hideforb, p.comboid, p.averageunitprice, p.whendeleted,');
        SQL.Add('    sum(p.amountsold)amountsold, sum(p.grosssold)grosssold, sum(p.salestax)salestax, sum(p.discount)discount, sum(p.nettsold)nettsold');
        SQL.Add('  from getcombosales (:poutletid, :premotelocationid, :pforb, :pcomboid, :pitemgroupid, :fromtime, :totime) p');
        SQL.Add(' group by p.itemabbrev, p.comboname, p.comboorder, p.allowcombineinreports, p.remotelocationid, p.remotelocationname,');
        SQL.Add('    p.itemid, p.itemorder, p.forb, p.itemgroupid, p.itemgroupabbrev, p.itemgrouporder, p.outletid, p.outletname,');
        SQL.Add('    p.superitemgroupid, p.superitemgroup, p.superitemgroupabbrev, p.superitemgrouporder, p.hideforb, p.comboid, p.averageunitprice, p.whendeleted');
        SQL.Add(' order by p.outletname, p.remotelocationname,p.superitemgrouporder, p.forb desc, p.itemgrouporder, p.itemgroupabbrev,p.itemorder , p.itemid');
        Title2 := ', ' + sComboItemOrder;
      end;

      if (frmItemGroups.Visible) and (radItemGroup.Checked) then
        ParamByName('pitemgroupid').AsInteger := dm.qrItemGroups.fieldbyname('itemgroupid').AsInteger
      else
        ParamByName('pitemgroupid').AsInteger := -1;

      if (radFAndB.Checked) then begin
        ParamByName('pforb').AsString := '';
        Title2 := ', ' + sFoodBev + Title2;
      end
      else if (radFOnly.Checked) then begin
        ParamByName('pforb').AsString := 'F';
        Title2 := ', ' + sFood + Title2;
      end
      else begin
        ParamByName('pforb').AsString := 'B';
        Title2 := ', ' + sBev + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then
      begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then
        begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else
        begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else
      begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
        Title2 := ', ' + Glbs.RemoteLocationName + Title2;
      end;

      if (cmbOutlets.ItemIndex = 0) then
      begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then
        begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else
        begin
          ParamByName('poutletid').AsInteger := -2;
        end;

        Title2 := sAllOutlets + Title2;
      end
      else
      begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName +  Title2;
      end;


      if (radComboAll.Checked) then begin
        ParamByName('pcomboid').AsInteger := -1;
        Title1 := Title1 + ', ' +  sAllCombo;
      end
      else
      begin
        ParamByName('pcomboid').AsInteger := Glbs.ComboId;
        Title1 := Title1 + ', ' + Glbs.ComboName;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then
    begin
      SaveDialog.Title := sSalesCatDlgTitle;
      SaveDialog.FileName := 'ComboSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then
      begin
        if radComboOrderCombos.Checked then
        begin
          formQRComboSales.WBCSV.FileName := SaveDialog.FileName;
          formQRComboSales.WBCSV.OpenToWrite;
        end
        else
        begin
          formQRComboItemSales.WBCSV.FileName := SaveDialog.FileName;
          formQRComboItemSales.WBCSV.OpenToWrite;
        end;
      end;
    end;

    if radComboOrderCombos.Checked then
    begin
      formQRComboSales.QRComboSales.ReportTitle := sSalesCatRepTitle;
      formQRComboSales.QRComboSales.Preview;
    end
    else
    begin
      formQRComboItemSales.QRComboSales.ReportTitle := sSalesCatRepTitle;
      formQRComboItemSales.QRComboSales.Preview;
    end;
  finally
    if (Assigned(formQRComboSales)) then
    begin
      formQRComboSales.Free;
    end;
    if (Assigned(formQRComboItemSales)) then
    begin
      formQRComboItemSales.Free;
    end;
  end;
end;

procedure TformReports.ShowEventSalesReport;
begin
  try
    formQREventSales := TformQREventSales.Create(Application);
    Title1 := sEventSalesRepTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrEventSales do
    begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from geteventsales (:poutletid, :premotelocationid, :pforb, :peventid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, eventname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sByGroupOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by eventname, amountsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by eventname, averageunitprice desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by eventname, grosssold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by eventname, discount desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by eventname, nettsold desc, outletname, remotelocationname, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if radFAndB.Checked then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end else if radFOnly.Checked then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radEventAll.Checked) then
      begin
        ParamByName('peventid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllEvents;
      end
      else if (radEventSelected.Checked) then
      begin
        ParamByName('peventid').AsInteger := Glbs.EventID;
        Title1 := Title1 + ', ' + Glbs.EventName;
      end;
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then
    begin
      SaveDialog.Title := sEventRepDlgTitle;
      SaveDialog.FileName := 'Event Sales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then
      begin
        formQREventSales.WBCSV.FileName := SaveDialog.FileName;
        formQREventSales.WBCSV.OpenToWrite;
      end;
    end;

    formQREventSales.QREventSales.ReportTitle := sEventRepDlgTitle;
    formQREventSales.QREventSales.Preview;
  finally
    formQREventSales.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAllStaffOpenPriceOrders;
begin
  try
    formQRStaffOpenPrice := TformQRStaffOpenPrice.Create(Application);

    if (radStaffAll.Checked) then begin
      Title1 := sAllStaffOpenPriceTitle1;
    end
    else begin
      Title1 := Glbs.FirstName + ' ' + Glbs.Surname + sOpenPriceRepTitle1;
    end;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrStaffOpenPrice do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getstaffopenprice(:poutletid, :premotelocationid, :pforb, :pstaffid, :fromtime, :totime)');

      if (radOpenPriceStaff.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, staffname, forb desc, whenordereddate, itemid, itemabbrev');
        OrdersHeader := sByStaff;
      end
      else if (radOpenPriceDate.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, whenordereddate, forb desc, staffname, itemid, itemabbrev');
        OrdersHeader := sByDateTime;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radStaffAll.Checked) then begin
        ParamByName('pstaffid').AsInteger := -1;
      end
      else begin
        ParamByName('pstaffid').AsInteger := Glbs.StaffID;
      end;

      ParamByName('pforb').AsString := '';
      if radFAndB.Checked then begin
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + sFoodBev;
      end else if radFOnly.Checked then begin
        ParamByName('pforb').AsString := 'F';
        Title2 := Title2 + ', ' + sFood;
      end else begin
        ParamByName('pforb').AsString := 'B';
        Title2 := Title2 + ', ' + sBev;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffOrdDlgTitle;
      SaveDialog.FileName := 'StaffOpenPriceOrders' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRStaffOpenPrice.WBCSV.FileName := SaveDialog.FileName;
        formQRStaffOpenPrice.WBCSV.OpenToWrite;
      end;
    end;
    formQRStaffOpenPrice.IsStaffOrderBy := radOpenPriceStaff.Checked;
    formQRStaffOpenPrice.QRStaffOpenPrice.ReportTitle := sStaffOrdRepTitle;
    formQRStaffOpenPrice.QRStaffOpenPrice.Preview;
  finally
    formQRStaffOpenPrice.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowTransListReport;
begin
  try
    formQRTransactionsListing := TformQRTransactionsListing.Create(Application);

    Title1 := sTransactionListingTitle;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrTransList do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from gettranslist (:poutletid, :premotelocationid, :ptillid, :pstaffid, :fromtime, :totime)');

      if (radTransListOptionsCashups.checked) then
      begin
        SQL.Add('where (cashupid is not null)');
        SQL.Add('order by outletname, remotelocationname, cashupid, ');
        if (ckbGroupStaff.Checked) then
          SQL.Add('staffname, ');
        OrdersHeader := sByCashUps;
      end
      else if (radTransListOptionsClockins.checked) then
      begin
        SQL.Add('where (clockinid is not null)');
        SQL.Add('order by outletname, remotelocationname, staffname, clockinid, ');
        if (ckbGroupTill.Checked) then
          SQL.Add('tillname, ');
        OrdersHeader := sByClockins;
      end
      else
      begin
        SQL.Add('order by outletname, remotelocationname, ');
        if (ckbGroupStaff.Checked) or (radTransListOptionsStaff.Checked) then
          SQL.Add('staffname, ');
        if (ckbGroupTill.Checked) then
          SQL.Add('tillname, ');
        OrdersHeader := sByStaff;
      end;

      if (radTransListSortByDate.checked) then
      begin
        SQL.Add('whentendered');
      end
      else if (radTransListSortByTender.checked) then
      begin
        SQL.Add('tenderlinetypeorder, whentendered');
      end
      else
      begin
        SQL.Add('tendertype, whentendered');
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        ParamByName('poutletid').AsInteger := -1;
        Title2 := sAllOutlets;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        ParamByName('premotelocationid').AsInteger := -1;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radTillAll.Checked) then begin
        ParamByName('ptillid').AsInteger := -1;
      end
      else begin
        ParamByName('ptillid').AsInteger := Glbs.TillID;
      end;

      if (radStaffAll.Checked) then begin
        ParamByName('pstaffid').AsInteger := -1;
      end
      else begin
        if (DBGridStaffMembers.SelectedRows.Count > 1) then
        begin
          ParamByName('pstaffid').AsString := GetSelectedIDString(DBGridStaffMembers, 'staffid');
        end
        else
        begin
          ParamByName('pstaffid').AsInteger := Glbs.StaffID;
        end;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sStaffOrdDlgTitle;
      SaveDialog.FileName := 'TransactionListing' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRTransactionsListing.WBCSV.FileName := SaveDialog.FileName;
        formQRTransactionsListing.WBCSV.OpenToWrite;
      end;
    end;
    formQRTransactionsListing.QRTransactionsListing.ReportTitle := sTransactionListingTitle;
    formQRTransactionsListing.QRTransactionsListing.Preview;
  finally
    formQRTransactionsListing.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.ShowAccTypeMovementsReport;
begin
  try
    formQRAccTypeMovements := TformQRAccTypeMovements.Create(Application);

    Title1 := sAccTypeMovements;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrAccountTypeMovements do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getacctypemovementtrans (:poutletid, :premotelocationid, :paccounttypeid, :ploayltyrewardid, :pnosaletypeid, :fromtime, :totime)');
      SQL.Add('  order by outletname, remotelocationname, accounttype, tendertype, invoicetype, loyaltyreward, nosaletype');

      if (radAccountTypeSelected.Checked) then begin
        ParamByName('paccounttypeid').AsInteger := Glbs.AccountTypeID;
        Title2 := Glbs.AccountType;
      end
      else begin
        Title2 := sAccTypeMovementsTitle2;
        ParamByName('paccounttypeid').AsInteger := -1;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutletsTitle2 + ', ' + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (chkShowLoyaltyReward.Checked) then
      begin
        ParamByName('ploayltyrewardid').AsInteger := 0;
      end
      else
      begin
        ParamByName('ploayltyrewardid').AsInteger := -1;
      end;

      if (chkShowNoSale.Checked) then
      begin
        ParamByName('pnosaletypeid').AsInteger := 0;
      end
      else
      begin
        ParamByName('pnosaletypeid').AsInteger := -1;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    formQRAccTypeMovements.ShowDetails := (chkShowLoyaltyReward.Checked) or (chkShowNoSale.Checked);

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sAccTypeMovementsDlgTitle;
      SaveDialog.FileName := 'AccType Movements' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRAccTypeMovements.WBCSV.FileName := SaveDialog.FileName;
        formQRAccTypeMovements.WBCSV.OpenToWrite;
      end;
    end;

    formQRAccTypeMovements.QRAccountTypeMovements.ReportTitle := sAccTypeMovementsRepTitle;
    formQRAccTypeMovements.QRAccountTypeMovements.Preview;
  finally
    formQRAccTypeMovements.Free;
  end;
end;
{******************************************************************************}
procedure TformReports.radStaffSelectedClick(Sender: TObject);
begin
  if ((Sender = radSaleCategorySelected) and (ReportType = rptypStaffOrders)) then begin
    Exit;
  end;
  radDGTDetails.Checked := True;
end;
{******************************************************************************}
procedure TformReports.EnableGroupSubTotalCheckBoxes(B: Boolean);
begin
  ckbGroupGroup.Enabled := B;
  ckbGroupForB.Enabled := B;
  ckbGroupStaff.Enabled := B;
  ckbGroupAccount.Enabled := B;
  ckbGroupOutlet.Enabled := ((B) and (chkOutletBreakdown.Checked));
  ckbGroupRemoteLocation.Enabled := ((B) and (chkRemoteLocationBreakdown.Checked));
  ckbGroupTerminal.Enabled := B;
  ckbGroupSaleCategory.Enabled := B;
  ckbGroupTill.Enabled := B;

  chkOutletBreakdown.Enabled := B;
  chkRemoteLocationBreakdown.Enabled := B;
end;
{******************************************************************************}
procedure TformReports.radOGroupClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(radOGroup.Checked);
  if (not radOGroup.Checked) then begin
    radDGTDetails.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.radGroupClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(radGroup.Checked);
  if (not radGroup.Checked) then begin
    if (frmDetailsGroupsTotals.Visible) then begin
      radDGTDetails.Checked := True;
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.radVStaffClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(radVStaff.Checked);
  if (not radVStaff.Checked) then begin
    radVoidsAllDetails.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.radMenuOrderGroupClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(radMenuOrderGroup.Checked);
end;
{******************************************************************************}
procedure TformReports.radDGTGroupsClick(Sender: TObject);
begin
  if (ReportType in [rptypStaffOrders, rptypOrdersSalesVoids]) then begin
    radOGroup.Checked := True;
  end
  else if (ReportType in [rptypSales, rptypTerminalSales, rptypSaleCategorySales, rptypSectionSales,
        rptypSalePeriodSales, rptypStaffSales, rptypAccountSales, rptypEventSales ]) then begin
    radGroup.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.radTCurrentClick(Sender: TObject);
begin
  dm.TerminalsCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radTillCurrentClick(Sender: TObject);
begin
  dm.TillsCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radAllItemsClick(Sender: TObject);
begin
  radFandB.Enabled := True;
  radFOnly.Enabled := True;
  radBOnly.Enabled := True;
end;
{******************************************************************************}
procedure TformReports.radItemGroupClick(Sender: TObject);
begin
  radFandB.Enabled := False;
  radFOnly.Enabled := False;
  radBOnly.Enabled := False;
  radFAndB.Checked := True;
end;
{******************************************************************************}
procedure TformReports.cmdFromDOBClick(Sender: TObject);
begin
  if SelectDate(FFromDOB) then begin
    if (FFromDOB <> 0) then begin
      txtFromDOB.Text := FormatDateTime('ddd dd mmm yyyy',FFromDOB);
    end
    else begin
      txtFromDOB.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdToDOBClick(Sender: TObject);
begin
  if SelectDate(FToDOB) then begin
    if (FToDOB <> 0) then begin
      txtToDOB.Text := FormatDateTime('ddd dd mmm yyyy',FToDOB);
    end
    else begin
      txtToDOB.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdFromWeddingDateClick(Sender: TObject);
begin
  if SelectDate(FFromWeddingDate) then begin
    if (FFromWeddingDate <> 0) then begin
      txtFromWeddingDate.Text := FormatDateTime('ddd dd mmm yyyy',FFromWeddingDate);
    end
    else begin
      txtFromWeddingDate.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdToWeddingDateClick(Sender: TObject);
begin
  if SelectDate(FToWeddingDate) then begin
    if (FToWeddingDate <> 0) then begin
      txtToWeddingDate.Text := FormatDateTime('ddd dd mmm yyyy',FToWeddingDate);
    end
    else begin
      txtToWeddingDate.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdPeopleLastWhenInvoicedAfterClick(Sender: TObject);
begin
  if SelectDate(FLastWhenInvoicedAfter) then begin
    if (FLastWhenInvoicedAfter <> 0) then begin
      txtPeopleLastWhenInvoicedAfter.Text := FormatDateTime('ddd dd mmm yyyy',FLastWhenInvoicedAfter);
    end
    else begin
      txtPeopleLastWhenInvoicedAfter.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdPeopleLastWhenInvoicedBeforeClick(Sender: TObject);
begin
  if SelectDate(FLastWhenInvoicedBefore) then begin
    if (FLastWhenInvoicedBefore <> 0) then begin
      txtPeopleLastWhenInvoicedBefore.Text := FormatDateTime('ddd dd mmm yyyy',FLastWhenInvoicedBefore);
    end
    else begin
      txtPeopleLastWhenInvoicedBefore.Text := '';
    end;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdFromBirthdayClick(Sender: TObject);
var
  DayAndMonthText: string;

begin
  if SelectDayAndMonth(FFromBirthDay, FFromBirthMonth, True, DayAndMonthText) then begin
    txtFromBirthday.Text := DayAndMonthText;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdToBirthdayClick(Sender: TObject);
var
  DayAndMonthText: string;
begin
  if SelectDayAndMonth(FToBirthDay, FToBirthMonth, True, DayAndMonthText) then begin
    txtToBirthday.Text := DayAndMonthText;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdFromAnniversaryClick(Sender: TObject);
var
  DayAndMonthText: string;

begin
  if SelectDayAndMonth(FFromAnniversaryDay, FFromAnniversaryMonth, True, DayAndMonthText) then begin
    txtFromAnniversary.Text := DayAndMonthText;
  end;
end;
{******************************************************************************}
procedure TformReports.cmdToAnniversaryClick(Sender: TObject);
var
  DayAndMonthText: string;

begin
  if SelectDayAndMonth(FToAnniversaryDay, FToAnniversaryMonth, True, DayAndMonthText) then begin
    txtToAnniversary.Text := DayAndMonthText;
  end;
end;
{******************************************************************************}
procedure TformReports.radModifiersOrderDefaultClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(True);
  if (not(radModifiersOrderDefault.Checked or radModifiersOrderModifier.Checked)) then begin
    ckbGroupForB.Enabled := False;
  end;
end;
{******************************************************************************}
procedure TformReports.CreateHistoryTimes;

  function IncPeriod(ADateTime: TDateTime; NoOfPeriods: Integer): TDateTime;
  var
    AHour, AMinute, ASecond, AMilliSecond: Word;
    AYear, AWeekOfYear, ADayOfWeek: Word;
  begin
    if (radSalesHistory15Min.Checked) then begin
      Result := DateUtils.IncMinute(ADateTime, NoOfPeriods * 15);
    end
    else if (radSalesHistory30Min.Checked) then begin
      Result := DateUtils.IncMinute(ADateTime, NoOfPeriods * 30);
    end
    else if (radSalesHistoryHour.Checked) then begin
      Result := DateUtils.IncHour(ADateTime, NoOfPeriods);
    end
    else if (radSalesHistory2Hour.Checked) then begin
      Result := DateUtils.IncHour(ADateTime, NoOfPeriods * 2);
    end
    else if (radSalesHistory3Hour.Checked) then begin
      Result := DateUtils.IncHour(ADateTime, NoOfPeriods * 3);
    end
    else if (radSalesHistory4Hour.Checked) then begin
      Result := DateUtils.IncHour(ADateTime, NoOfPeriods * 4);
    end
    else if (radSalesHistoryDay.Checked) then begin
      Result := DateUtils.IncDay(ADateTime, NoOfPeriods);
    end
    else if (radSalesHistoryWeek.Checked) then begin
      Result := DateUtils.IncWeek(ADateTime, NoOfPeriods);
    end
    else if (radSalesHistoryFortnight.Checked) then begin
      Result := DateUtils.IncWeek(ADateTime, NoOfPeriods * 2);
    end
    else if (radSalesHistoryMonth.Checked) then begin
      Result := IncMonth(ADateTime, NoOfPeriods);
    end
    else if (radSalesHistoryQuarter.Checked) then begin
      Result := IncMonth(ADateTime, NoOfPeriods * 3);
    end
    else if (radSalesHistoryYear.Checked) then begin
      //if week or fortnight time slice
      if (chkSalesHistoryUsePeriodLength.Checked) and
        ((radSalesHistoryLengthWeek.Checked) or (radSalesHistoryLengthFortnight.Checked)) then begin
        //compare same week number/day in each year, e.g Tue in 21st week of year
        //Use ISO 8601 std: Mon is first day of week, first calendar week is the one that includes the first Thu.
        //see https://www.timeanddate.com/calendar/?year=2017&country=30

        //if incremented year's week date doesn't exist, set datetime to zero
        DateUtils.DecodeDateWeek(ADateTime, AYear, AWeekOfYear, ADayOfWeek);
        if not DateUtils.TryEncodeDateWeek(AYear + NoOfPeriods, AWeekOfYear, Result, ADayOfWeek) then
        begin
          Result := 0;
        end else
        begin
          DecodeTime(ADateTime, AHour, AMinute, ASecond, AMilliSecond);
          Result := Result + EncodeTime(AHour, AMinute, ASecond, AMilliSecond);
        end;
      end
      else begin
        Result := DateUtils.IncYear(ADateTime, NoOfPeriods);
      end;
    end;
  end;

var
  AYear, AMonth, ADay: Word;
  AWeekOfYear, ADayOfWeek: Word;
  I: Integer;
begin
  //initialise time variables
  for I := 1 to SALEHIST_MAXPD do begin
    GlbSalesHistory.FromTime[I] := 0;
    GlbSalesHistory.ToTime[I] := 0;
    GlbSalesHistory.Caption[I] := '';
  end;
  GlbSalesHistory.IncludeZeroDate := False;
  GlbSalesHistory.LastTime := 0;
  GlbSalesHistory.NumberOfTimes := 0;

  //set daily start / period
  if (chkSalesHistoryBetweenTimes.Checked) then begin
    GlbSalesHistory.DailyStart := EncodeTime(udtxtSalesHistorySliceStartHour.Position, udtxtSalesHistorySliceStartMinute.Position, 0, 0);
    GlbSalesHistory.DailyEnd := EncodeTime(udtxtSalesHistorySliceEndHour.Position, udtxtSalesHistorySliceEndMinute.Position, 0, 0);
    GlbSalesHistory.DailyPeriod := GlbSalesHistory.DailyEnd - GlbSalesHistory.DailyStart;

    //if Daily Between Times cross over midnight (resulting in a negative period), we need to add 1 day to get the correct period
    if GlbSalesHistory.DailyPeriod < 0 then
      GlbSalesHistory.DailyPeriod := GlbSalesHistory.DailyPeriod + 1;
  end
  else begin
    GlbSalesHistory.DailyStart := 0;
    GlbSalesHistory.DailyEnd := 0;
    GlbSalesHistory.DailyPeriod := 0;
  end;

  GlbSalesHistory.NumberOfTimes := UpDownSalesHistoryTimeSpanQuantity.Position;
  if GlbSalesHistory.NumberOfTimes > SALEHIST_MAXPDPRINT then
    GlbSalesHistory.NoOfPeriodsToPrint := SALEHIST_MAXPDPRINT
  else
    GlbSalesHistory.NoOfPeriodsToPrint := GlbSalesHistory.NumberOfTimes;

  for I := 1 to GlbSalesHistory.NumberOfTimes do begin
    //set from time
    if I = 1 then begin
      GlbSalesHistory.FromTime[I] := FFromTime;
      if chkSalesHistoryBetweenTimes.Checked then begin
        //set first from time using selected daily between times' start time, so report and header are correct
        DecodeDate(GlbSalesHistory.FromTime[I], AYear, AMonth, ADay);
        GlbSalesHistory.FromTime[I] := EncodeDate(AYear, AMonth, ADay) + GlbSalesHistory.DailyStart;
      end;
    end else
    begin
      GlbSalesHistory.FromTime[I] := IncPeriod(GlbSalesHistory.FromTime[1], I - 1);
    end;

    //set to time
    if GlbSalesHistory.FromTime[I] = 0 then begin
      GlbSalesHistory.ToTime[I] := 0;
      GlbSalesHistory.IncludeZeroDate := True; //so non-existent Yearly Week Dates don't break rest of report
    end
    else if (chkSalesHistoryUsePeriodLength.Checked) then begin
      if (radSalesHistoryLength15Min.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncMinute(GlbSalesHistory.FromTime[I], 15);
      end
      else if (radSalesHistoryLength30Min.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncMinute(GlbSalesHistory.FromTime[I], 30);
      end
      else if (radSalesHistoryLengthHour.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncHour(GlbSalesHistory.FromTime[I]);
      end
      else if (radSalesHistoryLength2Hour.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncHour(GlbSalesHistory.FromTime[I], 2);
      end
      else if (radSalesHistoryLength3Hour.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncHour(GlbSalesHistory.FromTime[I], 3);
      end
      else if (radSalesHistoryLength4Hour.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncHour(GlbSalesHistory.FromTime[I], 4);
      end
      else if (radSalesHistoryLengthDay.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncDay(GlbSalesHistory.FromTime[I]);
      end
      else if (radSalesHistoryLengthWeek.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncWeek(GlbSalesHistory.FromTime[I]);
      end
      else if (radSalesHistoryLengthFortnight.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncWeek(GlbSalesHistory.FromTime[I], 2);
      end
      else if (radSalesHistoryLengthMonth.Checked) then begin
        GlbSalesHistory.ToTime[I] := IncMonth(GlbSalesHistory.FromTime[I]);
      end
      else if (radSalesHistoryLengthQuarter.Checked) then begin
        GlbSalesHistory.ToTime[I] := IncMonth(GlbSalesHistory.FromTime[I], 3);
      end
      else if (radSalesHistoryLengthYear.Checked) then begin
        GlbSalesHistory.ToTime[I] := DateUtils.IncYear(GlbSalesHistory.FromTime[I]);
      end;
    end
    else begin
      GlbSalesHistory.ToTime[I] := IncPeriod(GlbSalesHistory.FromTime[1], I);
    end;

    //adjust each ToTime to the Daily Between Times End Time
    if chkSalesHistoryBetweenTimes.Checked and (GlbSalesHistory.ToTime[I] <> 0) then begin
      //NB: assumes the minimum unit of Period Spacing and Time Slice is a Day (24 hours),
      //and ToTime has been set above by adding a full Day (24 hr), Week etc to the FromTime.
      //Use the to time's date, less a day, plus the daily between times' start time and duration
      DecodeDate(GlbSalesHistory.ToTime[I], AYear, AMonth, ADay);
      GlbSalesHistory.ToTime[I] := EncodeDate(AYear, AMonth, ADay) - 1 + GlbSalesHistory.DailyStart + GlbSalesHistory.DailyPeriod;
    end;

    //set any specific caption
    if (radSalesHistoryYear.Checked) then begin
      //if week or fortnight time slice
      if (chkSalesHistoryUsePeriodLength.Checked) and
        ((radSalesHistoryLengthWeek.Checked) or (radSalesHistoryLengthFortnight.Checked)) then begin
        if GlbSalesHistory.FromTime[I] = 0 then begin
          //use first period's week date, increment years to current period
          DateUtils.DecodeDateWeek(GlbSalesHistory.FromTime[1], AYear, AWeekOfYear, ADayOfWeek);
          GlbSalesHistory.Caption[I] := IntToStr(AYear + I - 1) + ' DNE'; //e.g. '2016 DNE' (does not exist);
        end
        else begin
          //use current period's week date
          DateUtils.DecodeDateWeek(GlbSalesHistory.FromTime[I], AYear, AWeekOfYear, ADayOfWeek);
          GlbSalesHistory.Caption[I] := IntToStr(AYear) + ' W' + IntToStr(AWeekOfYear) + '-' + IntToStr(ADayOfWeek); //e.g. '2015 W53-1'
        end;
      end;
    end;
  end;

  //get last time
  for I := GlbSalesHistory.NumberOfTimes downto 1 do begin
    GlbSalesHistory.LastTime := GlbSalesHistory.ToTime[I];
    if GlbSalesHistory.LastTime > 0 then
      Break;
  end;
end;
{******************************************************************************}
procedure TformReports.radVoidsTotalsOnlyClick(Sender: TObject);
begin
  radVStaff.Checked := True;
end;
{******************************************************************************}
procedure TformReports.radSCCurrentClick(Sender: TObject);
begin
  dm.SaleCategoriesCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radCashTotalsOrderCashupIDClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(True);
  ckbGroupTill.Enabled := radCashTotalsOrderTill.Checked;
//  ckbGroupOutlet.Enabled := radCashTotalsOrderTill.Checked;
//  ckbGroupRemoteLocation.Enabled := radCashTotalsOrderTill.Checked;
end;
{******************************************************************************}
procedure TformReports.chkSalesHistoryUsePeriodLengthClick(Sender: TObject);
begin
  frmSalesHistoryPeriodLength.Visible := chkSalesHistoryUsePeriodLength.Checked;

  //if Time Slice checked, Daily Between Times is showing and Time Slice value
  //is less than Day, auto-select to Day, so Daily Between Times stays visible
  if chkSalesHistoryUsePeriodLength.Checked and chkSalesHistoryBetweenTimes.Checked and
    (GetSelectedSalesHistoryLengthRadioButton.Tag < radSalesHistoryLengthDay.Tag) then
    radSalesHistoryLengthDay.Checked := True;

  EnableHistoryBetweenTimesCheckBox;
end;
{******************************************************************************}
procedure TformReports.radSalesHistoryHourClick(Sender: TObject);
begin
  radSalesHistoryLength15Min.Checked := radSalesHistory15Min.Checked;
  radSalesHistoryLength30Min.Checked := radSalesHistory30Min.Checked;
  radSalesHistoryLengthHour.Checked := radSalesHistoryHour.Checked;
  radSalesHistoryLength2Hour.Checked := radSalesHistory2Hour.Checked;
  radSalesHistoryLength3Hour.Checked := radSalesHistory3Hour.Checked;
  radSalesHistoryLength4Hour.Checked := radSalesHistory4Hour.Checked;
  radSalesHistoryLengthDay.Checked := radSalesHistoryDay.Checked;
  radSalesHistoryLengthWeek.Checked := radSalesHistoryWeek.Checked;
  radSalesHistoryLengthFortnight.Checked := radSalesHistoryFortnight.Checked;
  radSalesHistoryLengthMonth.Checked := radSalesHistoryMonth.Checked;
  radSalesHistoryLengthQuarter.Checked := radSalesHistoryQuarter.Checked;
  radSalesHistoryLengthYear.Checked := radSalesHistoryYear.Checked;

  if (Sender is TRadioButton) then begin
    radSalesHistoryLength15Min.Enabled := (TRadioButton(Sender).Tag >= 0);
    radSalesHistoryLength30Min.Enabled := (TRadioButton(Sender).Tag >= 1);
    radSalesHistoryLengthHour.Enabled := (TRadioButton(Sender).Tag >= 2);
    radSalesHistoryLength2Hour.Enabled := (TRadioButton(Sender).Tag >= 3);
    radSalesHistoryLength3Hour.Enabled := (TRadioButton(Sender).Tag >= 4);
    radSalesHistoryLength4Hour.Enabled := (TRadioButton(Sender).Tag >= 5);
    radSalesHistoryLengthDay.Enabled := (TRadioButton(Sender).Tag >= 6);
    radSalesHistoryLengthWeek.Enabled := (TRadioButton(Sender).Tag >= 7);
    radSalesHistoryLengthFortnight.Enabled := (TRadioButton(Sender).Tag >= 8);
    radSalesHistoryLengthMonth.Enabled := (TRadioButton(Sender).Tag >= 9);
    radSalesHistoryLengthQuarter.Enabled := (TRadioButton(Sender).Tag >= 10);
    radSalesHistoryLengthYear.Enabled := (TRadioButton(Sender).Tag >= 11);
  end;

  EnableHistoryBetweenTimesCheckBox;
end;
{******************************************************************************}
procedure TformReports.chkSalesHistoryBetweenTimesClick(Sender: TObject);
begin
  frmSalesHistoryDailyBetweenTimes.Visible := chkSalesHistoryBetweenTimes.Checked;
end;
{******************************************************************************}
procedure TformReports.radSalesHistoryLengthHourClick(Sender: TObject);
begin
  EnableHistoryBetweenTimesCheckBox;
end;
{******************************************************************************}
function TformReports.GetSelectedSalesHistoryPeriodRadioButton: TRadioButton;
begin
  if radSalesHistory15Min.Checked then
    Result := radSalesHistory15Min
  else if radSalesHistory30Min.Checked then
    Result := radSalesHistory30Min
  else if radSalesHistoryHour.Checked then
    Result := radSalesHistoryHour
  else if radSalesHistory2Hour.Checked then
    Result := radSalesHistory2Hour
  else if radSalesHistory3Hour.Checked then
    Result := radSalesHistory3Hour
  else if radSalesHistory4Hour.Checked then
    Result := radSalesHistory4Hour
  else if radSalesHistoryDay.Checked then
    Result := radSalesHistoryDay
  else if radSalesHistoryWeek.Checked then
    Result := radSalesHistoryWeek
  else if radSalesHistoryFortnight.Checked then
    Result := radSalesHistoryFortnight
  else if radSalesHistoryMonth.Checked then
    Result := radSalesHistoryMonth
  else if radSalesHistoryQuarter.Checked then
    Result := radSalesHistoryQuarter
  else if radSalesHistoryYear.Checked then
    Result := radSalesHistoryYear
  else
    Result := nil;
end;
{******************************************************************************}
function TformReports.GetSelectedSalesHistoryLengthRadioButton: TRadioButton;
begin
  if radSalesHistoryLength15Min.Checked then
    Result := radSalesHistoryLength15Min
  else if radSalesHistoryLength30Min.Checked then
    Result := radSalesHistoryLength30Min
  else if radSalesHistoryLengthHour.Checked then
    Result := radSalesHistoryLengthHour
  else if radSalesHistoryLength2Hour.Checked then
    Result := radSalesHistoryLength2Hour
  else if radSalesHistoryLength3Hour.Checked then
    Result := radSalesHistoryLength3Hour
  else if radSalesHistoryLength4Hour.Checked then
    Result := radSalesHistoryLength4Hour
  else if radSalesHistoryLengthDay.Checked then
    Result := radSalesHistoryLengthDay
  else if radSalesHistoryLengthWeek.Checked then
    Result := radSalesHistoryLengthWeek
  else if radSalesHistoryLengthFortnight.Checked then
    Result := radSalesHistoryLengthFortnight
  else if radSalesHistoryLengthMonth.Checked then
    Result := radSalesHistoryLengthMonth
  else if radSalesHistoryLengthQuarter.Checked then
    Result := radSalesHistoryLengthQuarter
  else if radSalesHistoryLengthYear.Checked then
    Result := radSalesHistoryLengthYear
  else
    Result := nil;
end;
{******************************************************************************}
procedure TformReports.EnableHistoryBetweenTimesCheckBox;
begin
  chkSalesHistoryBetweenTimes.Enabled := (
    (GetSelectedSalesHistoryPeriodRadioButton.Tag >= radSalesHistoryDay.Tag) and
      (not chkSalesHistoryUsePeriodLength.Checked or (chkSalesHistoryUsePeriodLength.Checked and
        ((GetSelectedSalesHistoryLengthRadioButton.Tag >= radSalesHistoryLengthDay.Tag)))));

  if not chkSalesHistoryBetweenTimes.Enabled then
    chkSalesHistoryBetweenTimes.Checked := False;
end;
{******************************************************************************}
procedure TformReports.radAccountSelectedTypeClick(Sender: TObject);
begin
  DBGridAccounts.DataSource := dm.dsAccountTypes;
  fmAccountSelectionClick(sender);
end;
{******************************************************************************}
procedure TformReports.radAccountSelectedClick(Sender: TObject);
begin
  DBGridAccounts.DataSource := dm.dsAccounts;
  fmAccountSelectionClick(sender);
end;
{******************************************************************************}
procedure TformReports.radClockInsOrderStaffNameClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(True);
  ckbGroupStaff.Enabled := radClockInsOrderStaffName.Checked;
  ckbGroupRemoteLocation.Enabled := radClockInsOrderStaffName.Checked;
  chkRemoteLocationBreakdown.Enabled := radClockInsOrderStaffName.Checked;
end;
{******************************************************************************}
procedure TformReports.radCoursesCurrentClick(Sender: TObject);
begin
  dm.CoursesCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radDiscountSchemesCurrentClick(Sender: TObject);
begin
  dm.DiscountSchemesCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radCourseAllClick(Sender: TObject);
begin
  DBGridCourses.DataSource := dm.dsCourses;
end;
{******************************************************************************}
procedure TformReports.radDiscountSchemeAllClick(Sender: TObject);
begin
  DBGridDiscountSchemes.DataSource := dm.dsDiscountSchemes;
end;
{******************************************************************************}
procedure TformReports.cmbOutletsChange(Sender: TObject);
begin
  if cmbOutlets.ItemIndex > -1 then //sinu
  begin
    dm.CurrentOutletID := FOutletList.Get(cmbOutlets.ItemIndex);  // Integer(cmbOutlets.Items.Objects[cmbOutlets.ItemIndex]);   sinu
  end;
  chkOutletBreakdown.Visible := ((ReportType in [rptypSales, rptypAccountSales, rptypEventSales, rptypTerminalSales,
      rptypTillSales, rptypDiscountSchemeSales, rptypCourseSales, rptypSaleCategorySales, rptypSectionSales, rptypStaffSales,
      rptypSalesByDate, rptypSalesHistory, rptypCashTotals, rptypItemModSales, rptypModItems,
      rptypModifiers, rptypNoSales, rptypStaffTips, rptypVoids, rptypOrders, rptypStaffOrders, rptypAccountTransactions,
      rptypOrdersSalesVoids, rptypAccountTabSales, rptypComboSales])
     and (cmbOutlets.ItemIndex = 0));
  chkOutletBreakdown.Checked := True;
end;
procedure TformReports.cmbPeopleCustomerChange(Sender: TObject);
begin

end;

{******************************************************************************}
procedure TformReports.cmbRemoteLocationsChange(Sender: TObject);
begin
  Glbs.RemoteLocationName := cmbRemoteLocations.Items[cmbRemoteLocations.ItemIndex];
  if cmbRemoteLocations.ItemIndex > 0 then//sinu
  begin
    Glbs.RemoteLocationID := FRemLocsList.Get(cmbRemoteLocations.ItemIndex); //Integer(cmbRemoteLocations.Items.Objects[cmbRemoteLocations.ItemIndex])
  end
  else//sinu
  begin
    Glbs.RemoteLocationID := -1;//sinu
  end;
  chkRemoteLocationBreakdown.visible := ((ReportType in [rptypSales, rptypAccountSales, rptypTerminalSales,
      rptypTillSales, rptypDiscountSchemeSales, rptypCourseSales, rptypSaleCategorySales, rptypSectionSales, rptypStaffSales,
      rptypSalesByDate, rptypSalesHistory, rptypCashTotals, rptypClockins, rptypItemModSales, rptypModItems,
      rptypModifiers, rptypNoSales, rptypStaffTips, rptypVoids, rptypOrders, rptypStaffOrders, rptypOrdersSalesVoids,
      rptypAccountTransactions, rptypAccountTabSales, rptypComboSales])
    and (cmbRemoteLocations.ItemIndex = 0));;
  chkRemoteLocationBreakdown.Checked := true;
end;
{******************************************************************************}
procedure TformReports.chkOutletBreakdownClick(Sender: TObject);
begin
  ckbGroupOutlet.Enabled := ((chkOutletBreakdown.Checked) and (radGroup.Checked));
end;
{******************************************************************************}
procedure TformReports.chkRemoteLocationBreakdownClick(Sender: TObject);
begin
  ckbGroupRemoteLocation.Enabled := ((chkRemoteLocationBreakdown.Checked) and (radGroup.Checked));
end;
{******************************************************************************}
procedure TformReports.DBGridModGroupsCellClick(Column: TColumn);
begin
  if (radModGroupAll.Checked) then begin
    radModGroupSelected.Checked := True;
  end;
end;
{******************************************************************************}
procedure TformReports.radModGroupsCurrentClick(Sender: TObject);
begin
  dm.ModGroupCurrent := (TRadioButton(Sender).Tag = 0);
end;
{******************************************************************************}
procedure TformReports.radModGroupAllClick(Sender: TObject);
begin
  DBGridModGroups.DataSource := dm.dsModGroups;
end;
{******************************************************************************}
procedure TformReports.radShowModGroupsAllClick(Sender: TObject);
  procedure PositionGroupBox(var GroupBox: TGroupBox; Left, Top: Integer);
  begin
    GroupBox.Visible := True;
    GroupBox.Left := ((Left * Screen.PixelsPerInch) div 96);
    GroupBox.Top := ((Top * Screen.PixelsPerInch) div 96);
  end;
begin
  if (radShowModGroupsMods.checked) then
  begin
    if (ReportType = rptypItemModSales) then
    begin
      PositionGroupBox(frmModGroups, 400, 164);
    end
    else if (ReportType = rptypModItems) then
    begin
      PositionGroupBox(frmModGroups, 4, 210);
    end;
  end
  else begin
    frmModGroups.Visible := False;
  end;
end;
{******************************************************************************}
procedure TformReports.SetComboCaptions;
var
  iIndex : Integer;
begin
  iIndex := cmbPeopleAccountTypes.ItemIndex;
  cmbPeopleAccountTypes.Items[0] := sAllAcctTypes;
  cmbPeopleAccountTypes.ItemIndex := iIndex;

  iIndex := cmbOutlets.ItemIndex;
  cmbOutlets.Items[0] := sAllOutlets;
  cmbOutlets.ItemIndex := iIndex;

  iIndex := cmbRemoteLocations.ItemIndex;
  cmbRemoteLocations.Items[0] := sAllLoc;
  cmbRemoteLocations.Items[1] := sOnlyThisLoc;
  cmbRemoteLocations.ItemIndex := iIndex;

  iIndex := cmbReportType.ItemIndex;
  cmbReportType.Items.Clear;
  cmbReportType.Items.Add(sAcctSummary);
  cmbReportType.Items.Add(sAcctTabSales);
  cmbReportType.Items.Add(sAcctTransactions);
  cmbReportType.Items.Add(sAccTypeMovements);
  cmbReportType.Items.Add(sRepCashTotals);
  cmbReportType.Items.Add(sRepClockIns);
  cmbReportType.Items.Add(sRepMenuItems);
  cmbReportType.Items.Add(sRepModifiers);
  cmbReportType.Items.Add(sModsWithItems);
  cmbReportType.Items.Add(sRepNoSales);
  cmbReportType.Items.Add(sRepOrders);
  cmbReportType.Items.Add(sRepOrdersStaff);
  cmbReportType.Items.Add(sRepStaffOpenPriceItems);
  cmbReportType.Items.Add(sRepOrdersSalesVoids);
  cmbReportType.Items.Add(sRepPeople);
  cmbReportType.Items.Add(sPerDayCalc);
  cmbReportType.Items.Add(sRepPeriodEndStat);
  cmbReportType.Items.Add(sRepperiodEndTran);
  cmbReportType.Items.Add(sRepSales);
  cmbReportType.Items.Add(sRepSalesAcct);
  cmbReportType.Items.Add(sComboSales);
  cmbReportType.Items.Add(sRepSalesCourse);
  cmbReportType.Items.Add(sSalesDiscScheme);
  cmbReportType.Items.Add(sRepSalesEvent);
  cmbReportType.Items.Add(sSalesItemwMods);
  cmbReportType.Items.Add(sRepSalesCat);
  cmbReportType.Items.Add(sRepSalesSec);
  cmbReportType.Items.Add(sRepSalesPeriod);
  cmbReportType.Items.Add(sRepSalesStaff);
  cmbReportType.Items.Add(sSalesTerminal);
  cmbReportType.Items.Add(sSalesTill);
  cmbReportType.Items.Add(sSalesByDate);
  cmbReportType.Items.Add(sSalesHistory);
  cmbReportType.Items.Add(sSatffTips);
  cmbReportType.Items.Add(sStaffDetails);
  cmbReportType.Items.Add(sRepTablesOpen);
  cmbReportType.Items.Add(sRepTablesSummary);
  cmbReportType.Items.Add(sTransactionList);
  cmbReportType.Items.Add(sVoidsWastage);

  cmbReportType.ItemIndex := iIndex;

  iIndex := cmboSalesHistoryShownData.ItemIndex;
  cmboSalesHistoryShownData.Items.Clear;
  cmboSalesHistoryShownData.Items.Add(sUnits);
  cmboSalesHistoryShownData.Items.Add(sNettValue);
  cmboSalesHistoryShownData.ItemIndex := iIndex;

  iIndex := cmbPeopleCustomer.ItemIndex;
  cmbPeopleCustomer.Items.Clear;
  cmbPeopleCustomer.Items.Add(sAllPeople);
  cmbPeopleCustomer.Items.Add(sCustomers);
  cmbPeopleCustomer.Items.Add(sNonCustomers);
  cmbPeopleCustomer.ItemIndex := iIndex;

  iIndex := cmbPeopleVIP.ItemIndex;
  cmbPeopleVIP.Items.Clear;
  cmbPeopleVIP.Items.Add(sAllPeople);
  cmbPeopleVIP.Items.Add(sVips);
  cmbPeopleVIP.Items.Add(sNonVipsColon);
  cmbPeopleVIP.ItemIndex := iIndex;

  iIndex := cmbPeopleGender.ItemIndex;
  cmbPeopleGender.Items.Clear;
  cmbPeopleGender.Items.Add(sAll);
  cmbPeopleGender.Items.Add(sMale);
  cmbPeopleGender.Items.Add(sFemale);
  cmbPeopleGender.ItemIndex := iIndex;
end;
{******************************************************************************}
procedure TformReports.SetDBGridCaptions;
begin
  dm.qrItemGroupsITEMGROUPABBREV.DisplayLabel := sGroup;
  dm.qrItemGroupsITEMGROUPNAME.DisplayLabel := sGroupName;
  dm.qrItemGroupsOUTLETNAME.DisplayLabel := sOwnerOutlet;

  dm.qrOutletsOUTLETNAME.DisplayLabel := sOutlet;

  dm.qrAccountTypesREMOTEOVERRIDEGROUPNAME.DisplayLabel := sQryROGName;
  dm.qrAccountTypesACCOUNTTYPE.DisplayLabel := sAccountType;
  dm.qrRemoteGroupsREMOTEOVERRIDEGROUPNAME.DisplayLabel := sQryROGName;

  dm.qrAccountsREMOTEOVERRIDEGROUPNAME.DisplayLabel := sQryROGName;
  dm.qrAccountsACCOUNTTYPE.DisplayLabel := sAccountType;
  dm.qrAccountsACCOUNTNAME.DisplayLabel := sAccountName;
  dm.qrAccountsWHENCLOSED.DisplayLabel := sWhenClosed;

  dm.qrTerminalsTERMINALID.DisplayLabel := sTerminalId;
  dm.qrTerminalsTERMINALNAME.DisplayLabel := sTerminal;
  dm.qrTerminalsWANAME.DisplayLabel := sWorkArea;
  dm.qrTerminalsOUTLETNAME.DisplayLabel := sOutlet;
  dm.qrTerminalsWHENDELETED.DisplayLabel := sWhenDeleted;

  dm.qrSaleCategoriesSALECATEGORY.DisplayLabel := sSaleCategory;
  dm.qrSaleCategoriesWHENDELETED.DisplayLabel := sWhenDeleted;

  dm.qrSectionsSECTION.DisplayLabel := sSection;
  dm.qrSectionsWHENDELETED.DisplayLabel := sWhenDeleted;

  dm.qrCoursesCOURSE.DisplayLabel := sCourse;
  dm.qrCoursesWHENDELETED.DisplayLabel := sWhenDeleted;

  dm.qrAccountPEsWHENUPD.DisplayLabel := sWhenEnded;
  dm.qrAccountPEsACCOUNTTYPE.DisplayLabel := sAccountType;

  dm.qrStaffREMOTEOVERRIDEGROUPNAME.DisplayLabel := sQryROGName;
  dm.qrStaffSTAFFNAME.DisplayLabel := sStaffName;
  dm.qrStaffFIRSTNAME.DisplayLabel := sFirstName;
  dm.qrStaffSURNAME.DisplayLabel := sSurname;
  dm.qrStaffWHENEND.DisplayLabel := sWhenEnd;
  dm.qrStaffWHENBEGIN.DisplayLabel := sWhenBegin;

  dm.qrCombosCOMBONAME.DisplayLabel := sComboName;
  dm.qrCombosWHENDELETED.DisplayLabel := sWhenDeleted;
end;
{******************************************************************************}
function TformReports.GetSelectedIDString(aDBGrid: TDBGrid; aIDFieldName: string): string;
var
  I: Integer;
  aBookmark: TBookmark;
  aResult: string;

begin
  aResult := '';
  try
    aBookmark := aDBGrid.DataSource.DataSet.GetBookmark;
    for I := 0 to (aDBGrid.SelectedRows.Count - 1) do
    begin
      aDBGrid.DataSource.DataSet.GotoBookmark(aDBGrid.SelectedRows.Items[I]);
      if (aResult <> '') then
      begin
        aResult := aResult + ',';
      end;
      aResult := aResult + aDBGrid.DataSource.DataSet.FieldByName(aIDFieldName).AsString;
    end;
    aDBGrid.DataSource.DataSet.GotoBookmark(aBookmark);
    aDBGrid.DataSource.DataSet.FreeBookmark(aBookmark);
  finally
    Result := aResult;
  end;
end;
{******************************************************************************}
procedure TformReports.radSalePeriodSelectedClick(Sender: TObject);
begin
  if ((Sender = radSalePeriodSelected) and (ReportType = rptypStaffOrders)) then begin
    Exit;
  end;
  radDGTDetails.Checked := True;
end;

procedure TformReports.DBGridSalePeriodsCellClick(Column: TColumn);
begin
  radSalePeriodSelected.Checked := True;
end;

procedure TformReports.menuHelpUpdatesClick(Sender: TObject);
begin
  if FileExists(AppDetails.UpdatePath) then
    ShellExecute(Handle, 'open', PChar(AppDetails.UpdatePath), PChar('/update'), nil, SW_SHOWNORMAL);
end;

procedure TformReports.chkSBExtendedDetailClick(Sender: TObject);
begin
  chkSBTransType.Checked := chkSBExtendedDetail.Checked;
  chkSBTransType.Enabled := not chkSBTransType.Checked;
  ckbGroupRemoteLocation.Checked := True;
  ckbGroupRemoteLocation.Enabled := not chkSBExtendedDetail.Checked;
  chkRemoteLocationBreakdown.Checked := True;
  chkRemoteLocationBreakdown.Enabled := not chkSBExtendedDetail.Checked;
end;

procedure TformReports.frmSBOptionsClick(Sender: TObject);
begin
  if (frmSBOptions.ItemIndex = 1) and (ReportType <> rptypAccTypeMovements) then
  begin
    frmSBDetails.ItemIndex := 0;
    frmSBDetails.Enabled := False;
  end
  else
    frmSBDetails.Enabled := True;
end;
{******************************************************************************}
procedure TformReports.fmAccountSelectionClick(Sender: TObject);
begin
  ckbGroupAccount.Enabled := not radAccountSelected.Checked;
  ckbGroupAccount.Checked := True;
end;
{******************************************************************************}
procedure TformReports.DBGridCombosCellClick(Column: TColumn);
begin
  radComboSelected.Checked := True;
end;
{******************************************************************************}
procedure TformReports.radComboCurrentClick(Sender: TObject);
begin
  dm.CombosCurrent := (TRadioButton(Sender).Tag = 0);
end;

procedure TformReports.radComboSelectedClick(Sender: TObject);
begin
  if ((Sender = radComboSelected) and (ReportType = rptypComboSales)) then begin
    Exit;
  end;
  radDGTDetails.Checked := True;
end;

procedure TformReports.radComboOrderItemClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(radGroup.Checked);
  if (not radGroup.Checked) then begin
    if (frmDetailsGroupsTotals.Visible) then begin
      radDGTDetails.Checked := True;
    end;
  end;

  frmItemGroups.Visible := radComboOrderItem.Checked;
  frmItemGroups.Enabled := radComboOrderItem.Checked;
end;

procedure TformReports.cmdDatesLockClick(Sender: TObject);
begin
  txtTo.Enabled := ((ReportType <> rptypSalesHistory) and (not cmdDatesLock.Down));
  cmdTo.Enabled := ((ReportType <> rptypSalesHistory) and (not cmdDatesLock.Down));
end;

procedure TformReports.MenuAboutClick(Sender: TObject);
var
  TitleBitMap: TBitMap;
begin
  TitleBitMap := TBitMap.Create;
  try
    TitleImageList.GetBitMap(0, TitleBitMap);
    ShowAboutBox('onetap Reports', TitleBitMap);
  finally
    TitleBitMap.Free;
  end;
end;

procedure TformReports.radECurrentClick(Sender: TObject);
begin
  dm.EventsCurrent := (TRadioButton(Sender).Tag = 0);
end;

procedure TformReports.DBGridEventsCellClick(Column: TColumn);
begin
  if (radEventAll.Checked) then
    radEventSelected.Checked := True;
end;

procedure TformReports.navEventsBeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
  if (radEventAll.Checked) then
    radEventSelected.Checked := True;
end;

procedure TformReports.radEventAllClick(Sender: TObject);
begin
  DBGridEvents.DataSource := dm.dsEvents;
//  fmEventSelectiono
end;

procedure ApplicationInitialization;
var
  k: Integer;
  OutputBuffer: PChar; // holds local info
  SelectedLCID: LCID; // holds the selected LCID
  iLangId: Integer;
  smldName : string;

begin
  smldName := ChangeFileExt(Application.ExeName, '.mld');
  AppDetails := TAppDetails.Create;
  if  FileExists(smldName) then
  begin
    {allocate memory for the string}
//    BinaryDictionary := TIvBinaryDictionary.Create(Application);
    OutputBuffer := StrAlloc(255);
    SelectedLCID := GetUserDefaultLCID;

    {get the numeric thousands separator}
    GetLocaleInfo(SelectedLCID, LOCALE_SISO639LANGNAME, OutputBuffer, 255);
    iLangId := -99;

//    BinaryDictionary.Enabled := False;
//    BinaryDictionary.FileName := smldName;
//    BinaryDictionary.CheckLevel := ivclCodePage;
//    for k := 0 to BinaryDictionary.LanguageCount - 1 do
//    begin
//      if TIvLanguage(BinaryDictionary.Languages[k]).ISOLanguage = OutputBuffer then
//      begin
//        iLangId := k;
//        Break;
//      end;
//    end;

    if iLangId = -99 then
      Exit;

    if iLangId <> AppDetails.Language then
    begin
      //No Need to set language . Let Delphi take the regional settings;
      MessageDlg(sLanguageMismatch,mtInformation,[mbOk],0);
      AppDetails.Language := iLangId;
    end;

//    try
//      BinaryDictionary.Language := AppDetails.Language;
//      BinaryDictionary.Enabled := True;
//    except
//      BinaryDictionary.Enabled := False;
//    end;
  end;
end;

procedure TformReports.radTransListSortByTenderClick(Sender: TObject);
begin
  EnableGroupSubTotalCheckBoxes(True);
  if (not (radTransListOptionsAll.Checked)) then
  begin
    if (not (radTransListOptionsCashups.Checked)) then
    begin
      ckbGroupStaff.Enabled := False;
    end
    else begin
      ckbGroupTill.Enabled := False;
    end;
  end;
  if (radTransListOptionsCashups.Checked) then
  begin
    chkCashupReference.Enabled := True;
    chkCashupNotes.Enabled := True;
  end
  else begin
    chkCashupReference.Enabled := False;
    chkCashupNotes.Enabled := False;
  end;
end;

procedure TformReports.FormActivate(Sender: TObject);
begin
  //Only do this first time in....
  if (LoggedInAlready) then begin
    Exit;
  end;

 if (not LogonStaffCheck(9)) then begin     //haven't passed the check...
    ExitProcess(0);
    Close;
    Exit;
  end;
  LoggedInAlready:= True;
end;

procedure TformReports.navAccountTypesBeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
  if (radAccountTypeAll.Checked) then
    radAccountTypeSelected.Checked := True;
end;

procedure TformReports.DBGridAccountTypesCellClick(Column: TColumn);
begin
  if (radAccountTypeAll.Checked) then
    radAccountTypeSelected.Checked := True;
end;

procedure TformReports.cmdStaffOrdersKPIClick(Sender: TObject);
begin
  lblReportType.Visible := False;
  cmdDoReport.Visible := False;
  cmdStaffOrdersKPI.Visible := False;
  pnlStaffOrdersKPISetup.Visible := True;
  pnlStaffOrdersKPISetup.Align := alClient;
  sgStaffOrdersKPIClick(Sender);

end;

procedure TformReports.cmdStaffOrdersKPIAddIG_NClick(Sender: TObject);
var
  i, j: Integer;
  FoundIG: Boolean;

begin
  with DBGridStaffOrdersKPI.DataSource.DataSet do
  begin
    for i:=0 to DBGridStaffOrdersKPI.SelectedRows.Count-1 do
    begin
      FoundIG := False;
      //GotoBookmark((DBGridStaffOrdersKPI.SelectedRows.Items[i]));
      for j:=0 to sgStaffOrdersKPIItemGroupsN.RowCount-1 do
      begin
        if (sgStaffOrdersKPIItemGroupsN.Cells[1, j] = FieldAsString(FieldByName('itemgroupname'), '')) then
        begin
          FoundIG := True;
          Break;
        end;
      end;
      if (FoundIG) then
        Continue;

      if (sgStaffOrdersKPIItemGroupsN.Cells[0, sgStaffOrdersKPIItemGroupsN.Row] <> '') then
        sgStaffOrdersKPIItemGroupsN.RowCount := sgStaffOrdersKPIItemGroupsN.RowCount+1;
      sgStaffOrdersKPIItemGroupsN.Row := sgStaffOrdersKPIItemGroupsN.RowCount-1;

      sgStaffOrdersKPIItemGroupsN.Cells[0, sgStaffOrdersKPIItemGroupsN.Row] := IntToStr(FieldAsInt(FieldByName('itemgroupid'), 0));
      sgStaffOrdersKPIItemGroupsN.Cells[1, sgStaffOrdersKPIItemGroupsN.Row] := FieldAsString(FieldByName('itemgroupname'), '');
    end;
  end;
  txtStaffOrdersKPINameChange(Sender);
end;

procedure TformReports.cmdStaffOrdersKPIRemoveIG_NClick(Sender: TObject);
var
  ARow, i, j: Integer;

begin
  ARow := sgStaffOrdersKPIItemGroupsN.Row;

  for i:=0 to sgStaffOrdersKPIItemGroupsN.ColCount-1 do
    for j:=ARow to sgStaffOrdersKPIItemGroupsN.RowCount-2 do
      sgStaffOrdersKPIItemGroupsN.Cells[i, j] := sgStaffOrdersKPIItemGroupsN.Cells[i, j+1];

  if (sgStaffOrdersKPIItemGroupsN.RowCount = 1) then
  begin
    sgStaffOrdersKPIItemGroupsN.Cells[0, 0] := '';
    sgStaffOrdersKPIItemGroupsN.Cells[1, 0] := '';
  end;

  sgStaffOrdersKPIItemGroupsN.RowCount := sgStaffOrdersKPIItemGroupsN.RowCount-1;
  txtStaffOrdersKPINameChange(Sender);
end;

procedure TformReports.cmdStaffOrdersKPIAddIG_DClick(Sender: TObject);
var
  i, j: Integer;
  FoundIG: Boolean;

begin
  with DBGridStaffOrdersKPI.DataSource.DataSet do
  begin
    for i:=0 to DBGridStaffOrdersKPI.SelectedRows.Count-1 do
    begin
      FoundIG := False;
      GotoBookmark(DBGridStaffOrdersKPI.SelectedRows.Items[i]);
      for j:=0 to sgStaffOrdersKPIItemGroupsD.RowCount-1 do
      begin
        if (sgStaffOrdersKPIItemGroupsD.Cells[1, j] = FieldAsString(FieldByName('itemgroupname'), '')) then
        begin
          FoundIG := True;
          Break;
        end;
      end;
      if (FoundIG) then
        Continue;

      if (sgStaffOrdersKPIItemGroupsD.Cells[0, sgStaffOrdersKPIItemGroupsD.Row] <> '') then
        sgStaffOrdersKPIItemGroupsD.RowCount := sgStaffOrdersKPIItemGroupsD.RowCount+1;
      sgStaffOrdersKPIItemGroupsD.Row := sgStaffOrdersKPIItemGroupsD.RowCount-1;

      sgStaffOrdersKPIItemGroupsD.Cells[0, sgStaffOrdersKPIItemGroupsD.Row] := IntToStr(FieldAsInt(FieldByName('itemgroupid'), 0));
      sgStaffOrdersKPIItemGroupsD.Cells[1, sgStaffOrdersKPIItemGroupsD.Row] := FieldAsString(FieldByName('itemgroupname'), '');
    end;
  end;   
  txtStaffOrdersKPINameChange(Sender);
end;

procedure TformReports.cmdStaffOrdersKPIRemoveIG_DClick(Sender: TObject);
var
  ARow, i, j: Integer;

begin
  ARow := sgStaffOrdersKPIItemGroupsD.Row;

  for i:=0 to sgStaffOrdersKPIItemGroupsD.ColCount-1 do
    for j:=ARow to sgStaffOrdersKPIItemGroupsD.RowCount-2 do
      sgStaffOrdersKPIItemGroupsD.Cells[i, j] := sgStaffOrdersKPIItemGroupsD.Cells[i, j+1];

  if (sgStaffOrdersKPIItemGroupsD.RowCount = 1) then
  begin
    sgStaffOrdersKPIItemGroupsD.Cells[0, 0] := '';
    sgStaffOrdersKPIItemGroupsD.Cells[1, 0] := '';
  end;

  sgStaffOrdersKPIItemGroupsD.RowCount := sgStaffOrdersKPIItemGroupsD.RowCount-1;
  txtStaffOrdersKPINameChange(Sender);
end;

procedure TformReports.sgStaffOrdersKPIClick(Sender: TObject);
var
  tmpStaffOrderKPI: TStaffOrderKPI;
  i, j: Integer;

begin
  PopulatingFields := True;
  try
    txtStaffOrdersKPIName.Text := '';
    txtStaffOrdersKPIConversionRate.Text := '1';
    txtStaffOrdersKPIItemGroupsNName.Text := '';
    txtStaffOrdersKPIItemGroupsDName.Text := '';
    rgStaffORdersKPIDisplayResultAs.ItemIndex := 0;
    rgStaffOrdersKPITotalsN.ItemIndex := 0;
    rgStaffOrdersKPITotalsD.ItemIndex := 0;
    for i:=0 to sgStaffOrdersKPIItemGroupsN.ColCount-1 do
      for j:=0 to sgStaffOrdersKPIItemGroupsN.RowCount-1 do
        sgStaffOrdersKPIItemGroupsN.Cells[i, j] := '';

    sgStaffOrdersKPIItemGroupsN.RowCount := 1;

    for i:=0 to sgStaffOrdersKPIItemGroupsD.ColCount-1 do
      for j:=0 to sgStaffOrdersKPIItemGroupsD.RowCount-1 do
        sgStaffOrdersKPIItemGroupsD.Cells[i, j] := '';

    sgStaffOrdersKPIItemGroupsD.RowCount := 1;

    tmpStaffOrderKPI := TStaffOrderKPI(sgStaffOrdersKPI.Objects[0, sgStaffOrdersKPI.Row]);
    if (not Assigned(tmpStaffOrderKPI)) then Exit;

    with tmpStaffOrderKPI do
    begin
      txtStaffOrdersKPIName.Text := KPIName;
      txtStaffOrdersKPIConversionRate.Text := FloatToStr(ConversionRatio);
      txtStaffOrdersKPIItemGroupsNName.Text := ItemGroupNName;
      txtStaffOrdersKPIItemGroupsDName.Text := ItemGroupDName;
      rgStaffORdersKPIDisplayResultAs.ItemIndex := DisplayAs;
      rgStaffOrdersKPITotalsN.ItemIndex := ItemGroupNTotalAs;
      rgStaffOrdersKPITotalsD.ItemIndex := ItemGroupDTotalAs;

      for i:=0 to ItemGroupN.Count-1 do
      begin
        if (i > 0) then
          sgStaffOrdersKPIItemGroupsN.RowCount := sgStaffOrdersKPIItemGroupsN.RowCount+1;
        sgStaffOrdersKPIItemGroupsN.Row := sgStaffOrdersKPIItemGroupsN.RowCount-1;
        sgStaffOrdersKPIItemGroupsN.Cells[0, i] := IntToStr(ItemGroupN.Get(i));
        if (DBGridStaffOrdersKPI.DataSource.DataSet.Locate('itemgroupid', sgStaffOrdersKPIItemGroupsN.Cells[0, i], [] )) then
          sgStaffOrdersKPIItemGroupsN.Cells[1, i] := FieldAsString(DBGridStaffOrdersKPI.DataSource.DataSet.FieldByName('itemgroupname'), '');
      end;

      for i:=0 to ItemGroupD.Count-1 do
      begin     
        if (i > 0) then
          sgStaffOrdersKPIItemGroupsD.RowCount := sgStaffOrdersKPIItemGroupsD.RowCount+1;
        sgStaffOrdersKPIItemGroupsD.Row := sgStaffOrdersKPIItemGroupsD.RowCount-1;
        sgStaffOrdersKPIItemGroupsD.Cells[0, i] := IntToStr(ItemGroupD.Get(i));
        if (DBGridStaffOrdersKPI.DataSource.DataSet.Locate('itemgroupid', sgStaffOrdersKPIItemGroupsD.Cells[0, i], [] )) then
          sgStaffOrdersKPIItemGroupsD.Cells[1, i] := FieldAsString(DBGridStaffOrdersKPI.DataSource.DataSet.FieldByName('itemgroupname'), '');
      end;

      DBGridSalePeriods.DataSource.DataSet.First;

    end;
  finally
    PopulatingFields := False;
  end;
end;

procedure TformReports.txtStaffOrdersKPINameChange(Sender: TObject);
var
  tmpStaffOrderKPI: TStaffOrderKPI;
  i: Integer;

begin
  if (PopulatingFields) then Exit;

  tmpStaffOrderKPI := TStaffOrderKPI(sgStaffOrdersKPI.Objects[0, sgStaffOrdersKPI.Row]);
  if (not Assigned(tmpStaffOrderKPI)) then
  begin
    tmpStaffOrderKPI := TStaffOrderKPI.Create;
    sgStaffOrdersKPI.Objects[0,sgStaffOrdersKPI.Row] := tmpStaffOrderKPI;
    AppDetails.ListStaffOrderKPI.Add(tmpStaffOrderKPI);
    tmpStaffOrderKPI.KPIID := AppDetails.ListStaffOrderKPI.Count;
  end;

  with tmpStaffOrderKPI do
  begin
    sgStaffOrdersKPI.Cells[0,sgStaffOrdersKPI.Row] := txtStaffOrdersKPIName.Text;
    KPIName := txtStaffOrdersKPIName.Text;
    ConversionRatio := StrToFloatDef(txtStaffOrdersKPIConversionRate.Text, 0);
    DisplayAs := rgStaffORdersKPIDisplayResultAs.ItemIndex;
    ItemGroupNName := txtStaffOrdersKPIItemGroupsNName.Text;
    ItemGroupDName := txtStaffOrdersKPIItemGroupsDName.Text;
    ItemGroupNTotalAs := rgStaffOrdersKPITotalsN.ItemIndex;
    ItemGroupDTotalAs := rgStaffOrdersKPITotalsD.ItemIndex;

    ItemGroupN.Clear;
    for i:=0 to sgStaffOrdersKPIItemGroupsN.RowCount-1 do
    begin
      if (StrToIntDef(sgStaffOrdersKPIItemGroupsN.Cells[0, i], 0) <> 0) then
        ItemGroupN.Add(StrToIntDef(sgStaffOrdersKPIItemGroupsN.Cells[0, i], 0));
    end;

    ItemGroupD.Clear;
    for i:=0 to sgStaffOrdersKPIItemGroupsD.RowCount-1 do
    begin
      if (StrToIntDef(sgStaffOrdersKPIItemGroupsD.Cells[0, i], 0) <> 0) then
        ItemGroupD.Add(StrToIntDef(sgStaffOrdersKPIItemGroupsD.Cells[0, i], 0));
    end;
  end;
  cmdStaffOrdersKPIOK.Enabled := True;
end;

procedure TformReports.cmdStaffOrdersKPINewClick(Sender: TObject);
begin
  sgStaffOrdersKPI.RowCount := sgStaffOrdersKPI.RowCount+1;
  sgStaffOrdersKPI.Row := sgStaffOrdersKPI.RowCount-1;
  sgStaffOrdersKPIClick(Sender);
end;

procedure TformReports.cmdStaffOrdersKPIDeleteClick(Sender: TObject);
var
  tmpStaffOrderKPI: TStaffOrderKPI;

begin
  tmpStaffOrderKPI := TStaffOrderKPI(sgStaffOrdersKPI.Objects[0, sgStaffOrdersKPI.Row]);
  if Assigned(tmpStaffOrderKPI) then
  begin
    if (AppDetails.ListStaffOrderKPI.IndexOf(tmpStaffOrderKPI) > -1) then
      AppDetails.ListStaffOrderKPI.Delete(AppDetails.ListStaffOrderKPI.IndexOf(tmpStaffOrderKPI));

    tmpStaffOrderKPI.Free;
    LoadStaffOrderKPI;
    sgStaffOrdersKPIClick(Sender);
  end;
  cmdStaffOrdersKPIOK.Enabled := True;
end;

procedure TformReports.LoadStaffOrderKPI;
var
  tmpStaffOrderKPI: TStaffOrderKPI;
  i, j: Integer;

begin
  for i:=0 to sgStaffOrdersKPI.ColCount-1 do
  begin
    for j:=1 to sgStaffOrdersKPI.RowCount-1 do
    begin
      sgStaffOrdersKPI.Cells[i, j] := '';
      sgStaffOrdersKPI.Objects[i, j] := nil;
    end;
  end;
  sgStaffOrdersKPI.RowCount := 2;
  sgStaffOrdersKPI.Row := 1;
  sgStaffOrdersKPI.Cells[0,1] := '';
  sgStaffOrdersKPI.Objects[0,1] := nil;

  for i:=0 to AppDetails.ListStaffOrderKPI.Count-1 do
  begin
    tmpStaffOrderKPI := TStaffOrderKPI(AppDetails.ListStaffOrderKPI.Items[i]);
    if (sgStaffOrdersKPI.Cells[0, sgStaffOrdersKPI.Row] <> '') then
      sgStaffOrdersKPI.RowCount := sgStaffOrdersKPI.RowCount+1;

    sgStaffOrdersKPI.Row := sgStaffOrdersKPI.RowCount-1;
    sgStaffOrdersKPI.Cells[0, sgStaffOrdersKPI.Row] := tmpStaffOrderKPI.KPIName;
    sgStaffOrdersKPI.Objects[0, sgStaffOrdersKPI.Row] := tmpStaffOrderKPI;
  end;

  sgStaffOrdersKPI.Row := 1;
end;

procedure TformReports.cmdStaffOrdersKPICancelClick(Sender: TObject);
begin
  lblReportType.Visible := True;
  cmdDoReport.Visible := True;
  cmdStaffOrdersKPI.Visible := True;
  pnlStaffOrdersKPISetup.Visible := False;
  AppDetails.LoadKPIDetails(AppDetails.Path, '\otreports.ini');
  LoadStaffOrderKPI;
  cmdStaffOrdersKPIOK.Enabled := False;

end;

procedure TformReports.cmdStaffOrdersKPIOKClick(Sender: TObject);
begin
  pnlStaffOrdersKPISetup.Visible := False;
  AppDetails.SaveKPIDetails;
  cmdStaffOrdersKPIOK.Enabled := False;
end;

procedure TformReports.chkIncludeStaffOrdersKPIClick(Sender: TObject);
begin
  AppDetails.IncludeStaffOrdersKPI := chkIncludeStaffOrdersKPI.Checked;
  AppDetails.SaveKPIDetails;
end;

procedure TformReports.rgStaffOrdersKPIItemGroupClick(Sender: TObject);
begin
  if (rgStaffOrdersKPIItemGroup.ItemIndex = 0) then Exit;

  DBGridStaffOrdersKPI.SelectedRows.Clear;
  with DBGridStaffOrdersKPI.DataSource.DataSet do
  begin
    First;
    while (not Eof) do
    begin
      if ((FieldByName('forb').AsString = 'F') and (rgStaffOrdersKPIItemGroup.ItemIndex in [1, 2])) or // aka All & Food
         ((FieldByName('forb').AsString = 'B') and (rgStaffOrdersKPIItemGroup.ItemIndex in [1, 3])) then// aka All & Beverage
        DBGridStaffOrdersKPI.SelectedRows.CurrentRowSelected := True;

      Next;
    end;
    First;
  end;
end;

procedure TformReports.ShowSectionSalesReport;
begin
try
    formQRSectionSales := TformQRSectionSales.Create(Application);

    Title1 := sSectionTitle1;
    Title2 := '';
    Title3 := Format(sDateRange,[FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FFromTime),FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM',FToTime)]);

    if (not CheckDate) then begin
      Exit;
    end;

    with dm.qrSectionSales do begin
      Close;
      UnPrepare;
      SQL.Clear;
      SQL.Add('select * from getsectionsales (:poutletid, :premotelocationid, :pforb, :psectionid, :pitemgroupid, :fromtime, :totime)');

      if (radGroup.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if (radUnits.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if (radPrice.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if (radGross.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if (radDiscount.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if (radNett.Checked) then begin
        SQL.Add('order by outletname, remotelocationname, sectionid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      if (radItemGroup.Checked) then begin
        ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
        ParamByName('pforb').AsString := '';
        Title2 := Title2 + ', ' + Glbs.ItemGroup;
      end
      else begin
        ParamByName('pitemgroupid').AsInteger := -1;
        if (radFAndB.Checked) then begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if (radFOnly.Checked) then begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      if (cmbOutlets.ItemIndex = 0) then begin
        if ((chkOutletBreakdown.Checked) and (chkOutletBreakdown.Enabled)) then begin
          ParamByName('poutletid').AsInteger := -1;
        end
        else begin
          ParamByName('poutletid').AsInteger := -2;
        end;
        Title2 := sAllOutlets + Title2;
      end
      else begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      if (cmbRemoteLocations.ItemIndex = 0) then begin
        if ((chkRemoteLocationBreakdown.Checked) and (chkRemoteLocationBreakdown.Enabled)) then begin
          ParamByName('premotelocationid').AsInteger := -1;
        end
        else begin
          ParamByName('premotelocationid').AsInteger := -2;
        end;
      end
      else begin
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;
      end;

      if (radSectionAll.Checked) then begin
        ParamByName('psectionid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllSections;
      end
      else begin
        ParamByName('psectionid').AsInteger := Glbs.SectionsSectionID;
        Title1 := Title1 + ', ' + Glbs.SectionsSection;
      end;

      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    if (chkExportFile.Checked) then begin
      SaveDialog.Title := sSectionDlgTitle;
      SaveDialog.FileName := 'SectionSales' + FormatDateTime('yyyymmddhhnn',Now) + '.csv';
      if (SaveDialog.Execute) then begin
        formQRSectionSales.WBCSV.FileName := SaveDialog.FileName;
        formQRSectionSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRSectionSales.QRSectionSales.ReportTitle := sSectionRepTitle;
    formQRSectionSales.QRSectionSales.Preview;
  finally
    formQRSectionSales.Free;
  end;
end;

procedure TformReports.DBGridSectionsCellClick(Column: TColumn);
begin
  radSectionSelected.Checked := True;
  radComboSelected.Checked := True; 
end;

procedure TformReports.radSecCurrentClick(Sender: TObject);
begin
  dm.SectionsCurrent := (TRadioButton(Sender).Tag = 0);
end;

initialization
begin
  ApplicationInitialization;
end;


end.

