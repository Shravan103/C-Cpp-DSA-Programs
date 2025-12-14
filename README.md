procedure TformReports.FormCreate(Sender: TObject);
begin
  cmbLocation.Items.Clear;

  // Explicit ALL option
  cmbLocation.Items.Add('ALL');

  with dm.qrTemp do
  begin
    Close;
    UnPrepare;
    SQL.Clear;
    SQL.Add('select distinct location');
    SQL.Add('from staff');
    SQL.Add('where staffok = 1');
    SQL.Add('and location is not null');
    SQL.Add('order by location');

    Open;

    while not EOF do
    begin
      cmbLocation.Items.Add(Trim(FieldByName('location').AsString));
      Next;
    end;

    Close;
  end;

  cmbLocation.ItemIndex := 0; // Default = ALL
end;

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

procedure TformReports.ShowStaffDetailsReport;
var
  FromDate: TDateTime;
begin
  try
    { 🔒 RESET GLOBAL TITLES }
    Title1 := sStaffDetailsTitle1;
    Title2 := '';
    Title3 := '';

    formQRStaffDetails := TformQRStaffDetails.Create(Application);

    if not CheckDate then
      Exit;

    with dm.qrStaffDetails do
    begin
      Close;
      UnPrepare;
      SQL.Clear;

      SQL.Add('select * from staff where staffok = 1');

      if radAllStaff.Checked then
        Title2 := sAllStaff
      else if radCurrentStaff.Checked then
      begin
        SQL.Add('and isinactive = 0');
        Title2 := sCurrentStaff;
      end
      else if radPastStaff.Checked then
      begin
        SQL.Add('and isinactive = 1');
        Title2 := sPastStaff;
      end;

      if spnWorkedYears.Value > 0 then
      begin
        FromDate := IncYear(Now, -spnWorkedYears.Value);
        SQL.Add('and whenjoined <= :pfromdate');
        ParamByName('pfromdate').AsDateTime := FromDate;

        Title2 := Title2 + ', ' +
          Format(sWorkedForYears, [spnWorkedYears.Value]);
      end;

      if cmbLocation.ItemIndex > 0 then
      begin
        SQL.Add('and location = :plocation');
        ParamByName('plocation').AsString := cmbLocation.Text;

        Title2 := Title2 + ', ' + cmbLocation.Text;
      end;

      SQL.Add('order by staffname');
      Open;
    end;

    { ✅ FINAL COMPOSED TITLE }
    Title2 := Glbs.OutletName + ' - ' + Title2;

    formQRStaffDetails.QRStaffDetails.Preview;
  finally
    formQRStaffDetails.Free;
  end;
end;
unit UQRStaffDetails;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes,
  Vcl.Graphics, Vcl.Controls, Vcl.Forms, Vcl.Dialogs,
  QRCtrls, QuickRpt, Vcl.ExtCtrls,
  UTypesConstants, UWBCSV{, IvDictio, IvMulti};

type
  TformQRStaffDetails = class(TForm)
    QRStaffDetails: TQuickRep;
    bndTitle: TQRBand;
    lblTitle1: TQRLabel;
    lblTitle2: TQRLabel;
    QRShape: TQRShape;
    lblTime: TQRLabel;
    lblWizBangWaiter: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    QRSysData1: TQRSysData;
    lblStaffId: TQRLabel;
    lblStaffName: TQRLabel;
    lblStaffPin: TQRLabel;
    lblStaffRole: TQRLabel;
    lbStaffLocation: TQRLabel;
    DetailBand: TQRBand;
    QRBSummary: TQRBand;
    txtStaffId: TQRDBText;
    txtStaffName: TQRDBText;
    txtStaffPin: TQRDBText;
    txtStaffRole: TQRDBText;
    txtRemoteLoc: TQRDBText;

    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure QRStaffDetailsBeforePrint(Sender: TCustomQuickRep;
      var PrintReport: Boolean);
    procedure bndTitleBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
    procedure QRBSummaryBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    TheTime: TDateTime;
  public
    WBCSV: TWBCSV;
  end;

var
  formQRStaffDetails: TformQRStaffDetails;

implementation
{$R *.dfm}

uses
  UDM, UReports, UResources, LMain;

{******************************************************************************}
procedure TformQRStaffDetails.FormCreate(Sender: TObject);
begin
  WBCSV := TWBCSV.Create;
end;

{******************************************************************************}
procedure TformQRStaffDetails.FormDestroy(Sender: TObject);
begin
  WBCSV.Free;
end;

{******************************************************************************}
procedure TformQRStaffDetails.QRStaffDetailsBeforePrint(
  Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
  TheTime := Now;

  with WBCSV do begin
    if Active then begin
      WriteString(Title1);
      WriteString(Title2);

      SendString(sStaffID);
      SendString(sStaffName);
      SendString(sStaffPin);
      SendString(sRole);
      SendString(sLocation);
      WriteLine;
    end;
  end;
end;

{******************************************************************************}
procedure TformQRStaffDetails.bndTitleBeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
begin
  lblTitle1.Caption := Title1;
  lblTitle2.Caption := Title2;

  lblTime.Caption :=
    Format(sPrinted,
      [FormatDateTime('ddd dd mmm yy hh:mm AM/PM', TheTime)]);
end;

{******************************************************************************}
procedure TformQRStaffDetails.DetailBandBeforePrint(
  Sender: TQRCustomBand; var PrintBand: Boolean);
begin
  with WBCSV do begin
    if Active then begin
      with DM.qrStaffDetails do begin
        SendInteger(FieldByName('staffid').AsInteger);
        SendString(FieldByName('staffname').AsString);
        SendString(FieldByName('staffpin').AsString);
        SendString(FieldByName('staffrole').AsString);
        SendString(FieldByName('remotelocation').AsString);
        WriteLine;
      end;
    end;
  end;
end;

{******************************************************************************}
procedure TformQRStaffDetails.QRBSummaryBeforePrint(
  Sender: TQRCustomBand; var PrintBand: Boolean);
begin
  with WBCSV do begin
    if Active then begin
      CloseFile;
    end;
  end;
end;

end.
