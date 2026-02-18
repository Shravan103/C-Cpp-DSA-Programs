I have added a TadvSmoothPanel on the form, I want to add a gradient from left to right of colors using skia to this smoothpanel, the values should be taken from the json file:
CURRENT JSON:
{
  "designTokens": {
    "color.primary": "#2E86C1",
    "color.primary.dark": "#1F618D",
    "color.cancel": "#C0392B",
    "color.panel": "#F4F6F7",
    "color.disabled.panel": "#D5D8DC",
    "color.text.light": "#FF00FF",
    "color.text.dark": "#212121",
    "font.default.name": "Segoe UI",
    "font.default.size": "10",
	"color.text.tmsbtn": "#00FF00",
  },

  "semanticTokens": {
    "button.background.color": "{color.primary}",
    "button.font.name": "{font.default.name}",
    "button.font.size": "{font.default.size}",
    "button.font.color": "{color.text.light}",
	"button.font.color2": "{color.text.tmsbtn}",

    "panel.background.color": "{color.panel}",
    "panel.font.color": "{color.text.dark}",

    "grid.background.color": "{color.panel}",
    "grid.font.color": "{color.text.dark}",

    "label.font.name": "{font.default.name}",
    "label.font.size": "{font.default.size}",
    "label.font.color": "{color.text.dark}"
  },

  "roles": {
    "control.button": {
      "FontName": "{button.font.name}",
      "FontSize": "{button.font.size}",
      "FontColor": "{button.font.color}"
    },

    "control.actionbutton": {
      "inherits": "control.button",
      "BackgroundColor": "{button.font.color2}"
    },

    "control.TMSbutton": {
      "inherits": "control.button",
      "BackgroundColor": "{color.cancel}"
    },

    "control.panel": {
      "BackgroundColor": "{panel.background.color}",
      "FontColor": "{panel.font.color}"
    },

    "control.disabledpanel": {
      "inherits": "control.panel",
      "BackgroundColor": "{color.disabled.panel}"
    },

    "control.grid": {
      "BackgroundColor": "{grid.background.color}",
      "FontColor": "{grid.font.color}"
    },

    "control.label": {
      "FontName": "{label.font.name}",
      "FontSize": "{label.font.size}",
      "FontColor": "{label.font.color}"
    }
  }
}

CODE:
unit Unit1;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Theme.Manager, Vcl.Grids, IB_Grid, Vcl.ExtCtrls, Vcl.Buttons,
  AdvLabel, AdvPanel, AdvGlassButton, AdvGlowButton, System.Skia, Vcl.Skia,
  AdvSmoothPanel;

type
  TForm1 = class(TForm)
    btnToggleTheme: TButton;
    Button1: TButton;
    Panel1: TPanel;
    IB_Grid1: TIB_Grid;
    BitBtn1: TBitBtn;
    ActionBitBtn4: TBitBtn;
    BitBtn2: TBitBtn;
    SpeedButton1: TSpeedButton;
    TMSAdvGlowButton1: TAdvGlowButton;
    TMSAdvGlassButton1: TAdvGlassButton;
    AdvPanel1: TAdvPanel;
    AdvSmoothPanel1: TAdvSmoothPanel;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure btnToggleThemeClick(Sender: TObject);
    procedure ActionBitBtn4Click(Sender: TObject);
    procedure BitBtn2Click(Sender: TObject);
  private
    FThemeManager: TThemeManager;
    FCurrentTheme: string;
    FThemeFolder: string;
    procedure LoadAndApplyTheme(const FileName: string);
  public
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

uses UItemGroups;

procedure TForm1.FormCreate(Sender: TObject);
begin
  FThemeManager := TThemeManager.Create;

  // Default theme
  FCurrentTheme := 'light.theme.json';
  LoadAndApplyTheme(FCurrentTheme);

  TMSAdvGlassButton1.Font.Style := TMSAdvGlassButton1.Font.Style + [fsBold];
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  FThemeManager.Free;
end;

procedure TForm1.LoadAndApplyTheme(const FileName: string);
var
  i: Integer;
begin
  FThemeFolder := '';

  //Theme file must be in same folder as exe
  if not FileExists(FileName) then
  begin
    ShowMessage('Theme file not found: ' + FileName);
    Exit;
  end;
  FThemeFolder := ChangeFileExt(FileName, '');

  Screen.Cursor := crHourGlass;
  try
    FThemeManager.LoadTheme(FileName);

    for i := 0 to Screen.FormCount - 1 do
      FThemeManager.ApplyToForm(Screen.Forms[I]);
  finally
    Screen.Cursor := crDefault;
  end;
end;

procedure TForm1.ActionBitBtn4Click(Sender: TObject);
begin
  Panel1.Enabled := not Panel1.Enabled;
  FThemeManager.ApplyToControl(Panel1);

  AdvPanel1.Enabled := not AdvPanel1.Enabled;
  FThemeManager.ApplyToControl(AdvPanel1);
end;

procedure TForm1.BitBtn2Click(Sender: TObject);
begin
  formItemGroups := TformItemGroups.Create(Self);
  FThemeManager.ApplyToForm(formItemGroups);
  formItemGroups.Show;
end;

procedure TForm1.btnToggleThemeClick(Sender: TObject);
begin
  if SameText(FCurrentTheme, 'light.theme.json') then
    FCurrentTheme := 'dark.theme.json'
  else
    FCurrentTheme := 'light.theme.json';

  LoadAndApplyTheme(FCurrentTheme);
end;
{


UListForm, inherited many times, e.g., UItemGroups, has buttons that are all the same style besides the image.
designtoken
image_first = first.bmp //Glyph
image_last = last.bmp //Glyph

control.button
  -font.size = 10

control.button.list_form.navigation
  -inherits from control.button
  -font.size = 8

control.button.list_form.first
  -inherits from control.button.list_form.navigation
  -image_file = image_first

control.button.list_form.last
  -inherits from control.button.list_form.navigation
  -image_file = image_last

control.button.list_form.option
  -inherits from control.button
  -font.size = 12
  -flat = True

control.button.list_form.current
  -inherits from control.button.list_form.option
  -image_file = current.bmp

control.button.list_form.deleted
  -inherits from control.button.list_form.option
  -image_file = deleted.bmp
}
end.
========================================================
unit Theme.Manager;

interface

uses
  System.Generics.Collections, System.JSON,
  Vcl.Forms, Vcl.Controls, AdvGlowButton, AdvGlassButton, AdvPanel, AdvLabel,
  Theme.Types, Theme.Stylers;

type
  TThemeManager = class
  private
    FDesignTokens: TDictionary<string,string>;
    FSemanticTokens: TDictionary<string,string>;
    FRoles: TDictionary<string,TJSONObject>;
    FResolvedCache: TDictionary<string,TResolvedStyle>;
    FStylers: TList<IControlStyler>;

    function ResolveValue(const S: string): string;
    function InternalResolveRole(const Role: string): TResolvedStyle;
    function ResolveRole(const Role: string): TResolvedStyle;
    function GetRoleForControl(AControl: TControl): string;

    procedure ClearResolvedCache;
  public
    constructor Create;
    destructor Destroy; override;

    procedure RegisterStyler(AStyler: IControlStyler);
    procedure LoadTheme(const FileName: string);

    procedure ApplyToForm(AForm: TForm);
    procedure ApplyToControl(AControl: TControl);
  end;

implementation

uses
  System.SysUtils, System.IOUtils, Vcl.StdCtrls, System.StrUtils, Vcl.ExtCtrls, Vcl.Buttons;

constructor TThemeManager.Create;
begin
  FDesignTokens := TDictionary<string,string>.Create;
  FSemanticTokens := TDictionary<string,string>.Create;
  FRoles := TDictionary<string,TJSONObject>.Create;
  FResolvedCache := TDictionary<string,TResolvedStyle>.Create;
  FStylers := TList<IControlStyler>.Create;

  RegisterStyler(TButtonStyler.Create);
  RegisterStyler(TSpeedButtonStyler.Create);
  RegisterStyler(TBitButtonStyler.Create);
  RegisterStyler(TPanelStyler.Create);

  // === NEW TMS stylers ===
  RegisterStyler(TAdvGlowButtonStyler.Create);
  RegisterStyler(TAdvGlassButtonStyler.Create);
  RegisterStyler(TPanelStyler.Create);
  RegisterStyler(TAdvPanelStyler.Create);

  RegisterStyler(TIBGridStyler.Create);
end;

destructor TThemeManager.Destroy;
begin
  ClearResolvedCache;
  FResolvedCache.Free;
  FRoles.Free;
  FSemanticTokens.Free;
  FDesignTokens.Free;
  FStylers.Free;
  inherited;
end;

procedure TThemeManager.ClearResolvedCache;
var Style: TResolvedStyle;
begin
  for Style in FResolvedCache.Values do
    Style.Free;
  FResolvedCache.Clear;
end;

procedure TThemeManager.RegisterStyler(AStyler: IControlStyler);
begin
  FStylers.Add(AStyler);
end;

procedure TThemeManager.LoadTheme(const FileName: string);
var
  JSON: TJSONObject;
  Obj: TJSONObject;
  Pair: TJSONPair;
begin
  ClearResolvedCache;
  FDesignTokens.Clear;
  FSemanticTokens.Clear;
  FRoles.Clear;

  JSON := TJSONObject.ParseJSONValue(
    TFile.ReadAllText(FileName)) as TJSONObject;

  try
    Obj := JSON.GetValue('designTokens') as TJSONObject;
    for Pair in Obj do
      FDesignTokens.AddOrSetValue(
        Pair.JsonString.Value,
        Pair.JsonValue.Value);

    Obj := JSON.GetValue('semanticTokens') as TJSONObject;
    for Pair in Obj do
      FSemanticTokens.AddOrSetValue(
        Pair.JsonString.Value,
        Pair.JsonValue.Value);

    Obj := JSON.GetValue('roles') as TJSONObject;
    for Pair in Obj do
      FRoles.AddOrSetValue(
        Pair.JsonString.Value,
        Pair.JsonValue as TJSONObject);
  finally
    //JSON.Free; //if Free, get errors when call InternalResolveRole()
  end;
end;

function TThemeManager.ResolveValue(const S: string): string;
var Key: string;
begin
  Result := S;

  //'{' used to indicate that value needs to be resolved
  if (Length(S) > 2) and (S.StartsWith('{')) then
  begin
    //remove {}
    Key := S.Substring(1, S.Length-2);

    //try get result from semantic tokens
    FSemanticTokens.TryGetValue(Key, Result);

    //if not found in semantic tokens
    if Result = '' then
      //look for in design tokens
      FDesignTokens.TryGetValue(Key, Result)
    //if semantic token starts with '{' indicating value needs to be resolved
    else if (Length(Result) > 2) and (S.StartsWith('{')) then
    begin
      //remove {} and look for in design tokens
      Key := Result.Substring(1, Result.Length-2);
      FDesignTokens.TryGetValue(Key, Result);
    end;
  end;
end;

function TThemeManager.InternalResolveRole(
  const Role: string): TResolvedStyle;
var
  Obj: TJSONObject;
  Parent: string;
  ParentStyle: TResolvedStyle;
  ParentStylePair: TPair<string, string>;
  Pair: TJSONPair;
begin
  Result := TResolvedStyle.Create;

  if not FRoles.TryGetValue(Role, Obj) then
    Exit;

  Parent := Obj.GetValue<string>('inherits', '');

  //original code, where a parent role is passed back and then overridden
  //if Parent <> '' then
  //  Result := ResolveRole(Parent);

  if Parent <> '' then
  begin
    ParentStyle := ResolveRole(Parent);

    for ParentStylePair in ParentStyle.Values do
      Result.SetValue(ParentStylePair.Key, ParentStylePair.Value);
  end;

  for Pair in Obj do
    if Pair.JsonString.Value <> 'inherits' then
      Result.SetValue(
        Pair.JsonString.Value,
        ResolveValue(Pair.JsonValue.Value));
end;
{
Issue with function TThemeManager.InternalResolveRole() above:
-when a style inherits another style, that parent style is passed back and then any
new values are used to override existing parent
role, instead of adding all the properties to the new resolved role.
}

function TThemeManager.ResolveRole(
  const Role: string): TResolvedStyle;
begin
  if FResolvedCache.TryGetValue(Role, Result) then
    Exit;

  Result := InternalResolveRole(Role);
  FResolvedCache.Add(Role, Result);
end;

function TThemeManager.GetRoleForControl(AControl: TControl): string;
begin
  // Buttons: include TMS variants
  if (AControl is TButton) or (AControl is TSpeedButton) or (AControl is TBitBtn)
     or (AControl is TAdvGlowButton) or (AControl is TAdvGlassButton) then
  begin
    if ContainsText(AControl.Name, 'TMS') then
      Exit('control.TMSbutton')
    else if ContainsText(AControl.Name, 'Action') then
      Exit('control.actionbutton')
    else
      Exit('control.button');
  end;

  // Panels: include TMS
  if (AControl is TPanel) or (AControl is TAdvPanel) then
  begin
    if AControl.Enabled then
      Exit('control.panel')
    else
      Exit('control.disabledpanel');
  end;

  // Labels: include TMS label
  if (AControl is TLabel) then
    Exit('control.label');

  // Grids
  if AControl.ClassName = 'TIB_Grid' then
    Exit('control.grid');

  Result := '';
end;

procedure TThemeManager.ApplyToControl(AControl: TControl);
var
  Role: string;
  Style: TResolvedStyle;
  Styler: IControlStyler;
  I: Integer;
  WinCtrl: TWinControl;
begin
  if AControl = nil then
    Exit;

  Role := GetRoleForControl(AControl);
  if Role = '' then Exit;

  Style := ResolveRole(Role);

  for Styler in FStylers do
    if Styler.Supports(AControl) then
    begin
      Styler.ApplyStyle(AControl, Style);
      Break;
    end;

  if AControl is TWinControl then
  begin
    WinCtrl := TWinControl(AControl);

    for I := 0 to WinCtrl.ControlCount - 1 do
      ApplyToControl(WinCtrl.Controls[I]);
  end;
end;

procedure TThemeManager.ApplyToForm(AForm: TForm);
var I: Integer;
begin
  for I := 0 to AForm.ControlCount-1 do
    ApplyToControl(AForm.Controls[I]);
end;

end.

======================================================================

unit Theme.Stylers;

interface

uses
  Vcl.Controls, Vcl.StdCtrls, Vcl.ExtCtrls,
  Theme.Types,
    System.SysUtils,
  System.Classes,
  System.JSON,
  System.IOUtils,
  Vcl.Graphics,
  Vcl.Forms,
  Vcl.Buttons, TypInfo,
  System.Skia, Vcl.Skia;

type
  IControlStyler = interface
    ['{A1111111-2222-3333-4444-555555555555}']
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TButtonStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TSpeedButtonStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  //NB: TBitBtn doesn't inherit from TButton, but it will share same 'control.button' role as TButton.
  TBitButtonStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TPanelStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TIBGridStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

    // === NEW: TMS stylers ===
  TAdvGlowButtonStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TAdvGlassButtonStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  TAdvPanelStyler = class(TInterfacedObject, IControlStyler)
  public
    function Supports(AControl: TControl): Boolean;
    procedure ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
  end;

  procedure TrySetColorProp(AControl: TObject; const PropName: string; AColor: TColor);

implementation

uses
  IB_Grid, AdvGlowButton, AdvGlassButton, AdvPanel, AdvLabel;

function TButtonStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TButton;
end;

procedure TButtonStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var Btn: TButton;
begin
  Btn := TButton(AControl);
  Btn.Font.Name := AStyle.GetValue('FontName');
  Btn.Font.Size := StrToInt(AStyle.GetValue('FontSize'));

  //NB: TButton ignores Font.Color because Windows theme engine paints the control and chooses the text color, so the
  //line below has no effect. It does work for TBitBtn though. Use a different control to be able to control font color.
  Btn.Font.Color := AStyle.Color('FontColor', Btn.Font.Color); //is ignored
end;

function TSpeedButtonStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TSpeedButton;
end;

procedure TSpeedButtonStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var Btn: TSpeedButton;
begin
  Btn := TSpeedButton(AControl);
  Btn.Font.Name := AStyle.GetValue('FontName');
  Btn.Font.Size := StrToInt(AStyle.GetValue('FontSize'));
  Btn.Font.Color := AStyle.Color('FontColor', Btn.Font.Color);
end;

function TBitButtonStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TBitBtn;
end;

procedure TBitButtonStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var Btn: TBitBtn;
begin
  Btn := TBitBtn(AControl);
  Btn.Font.Name := AStyle.GetValue('FontName');
  Btn.Font.Size := StrToInt(AStyle.GetValue('FontSize'));
  Btn.Font.Color := AStyle.Color('FontColor', Btn.Font.Color);
end;

function TPanelStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TPanel;
end;

procedure TPanelStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var P: TPanel;
begin
  P := TPanel(AControl);
  P.Color := AStyle.Color('BackgroundColor', P.Color);
  P.Font.Color := AStyle.Color('FontColor', P.Font.Color);
end;

function TIBGridStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl.ClassName = 'TIB_Grid';
end;

procedure TIBGridStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var G: TIB_Grid;
begin
  G := TIB_Grid(AControl);
  G.Color := AStyle.Color('BackgroundColor', G.Color);
  G.Font.Color := AStyle.Color('FontColor', G.Font.Color);
end;


{ TAdvGlowButtonStyler }

function TAdvGlowButtonStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TAdvGlowButton;
end;

procedure TAdvGlowButtonStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var
  Btn: TAdvGlowButton;
  bg: TColor;
begin
  Btn := TAdvGlowButton(AControl);

  // font
  if AStyle.GetValue('FontName') <> '' then
    Btn.Font.Name := AStyle.GetValue('FontName');
  if AStyle.GetValue('FontSize') <> '' then
    Btn.Font.Size := StrToIntDef(AStyle.GetValue('FontSize'), Btn.Font.Size);
  Btn.Font.Color := AStyle.Color('FontColor', Btn.Font.Color);
end;

{ TAdvGlassButtonStyler }

function TAdvGlassButtonStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TAdvGlassButton;
end;

procedure TAdvGlassButtonStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var
  Btn: TAdvGlassButton;
  bg: TColor;
begin
  Btn := TAdvGlassButton(AControl);

  // font
  if AStyle.GetValue('FontName') <> '' then
    Btn.Font.Name := AStyle.GetValue('FontName');
  if AStyle.GetValue('FontSize') <> '' then
    Btn.Font.Size := StrToIntDef(AStyle.GetValue('FontSize'), Btn.Font.Size);
  Btn.Font.Color := AStyle.Color('FontColor', Btn.Font.Color);

  bg := AStyle.Color('BackgroundColor', clNone);
  if bg <> clNone then
    Btn.BackColor := bg;

end;

{ TAdvPanelStyler }

function TAdvPanelStyler.Supports(AControl: TControl): Boolean;
begin
  Result := AControl is TAdvPanel;
end;

procedure TAdvPanelStyler.ApplyStyle(AControl: TControl; AStyle: TResolvedStyle);
var
  P: TAdvPanel;
begin
  P := TAdvPanel(AControl);
  TrySetColorProp(P, 'Color', AStyle.Color('BackgroundColor', P.Color));
  P.Font.Color := AStyle.Color('FontColor', P.Font.Color);
end;

{ TLabelStyler }


procedure TrySetColorProp(AControl: TObject; const PropName: string; AColor: TColor);
begin
  if IsPublishedProp(AControl, PropName) and (PropType(AControl, PropName) = tkInteger) then
    SetOrdProp(AControl, PropName, AColor);
end;

end.

==================================================

unit Theme.Types;

interface

uses
  System.Generics.Collections, Vcl.Graphics, System.SysUtils, Vcl.Controls;

type
  TResolvedStyle = class
  private
    FValues: TDictionary<string, string>;
  public
    property Values: TDictionary<string, string> read FValues;

    constructor Create;
    destructor Destroy; override;

    procedure SetValue(const Key, Value: string);
    function GetValue(const Key: string): string;

    function Color(const Key: string; Default: TColor = clNone): TColor;
    function Bool(const Key: string; Default: Boolean = False): Boolean;
  end;

implementation

constructor TResolvedStyle.Create;
begin
  FValues := TDictionary<string, string>.Create;
end;

destructor TResolvedStyle.Destroy;
begin
  FValues.Free;
  inherited;
end;

procedure TResolvedStyle.SetValue(const Key, Value: string);
begin
  FValues.AddOrSetValue(Key, Value);
end;

function TResolvedStyle.GetValue(const Key: string): string;
begin
  if not FValues.TryGetValue(Key, Result) then
    Result := '';
end;

function TResolvedStyle.Color(const Key: string; Default: TColor): TColor;
var
  S: string;
begin
  S := GetValue(Key);
  if S = '' then
    Exit(Default);

  try
    if S.StartsWith('#') then
      Result := StringToColor('$00' + Copy(S, 2, 6))
    else
      Result := StringToColor(S);
  except
    Result := Default;
  end;
end;

function TResolvedStyle.Bool(const Key: string; Default: Boolean): Boolean;
begin
  if GetValue(Key) = '' then
    Exit(Default);

  Result := SameText(GetValue(Key), 'true');
end;

end.
