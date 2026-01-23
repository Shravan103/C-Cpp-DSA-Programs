unit UTheme;

interface

uses
  System.Classes, System.Types, System.UITypes, System.SysUtils, System.UIConsts,
  System.Generics.Collections, System.Generics.Defaults, System.JSON, System.Skia,
  System.IOUtils, System.Math, System.StrUtils,
  Vcl.Forms, Vcl.Graphics, Vcl.Buttons, Vcl.Skia, Vcl.ExtCtrls, Vcl.Controls,
  REST.Types, REST.JSON.Types, REST.JsonCustom.Helper, REST.JsonCustom,
  Vcl.fcButton, Vcl.fcImager, Vcl.fcImgBtn, Vcl.BaseImageCollection, Vcl.ImageCollection,
  SVGIconImageListBase, SVGIconImageList,
  UData.Helper, UGlobalMenuObjects, Winapi.Windows, Vcl.Grids;

type
  TThemeGlyph = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    FsvgAsset: string;
    FSize: Integer;
    FWidth: Integer;
    FHeight: Integer;
    FSpacing: Integer;
    FOffsetX: Single;
    FOffSetY: Single;
    [JSONMarshalled(False)]
    FSVGIconImageList: TSVGIconImageList;

    function GetSVGIconImageList: TSVGIconImageList;
  public
    property svgAsset: string read FsvgAsset write FsvgAsset;
    property Size: Integer read FSize write FSize;
    property Width: Integer read FWidth write FWidth;
    property Height: Integer read FHeight write FHeight;
    property Spacing: Integer read FSpacing write FSpacing;
    property OffsetX: Single read FOffsetX write FOffsetX;
    property OffSetY: Single read FOffSetY write FOffSetY;
    property SVGIconImage: TSVGIconImageList read GetSVGIconImageList;
  end;

  TThemeFont = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    FText: string;
    FFontName: string;
    FSize: Single;
    FWeight: TSkFontComponent.TSkFontWeight;
    FWidth: TSkFontComponent.TSkFontStretch;
    FSlant: TSkFontComponent.TSkFontSlant;
    [JSONAlphaColor]
    FColor: TAlphaColor;
    FOffsetX: Single;
    FOffSetY: Single;
  public
    property Text: string read FText write FText;
    property FontName: string read FFontName write FFontName;
    property Size: Single read FSize write FSize;
    property Weight: TSkFontComponent.TSkFontWeight read FWeight write FWeight;
    property Width: TSkFontComponent.TSkFontStretch read FWidth write FWidth;
    property Slant: TSkFontComponent.TSkFontSlant read FSlant write FSlant;
    property AColor: TAlphaColor read FColor write FColor;
    property OffsetX: Single read FOffSetX write FOffSetX;
    property OffSetY: Single read FOffSetY write FOffSetY;
  end;

  TThemeGradient = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    FStartX: Single;
    FStartY: Single;
    FEndX: Single;
    FEndY: Single;
    [JSONAlphaColor]
    FStartColor: TAlphaColor;
    [JSONAlphaColor]
    FEndColor: TAlphaColor;

    function GetStart: TPointF;
    function GetEnd: TPointF;
    procedure SetStart(value: TPointF);
    procedure SetEnd(value: TPointF);
  public
    property AStart: TPointF read GetStart write SetStart;
    property AEnd: TPointF read GetEnd write SetEnd;
    property StartColor: TAlphaColor read FStartColor write FStartColor;
    property EndColor: TAlphaColor read FEndColor write FEndColor;
  end;

  TThemeDropShadow = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    FDeltaX: Single;
    FDeltaY: Single;
    FSigmaX: Single;
    FSigmaY: Single;
    [JSONAlphaColor]
    FColor: TAlphaColor;
    FOffsetX: Single;
    FOffsetY: Single;
  public
    property DeltaX: Single read FDeltaX write FDeltaX;
    property DeltaY: Single read FDeltaY write FDeltaY;
    property SigmaX: Single read FSigmaX write FSigmaX;
    property SigmaY: Single read FSigmaY write FSigmaY;
    property AColor: TAlphaColor read FColor write FColor;
    property OffsetX: Single read FOffsetX write FOffsetX;
    property OffsetY: Single read FOffsetY write FOffsetY;
  end;

  TThemeButton = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    FName: string;
    [JSONMarshalled(False)]
    FImageUp: TfcImager;
    [JSONMarshalled(False)]
    FImageDown: TfcImager;

    FGlyph: TThemeGlyph;
    FCaption: TThemeFont;
    FWidth: Integer;
    FHeight: Integer;
    FCorner: Integer;
    [JSONAlphaColor]
    FBackgroundColor: TAlphaColor;
    FGradient: TThemeGradient;
    FDropShadow: TThemeDropShadow;

    function GetImageUp: TfcImager;
    function GetImageDown: TfcImager;
    function GetInnerSize: TRectF;
    function GetOuterSize: TRectF;

    procedure CreateBackground(const ACanvas: ISKCanvas);
    procedure CreateButton(const ACanvas: ISKCanvas; UpImmage: Boolean);
//    procedure CreateGlyph(const ACanvas: ISKCanvas; UpImmage: Boolean);
//    procedure CreateCaption(const ACanvas: ISKCanvas; UpImmage: Boolean);
  public
    property Name: string read FName write FName;
    property ImageUp: TfcImager read GetImageUp write FImageUp;
    property ImageDown: TfcImager read GetImageDown write FImageDown;
    property Glyph: TThemeGlyph read FGlyph write FGlyph;
    property Caption: TThemeFont read FCaption write FCaption;
    property Width: Integer read FWidth write FWidth;
    property Height: Integer read FHeight write FHeight;
    property InnerSize: TRectF read GetInnerSize;
    property OuterSize: TRectF read GetOuterSize;
    property Corner: Integer read FCorner write FCorner;
    property BackgroundColor: TAlphaColor read FBackgroundColor write FBackgroundColor;
    property Gradient: TThemeGradient read FGradient write FGradient;
    property DropShadow: TThemeDropShadow read FDropShadow write FDropShadow;
  end;

  TThemeButtonList = class(TObjectList<TThemeButton>)
  public
    function NewThemeButton: TThemeButton;
    function GetThemeButton(Button: String): TThemeButton;
  end;

  TThemeButtonListInterceptor = TObjectListInterceptor<TThemeButton>;

type
  TThemeData = class(TObject)
  public
    constructor Create;
    destructor Destroy; override;
  private
    [JSONOwned(False), JsonObjectList(TThemeButtonListInterceptor)]
    FThemeButtonList: TThemeButtonList;

    [JSONAlphaColor]
    FBackgroundColour: TAlphaColor;
    [JSONAlphaColor]
    FFormToolbarColour: TAlphaColor;
    [JSONAlphaColor]
    FFormFunctionsColour: TAlphaColor;

    FFormWindowBorder: Integer;
    [JSONAlphaColor]
    FFormWindowBorderColour: TAlphaColor;
    FFormWindowCurve: Integer;
    FFormWindowMessageBorder: Integer;
    [JSONAlphaColor]
    FFormWindowMessageColour: TAlphaColor;
    FFormWindowMessageCurve: Integer;
    [JSONAlphaColor]
    FFormWindowMessageSelectColour: TAlphaColor;
    [JSONAlphaColor]
    FGridLineColour: TAlphaColor;

    [JSONMarshalled(False)]
    FThemePath: string;

    function GetAssetPath: string;
  public
    property ThemeButtonList: TThemeButtonList read FThemeButtonList write FThemeButtonList;
    property BackgroundColour: TAlphaColor read FBackgroundColour write FBackgroundColour;
    property FormToolbarColour: TAlphaColor read FFormToolbarColour write FFormToolbarColour;
    property FormFunctionsColour: TAlphaColor read FFormFunctionsColour write FFormFunctionsColour;
    property FormWindowBorder: Integer read FFormWindowBorder write FFormWindowBorder;
    property FormWindowBorderColour: TAlphaColor read FFormWindowBorderColour write FFormWindowBorderColour;
    property FormWindowCurve: Integer read FFormWindowCurve write FFormWindowCurve;
    property FormWindowMessageBorder: Integer read FFormWindowMessageBorder write FFormWindowMessageBorder;
    property FormWindowMessageColour: TAlphaColor read FFormWindowMessageColour write FFormWindowMessageColour;
    property FormWindowMessageCurve: Integer read FFormWindowMessageCurve write FFormWindowMessageCurve;
    property FormWindowMessageSelectColour: TAlphaColor read FFormWindowMessageSelectColour write FFormWindowMessageSelectColour;
    property GridLineColour: TAlphaColor read FGridLineColour write FGridLineColour;
    property ThemePath: string read FThemePath write FThemePath;
    property AssetPath: string read GetAssetPath;

    procedure Clear;
    function GetThemeButton(Name: string): TThemeButton;
  end;

  TTheme = Class(TComponent)
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  private

  public
    procedure LoadTheme(ThemePath: string);
    procedure NewButtonFromOldButton(var NewButton: TfcCustomImageBtn; var OldButton: TSpeedButton; ButtonName: string);
    procedure ThemeQSButton(var Button: TfcCustomImageBtn; QSButton: TQSButton);
    procedure ApplySkinOnStringGridDraw(AIBList: TStringGrid; ARow, ACol: Integer; Rect: TRect; State: TGridDrawState; FontColor: TColor = clBlack; Italic: Boolean = False; Bold: Boolean = False; aAlignment: TAlignment = taLeftJustify);
    procedure ApplySkinToGrid(AIBList: TStringGrid); overload;

    procedure ApplyMessageCurveToInnerAndOuter(aInnerPanel: TPanel);
    procedure ApplyMessageCurve(aWinControl: TWinControl; const aUpperLeftY: Integer = 0);
    procedure ApplyWindowCurveToInner(aWinControl: TWinControl);
    procedure ApplyWindowCurveToPanelAndForm(aWinControl: TWinControl);
    procedure ApplyWindowCurve(aForm: TForm);

    procedure SetFormSpacing(form: TForm);
  end;

var
  Theme: TTheme;
  ThemeData: TThemeData;

const
  SkFontSlant: array[TSkFontComponent.TSkFontSlant] of TSkFontSlant = (TSkFontSlant.Upright, TSkFontSlant.Italic, TSkFontSlant.Oblique);
  SkFontWeightValue: array[TSkFontComponent.TSkFontWeight] of Integer = (100, 200, 300, 350, 400, 500, 600, 700, 800, 900, 950);
  SkFontWidthValue: array[TSkFontComponent.TSkFontStretch] of Integer = (1, 2, 3, 4, 5, 6, 7, 8, 9);

implementation

uses
  LMain;

constructor TThemeGlyph.Create;
begin
  inherited Create;
  FSVGAsset := '';
  FSize := 0;
  FWidth := 0;
  FHeight := 0;
  FSpacing := 0;
  FOffsetX := 0;
  FOffSetY := 0;
  FSVGIconImageList := nil;
end;

destructor TThemeGlyph.Destroy;
begin
  if Assigned(FSVGIconImageList) then
    FSVGIconImageList.Free;
  inherited;
end;

function TThemeGlyph.GetSVGIconImageList: TSVGIconImageList;
var
  FileName: string;
begin
  if (FileExists(ThemeData.AssetPath + FSVGAsset)) then
  begin
    if not Assigned(FSVGIconImageList) then
    begin
      FSVGIconImageList := TSVGIconImageList.Create(nil);
      FSVGIconImageList.SetSize(FWidth, FHeight);
      FSVGIconImageList.SVGIconItems.LoadFromFile(ThemeData.AssetPath + FSVGAsset, FileName);
    end;
  end;
  Result := FSVGIconImageList;
end;


constructor TThemeFont.Create;
begin
  inherited Create;
  FText := '';
  FFontName := 'Roboto';
  FSize := 10;
  FWeight := TSkFontComponent.TSkFontWeight.Regular;
  FWidth := TSkFontComponent.TSkFontStretch.Regular;
  FSlant :=  TSkFontComponent.TSkFontSlant.Regular;
  FColor := TAlphaColors.Black;
  FOffsetX := 0;
  FOffSetY := 0;
end;

destructor TThemeFont.Destroy;
begin
  inherited;
end;

constructor TThemeGradient.Create;
begin
  inherited Create;
  FStartX := 0;
  FStartY := 0;
  FEndX := 0;
  FEndY := 0;
  FStartColor := TAlphaColors.Alpha;
  FEndColor := TAlphaColors.Alpha;
end;

destructor TThemeGradient.Destroy;
begin
  inherited;
end;

function TThemeGradient.GetStart: TPointF;
begin
  Result := PointF(FStartX, FStartY);
end;

function TThemeGradient.GetEnd: TPointF;
begin
  Result := PointF(FEndX, FEndY);
end;

procedure TThemeGradient.SetStart(value: TPointF);
begin
  FStartX := value.X;
  FStartY := value.Y;
end;

procedure TThemeGradient.SetEnd(value: TPointF);
begin
  FEndX := value.X;
  FEndY := value.Y;
end;

constructor TThemeDropShadow.Create;
begin
  inherited Create;
  FDeltaX := 0;
  FDeltaY := 0;
  FSigmaX := 0;
  FSigmaY := 0;
  FColor := TAlphaColors.Alpha;
end;

destructor TThemeDropShadow.Destroy;
begin
  inherited;
end;

constructor TThemeButton.Create;
begin
  inherited Create;
  FImageUp := nil;
  FGlyph := TThemeGlyph.Create;
  FCaption := TThemeFont.Create;
  FGradient := TThemeGradient.Create;
  FDropShadow := TThemeDropShadow.Create;
end;

destructor TThemeButton.Destroy;
begin
  if Assigned(FImageUp) then
    ImageUp.Free;
  if Assigned(FImageDown) then
    ImageDown.Free;
  FGlyph.Free;
  FCaption.Free;
  FGradient.Free;
  FDropShadow.Free;
  inherited;
end;

function TThemeButton.GetImageUp: TfcImager;
begin
  if not Assigned(FImageUp) then
  begin
    FImageUp := TfcImager.Create(nil);
    FImageUp.Picture.Bitmap.Create(FWidth, FHeight);
    FImageUp.Picture.Bitmap.SkiaDraw(
      procedure (const ACanvas: ISKCanvas)
      begin
        if (FBackgroundColor <> TAlphaColors.Alpha) then
          CreateBackground(ACanvas);
        CreateButton(ACanvas, True);
      end);
  end;
  Result := FImageUp;
end;

function TThemeButton.GetImageDown: TfcImager;
begin
  if not Assigned(FImageDown) then
  begin
    FImageDown := TfcImager.Create(nil);
    FImageDown.Picture.Bitmap.Create(FWidth, FHeight);
    FImageDown.Picture.Bitmap.SkiaDraw(
      procedure (const ACanvas: ISKCanvas)
      begin
        if (FBackgroundColor <> TAlphaColors.Alpha) then
          CreateBackground(ACanvas);
        CreateButton(ACanvas, False);
      end);
  end;
  Result := FImageDown;
end;

function TThemeButton.GetInnerSize: TRectF;
begin
  Result := TRectF.Create(PointF(0, 0), PointF(FWidth - Max(FDropshadow.DeltaX + 1, FDropshadow.OffsetX), FHeight- Max(FDropshadow.DeltaY + 1, FDropshadow.OffsetY)));
end;

function TThemeButton.GetOuterSize: TRectF;
begin
  Result := TRectF.Create(PointF(0, 0), PointF(FWidth, FHeight));
end;

procedure TThemeButton.CreateBackground(const ACanvas: ISKCanvas);
var
  LPaint: ISkPaint;
begin
  LPaint := TSkPaint.Create;
  LPaint.Color := BackgroundColor;
  LPaint.AntiAlias := True;
  ACanvas.DrawRect(OuterSize, LPaint);
end;

procedure TThemeButton.CreateButton(const ACanvas: ISKCanvas; UpImmage: Boolean);
var
  LPaint: ISkPaint;
  LRect: TRectF;
begin
  LPaint := TSkPaint.Create;
  LRect := InnerSize;
  if (FCorner > 0) then
    LPaint.PathEffect := TSkPathEffect.MakeCorner(FCorner);
  //Shader
  LPaint.Shader := TSkShader.MakeGradientLinear(FGradient.AStart, FGradient.AEnd, FGradient.StartColor, FGradient.EndColor, TSkTileMode.Clamp);
  if (UpImmage) then
  begin
    if (FDropshadow.DeltaX > 0) or (FDropshadow.DeltaY > 0) then
      LPaint.ImageFilter := TSkImageFilter.MakeDropShadow(FDropshadow.DeltaX, FDropshadow.DeltaY, FDropshadow.SigmaX, FDropshadow.SigmaY, FDropshadow.AColor);
  end
  else
    LRect.Offset(PointF(FDropshadow.OffsetX, FDropshadow.OffsetY));

  LPaint.AntiAlias := True;

  ACanvas.DrawRect(LRect, LPaint);
end;

{procedure TThemeButton.CreateGlyph(const ACanvas: ISKCanvas; UpImmage: Boolean);
var
  LSvgBrush: TSkSvgBrush;
  LRect: TRectF;
  FileName: string;
begin
  if (FileExists(ThemeData.AssetPath + FGlyph.FSVGAsset)) then
  begin
//   FImageCollection.Add(FGlyph.FSVGAsset, ThemeData.AssetPath + FGlyph.FSVGAsset);
   FSVGIconImageList.SetSize(FGlyph.Width, FGlyph.Height);
   FSVGIconImageList.SVGIconItems.LoadFromFile(ThemeData.AssetPath + FGlyph.FSVGAsset, FileName);

   LRect := TRectF.Create(PointF(((InnerSize.Width - FGlyph.Width) / 2), ((FGlyph.FSize - FGlyph.Height) / 2) + FGlyph.Spacing),
      PointF(((InnerSize.Width - FGlyph.Width) / 2) + FGlyph.Width, ((FGlyph.FSize - FGlyph.Height) / 2) + FGlyph.Spacing + FGlyph.Height));
    if (not UpImmage) then
      LRect.Offset(PointF(FDropshadow.OffsetX, FDropshadow.OffsetY));
    LRect.Offset(PointF(FGlyph.OffsetX, FGlyph.OffsetX));
    LSvgBrush := TSkSvgBrush.Create;
    try
      LSvgBrush.Source := TFile.ReadAllText(ThemeData.AssetPath + FGlyph.FSVGAsset);
      LSvgBrush.Render(ACanvas, LRect, 1);
    finally
      LSvgBrush.Free;
    end;
  end;
end; }

{procedure TThemeButton.CreateCaption(const ACanvas: ISKCanvas; UpImmage: Boolean);
var
  FontWeight: TSkFontWeight;
  FontWidth: TSkFontWidth;
  FontSlant: TSkFontSlant;
  LTypeface: ISkTypeface;
  LFont: ISkFont;
  LPaint: ISkPaint;
  TargetBoundsRect: TRectF;
  LRect: TRectF;
begin
  if (FCaption.FText <> '') then
  begin
    LRect := InnerSize;
    if (not UpImmage) then
      LRect.Offset(PointF(FCaption.OffsetX, FCaption.OffsetY));

    LPaint := TSkPaint.Create;
    LPaint.Color := FCaption.Color;

    FontWeight := TSkFontWeight(SkFontWeightValue[FCaption.FWeight]);
    FontWidth := TSkFontWidth(SkFontWidthValue[FCaption.FWidth]);
    FontSlant := TSkFontSlant(SkFontSlant[FCaption.FSlant]);

    LTypeface := TSkTypeface.MakeFromName(FCaption.FFontName, TSkFontStyle.Create(FontWeight, FontWidth, FontSlant));
//    LTypeface := TSkTypeface.MakeFromName(FCaption.FFontName, TSkFontStyle.Create(TSkFontWeight.Normal, TSkFontWidth.Normal, TSkFontSlant.Upright));
    LFont := TSkFont.Create(LTypeface, FCaption.FSize, 1);
    LFont.MeasureText(FCaption.FText, TargetBoundsRect, LPaint);
    ACanvas.DrawSimpleText(FCaption.FText, LRect.Left + LRect.CenterPoint.X - TargetBoundsRect.CenterPoint.X,
      LRect.Top + LRect.Height - (LRect.Height - FGlyph.FSize - (2 * FGlyph.Spacing) + TargetBoundsRect.Top), LFont, LPaint);
  end;
end;  }

function TThemeButtonList.NewThemeButton: TThemeButton;
begin
  Result := TThemeButton.Create;
  Add(Result);
end;

function TThemeButtonList.GetThemeButton(Button: String): TThemeButton;
var
  ThemeButton: TThemeButton;
begin
  Result := nil;
  for ThemeButton in self do
    if (ThemeButton.Name = Button) then
    begin
      Result := ThemeButton;
      exit;
    end;
end;

constructor TThemeData.Create;
begin
  inherited Create;

  FThemeButtonList := TThemeButtonList.Create;
  FBackgroundColour := $FF1E1E1E;
  FFormToolbarColour := $FF1E1E1E;
  FFormFunctionsColour := $FF1E1E1E;
  FFormWindowBorder := 2;
  FFormWindowBorderColour := $FF404040;
  FFormWindowCurve := 40;
  FFormWindowMessageBorder := 2;
  FFormWindowMessageColour := $FFE0E0E0;
  FFormWindowMessageCurve := 15;
  FFormWindowMessageSelectColour := $FF00B0B0;
  FGridLineColour := $FF333333;
  FThemePath := '';
end;

destructor TThemeData.Destroy;
begin
  FThemeButtonList.Free;

  inherited;
end;

function TThemeData.GetAssetPath: string;
begin
  Result := IncludeTrailingPathDelimiter(ThemeData.ThemePath) + IncludeTrailingPathDelimiter('asset');
end;

procedure TThemeData.Clear;
begin
  FThemeButtonList.Clear;
end;

function TThemeData.GetThemeButton(Name: string): TThemeButton;
begin
  Result := FThemeButtonList.GetThemeButton(Name);
end;

constructor TTheme.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  ThemeData := TThemeData.Create;
end;

destructor TTheme.Destroy;
begin
  ThemeData.Free;

  inherited;
end;

procedure TTheme.LoadTheme(ThemePath: string);
var
  JSONFile: TStringList;
  JSONValue: TJSONValue;
begin
  ThemeData.ThemePath := ThemePath;
  JSONFile := TStringList.Create;
  try
    if (FileExists(IncludeTrailingPathDelimiter(ThemeData.ThemePath) + 'theme.json')) then
    begin
      JSONFile.LoadFromFile(IncludeTrailingPathDelimiter(ThemeData.ThemePath) + 'theme.json');
      ThemeData.Clear;
      JSONValue := TJSONObject.ParseJSONValue(JSONFile.Text);
      if (JSONValue <> nil) then
        TJsonCustom.JsonToObject(ThemeData, JSONValue as TJSONObject);
    end;

    JSONFile.Text := TJsonCustom.ObjectToJsonString(ThemeData);
    JSONFile.SaveToFile(IncludeTrailingPathDelimiter(ThemeData.ThemePath) + 'theme.json');
  finally
    JSONFile.Free;
  end;
end;

procedure TTheme.NewButtonFromOldButton(var NewButton: TfcCustomImageBtn; var OldButton: TSpeedButton; ButtonName: string);
var
  Button: TThemeButton;
  WasDown: Boolean;
begin
  Button := ThemeData.GetThemeButton(ButtonName);

  if (Assigned(Button)) then
  begin
    NewButton := TfcImageBtn.Create(OldButton.Owner);
    NewButton.Parent := OldButton.Parent;
    NewButton.TabStop := False;
    NewButton.Width := Button.Width + Round(Button.FDropShadow.DeltaX + Button.FDropshadow.SigmaX);
    NewButton.Height := Button.Height + Round(Button.FDropShadow.DeltaY + Button.FDropshadow.SigmaY);
    if (Assigned(Button.ImageUp)) then
      NewButton.Image.LoadFromGraphic(Button.ImageUp.Picture.Graphic);
    if (Assigned(Button.ImageDown)) then
      NewButton.ImageDown.LoadFromGraphic(Button.ImageDown.Picture.Graphic);
    NewButton.Caption := Button.Caption.FText;
    TfcImageBtn(NewButton).ParentFont := False;
    NewButton.Font.Size := round(Button.Caption.Size);
    NewButton.Font.Name := Button.Caption.FontName;
    NewButton.Font.Style := [];
    NewButton.Font.Color := TColor(AlphaColorToColor(Button.Caption.AColor));
    NewButton.Offsets.GlyphY := Round(Button.Glyph.FOffSetY);
    NewButton.Offsets.TextX := Round(Button.Caption.FOffsetX);
    NewButton.Offsets.TextY := Round(Button.Caption.FOffsetY);
    NewButton.Images := Button.Glyph.SVGIconImage;
    NewButton.ImageIndex := 0;

    NewButton.Left := OldButton.Left;
    NewButton.Top := OldButton.Top;
  //  NewButton.Font := OldButton.Font;
    NewButton.Layout := OldButton.Layout;


    WasDown := OldButton.Down;
    NewButton.GroupIndex := OldButton.GroupIndex;
    NewButton.AllowAllUp := OldButton.AllowAllUp;
    NewButton.Enabled := OldButton.Enabled;
    NewButton.Margin := OldButton.Margin;
    NewButton.NumGlyphs := OldButton.NumGlyphs;
    NewButton.Tag := OldButton.Tag;
    NewButton.Visible := OldButton.Visible;
    NewButton.Hint := OldButton.Hint;
    NewButton.ShowHint := OldButton.ShowHint;

    NewButton.OnClick := OldButton.OnClick;
    OldButton.Free;
    OldButton := nil;
    NewButton.Down := WasDown;
  end;
end;

procedure TTheme.ThemeQSButton(var Button: TfcCustomImageBtn; QSButton: TQSButton);
var
  ThemeButton: TThemeButton;
begin
  ThemeButton := ThemeData.GetThemeButton('QSButton');

  if (Assigned(ThemeButton)) then
  begin
    ThemeButton.BackgroundColor := ThemeData.FBackgroundColour;
    if (QSButton.BGColour < 0) then
      QSButton.BGColour := 0;
    ThemeButton.Gradient.StartColor := MakeColor(GetRValue(QSButton.BGColour), GetGValue(QSButton.BGColour), GetBValue(QSButton.BGColour));
    ThemeButton.Gradient.EndColor := MakeColor(GetRValue(QSButton.BGColour), GetGValue(QSButton.BGColour), GetBValue(QSButton.BGColour));
    tfcImageBtn(Button).Color := clNone;
    ThemeButton.Width := QSButton.Width;
    ThemeButton.Height := QSButton.Height;
    ThemeButton.Gradient.FEndX := QSButton.Width;

    tfcImageBtn(Button).Width := ThemeButton.Width + Round(ThemeButton.FDropShadow.DeltaX + ThemeButton.FDropshadow.SigmaX);
    tfcImageBtn(Button).Height := ThemeButton.Height + Round(ThemeButton.FDropShadow.DeltaY + ThemeButton.FDropshadow.SigmaY);
    if (Assigned(ThemeButton.ImageUp)) then
      tfcImageBtn(Button).Image.LoadFromGraphic(ThemeButton.ImageUp.Picture.Graphic);
    if (Assigned(ThemeButton.ImageDown)) then
      tfcImageBtn(Button).ImageDown.LoadFromGraphic(ThemeButton.ImageDown.Picture.Graphic);
    FreeandNil(ThemeButton.FImageUp);
    FreeandNil(ThemeButton.FImageDown);
  end;
end;

procedure TTheme.ApplySkinOnStringGridDraw(AIBList: TStringGrid; ARow, ACol: Integer; Rect: TRect; State: TGridDrawState; FontColor: TColor = clBlack; Italic: Boolean = False; Bold: Boolean = False; aAlignment: TAlignment = taLeftJustify);
var
  SavedAlign: word;
  CellText: string;
  SecondLine: string;
  RectWidth: Integer;

begin
  with AIBList, Canvas do
  begin
    Brush.Style := bsSolid;          // 01/2023 : Setting solid backgroud as text was overwritten in headers
    if ((State = [gdSelected, gdFocused]) or (State = [gdSelected])) then
    begin
      if (ThemeData.FormWindowMessageSelectColour <> -1) then
      begin
        Brush.Color := TColor(AlphaColorToColor(ThemeData.FormWindowMessageSelectColour));
      end;
      Font.Color := FontColor;//0; //Black
      FillRect(Rect);
    end;
    Font.Color := FontColor;//4868682;  //Dark Grey
    FillRect(Rect);

    if (Italic) then
    begin
      Font.Style := [fsItalic];
    end;
    if (Bold) then
    begin
      Font.Style := [fsBold];
    end;
    if (aAlignment = taRightJustify) then
    begin
      SavedAlign := SetTextAlign(Canvas.Handle, TA_RIGHT);
      if (gdFixed in State) then
      begin
        TextRect(Rect, Rect.Right - 3, Rect.Top + 2, Cells[aCol, aRow]);
      end
      else
      begin
        TextRect(Rect, Rect.Right - 2, Rect.Top + ((Rect.Height - Canvas.TextHeight(Cells[aCol, aRow])) div 2), Cells[aCol, aRow]);
//        TextRect(Rect, Rect.Right - 2, Rect.Top + 2, Cells[aCol, aRow]);
      end;
      SetTextAlign(Canvas.Handle, SavedAlign);
    end
    else if (aAlignment = taCenter) then
    begin
      SavedAlign := SetTextAlign(Canvas.Handle, TA_CENTER);
      TextRect(Rect, Rect.Left + (Rect.Right - Rect.Left) div 2, Rect.Top + 2, Cells[aCol, aRow]);
      SetTextAlign(Canvas.Handle, SavedAlign);
    end
    else
    begin
      CellText := Cells[aCol, aRow];
      SecondLine := '';
      if (StrPos(PChar(CellText), #13) <> nil) then
      begin
        CellText := StrBeforeChar(Cells[aCol, aRow], #13);
        SecondLine := StrAfterChar(Cells[aCol, aRow], #13);
      end;

      if (SecondLine = '') then
        TextRect(Rect, Rect.Left + 2, Rect.Top + ((Rect.Height - Canvas.TextHeight(Cells[aCol, aRow])) div 2), CellText)
//        TextRect(Rect, Rect.Left + 2, Rect.Top + 2, CellText)
      else
      begin
        RectWidth := Rect.Right-Rect.Left-TextWidth('W');
        TextOut(Rect.Left + 2, Rect.Top + 2, CellText);
        Canvas.Font.Size := Canvas.Font.Size-3;
        while (TextWidth(SecondLine) > RectWidth ) do
          SecondLine := LeftStr(SecondLine, StrLen(PChar(SecondLine))-1);
        TextOut(Rect.Left + 2, (Rect.Top + TextHeight(SecondLine)+ 6) , SecondLine);
      end;
    end;

    Pen.Color := TColor(AlphaColorToColor(ThemeData.GridLineColour));
    Pen.Width := 1;

    MoveTo(Rect.Left, Rect.Bottom - 1);
    LineTo(Rect.Right, Rect.Bottom - 1);
  end;
end;

procedure TTheme.ApplySkinToGrid(AIBList: TStringGrid);
begin
  AIBList.DrawingStyle := gdsClassic;

//  if (FormWindowMessageBorderColour <> -1) then
//    AIBList.FixedColor := FormWindowMessageBorderColour;
  if (ThemeData.FBackgroundColour <> -1) then
    AIBList.Color := TColor(AlphaColorToColor(ThemeData.FBackgroundColour));

  AIBList.Ctl3D := False;
  AIBList.Options := [goRowSelect];
  AIBList.DefaultRowHeight := AppDetails.DefaultRowHeight;
end;

procedure  TTheme.ApplyMessageCurveToInnerAndOuter(aInnerPanel: TPanel);
var
  aOuter: TWinControl;
begin
  if (ThemeData.FormWindowMessageCurve >= 0) then
  begin
    aOuter := aInnerPanel.Parent;
    if (aOuter is TPanel) then
    begin
      if (ThemeData.FormWindowMessageBorder <> - 1) then begin
        TPanel(aOuter).BorderWidth := ThemeData.FormWindowMessageBorder;
        ApplyMessageCurve(aInnerPanel, 1);
      end;
      ApplyMessageCurve(TPanel(aOuter));
    end;
  end;
end;

procedure TTheme.ApplyMessageCurve(aWinControl: TWinControl; const aUpperLeftY: Integer = 0);
var
  regn: HRGN;

begin
  if (ThemeData.FormWindowMessageCurve >= 0) then
  begin
    regn := CreateRoundRectRgn(0, aUpperLeftY, aWinControl.Width, aWinControl.Height, ThemeData.FormWindowMessageCurve - aUpperLeftY, ThemeData.FormWindowMessageCurve - aUpperLeftY);
    SetWindowRgn(aWinControl.Handle, regn, True);
  end;
end;

procedure TTheme.ApplyWindowCurveToInner(aWinControl: TWinControl);
var
  aParent: TWinControl;
  regn: HRGN;

begin
  if (ThemeData.FormWindowMessageCurve >= 0) then
  begin
    aParent := aWinControl.Parent;
    if (aParent is TForm) then
    begin
      regn := CreateRoundRectRgn(ThemeData.FormWindowBorder, ThemeData.FormWindowBorder, TForm(aParent).ClientWidth - ThemeData.FormWindowBorder, TForm(aParent).ClientHeight - ThemeData.FormWindowBorder, (ThemeData.FormWindowCurve - ThemeData.FormWindowBorder), (ThemeData.FormWindowCurve - ThemeData.FormWindowBorder));
      SetWindowRgn(aWinControl.Handle, regn, True);
    end;
  end;
end;

procedure TTheme.ApplyWindowCurveToPanelAndForm(aWinControl: TWinControl);
var
  aParent: TWinControl;

begin
  if (ThemeData.FormWindowMessageCurve >= 0) then
  begin
    aParent := aWinControl.Parent;
    if (aParent is TForm) then
    begin
      if (ThemeData.FormWindowBorder > 0) and (ThemeData.FormWindowCurve <> -1) then
      begin
        ApplyWindowCurveToInner(aWinControl);
      end;
      if (ThemeData.FormWindowCurve <> -1) then
      begin
        ApplyWindowCurve(TForm(aParent));
      end;
    end;
  end;
end;

procedure TTheme.ApplyWindowCurve(aForm: TForm);
var
  regn: HRGN;

begin
  if (ThemeData.FormWindowMessageCurve >= 0) then
  begin
    regn := CreateRoundRectRgn(0, 0, aForm.ClientWidth, aForm.ClientHeight, ThemeData.FormWindowCurve, ThemeData.FormWindowCurve);
    SetWindowRgn(aForm.Handle, regn, True);
  end;
end;

procedure TTheme.SetFormSpacing(form: TForm);
begin
  form.Top := 0;
  form.Left := 0;
  form.Width := Screen.Width;
  form.Height := Screen.Height;
end;

end.


=======================================================================================================================================================

unit UQuickSaleUser;
{******************************************************************************}
interface
{******************************************************************************}
uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  UBaseDetailForm, ComCtrls, Grids, StdCtrls, Mask, IB_Controls, ExtCtrls,
  Buttons, IB_Grid, IB_Components, fcImgBtn, fcShapeBtn,
  JustifiedEdit, DateUtils, IB_EditButton, Contnrs{,IvDictio, IvMulti}, IB_Access,
  UTenderLine, UTendering, USelectLoyaltyReward, USkin, UTheme,
  UAppDetails, UGlobalMenuObjects, UGlobalMenuList, UOpenArch, UIntegerList, UCommonOrder,
  UPrintJob,  UGlobalVariables,
  UROSSCustomerTransaction, UROSSDataCustomer, USwipeCard,
  UDiscountCalc, UOrderLine, UCombos, UCombosInSale, UCDAServer, USubsidyCalc, UExternalAccount,
  AdvSmoothPanel, SVGIconImage, System.Types, System.UITypes, System.UIConsts, System.Skia, Vcl.Skia;
{******************************************************************************}
const
  UPD_POBCOLOR = WM_USER + 4242;

type
  TStaffLogin = class(TObject)
    constructor Create;
  public
    StaffID: Integer;
    LoginID: Integer;
    LastUsed: TDateTime;
    LoginButton: TfcCustomImageBtn;
    BeenChecked: Boolean;
  end;

  (**
    Open Architechture version of the Cash Sale Screen,
    originally based upon formQuickSale.
    It is a Key form used often.
    The Cash Sale screen is used commonly in a bar or cafe, where money is
    exchanged at the same time as drinks/food is ordered, as opposed to the
    Place Order screen where we pay for the goods later.

    We are able to do anything that we could do in the QuickSale Screen,
    but now we get to choose where the item buttons sit by using the
    QuickSalesDesigner. We are also able to refund Items, and Park Sales.
    Future improvments will often be only in this version of the
    Quick Sales Screen, to encourage people to move.
  *)

  TformQuickSaleUser = class(TformBaseDetailForm)
    cmdTableDes: TSpeedButton;
    cmdTablesDes: TSpeedButton;
    cmdSelectDes: TSpeedButton;
    cmdLastDes: TSpeedButton;
    StaffPromptTimeoutTimer: TTimer;
    cmdNoSaleDes: TSpeedButton;
    cmdHomeDes: TSpeedButton;
    txtStaff: TIB_Edit;
    cmdSelectNameDes: TSpeedButton;
    cmdBackDes: TSpeedButton;
    pnlButtonList: TPanel;
    cmdButtonsDes: TSpeedButton;
    cmdListDes: TSpeedButton;
    pnlToPay: TPanel;
    pnlButtonsOrList: TPanel;
    pnlList: TPanel;
    pnlContainer: TPanel;
    pnlSpacing: TPanel;
    grdItems: TIB_Grid;
    scrB: TScrollBox;
    pnlBev: TPanel;
    scrF: TScrollBox;
    pnlFood: TPanel;
    pnlNav: TPanel;
    cmdAllGroupsDes: TSpeedButton;
    cmdUpDes: TSpeedButton;
    cmdDownDes: TSpeedButton;
    cmdFoodDes: TSpeedButton;
    cmdBevDes: TSpeedButton;
    pnlButtons: TPanel;
    pnlFunctions: TPanel;
    pnlOrderLinesButtons: TPanel;
    cmdVoidDes: TSpeedButton;
    cmdMoreDes: TSpeedButton;
    cmdLessDes: TSpeedButton;
    pnlTenderBottom: TPanel;
    pnlTenderRight: TPanel;
    pnlSummaryBottom: TPanel;
    pnlTender: TPanel;
    pnlAccounts: TPanel;
    pnlTenderLines: TPanel;
    cmdReturnToSaleDes: TSpeedButton;
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
    cmdEditDes: TSpeedButton;
    txtTTender: TEdit;
    txtTTip: TEdit;
    txtTPayment: TEdit;
    txtTChange: TEdit;
    grdTLs: TStringGrid;
    cmdRefundDes: TSpeedButton;
    cmdParkedSaleDes: TSpeedButton;
    tmrBarCode: TTimer;
    cmdPMSDes: TSpeedButton;
    pnlOrderLineEditButtons: TPanel;
    pnlButtonsGap: TPanel;
    pnlTenderLineButtons: TPanel;
    cmdTenderAndDestnDes: TSpeedButton;
    cmdToPayEFTPOSDes: TSpeedButton;
    cmdCancelDes: TSpeedButton;
    cmdQuickCashDes: TSpeedButton;
    cmdToPayCashDes: TSpeedButton;
    cmdToPayPMSDes: TSpeedButton;
    cmdCash20Des: TSpeedButton;
    cmdCash10Des: TSpeedButton;
    cmdCash5Des: TSpeedButton;
    cmdCash50Des: TSpeedButton;
    pnlTenderTenders: TPanel;
    pnlOrderLinesButtonsRight: TPanel;
    pnlAvail: TPanel;
    lblAvailDollarSign: TLabel;
    lblAvail: TLabel;
    txtAvail: TEdit;
    cmdPhoneOrderDes: TSpeedButton;
    cmdWasteDes: TSpeedButton;
    tmrDeactivateButtons: TTimer;
    pnlAccountButtons: TPanel;
    scrAccounts: TScrollBox;
    pnlAccountControlsBorder: TPanel;
    pnlAccountControls: TPanel;
    lblCustomerAC: TLabel;
    lblDiscountPercent: TLabel;
    lblDiscountAmount: TLabel;
    cmdDiscountPercentDes: TSpeedButton;
    cmdDiscountAmountDes: TSpeedButton;
    cmdUpAccountsDes: TSpeedButton;
    cmdDownAccountsDes: TSpeedButton;
    cmdSelectAccountDes: TSpeedButton;
    cmdDeselectAccountDes: TSpeedButton;
    lblAllowCredit: TLabel;
    cmdAccountNoLookupDes: TSpeedButton;
    cmdOnAccountDes: TSpeedButton;
    txtAccountName: TEdit;
    txtDiscountPercent: TEdit;
    txtDiscountAmount: TEdit;
    pnlToPayRight: TPanel;
    pnlSecondControlButtons: TPanel;
    pnlSecondControlButtonsRight: TPanel;
    cmdSecondControlHideDes: TSpeedButton;
    cmdSecondControlParkedSaleDes: TSpeedButton;
    cmdSecondControlSelectNameDes: TSpeedButton;
    cmdSecondControlRefundDes: TSpeedButton;
    cmdQuickCashOKDes: TSpeedButton;
    pnlStaffLogins: TPanel;
    pnlStaffLoginsRight: TPanel;
    cmdTenderSelectAccountDes: TSpeedButton;
    cmdCPFDes: TSpeedButton;
    cmdCNPJDes: TSpeedButton;
    pnlAcct: TPanel;
    tblAcctName: TLabel;
    lblAcctName: TLabel;
    cmdBookingsDes: TSpeedButton;
    tmrQSInactivate: TTimer;
    lblComboDisc: TLabel;
    txtComboDisc: TEdit;
    chkDiscountCalcType: TCheckBox;
    pnlDiscounts: TPanel;
    cmdComboIndexDes: TSpeedButton;
    lblAllowItemLevelDiscount: TLabel;
    cmdExchangeDes: TSpeedButton;
    cmdEventDes: TSpeedButton;
    lblEventName: TLabel;
    cmdLoyaltyDes: TSpeedButton;
    pnlLoyalty: TPanel;
    pnlAcctSpace: TPanel;
    cmdSearchDes: TSpeedButton;
    cmdExternalAccLookupDes: TSpeedButton;
    cmdExternalAccRIPDes: TSpeedButton;
    cmdTenderExternalAccRIPDes: TSpeedButton;
    cmdTaxCoreBuyerDetailDes: TSpeedButton;
    cmdGoodyDes: TSpeedButton;
    AdvSmoothPanel1: TAdvSmoothPanel;
    SVGIconImage1: TSVGIconImage;
    Image1: TImage;
    cmdViewOrderLinesDes: TSpeedButton;
    cmdDestnDes: TSpeedButton;
    cmdModsDes: TSpeedButton;
    cmdQtyDes: TSpeedButton;
    lblBalDue: TLabel;
    lblCChange: TLabel;
    lblLoyaltyPoints1: TLabel;
    lblLoyaltyPoints2: TLabel;
    lblStillDue: TLabel;
    pnlSummaryRight: TSkPaintBox;
    txtBalDue: TLabel;
    txtStillDue: TLabel;
    txtCChange: TLabel;
    txtLoyaltyPoints1: TLabel;
    txtLoyaltyPoints2: TLabel;
    SVGIconImage2: TSVGIconImage;
    lblDateTime: TLabel;
    lblTerminal: TLabel;
    lblStaffName: TLabel;
    pnlOLs: TPanel;
    grdOLs: TStringGrid;
    pnlQty: TPanel;
    cmdQtyZeroDes: TSpeedButton;
    cmdQtyBackDes: TSpeedButton;
    cmdQtyOneDes: TSpeedButton;
    cmdQtyTwoDes: TSpeedButton;
    cmdQtyThreeDes: TSpeedButton;
    cmdQtySixDes: TSpeedButton;
    cmdQtyFiveDes: TSpeedButton;
    cmdQtyFourDes: TSpeedButton;
    cmdQtySevenDes: TSpeedButton;
    cmdQtyEightDes: TSpeedButton;
    cmdQtyNineDes: TSpeedButton;
    lblQty: TLabel;
    cmdQtyMoreDes: TSpeedButton;
    cmdQtyLessDes: TSpeedButton;
    txtNextQty: TEdit;
    pnlPartition: TPanel;

    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormDestroy(Sender: TObject);
    procedure cmdButtonsClick(Sender: TObject);
    procedure cmdSelectNameClick(Sender: TObject);
    procedure cmdFUpClick(Sender: TObject);
    procedure cmdFDownClick(Sender: TObject);
    procedure UPDPOBCOLOR(var Msg: TMessage); message UPD_POBCOLOR;

    procedure grdItemsCellClick(Sender: TObject; ACol, ARow: Integer; AButton: TMouseButton; AShift: TShiftState);
    procedure cmdVoidDesClick(Sender: TObject);
    procedure cmdUpClick(Sender: TObject);
    procedure cmdDownClick(Sender: TObject);
    procedure ItemGroupClick(Sender: TObject);
    procedure cmdTenderAndDestnDesClick(Sender: TObject);
    procedure cmdReturnToSaleClick(Sender: TObject);
    procedure cmdDestnClick(Sender: TObject);
    procedure cmdFoodClick(Sender: TObject);
    procedure cmdBevClick(Sender: TObject);
    procedure cmdListClick(Sender: TObject);
    procedure cmdHideClick(Sender: TObject);
    procedure cmdCancelDesClick(Sender: TObject);
    procedure cmdSelectClick(Sender: TObject);
    procedure cmdLastClick(Sender: TObject);
    procedure cmdTableClick(Sender: TObject);

    procedure cmdDeselectAccountClick(Sender: TObject);
    procedure cmdUpAccountsClick(Sender: TObject);
    procedure cmdDownAccountsClick(Sender: TObject);
    procedure cmdSelectAccountClick(Sender: TObject);
    procedure cmdDiscountPercentClick(Sender: TObject);
    procedure cmdDiscountAmountClick(Sender: TObject);
    procedure cmdAccountNoLookupClick(Sender: TObject);
    procedure cmdAllGroupsClick(Sender: TObject);

    procedure cmdModsDesClick(Sender: TObject);

    procedure cmdTLUpDesClick(Sender: TObject);
    procedure cmdTLDownDesClick(Sender: TObject);
    procedure cmdTLDeleteClick(Sender: TObject);
    procedure cmdTLClearClick(Sender: TObject);
    procedure cmdDetailsClick(Sender: TObject);
    procedure cmdCashClick(Sender: TObject);
    procedure cmdEFTPOSClick(Sender: TObject);
    procedure cmdVisaClick(Sender: TObject);
    procedure cmdDinersClick(Sender: TObject);
    procedure cmdAMEXClick(Sender: TObject);
    procedure cmdMasterCardClick(Sender: TObject);
    procedure cmdOtherDesClick(Sender: TObject);
    procedure cmdEditClick(Sender: TObject);
    procedure grdTLsDblClick(Sender: TObject);
    procedure cmdQuickCashClick(Sender: TObject);
    procedure cmdChequeClick(Sender: TObject);
    procedure cmdVoucherClick(Sender: TObject);
    procedure cmdQtyDesClick(Sender: TObject);
    procedure cmdTablesClick(Sender: TObject);
    procedure cmdViewOrderLinesDesClick(Sender: TObject);
    procedure StaffPromptTimeoutTimerTimer(Sender: TObject);
    procedure cmdNoSaleClick(Sender: TObject);
    procedure cmdHomeClick(Sender: TObject);
    procedure txtStaffChange(Sender: TObject);
    procedure cmdBackClick(Sender: TObject);
    procedure cmdMoreDesClick(Sender: TObject);
    procedure cmdLessDesClick(Sender: TObject);
    procedure cmdRefundClick(Sender: TObject);
    procedure txtNextQtyChange(Sender: TObject);
    procedure cmdQtyZeroDesClick(Sender: TObject);
    procedure cmdQtyBackDesClick(Sender: TObject);
    procedure cmdQtyMoreDesClick(Sender: TObject);
    procedure cmdQtyLessDesClick(Sender: TObject);
    procedure txtNextQtyClick(Sender: TObject);
    procedure cmdParkedSaleClick(Sender: TObject);
    procedure txtNextQtyEnter(Sender: TObject);
    procedure tmrBarCodeTimer(Sender: TObject);
    procedure cmdPMSClick(Sender: TObject);
    procedure cmdCash5Click(Sender: TObject);
    procedure cmdCash10Click(Sender: TObject);
    procedure cmdCash20Click(Sender: TObject);
    procedure cmdCash50Click(Sender: TObject);
    procedure cmdOnAccountClick(Sender: TObject);
    procedure cmdPhoneOrderClick(Sender: TObject);
    procedure tmrDeactivateButtonsTimer(Sender: TObject);
    procedure cmdLoyaltyClick(Sender: TObject);
    procedure grdOLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
    procedure cmdCPFDesClick(Sender: TObject);
    procedure cmdCNPJDesClick(Sender: TObject);
    procedure tmrQSInactivateTimer(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure FormMouseWheel(Sender: TObject; Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure FormMouseWheelDown(Sender: TObject; Shift: TShiftState; MousePos: TPoint; var Handled: Boolean);
    procedure FormMouseUp(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure scrAccountsClick(Sender: TObject);
    procedure grdTLsClick(Sender: TObject);
    procedure scrBClick(Sender: TObject);
    procedure scrFClick(Sender: TObject);
    procedure grdOLsDblClick(Sender: TObject);
    procedure cmdComboIndexDesClick(Sender: TObject);
    procedure cmdExchangeDesClick(Sender: TObject);
    procedure cmdEventDesClick(Sender: TObject);
    procedure grdTLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
    procedure cmdSearchDesClick(Sender: TObject);
    procedure cmdExternalAccLookupDesClick(Sender: TObject);
    procedure cmdExternalAccRIPDesClick(Sender: TObject);
    procedure DoExternalAccountButtonClick(RIPButtonClicked: Boolean = False; ExternalAccType: Integer = 0);
    procedure cmdTaxCoreBuyerDetailDesClick(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure cmdGoodyDesClick(Sender: TObject);
    procedure pnlSummaryRightDraw(ASender: TObject; const ACanvas: ISkCanvas; const ADest: TRectF; const AOpacity: Single);
    procedure pnlTitleClick(Sender: TObject);
  private
    NewGuestCount: Integer;
    FAllCombos: TCombos;
    FDelivery: string;
    FDeliverySetByUser: Boolean;
    FToPay, FFoodTotal, FBevTotal, FPayment, FTip,
    FChange, FTender, FRounding, FStillDue,
    FAvail, FRequired, FGuestSubsidy, FHostSubsidy: Currency;
    FSalesTax: Currency;     //Jon 29-5-2001
    FSalesTaxCalculated: Boolean;  //Jon 30-5-2001
    FSurcharge: Currency;          //Jon 26-05-2003
    FDriveThroughGroupIDs: TIntegerList;

    FormActivated: Boolean;
    FQSInactiveTime: TDateTime;

    FDiscountPercent: Currency;
    FDiscountAmount, FOrigDiscountAmount: Currency;
    FComboDiscount: Currency;
    FLoyaltyFreeItemDiscount: Currency;
    FLoyaltyPoints1: Currency;          //Jon 19-10-2005
    FLoyaltyPoints2: Currency;          //Jon 19-10-2005
    FUseDiscountAmount: Boolean;
    FUseItemDiscounts: Boolean; //Jon 14-07-2004
    FUseDiscountPeriods: Boolean;    //RPC 21-10-2010
    FLimitMaximum: Boolean;          //Jon 13-12-2002
    FMaxDiscount: Currency;          //Jon 13-12-2002
    FOrigDiscountPercent: Currency;      //Jon 17-12-2002
    FCurrentBalance: Currency;        //Jon 17-10-2003
    FHeldGroupID: Integer;            //Jon 11-10-2004
    FTabNotes: string;
    FModifierFunction: Integer;             //Jon 9-8-2001

    RevertedSaleCategory: Boolean; //AKM 4.3.1

    OLList: TList;
    VoidOLList: TList;
    TLList: TTLList;
    ComboFreeItems: TObjectList;
    StaffLoginsList: TList;
    BackPagesList: TIntegerList;
    CurrentHour, CurrentMin, CurrentSec, CurrentMSec: Word;   //Jon 25-06-2002
    CurrentTime, PrevTime: Integer;
    CurrentDay: Integer;

    StillAddingAnItem: Boolean;
    ConfirmBeforeCancel: Boolean;
    DiscCalculator: TDiscountCalc;
    SubsidyCalc: TSubsidyCalc;
    ConfirmedAccountDiscountSelection, ConfirmComboDiscountSelection: Boolean;
    RemoveOrderLine: Boolean;
    IsShowing: Boolean;

    CloseFromCashSales: Boolean;   //Jon 11-7-2001
    StaffPromptTimeoutTimerActive: Boolean;    //Jon 12-7-2001

    EatInTakeawayPromptValue: Integer;      //Jon 8-8-2001
    PrintPhoneOrder: Boolean;               //Jon 28-11-2003
    SurplusOnAccount: Boolean;              //Jon 04-11-2003

    PriceLevel: Integer;
    PriceLevelType: Integer;
    SalePriceLevel: Integer;
    PersistentPriceLevel: Integer;
    CurrentCourseID: Integer;     //Jon 28-02-2002
    CurrentCourseName: string;
    CurrentCourseType: Integer;   //Jon 15-11-2006
    CurrentCourseIsMain: Boolean; //Jon 07-05-2003
    NextItemButtonState: Integer; //Jon 22-04-2002
    FDoingRefund: Boolean;
    DoingExchange: Boolean;
    DoingWaste: Boolean;
    OnAccount: Boolean;           //Jon 16-10-2003
    OnTable: Boolean;             //Jon 17-12-2003

    DoingTransaction: Boolean;    //Jon 25-02-2002 to stop other buttons clicking when doing transaction

    ShowButtons: Boolean;         //Jon 27-03-2002 moved here instead of global

    OrderingQty: Currency;         //Jon 29-02-2008
    ShowingQty: Boolean;          //Jon 10-04-2002
    NextQty: Integer;            //Jon 10-04-2002
    BarCodePriceCode: string;

    CurrentSelectedTEdit: TEdit;        //Jon 15-08-2002
    CurrentSelectedTEditText: string;   //Jon 15-08-2002

    LibAccount, LibRoom, LibName, LibStatus: string;
    NZAAccount: Integer;    //Jon 01-02-2003
    NZAAkey: string;        //Jon 01-02-2003
    NZAName: string;        //Jon 01-02-2003

    EvolutionAccountNo: Integer;
    EvolutionAccount: string;
    EvolutionName: string;
    EvolutionAccExtra: string;

    AvailQtyTested: Boolean;    //Jon 14-05-2003  used to test if we are using the countdown feature, have passed the zero mark but want to continue anyway.

    SurchargeList: TList;                         //Jon 26-05-2003
    SaleCategorySurchargeItem: TSurchargeItem;   //Jon 26-05-2003

    AllowActivateTesting: Boolean;      //Jon 24-07-2003

    LoyaltyReward: TLoyaltyReward;      //Jon 21-12-2004

    DoingPhoneOrder: Boolean;           //Jon 20-01-2005    Set this to declare that this is a phone order, so should be saved to a tab instead.
    IntendedTenderTypeID: Integer;      //Jon 24-01-2005    This is the tendertype that the customer will most likely pay with.
    IntendedTenderTypeProviderID: Integer;
    PhoneOrderPersistant: Boolean;      //Jon 25-01-2005
    PromptedWhere: Boolean;             //Jon 12-01-2006

    ButtonsDeactivated: Boolean;       //Jon 15-03-2006

    ItemAutoTenderActive: Boolean;     //RPC 05-03-2010
    TheVoidOrderID: Integer;           //RPC 22-03-2010    Here we try and use 1 order id for voiding cleared items
    FVoidID: Integer;

    InactivePeriodIndex: Integer;      //This is current place in index + 1 (we using 0)
    DoSpecialNYHappyHour: Boolean;     //when true we doing SpecialVersion of NY Happy Hour

    FExchangeOLs: TList;
    FExchangeInvoiceID: Integer;
    FExchangeAmount: Currency;
    FExchangeDiscountAmount: Currency;
    FExchangeDiscountSchemeID: Integer;
    FExchangeSalesTax: Currency;
    FExchangeDiscountPercent: Currency;
    FExchangeAccountID: Integer;
    FExchangeNotes: string;

    cmdHideSkin: TfcCustomImageBtn;
    cmdTables: TfcCustomImageBtn;
    cmdBookings: TfcCustomImageBtn;
    cmdParkedSale: TfcCustomImageBtn;
    cmdNoSale: TfcCustomImageBtn;
    cmdSelectName: TfcCustomImageBtn;
    cmdLast: TfcCustomImageBtn;
    cmdSelect: TfcCustomImageBtn;
    cmdTable: TfcCustomImageBtn;
    cmdBack: TfcCustomImageBtn;
    cmdHome: TfcCustomImageBtn;
    cmdButtons: TfcCustomImageBtn;
    cmdList: TfcCustomImageBtn;
    cmdRefund: TfcCustomImageBtn;
    cmdTaxCoreBuyerDetail: TfcCustomImageBtn;
    cmdWaste: TfcCustomImageBtn;
    cmdLoyalty: TfcCustomImageBtn;
    cmdPhoneOrder: TfcCustomImageBtn;
    cmdSecondControlRefund: TfcCustomImageBtn;
    cmdSecondControlSelectName: TfcCustomImageBtn;
    cmdSecondControlParkedSale: TfcCustomImageBtn;
    cmdSecondControlHide: TfcCustomImageBtn;

    cmdCPF: TfcCustomImageBtn;
    cmdCNPJ: TfcCustomImageBtn;
    cmdMore: TfcCustomImageBtn;
    cmdLess: TfcCustomImageBtn;
    cmdViewOrderLines: TfcCustomImageBtn;
    cmdVoid: TfcCustomImageBtn;

    cmdMods: TfcCustomImageBtn;
    cmdDestn: TfcCustomImageBtn;
    cmdQty: TfcCustomImageBtn;
    cmdTenderAndDestn: TfcCustomImageBtn;
    cmdToPayEFTPOS: TfcCustomImageBtn;
    cmdTenderSelectAccount: TfcCustomImageBtn;
    cmdTenderExternalAccRIP: TfcCustomImageBtn;
    cmdCancel: TfcCustomImageBtn;
    cmdToPayCash: TfcCustomImageBtn;
    cmdToPayPMS: TfcCustomImageBtn;
    cmdCash50: TfcCustomImageBtn;
    cmdCash20: TfcCustomImageBtn;
    cmdCash10: TfcCustomImageBtn;
    cmdCash5: TfcCustomImageBtn;
    cmdQuickCash: TfcCustomImageBtn;

    cmdQtyZero: TfcCustomImageBtn;
    cmdQtyOne: TfcCustomImageBtn;
    cmdQtyTwo: TfcCustomImageBtn;
    cmdQtyThree: TfcCustomImageBtn;
    cmdQtyFour: TfcCustomImageBtn;
    cmdQtyFive: TfcCustomImageBtn;
    cmdQtySix: TfcCustomImageBtn;
    cmdQtySeven: TfcCustomImageBtn;
    cmdQtyEight: TfcCustomImageBtn;
    cmdQtyNine: TfcCustomImageBtn;
    cmdQtyBack: TfcCustomImageBtn;
    cmdQtyMore: TfcCustomImageBtn;
    cmdQtyLess: TfcCustomImageBtn;

    cmdUp: TfcCustomImageBtn;
    cmdDown: TfcCustomImageBtn;
    cmdFood: TfcCustomImageBtn;
    cmdBev: TfcCustomImageBtn;
    cmdAllGroups: TfcCustomImageBtn;
    cmdSearch: TfcCustomImageBtn;

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
    cmdReturnToSale: TfcCustomImageBtn;
    cmdMasterCard: TfcCustomImageBtn;
    cmdAMEX: TfcCustomImageBtn;
    cmdCash: TfcCustomImageBtn;
    cmdQuickCashOK: TfcCustomImageBtn;

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
    cmdFiscalMenuSkin: TfcCustomImageBtn;
    cmdComboIndex: TfcCustomImageBtn;
    cmdExchange: TfcCustomImageBtn;
    cmdEvent: TfcCustomImageBtn;
    cmdGoody: TfcCustomImageBtn;

    OpenArch: TOpenArch;

    FCPFNumber: string; // for fiscal printing
    FPrevAccountID: Integer;
    FAccountCPFNumberChanged, ForceCPFNumber: Boolean; //for fiscal printing
    ShowCPFNUmberInActivate: Boolean;
    FAddingFiscalItem: Boolean;
    FAddedFiscalRow: Integer;
    ForcedNextQty, AddingComboItems, FAddingDuplicateItem: Boolean;

    FManualSeries, FManualSubSeries, FManualInvNo: string;
    NFManual: Boolean;
    ShowManualSelection, NFManualSecond: Boolean;
    NFEMode: Boolean;
    FPerorgID: Integer;
    PrevOLCount: Integer;
    RSC: TReplacementSwipeCard;
    DeletingComboItems: Boolean;

    ComboIDToDelete: Integer;
    FComboIndexToDelete: Integer;
    LoadingAccount: Boolean;
    FCreditCardDiscount: Double;
    FItemNote: string;
    FInvCOO, FInvCCF: string;
    FTotalNCMTax: Currency;

    FToWaste: Boolean;
    FVoidReasonID: Integer;
    FExp1: string;
    FExp2: string;

    function ValidateGoodySale: Boolean;
    function ValidateCashSale(ShowTheError: Boolean): Boolean;
    function DoCashSale(ShowTheError: Boolean; CalledFrom: Integer): Boolean;
    function DoCashSaleRefund(ShowTheError: Boolean): Boolean;
    function TryCashSale(Print: Boolean): Boolean;
    function TryCashSaleRefund(Print: Boolean): Boolean;
    function TryRemotePhoneOrder(Print: Boolean): Boolean;
    function ResetCashSale(HoldChangeWindow: Boolean; ResetPhoneOrder: Boolean): Boolean;

    function SendLibicaTransactions(SPNumber: Integer): Boolean;
    function SendFidelioTransactions(SPNumber: Integer): Boolean;
    function SendPhoenixTransactions(SPNumber: Integer): Boolean;
    function SendMicros4700Transactions(SPNumber: Integer): Boolean;
    function SendNZATransactions(SPNumber: Integer): Boolean;
    function SendEvolutionTransactions(AGuestCount: Integer; SPNumber: Integer): Boolean;
    function SendICRTouchTransactions(SPNumber: Integer): Boolean;
    function SendMewsTransactions(SPNumber, TheTenderID: Integer): Boolean;
    function SendAgoraTransactions(SPNumber, TheTenderID: Integer): Boolean;
    function SendRMSTransactions(SPNumber, TheTenderID: Integer): Boolean;

    function SendTaxCoreTransactions: Boolean;

    procedure AddPrinterToPrinterList(List: TIntegerList; PrinterID: Integer);
    procedure AddCCPrintersToPrinterList(List: TIntegerList);

    procedure OpenArchButtonClick(Sender: TObject);
    procedure OpenArchButtonHeld(Sender: TObject);

    function PromptAndGetDifferentSizeItem(aItemID: Integer; aItemAbbrev: string; aCurrentTime: Integer; aCurrentDay: Integer; var aSelectedItemID: Integer): Boolean;

    function OrderItem(ItemID: Integer; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
    function OrderItemSP(ItemID: Integer; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
    function OrderItemST(Statement: TIB_Statement; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
    function FindOLJustAddedIndex: Integer;

    procedure SendOrderLineToFiscalPrinter(aOL: TOrderLine; GridIndex: Integer);

    procedure OrderModifier(ModifierID: Integer; ModGroupID: Integer);
    procedure OrderCourse(CourseID: Integer; CourseType: Integer);
    procedure OrderCombo(aComboID: Integer);

    procedure BroadcastCDAMessage;
    procedure ClearCDAMessage;

    procedure CancelCashSale(DoCancelFiscalPrinterInvoice: Boolean = False);
    procedure ClearAllOrderLines;
    procedure ClearAllSurcharges;
    procedure ClearItemLevelSubsidy;
    procedure VoidClearedOL(SPNumber: Integer; DoCancel: Boolean);

    procedure SetOLCols;

    procedure CalculateToPay;
    procedure CalculateDiscountSchemeItemDiscount(RecalculateAll: Boolean);
    procedure CalculateDiscount;
    procedure CalculateComboAndAccountDiscounts;
    procedure CalculateSubsidy(RecalculateAll:Boolean);

    procedure CheckForFiscalDiscountChanges;
    procedure AddNewFiscalItem(AQty: Integer = 1);

    procedure BuildAccountButtons;
    function CheckLoyaltyReward(Prompt: Boolean): Boolean;
    procedure ClearItemLevelLoyalty;
    procedure AccountClick(Sender: TObject);
    procedure InitialiseAccountInfo;
    function CanSelectAccount: Boolean;

    procedure AddFreeComboItemsToOrderList;

    procedure InsertLine;
    procedure AddLine;
    procedure VoidLine;
    procedure DeleteCombos(AIndex: Integer; ADeleteAllItems: Boolean=False);
    procedure BuildItemGroups;
    procedure BuildItems;
    function SetSaleCategory(SCID: Integer): Boolean;
    function SelectSaleCategory(SaleCategoryName: string; const SaleCategoryID: Integer = 0): Boolean;

    procedure SetButtons;
    procedure SetView;
    procedure AddMods;
    procedure AddFiscalItemNote;
    procedure ClearMods;
    procedure LoadMods;

    function LoadAccount(AccountID: Integer; ForGoodyDiscount: Boolean = False): Boolean;
    function QSCreateAccount(AccountTypeID: Integer): Boolean;
    procedure DisplayAccount;

    function GetDue(AllowNegative: Boolean; IncludeExternalTenders: Boolean = True): Currency;
    procedure LoadTLs(LAction: TLA);

    procedure AddTenderLine(TLType: TTLT; pTL: TTL = nil);
    function GetCPFNumber: string;
    function CheckFiscalPrinterFunctions: Boolean;
    function GetNFManualExtraDetails: Boolean;
    function GetNFEDetails:Boolean;
    procedure RetrieveCPFAccount;
    procedure GetFiscalAutoCustomer(APerOrgID: Integer);
    procedure SelectCustomerRecord(APerorgID: Integer);
    procedure SetAccount;
    procedure EnableAvailBox(AEnable: Boolean);
    procedure EditTenderLine(UseDueAmount: Boolean);
    procedure DeleteTenderLine;
    procedure ClearAllTenderLines;

    procedure QuickSaleCashCam;
    procedure NoSaleCashCam;
    function EFTPOSTransExists: Boolean;

    procedure TableSelected(Selected: Boolean); //AMANDA 12/10/00
    function SaleCategorySelected: Boolean; //AKM 3.3.1
    procedure PauseStaffPromptTimeoutTimer;
    procedure ResetStaffPromptTimeoutTimer;

    procedure StartInactiveTimer;
    procedure StopInactiveTimer;
    procedure SetTLCols;
    function ForBToString(aForB: string): string;
    procedure ModifierFunctionChange(ModFunction: Integer);

    procedure AttemptDefaultNoSale;   //Jon 17-06-2003
    procedure DeleteModifier;
    procedure ShowPnlToPay(B: Boolean);

    procedure PriceLevelChange(NewPriceLevel: Integer; NewPriceLevelType: Integer);
    procedure SetDoingRefund(State: Boolean; Force: Boolean);
    procedure SetDoingExchange(State: Boolean; Force: Boolean);
    procedure SetDoingWaste(State: Boolean; Force: Boolean);
    procedure SetOnAccount(State: Boolean);
    procedure ChangeQuickCashButtonCaption;
    procedure CourseChange(NewCourseID: Integer; NewCourseName: string; NewCourseType: Integer; NewCourseIsMain: Boolean);
    procedure NextItemButtonStateChange(NewNextItemButtonState: Integer);

    procedure SetNextQty(I: Integer);
    procedure ShowQty(B: Boolean);
    function TryParkSale(var ParkSale: Integer; LoginID: Integer; Prompt: Boolean = False): Boolean;
    function TryLoadParkedSale: Boolean;

    procedure SetCurrentTime;
    procedure BuildTenderSurchargeList;
    procedure AddCashTender(TLTender: Currency; UsingRequired: Boolean = False);
    procedure ChangeCurrentOrdersPriceLevel(NewPriceLevel: Integer);    //Jon 08-10-2003
    procedure ChangeAllowCreditLabel(AllowCredit: Boolean);
    procedure ChangeHeldGroup(HeldGroupID: Integer; HeldGroupName: string);
    procedure TestAllOLsLoyaltyItem(FindNewOnly: Boolean);
    function TestLoyaltyItem(LoyaltyRewardID: Integer; ItemID: Integer): Integer;
    function AllocateItemLoyalty(var OL: TOrderLine): Boolean;
    procedure ReDisplayOLGrid;
    procedure LoadOLGrid;
    procedure UpdateOLGridRow(aRow: Integer; aOL: TOrderLine);
    procedure TemporarilyDeactiveButtons;
    procedure CombineOrderLines;
    procedure ManualBarCodeEntry;
    procedure AddStaffLoginButton;
    procedure StaffLoginButtonClick(Sender: TObject);
    procedure CheckStaffLoginButtons;

    procedure PrintTicket;
    procedure PrintOrder(OrderPrinterList: TIntegerList);
    procedure PrintInvoice;
    procedure DoCashup(DoCashup: Boolean);
    procedure DoStats;
    procedure setinactiveitems;
    procedure AddItemQtyToSpecialNYHappyHour(ItemID: Integer; Qty: Currency);
    procedure LogSepicalNYHappyHour(ItemID: Integer; Qty: Currency; Price: Currency; IsFood: Boolean);

    function OpenDefaultTabForWastage:Boolean;

    procedure ReplaceSwipeCardQS;
    function UpdateSwipeCardAfterReplacement: Boolean;
    procedure RemoveSwipeCardReplacement;

    procedure AddSameCombo(aComboID: Integer; aComboIndex: Integer);
    procedure ResetGlobalVariables;
    procedure ResetTabDiscounts;
    function GetItemCount:Integer;
    procedure ClearExchageTender;
    procedure AddAutoExchangeTender(AmountToAdd: Currency);
    function LoadEventAccount: Boolean;
    function TryExchangeRefund(TheSP: TIB_StoredProc; SPNumber: Integer): Boolean;
    procedure DeselectEvent;

    function POEatInTakeaway(ReturnedFromCancel: Boolean): Boolean;
    function POSelectCustomer(PerorgID: Integer): Boolean;
    function POCheckDeliveryDetails: Boolean;
    function POSelectRemoteLocation: Boolean;
    function POSetUp: Boolean;

    procedure LoadDriveThroughOLs;
    procedure ClearDriveThroughOLs;
    function DoPhoneOrder(Enable: Boolean; PhoneOrderMode: Boolean): Boolean;
    procedure ClearCustomerDeliveryDetails;
    procedure ClearSelectedCustomerDetails;
    procedure CancelPOSInvoiceIfFiscalInvCancelled;

    procedure GetSingleItemNote;
    function LoadQuickServiceComboPayToTabPay:Boolean;
    procedure ClearComboDiscounts;
    procedure UpdateTabDiscount;
    procedure UpdateOrderLines(TheOrderID, SPNumber: Integer; TheSP: TIB_StoredProc);
    procedure SetGrdItemsToItemGroup(ItemGroupID: Integer);
    procedure ShowLastReceipt;
    procedure SelectModifiersItemInGrid;
    function AllowTendering: Boolean;
    procedure AddExternalTenders;
    procedure DeleteCurrentExternalTenders;
  public
    property DoingRefund: Boolean read FDoingRefund;
    property IsOnTable: Boolean read OnTable;
    property IsDoingTransaction: Boolean read DoingTransaction;
    procedure StopStaffPromptTimeoutTimer;
    function OkToClose: Boolean;
    procedure DoSelectName; //akm 01.10.01
    procedure DoSelectNameProximityCode(ProximityCard: string); //Jon 16-06-2003
    procedure DoSelectNameStaffID(StaffID: Integer; ProveStaff: Boolean = False);
    procedure DoSelectTable;  //Jon 27-06-2002
    procedure TryLookupCashSaleAccount(AccountID: Integer; const SwipingCard: Boolean = True);
    function TryLookupCashSaleItem(BarCode: string; PriceCode: string): Boolean;
    procedure SetGoodyAccountDiscountAmount;
    function TryAddGoodyDiscountAmount(ScannedAmount: string): Boolean;
    procedure ChangePage(QSPageID: Integer);
    procedure ShowExternalAccounts(TL: TTL = nil);
    procedure CloseExternalAccounts;
    procedure UpdateTLListExternalAccounts;
    procedure DeselectAccount;
    procedure DeselectGoodyAccount;
  end;
{******************************************************************************}
var
  formQuickSaleUser: TformQuickSaleUser;
{******************************************************************************}
implementation
{******************************************************************************}
{$R *.DFM}
{******************************************************************************}
uses
  LItemQueries, LServiceQueries, USelectTable, LWBQuery, UMessage, LMain,
  DM_WBWaiter, USelectName, UPrintSummary, UTables, USelectCourse,
  USingleNote, UConfirmation, USelectCashSale,
  UDiscountPercent, UDiscountAmount, USelectAccount, LErrors, UQuickMessage,
  UGetAccountNo, UItemModifiers, LEvent, UQuantity, UEFTPOSIntegration,
  UDMCashCam, UMain, UViewOrderLines, UOpenPrice, UEatInTakeawayPrompt,
  UShowRecipe, LStockSystemQueries {<-- Willie 12/09/20001}, UVoid,
  UDMComponents, UParkSale, USelectParkedSale,
  ULibicaTransaction, UFidelioTransaction, UPhoenixTransaction, UMicros4700Transaction,
  UNZATransaction, UEvolutionTransaction, UICRTouchPMSInterface, USmartCardTransaction,
  UGetNewGuests, UNoSale, UWastage, USelectHeldGroup, USelectCustomer,
  USelectTenderType, USelectRemoteLocation, UDialogPhoneOrder, UDeliveryDetails,
  UROSSDataRemoteOrder, UROSSOrderTransaction, UROSSTransMasIDs, UNumPadText,
  UPocketVoucherTransaction, UAccountScanned, UCashUps, USelectTill,
  USelectStartStopTimes, UPaymentTable, UAlphaBlend, UResources, IniFiles,
  UGetCPFNumber, UNFEDetail, UFiscalPrinting, Math, LDetailForm, UEncryption, ULogFile, ULogTypes,//for fiscal printing
  UROSSAccountData, UCallerIdList, UShowDiffSizeItems, USelectCombo, UIDList,
  UTEFDaruma, USelectExchangeItems, USelectInvoiceNo, USelectEvent, UFiscalDeletedData, USelectItem,
  UCashupCountDetail, UExternalAccountIntegration, USelectExternalAccount, USearchExternalAccount,
  wiGroupPOS, UExternalAccountRip, UAPICollection,
  UTaxCoreTransaction, UTaxCoreBuyerDetail, UTaxCoreHTTPSocket, LGoody, UMewsHTTP, UMewsTransaction,
  UTableManagementIntegration, UAgoraHTTP, UAgoraTransaction, URMSHTTP, URMSTransaction;
{******************************************************************************}

{ TStaffLogin }
constructor TStaffLogin.Create;
begin
  StaffID := 0;
  LoginID := 0;
  LastUsed := 0;
  LoginButton := nil;
  BeenChecked := False;
end;
{******************************************************************************}

{ TformQuickSaleUser }
procedure TformQuickSaleUser.FormCreate(Sender: TObject);
var
  PMSSize: TBSType;
begin
  inherited;

  PrevTime := 0;
  tmrQSInactivate.Enabled := False;
  FormActivated := False;
  FCPFNumber := '';
  InactivePeriodIndex := -1; // Forces it to first time set inactive items
  FAccountCPFNumberChanged := False;
  if (WBPrinters.QueryType <> qtPrintersCurrent) then begin
    FilterQuery(dm.qrPrinters, qtPrintersCurrent);
  end;

  if (WBOItemGroups.QueryType <> qtOItemGroups) then begin
    FilterQuery(dm.qrOItemGroups, qtOItemGroups);
  end;

  if (WBOBItems.QueryType <> qtOItemsB) then begin
    FilterQuery(dm.qrOBItems, qtOItemsB);
  end;

  if (WBOFItems.QueryType <> qtOItemsF) then begin
    FilterQuery(dm.qrOFItems, qtOItemsF);
  end;

  if (WBCourses.QueryType <> qtCoursesCurrent) then begin
    FilterQuery(dm.qrCourses, qtCoursesCurrent);
  end;

  if (WBSaleCategories.QueryType <> qtSaleCategoriesCurrent) then begin
    FilterQuery(dm.qrSaleCategories, qtSaleCategoriesCurrent);
  end;

  if (Skin.FormFormBorder <> -1) then begin
    pnlForm.BorderWidth := Skin.FormFormBorder;
    pnlAccountButtons.BorderWidth := (4 - Skin.FormFormBorder);
    pnlAccountControlsBorder.BorderWidth := (4 - Skin.FormFormBorder);
    pnlAccountControlsBorder.Height := pnlAccountControlsBorder.Height + (2 * (4 - Skin.FormFormBorder));
    pnlList.BorderWidth := (4 - Skin.FormFormBorder);
  end;
  if (Skin.FormColour <> -1) then begin
    pnlForm.Color := Skin.FormColour;
    pnlForm.BorderStyle := bsNone;
  end;

  if (Skin.FormToolbarColour <> -1) then begin
    pnlTitle.ParentColor := False;
    pnlTitle.ParentBackground := False;
    pnlTitle.Color := TColor(AlphaColorToColor(ThemeData.FormToolbarColour));

    pnlSecondControlButtons.ParentColor := False;
    pnlSecondControlButtons.ParentBackground := False;
    pnlSecondControlButtons.Color := Skin.FormToolbarColour;
  end;

  if (Skin.FormFunctionsBorder <> -1) then begin
    pnlFunctions.BorderWidth := Skin.FormFunctionsBorder;
    pnlFunctions.Width := pnlFunctions.Width + (2 * Skin.FormFunctionsBorder);
    pnlToPayRight.Width := pnlFunctions.Width;

    pnlTenderLines.BorderWidth := Skin.FormFunctionsBorder;
    pnlTenderLines.Width := pnlTenderLines.Width + (2 * Skin.FormFunctionsBorder);
  end;
  if (Skin.FormFunctionsColour <> -1) then begin
    pnlToPayRight.ParentColor := False;
    pnlToPayRight.ParentBackground := False;
    pnlToPayRight.Color := TColor(AlphaColorToColor(ThemeData.FormFunctionsColour));
    pnlTenderLines.ParentColor := False;
    pnlTenderLines.ParentBackground := False;
    pnlTenderLines.Color := Skin.FormFunctionsColour;
  end;
  if (Skin.FormFunctionsEditBoxColour <> -1) then begin
    txtTPayment.Color := Skin.FormFunctionsEditBoxColour;
    txtTTip.Color := Skin.FormFunctionsEditBoxColour;
    txtTTender.Color := Skin.FormFunctionsEditBoxColour;
    txtTChange.Color := Skin.FormFunctionsEditBoxColour;
  end;
  if (Skin.FormFunctionsEditBoxEditColour <> -1) then begin
    txtNextQty.Color := Skin.FormFunctionsEditBoxEditColour;
  end;

  if (Skin.CSTenderSummaryBoxColour <> -1) then begin
    pnlSummaryBottom.ParentColor := False;
    pnlSummaryBottom.ParentBackground := False;
    pnlSummaryBottom.Color := TColor(AlphaColorToColor(ThemeData.FormFunctionsColour)); //Skin.CSTenderSummaryBoxColour;
  end;
  if (Skin.CSTenderSummaryEditBoxColour <> -1) then begin
    txtBalDue.Color := Skin.CSTenderSummaryEditBoxColour;
    txtStillDue.Color := Skin.CSTenderSummaryEditBoxColour;
    txtCChange.Color := Skin.CSTenderSummaryEditBoxColour;
    txtLoyaltyPoints1.Color := Skin.CSTenderSummaryEditBoxColour;
    txtLoyaltyPoints2.Color := Skin.CSTenderSummaryEditBoxColour;
  end;

  if (Skin.CSListColour <> -1) then begin
    pnlList.ParentColor := False;
    pnlList.ParentBackground := False;
    pnlList.Color := Skin.CSListColour;
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

  Skin.ApplySkinToGrid(grdItems);
//  Skin.ApplySkinToGrid(grdOLs);
  Theme.ApplySkinToGrid(grdOLs);
  Skin.ApplySkinToGrid(grdTLs);

//  Skin.NewButtonFromOldButton(cmdHideSkin, cmdHide, bstToolbar, gstClose);
  Theme.NewButtonFromOldButton(cmdHideSkin, cmdHide, 'QSClose');
  Skin.NewButtonFromOldButton(cmdTables, cmdTablesDes, bstToolbar, gstTabs);
  Skin.NewButtonFromOldButton(cmdBookings, cmdBookingsDes, bstToolbar, gstAdmin);
//  Skin.NewButtonFromOldButton(cmdParkedSale, cmdParkedSaleDes, bstToolbar, gstParkSale);
  Theme.NewButtonFromOldButton(cmdParkedSale, cmdParkedSaleDes, 'QSParkSale');
//  Skin.NewButtonFromOldButton(cmdNoSale, cmdNoSaleDes, bstToolbar, gstCashDrawer);
  Theme.NewButtonFromOldButton(cmdNoSale, cmdNoSaleDes, 'QSNoSale');
//  Skin.NewButtonFromOldButton(cmdSelectName, cmdSelectNameDes, bstToolbar, gstStaff);
  Theme.NewButtonFromOldButton(cmdSelectName, cmdSelectNameDes, 'QSMore');
//  Skin.NewButtonFromOldButton(cmdLast, cmdLastDes, bstToolbar, gstLast);
  Theme.NewButtonFromOldButton(cmdLast, cmdLastDes, 'QSLast');
//  Skin.NewButtonFromOldButton(cmdSelect, cmdSelectDes, bstToolbar, gstPrinter);
  Theme.NewButtonFromOldButton(cmdSelect, cmdSelectDes, 'QSSelect');
  Skin.NewButtonFromOldButton(cmdFiscalMenuSkin, cmdFiscalMenu, bstToolbar, gstSummary);
  Skin.NewButtonFromOldButton(cmdComboIndex, cmdComboIndexDes, bstToolbar, gstSummary);
  Skin.NewButtonFromOldButton(cmdEvent, cmdEventDes, bstToolbar, gstChargeAccs);
  Skin.NewButtonFromOldButton(cmdGoody, cmdGoodyDes, bstToolbar, gstGoody);
//  Skin.NewButtonFromOldButton(cmdTable, cmdTableDes, bstToolbar, gstTabs);
  Theme.NewButtonFromOldButton(cmdTable, cmdTableDes, 'QSTabs');
  Skin.NewButtonFromOldButton(cmdBack, cmdBackDes, bstToolbar, gstBack);
  Skin.NewButtonFromOldButton(cmdHome, cmdHomeDes, bstToolbar, gstHome);
  Skin.NewButtonFromOldButton(cmdButtons, cmdButtonsDes, bstToolbar, gstButtonView);
  Skin.NewButtonFromOldButton(cmdList, cmdListDes, bstToolbar, gstListView);
//  Skin.NewButtonFromOldButton(cmdRefund, cmdRefundDes, bstToolbar, gstRefund);
  Theme.NewButtonFromOldButton(cmdRefund, cmdRefundDes, 'QSRefund');
  Skin.NewButtonFromOldButton(cmdTaxCoreBuyerDetail, cmdTaxCoreBuyerDetailDes, bstToolbar, gstRefund);
//  Skin.NewButtonFromOldButton(cmdWaste, cmdWasteDes, bstToolbar, gstWaste);
  Theme.NewButtonFromOldButton(cmdWaste, cmdWasteDes, 'QSWasted');
  Skin.NewButtonFromOldButton(cmdLoyalty, cmdLoyaltyDes, bstToolbar, gstAccount);
//  Skin.NewButtonFromOldButton(cmdPhoneOrder, cmdPhoneOrderDes, bstToolbar, gstPhoneOrder);
  Theme.NewButtonFromOldButton(cmdPhoneOrder, cmdPhoneOrderDes, 'QSPhOrder');
//  Skin.NewButtonFromOldButton(cmdExchange, cmdExchangeDes, bstToolbar, gstRefund);
  Theme.NewButtonFromOldButton(cmdExchange, cmdExchangeDes, 'QSExchange');
//  Skin.NewButtonFromOldButton(cmdViewOrderLines, cmdViewOrderLinesDes, bstView);
  Theme.NewButtonFromOldButton(cmdViewOrderLines, cmdViewOrderLinesDes, 'QSView');
//  Skin.NewButtonFromOldButton(cmdDestn, cmdDestnDes, bstWhere);
  Theme.NewButtonFromOldButton(cmdDestn, cmdDestnDes, 'QSWhere');

  Skin.NewButtonFromOldButton(cmdSecondControlRefund, cmdSecondControlRefundDes, bstToolbar, gstRefund);
  Skin.NewButtonFromOldButton(cmdSecondControlSelectName, cmdSecondControlSelectNameDes, bstToolbar, gstStaff);
  Skin.NewButtonFromOldButton(cmdSecondControlParkedSale, cmdSecondControlParkedSaleDes, bstToolbar, gstParkSale);
  Skin.NewButtonFromOldButton(cmdSecondControlHide, cmdSecondControlHideDes, bstToolbar, gstClose);

  Skin.NewButtonFromOldButton(cmdCPF, cmdCPFDes, bstToolbar, gstRefund);
  Skin.NewButtonFromOldButton(cmdCNPJ, cmdCNPJDes, bstToolbar, gstRefund);

//  Skin.NewButtonFromOldButton(cmdQty, cmdQtyDes, bstNextQty);
  Theme.NewButtonFromOldButton(cmdQty, cmdQtyDes, 'QSQty');
//  Skin.NewButtonFromOldButton(cmdMore, cmdMoreDes, bstPlus);
  Theme.NewButtonFromOldButton(cmdMore, cmdMoreDes, 'QSPlus');
//  Skin.NewButtonFromOldButton(cmdLess, cmdLessDes, bstMinus);
  Theme.NewButtonFromOldButton(cmdLess, cmdLessDes, 'QSMinus');
//  Skin.NewButtonFromOldButton(cmdVoid, cmdVoidDes, bstClear60);
  Theme.NewButtonFromOldButton(cmdVoid, cmdVoidDes, 'QSClear');
//  Skin.NewButtonFromOldButton(cmdMods, cmdModsDes, bstMods);
  Theme.NewButtonFromOldButton(cmdMods, cmdModsDes, 'QSModify');

//  Skin.NewButtonFromOldButton(cmdTenderAndDestn, cmdTenderAndDestnDes, bstTender);
  Theme.NewButtonFromOldButton(cmdTenderAndDestn, cmdTenderAndDestnDes, 'QSMoreTender');
//  Skin.NewButtonFromOldButton(cmdToPayEFTPOS, cmdToPayEFTPOSDes, bstEFTPOS80);
  Theme.NewButtonFromOldButton(cmdToPayEFTPOS, cmdToPayEFTPOSDes, 'QSEftpos');
  Skin.NewButtonFromOldButton(cmdTenderSelectAccount, cmdTenderSelectAccountDes, bstSelectAcc80);
  Skin.NewButtonFromOldButton(cmdTenderExternalAccRIP, cmdTenderExternalAccRIPDes, bstExternalAccountRIP160);

//  Skin.NewButtonFromOldButton(cmdCancel, cmdCancelDes, bstCancel80);
  Theme.NewButtonFromOldButton(cmdCancel, cmdCancelDes, 'QSCancel');
//  Skin.NewButtonFromOldButton(cmdToPayCash, cmdToPayCashDes, bstCash80);
  Theme.NewButtonFromOldButton(cmdToPayCash, cmdToPayCashDes, 'QSCash');
  Skin.NewButtonFromOldButton(cmdToPayPMS, cmdToPayPMSDes, bstGen160);
  Skin.NewButtonFromOldButton(cmdCash50, cmdCash50Des, bst50Dollars);
  Skin.NewButtonFromOldButton(cmdCash20, cmdCash20Des, bst20Dollars);
  Skin.NewButtonFromOldButton(cmdCash10, cmdCash10Des, bst10Dollars);
  Skin.NewButtonFromOldButton(cmdCash5, cmdCash5Des, bst5Dollars);
//  Skin.NewButtonFromOldButton(cmdQuickCash, cmdQuickCashDes, bstOK);
  Theme.NewButtonFromOldButton(cmdQuickCash, cmdQuickCashDes, 'QSOK');

  Skin.NewButtonFromOldButton(cmdQtyZero, cmdQtyZeroDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyOne, cmdQtyOneDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyTwo, cmdQtyTwoDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyThree, cmdQtyThreeDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyFour, cmdQtyFourDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyFive, cmdQtyFiveDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtySix, cmdQtySixDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtySeven, cmdQtySevenDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyEight, cmdQtyEightDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyNine, cmdQtyNineDes, bstGen48);
  Skin.NewButtonFromOldButton(cmdQtyBack, cmdQtyBackDes, bstBackspace96);
  Skin.NewButtonFromOldButton(cmdQtyMore, cmdQtyMoreDes, bstPlus80x60);
  Skin.NewButtonFromOldButton(cmdQtyLess, cmdQtyLessDes, bstMinus80x60);

  Skin.NewButtonFromOldButton(cmdUp, cmdUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdDown, cmdDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdFood, cmdFoodDes, bstFood);
  Skin.NewButtonFromOldButton(cmdBev, cmdBevDes, bstBeverage);
  Skin.NewButtonFromOldButton(cmdAllGroups, cmdAllGroupsDes, bstItemGroups);
  Skin.NewButtonFromOldButton(cmdSearch, cmdSearchDes, bstSearch96);

  //Tender Panel
  Skin.NewButtonFromOldButton(cmdTLDelete, cmdTLDeleteDes, bstClear112);
  Skin.NewButtonFromOldButton(cmdTLClear, cmdTLClearDes, bstAll112);
  Skin.NewButtonFromOldButton(cmdDetails, cmdDetailsDes, bstDetails);
  Skin.NewButtonFromOldButton(cmdTLUp, cmdTLUpDes, bstUp);
  Skin.NewButtonFromOldButton(cmdTLDown, cmdTLDownDes, bstDown);
  Skin.NewButtonFromOldButton(cmdVoucher, cmdVoucherDes, bstVoucher);
  Skin.NewButtonFromOldButton(cmdOther, cmdOtherDes, bstOther);
  Skin.NewButtonFromOldButton(cmdPMS, cmdPMSDes, bstGen112);
  Skin.NewButtonFromOldButton(cmdEdit, cmdEditDes, bstEdit);
  Skin.NewButtonFromOldButton(cmdDiners, cmdDinersDes, bstDiners);
  Skin.NewButtonFromOldButton(cmdVisa, cmdVisaDes, bstVisa);
  Skin.NewButtonFromOldButton(cmdEFTPOS, cmdEFTPOSDes, bstEFTPOS);
  Skin.NewButtonFromOldButton(cmdReturnToSale, cmdReturnToSaleDes, bstSale48x96);
  Skin.NewButtonFromOldButton(cmdMasterCard, cmdMasterCardDes, bstMasterCard);
  Skin.NewButtonFromOldButton(cmdAMEX, cmdAMEXDes, bstAmex);
  Skin.NewButtonFromOldButton(cmdCash, cmdCashDes, bstCash);
  Skin.NewButtonFromOldButton(cmdQuickCashOK, cmdQuickCashOKDes, bstOK);

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

  CSStartPhoneOrder := False;    //Setting Global variable at start

  OLList := TList.Create;
  VoidOLList := TList.Create;
  TLList := TTLList.Create;
  StaffLoginsList := TList.Create;
  SurchargeList := TList.Create;
  DiscCalculator := TDiscountCalc.Create;
  FExchangeOLs := TList.Create;
  SubsidyCalc := TSubsidyCalc.Create;
  ComboFreeItems := TObjectList.Create;
  ComboFreeItems.OwnsObjects := False;
  FAllCombos := TCombos.Create;
  FDriveThroughGroupIDs := TIntegerList.Create;
  FDriveThroughGroupIDs.AllowDuplicates := False;
  FDriveThroughGroupIDs.AllowZero := False;

  SaleCategorySurchargeItem := nil;
  //ItemDiscountList := TObjectList.Create;
  SetOLCols;
  SetTLCols;
  GlbScan.SwipedStaffCard := False; //akm 26.11.01

  BackPagesList := TIntegerList.Create;
  PriceLevel := 0;

  FModifierFunction := 1;
  BarCodePriceCode := '';

  BuildItemGroups;
  BuildItems;     //Builds the Open Arch Pages

  ShowButtons := True;
  SetButtons;

  if (WBAccountButtons.QueryType <> qtAccountButtonsCurrent) then begin
    FilterQuery(dm.qrAccountButtons, qtAccountButtonsCurrent);
  end;

  grdItems.DefaultRowHeight := AppDetails.DefaultRowHeight;    //Jon 28-8-2001
  grdOLs.DefaultRowHeight := AppDetails.DefaultRowHeight;      //Jon 28-8-2001
  grdTLs.DefaultRowHeight := AppDetails.DefaultRowHeight;      //Jon 28-8-2001

  if (AppDetails.CashSaleShowLoyaltyPoints) then begin
    if (AppDetails.CashSaleShowLoyaltyPoints1 and AppDetails.CashSaleShowLoyaltyPoints2) then begin
      pnlSummaryBottom.Height := 131;
      pnlSummaryRight.Redraw;
    end
    else begin
      pnlSummaryBottom.Height := 106;
      pnlSummaryRight.Redraw;
    end;

    if (AppDetails.CashSaleShowLoyaltyPoints1) then begin
      lblLoyaltyPoints1.Visible := True;
      txtLoyaltyPoints1.Visible := True;
      lblLoyaltyPoints1.Caption := AppDetails.LoyaltyPoints1Name;
    end;
    if (AppDetails.CashSaleShowLoyaltyPoints2) then begin
      lblLoyaltyPoints2.Visible := True;
      txtLoyaltyPoints2.Visible := True;
      lblLoyaltyPoints2.Caption := AppDetails.LoyaltyPoints2Name;
      if (not AppDetails.CashSaleShowLoyaltyPoints1) then begin
        lblLoyaltyPoints2.Top := 78;
        txtLoyaltyPoints2.Top := 78;
      end;
    end;
  end
  else begin
    pnlSummaryBottom.Height := 81;
  end;

  cmdRefund.Visible := AppDetails.FiscalType = fiscalNone;
  cmdTaxCoreBuyerDetail.Visible := AppDetails.FiscalType = fiscalTaxCore;
  cmdSecondControlRefund.Visible := AppDetails.FiscalType = fiscalNone;
  cmdCPF.Visible := AppDetails.EnableFiscalPrinting;
  cmdCNPJ.Visible := AppDetails.EnableFiscalPrinting;

  cmdToPayEFTPOS.Enabled := GetTenderLineTypeRecord(TLTEFTPOS).TenderTypeEnabled;
  cmdToPayCash.Enabled := GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled;
  cmdEFTPOS.Enabled := GetTenderLineTypeRecord(TLTEFTPOS).TenderTypeEnabled;
  cmdCash.Enabled := GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled;
  if ((AppDetails.PaymentAllowPrePay) and (GlbTable.PrepaidBalance <> 0)) then begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTPrepaid).TenderTypeEnabled;
    cmdCheque.Caption := sPrePaid;
  end
  else if (AppDetails.SmartCardInterface > 0) then begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTSmartCard).TenderTypeEnabled;
    cmdCheque.Caption := sSmartCard;
  end
  else if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '')) then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTPocketVoucher).TenderTypeEnabled;
    cmdCheque.Caption := 'POCKETvoucher';
  end
  else if AppDetails.wiGroupEnabled then
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstGen112);
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTwiGroup).TenderTypeEnabled;
    cmdCheque.Caption := swiGroupTL;
  end
  else
  begin
    Skin.NewButtonFromOldButton(cmdCheque, cmdChequeDes, bstCheque);
    cmdCheque.Enabled := GetTenderLineTypeRecord(TLTCheque).TenderTypeEnabled;
  end;
  cmdVoucher.Enabled := GetTenderLineTypeRecord(TLTVoucher).TenderTypeEnabled;
  cmdOther.Enabled := GetTenderLineTypeRecord(TLTOther).TenderTypeEnabled;
  cmdVisa.Enabled := (GetTenderLineTypeRecord(TLTVisaEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTVisaManual).TenderTypeEnabled);
  cmdMasterCard.Enabled := (GetTenderLineTypeRecord(TLTMasterCardEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTMasterCardManual).TenderTypeEnabled);
  cmdAMEX.Enabled := (GetTenderLineTypeRecord(TLTAmexEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTAmexManual).TenderTypeEnabled);
  cmdDiners.Enabled := (GetTenderLineTypeRecord(TLTDinersEFTPOS).TenderTypeEnabled or GetTenderLineTypeRecord(TLTDinersManual).TenderTypeEnabled);

  if (AppDetails.CashSaleShowTenderOKButton) then begin
    pnlTenderTenders.Height := 274;
    PositionControl(TControl(cmdQuickCashOK), 224, 226, -1, -1);
    cmdQuickCashOK.Visible := True;
  end;

  if (AppDetails.PMSType <> pmsNone) then begin
    PositionControl(TControl(cmdVoucher), 112, -1, 76, -1);
    PositionControl(TControl(cmdOther), 188, -1, 76, -1);
    PositionControl(TControl(cmdPMS), 264, 82, 72, 48);
    cmdPMS.Visible := True;
    case AppDetails.PMSType of
      pmsLibica: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biLibica);
      pmsFidelio, pmsPhoenix, pmsMicros4700, pmsICRTouch, pmsMews, pmsAgora, pmsRMS: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biRoomCharge);
      pmsNZAGold: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biNZAGold);
      pmsEvolution: Skin.ChangeButton(cmdPMS, bstGen72, gstNoChange, biEvolution);
    end;

    if (AppDetails.CashSaleAutoComplete) then begin
      cmdQuickCash.Visible := False;
      PositionControl(TControl(cmdQuickCash), 80, -1, 160, -1);
      PMSSize := bstGen160;
      cmdToPayPMS.Width := 160;
    end
    else begin
      cmdQuickCash.Visible := True;
      PositionControl(TControl(cmdQuickCash), 160, -1, 80, -1);
      PMSSize := bstGen80;
      cmdToPayPMS.Width := 80;
    end;
    PositionControl(TControl(cmdToPayPMS), 80, 48, -1, 48);
    cmdToPayPMS.Visible := True;

    case AppDetails.PMSType of
      pmsLibica: Skin.ChangeButton(cmdToPayPMS, PMSSize, gstNoChange, biLibica);
      pmsFidelio, pmsPhoenix, pmsMicros4700, pmsICRTouch, pmsMews, pmsAgora, pmsRMS: Skin.ChangeButton(cmdToPayPMS, PMSSize, gstNoChange, biRoomCharge);
      pmsNZAGold: Skin.ChangeButton(cmdToPayPMS, PMSSize, gstNoChange, biNZAGold);
      pmsEvolution: Skin.ChangeButton(cmdToPayPMS, PMSSize, gstNoChange, biEvolution);
    end;

    if (((AppDetails.PMSType = pmsNZAGold) and AppDetails.NZAOnly) or
        ((AppDetails.PMSType = pmsEvolution) and AppDetails.EvolutionOnly) or
        ((AppDetails.PMSType = pmsMiniBar) and AppDetails.MinibarRoomTransferOnly)) then
    begin
      cmdToPayCash.Visible := False;
      cmdToPayEFTPOS.Visible := False;
      cmdTenderAndDestn.Width := 160;
      cmdQuickCash.Visible := False;
      PositionControl(TControl(cmdQuickCash), 0, 48, 240, 48);

      cmdMasterCard.Visible := False;
      cmdAmex.Visible := False;
      cmdDiners.Visible := False;
      cmdVisa.Visible := False;
      cmdCash.Visible := False;
      cmdEFTPOS.Visible := False;
      cmdCheque.Visible := False;
      cmdVoucher.Visible := False;
      cmdOther.Visible := False;

      PositionControl(TControl(cmdToPayPMS), 0, 48, 240, 48);
      PositionControl(TControl(cmdPMS), 84, 118, 168, 72);

{      if (AppDetails.PMSType = pmsNZAGold) then begin
        PositionControl(TControl(cmdToPayNZAGold), 0, 48, 240, 48);
        PositionControl(TControl(pnlNZAGold), 0, 82, 336, 144);
        PositionControl(TControl(cmdNZAGold), 84, 36, 168, 72);
        pnlNZAGold.BevelOuter := bvRaised;
      end
      else if (AppDetails.PMSType = pmsEvolution) then begin
        PositionControl(TControl(cmdToPayEvolution), 0, 48, 240, 48);
        PositionControl(TControl(pnlEvolution), 0, 82, 336, 144);
        PositionControl(TControl(cmdEvolution), 84, 36, 168, 72);
        pnlEvolution.BevelOuter := bvRaised;
      end;}
    end;
  end;

{  Skin.ChangeButton(cmdTables, bstToolbar, gstTabs, biPeopleTable);
  Skin.ChangeButton(cmdTable, bstToolbar, gstTabs, biPeopleTable);
  Skin.ChangeButton(cmdBookings, bstToolbar, gstAdmin, biPeopleTable);
  Skin.ChangeButton(cmdParkedSale, bstToolbar, gstParkSale, biParkedCar);
  Skin.ChangeButton(cmdSelectName, bstToolbar, gstStaff, biThreePeople);
  Skin.ChangeButton(cmdRefund, bstToolbar, gstRefund, biMoneyBag);
  Skin.ChangeButton(cmdTaxCoreBuyerDetail, bstToolbar, gstRefund, biMoneyBag);
  Skin.ChangeButton(cmdWaste, bstToolbar, gstWaste, biTrashSmall);
  Skin.ChangeButton(cmdLoyalty, bstToolbar, gstAccount, biPaper);
  Skin.ChangeButton(cmdPhoneOrder, bstToolbar, gstPhoneOrder, biPhone);
  Skin.ChangeButton(cmdExchange, bstToolbar, gstRefund, biMoneyBag);

  Skin.ChangeButton(cmdSecondControlParkedSale, bstToolbar, gstParkSale, biParkedCar);
  Skin.ChangeButton(cmdSecondControlSelectName, bstToolbar, gstStaff, biThreePeople);
  Skin.ChangeButton(cmdSecondControlRefund, bstToolbar, gstRefund, biMoneyBag);    }

//  if (AppDetails.CashSaleRemoveNavigation) then begin
    pnlButtonList.Visible := False;
    cmdHome.Visible := False;
    cmdBack.Visible := False;
//  end;
  cmdTables.Visible := False;
  if (AppDetails.CashSaleRemoveTabs) then begin
    cmdTable.Visible := False;
    cmdTables.Visible := False;
  end;
  if (AppDetails.CashSaleRemoveReprintInvoices) then begin
    cmdSelect.Visible := False;
    cmdLast.Visible := False;
  end;
  if (AppDetails.CashSaleRemoveRefund) then begin
    cmdRefund.Visible := False;
    cmdWaste.Visible := False;
  end
  else if (AppDetails.VoidWastageMod) then begin
    cmdWaste.Visible := True;
  end;
  if (((AppDetails.CashSaleRemoveExchange) and (not AppDetails.EnableFiscalPrinting)) or
      ((AppDetails.CashSaleRemoveRefund) and (AppDetails.EnableFiscalPrinting)) or
       (AppDetails.FiscalType = fiscalTaxCore)) then
  begin
    cmdExchange.Visible := False;
  end;
  if (AppDetails.CashSaleRemoveNoSale) then begin
    cmdNoSale.Visible := False;
  end;
  if (AppDetails.CashSaleRemoveParkSale) then begin
    cmdParkedSale.Visible := False;
  end;

  if ((AppDetails.EnablePhoneOrders <= 0) or ((AppDetails.EnableFiscalPrinting) and (DoingPhoneOrder))) then begin
    cmdPhoneOrder.Visible := False;
  end;

  cmdBookings.Visible := False;//(((sAPIConnecions.GetAPIConnection(actResDiary).isEnabled) or
                         // ((AppDetails.TMTableAreasExist) and ((AppDetails.BookingsGeneralEnabled) or (AppDetails.BookingsPartyEnabled)))) and
                         //(TheActiveForm <> afBookings));
  cmdComboIndex.Visible := False;//AppDetails.EnableCombo;
  cmdComboIndex.Enabled := False;
  cmdEvent.Visible := False;//AppDetails.EnableEvent;
 if (AppDetails.CashSaleRemoveOrderLineChange) then begin
    pnlOrderLineEditButtons.Visible := False;
    pnlTenderLineButtons.Top := 4;
  end;

  if ((AppDetails.CashSaleRemoveTenderChange) or (AppDetails.CashSaleCashOnlyTenders > 0)) then
  begin
    cmdToPayEFTPOS.Visible := False;

    cmdCash50.Visible := (AppDetails.CashSaleCashOnlyTenders > 0);
    cmdCash20.Visible := (AppDetails.CashSaleCashOnlyTenders > 0);
    cmdCash10.Visible := (AppDetails.CashSaleCashOnlyTenders > 0);
    cmdCash5.Visible := (AppDetails.CashSaleCashOnlyTenders > 0);
    cmdTenderAndDestn.Visible := (AppDetails.CashSaleCashOnlyTenders <> 1);
    cmdTenderSelectAccount.Visible := (AppDetails.CashSaleCashOnlyTenders = 3);
    cmdCancel.Visible := (AppDetails.CashSaleCashOnlyTenders in [0, 3]);
    cmdToPayCash.Visible := (AppDetails.CashSaleCashOnlyTenders > 0);

    if (AppDetails.CashSaleCashOnlyTenders in [1, 2]) then
    begin
      PositionControl(TControl(cmdCash50), 0, 0, 80, 48);
      PositionControl(TControl(cmdCash20), 80, 0, 80, 48);
      PositionControl(TControl(cmdCash10), 160, 0, 80, 48);
      PositionControl(TControl(cmdCash5), 0, 48, 80, 48);
      PositionControl(TControl(cmdToPayCash), 80, 48, 80, 48);
      PositionControl(TControl(cmdQuickCash), 160, 48, 80, 48);
      pnlTenderLineButtons.Height := 96;
      if (AppDetails.CashSaleCashOnlyTenders = 2) then
      begin
{        cmdDestn.Visible := False;
//          cmdTenderAndDestn.Owner := cmdDestn.Owner;
        cmdTenderAndDestn.Parent := cmdDestn.Parent;
        PositionControl(TControl(cmdTenderAndDestn), cmdDestn.Left, cmdDestn.Top, cmdDestn.Width, cmdDestn.Height);
        cmdTenderAndDestn.Visible := True;   }
      end;
    end
    else if (AppDetails.CashSaleCashOnlyTenders = 3) then
    begin
      PositionControl(TControl(cmdTenderAndDestn), 0, 0, 80, 48);
      PositionControl(TControl(cmdTenderSelectAccount), 80, 0, 80, 48);
      PositionControl(TControl(cmdCancel), 160, 0, 80, 48);
      PositionControl(TControl(cmdCash50), 0, 48, 80, 48);
      PositionControl(TControl(cmdCash20), 80, 48, 80, 48);
      PositionControl(TControl(cmdCash10), 160, 48, 80, 48);
      PositionControl(TControl(cmdCash5), 0, 96, 80, 48);
      PositionControl(TControl(cmdToPayCash), 80, 96, 80, 48);
      PositionControl(TControl(cmdQuickCash), 160, 96, 80, 48);
      pnlTenderLineButtons.Height := 144;
    end
    else
    begin
      cmdQuickCash.Top := 0;
      cmdCancel.Top := 0;
      cmdCancel.Left := 0;
      pnlTenderLineButtons.Height := 48;
    end;

    if (((AppDetails.PMSType = pmsNZAGold) and AppDetails.NZAOnly) or
        ((AppDetails.PMSType = pmsEvolution) and AppDetails.EvolutionOnly) or
        ((AppDetails.PMSType = pmsMiniBar) and AppDetails.MinibarRoomTransferOnly)) then
    begin
      PositionControl(TControl(cmdToPayPMS), cmdQuickCash.Left, cmdQuickCash.Top, cmdQuickCash.Width, 48);
    end;
  end;

  cmdToPayEFTPOS.Top := 4;
  cmdToPayEFTPOS.Left := 0;
  PositionControlAlign(TControl(cmdToPayCash), TControl(cmdToPayEFTPOS), alRight, 4);
  PositionControlAlign(TControl(cmdTenderAndDestn), TControl(cmdToPayCash), alRight, 4);
  PositionControlAlign(TControl(cmdCancel), TControl(cmdToPayEFTPOS), alBottom, 4);
  PositionControlAlign(TControl(cmdQuickCash), TControl(cmdCancel), alRight, 4);

//  cmdToPayPMS.Top := cmdQuickCash.Top;
//  cmdToPayCash.Top := cmdQuickCash.Top;
  pnlTenderBottom.Height := pnlTenderLineButtons.Top + pnlTenderLineButtons.Height;

  if (Screen.Width <= 800) then
  begin
    pnlControlButtons.Width := 650;
    cmdHideSkin.Width := 48;
    cmdHideSkin.Left := pnlControlButtons.Width - 49;//cmdHideSkin.Left - 192;
  end
  else begin
    pnlControlButtons.Width := 868; //Set based on 1024 width
    cmdHideSkin.Left := pnlControlButtons.Width - 60;
  end;

{  PositionControlAlignWithin(TControl(cmdTables), TControl(cmdHideSkin), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdBookings), TControl(cmdTables),  alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdParkedSale), TControl(cmdBookings), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdNoSale), TControl(cmdParkedSale), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdSelectName), TControl(cmdNoSale), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdLast), TControl(cmdSelectName), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdSelect), TControl(cmdLast), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdTable), TControl(cmdSelect), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdBack), TControl(cmdTable), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdHome), TControl(cmdBack), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(pnlButtonList), TControl(cmdHome), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdEvent), TControl(pnlButtonList), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdExchange), TControl(cmdEvent), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdRefund), TControl(cmdExchange), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdTaxCoreBuyerDetail), TControl(cmdRefund), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdWaste), TControl(cmdTaxCoreBuyerDetail), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdPhoneOrder), TControl(cmdWaste), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdFiscalMenuSkin), TControl(cmdPhoneOrder), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdCNPJ), TControl(cmdFiscalMenuSkin), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdComboIndex), TControl(cmdCNPJ), alLeft, pnlControlButtons.Width);
  PositionControlAlignWithin(TControl(cmdGoody), TControl(cmdComboIndex), alLeft, pnlControlButtons.Width); }


//  PositionControlAlign(TControl(cmdTables), TControl(cmdHideSkin), alLeft, 4);
//  PositionControlAlign(TControl(cmdBookings), TControl(cmdTables),  alLeft, 4);
  PositionControlAlign(TControl(cmdSelectName), TControl(cmdHideSkin), alLeft, 4);
  PositionControlAlign(TControl(cmdDestn), TControl(cmdSelectName), alLeft, 4);
  PositionControlAlign(TControl(cmdViewOrderLines), TControl(cmdDestn), alLeft, 4);
  PositionControlAlign(TControl(cmdParkedSale), TControl(cmdViewOrderLines), alLeft, 4);
  PositionControlAlign(TControl(cmdTable), TControl(cmdParkedSale), alLeft, 4);
  PositionControlAlign(TControl(cmdNoSale), TControl(cmdTable), alLeft, 4);
  PositionControlAlign(TControl(cmdSelect), TControl(cmdNoSale), alLeft, 4);
  PositionControlAlign(TControl(cmdLast), TControl(cmdSelect), alLeft, 4);
  PositionControlAlign(TControl(cmdExchange), TControl(cmdLast), alLeft, 4);
  PositionControlAlign(TControl(cmdRefund), TControl(cmdExchange), alLeft, 4);
  PositionControlAlign(TControl(cmdWaste), TControl(cmdRefund), alLeft, 4);
  PositionControlAlign(TControl(cmdPhoneOrder), TControl(cmdWaste), alLeft, 4);

{  PositionControlAlign(TControl(cmdBack), TControl(cmdTable), alLeft, 4);
  PositionControlAlign(TControl(cmdHome), TControl(cmdBack), alLeft, 4);
  PositionControlAlign(TControl(pnlButtonList), TControl(cmdHome), alLeft, 4);
  PositionControlAlign(TControl(cmdEvent), TControl(pnlButtonList), alLeft, 4);
  PositionControlAlign(TControl(cmdTaxCoreBuyerDetail), TControl(cmdRefund), alLeft, 4);
  PositionControlAlign(TControl(cmdFiscalMenuSkin), TControl(cmdPhoneOrder), alLeft, 4);
  PositionControlAlign(TControl(cmdCNPJ), TControl(cmdFiscalMenuSkin), alLeft, 4);
  PositionControlAlign(TControl(cmdComboIndex), TControl(cmdCNPJ), alLeft, 4);
  PositionControlAlign(TControl(cmdGoody), TControl(cmdComboIndex), alLeft, 4);    }

  PositionControlAlign(TControl(cmdMore), TControl(cmdQty), alRight, 4);
  PositionControlAlign(TControl(cmdLess), TControl(cmdMore), alRight, 4);
  PositionControlAlign(TControl(cmdVoid), TControl(cmdLess), alRight, 4);
  PositionControlAlign(TControl(cmdMods), TControl(cmdVoid), alRight, 4);

  pnlTitle.Visible := (not AppDetails.CashSaleUseBigScreen);
  pnlSecondControlButtons.Visible := (AppDetails.CashSaleUseBigScreen);
  pnlStaffLogins.Visible := False;//AppDetails.CashSaleStaffLoginButtons;
  if ((AppDetails.CashSaleUseBigScreen) and (AppDetails.CashSaleBigScreenButtonType > 0)) then begin
    cmdViewOrderLines.Caption := sParkSale;
    //ChangeButtonIcon(cmdViewOrderLines, biParkedCar);     Jon 21-06-2006
    Skin.ChangeButton(cmdViewOrderLines, bstGen60, gstParkSale, biParkedCar);
    cmdViewOrderLines.OnClick := cmdParkedSaleClick;

    case AppDetails.CashSaleBigScreenButtonType of
      1: begin
        cmdSecondControlParkedSale.Caption := sTabs;
        Skin.ChangeButton(cmdSecondControlParkedSale, bstToolbar, gstTabs, biPeopleTable);
        cmdSecondControlParkedSale.OnClick := cmdTablesClick;
      end;
      2: begin
        cmdSecondControlParkedSale.Caption := sTab;
        Skin.ChangeButton(cmdSecondControlParkedSale, bstToolbar, gstTabs, biPeopleTable);
        cmdSecondControlParkedSale.OnClick := cmdTableClick;
      end;
      3: begin
        cmdSecondControlParkedSale.Caption := sDrive;
        Skin.ChangeButton(cmdSecondControlParkedSale, bstToolbar, gstTabs, biStreet);
        cmdSecondControlParkedSale.OnClick := cmdTableClick;
      end;
      4: begin
        cmdSecondControlParkedSale.Caption := sKiosk;
        Skin.ChangeButton(cmdSecondControlParkedSale, bstToolbar, gstTabs, biPeopleTable);
        cmdSecondControlParkedSale.OnClick := cmdTableClick;
      end;
    end;
  end;

  if (AppDetails.TenLnTypOther <> 'Other') then begin
    Skin.ChangeButton(cmdOther, bstGen112, gstNoChange, biNone, AppDetails.TenLnTypOther);
  end;
  if (AppDetails.CashSaleSwapStillDueForTenders) then begin   //Jon 21-08-2003
    //We are just reusing the StillDue boxs for the Tender Boxs.
    lblStillDue.Caption := sTender;
  end;

  lblAvailDollarSign.Caption := AppDetails.CurrencyType;
  lblDiscountAmount.Caption := format(sDiscountAmount, [AppDetails.CurrencyType]);

  DoingPhoneOrder := False;        //Jon 24-01-2005 Setting it for the first time
  PhoneOrderPersistant := False;   //Jon 25-01-2005 Setting it for the first time
  PromptedWhere := False;

  cmdCPF.Visible := False;
  if AppDetails.EnableFiscalPrinting then
    cmdCPF.Visible := True;
  cmdCNPJ.Visible := cmdCPF.Visible;

  RSC.AllowUpdate := False;  //After completing the Transaction when replacing a Swipe Card need to update the swipe card for the selected account.
  RSC.ReasonAlreadyOrdered := False;
  RSC.CardCode := '';
  RSC.ItemID := 0;
  RSC.CancelPressed := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.FormShow(Sender: TObject);
begin
  inherited;
  lblTerminal.Caption := AppDetails.TerminalName;
  lblStaffName.Caption := '';
  IsShowing := True;
  GlbAccount.AccountID := 0;
  FPrevAccountID := 0;

  GlbCustomer.AccountID := 0;
  GlbAccount.PerorgName := '';
  GlbAccount.PerorgFirstName := '';
  GlbAccount.PerorgAddress := '';

  FAllCombos.LoadCombos;

  DeselectEvent; // call to clear if selected any

  ConfirmBeforeCancel := True;
  ShowCPFNUmberInActivate := True;
  FCPFNumber := '';
  FAccountCPFNumberChanged := False;

  //to speed up cash sale screen 7Dec99 AM
  InCashSale := True;
  Theme.SetFormSpacing(Self);

  //pcDetail.ActivePage := tsToPay;     //Jon 4-9-2001
  //tsToPay.TabVisible := False;        //Jon 4-9-2001
  //tsTender.TabVisible := False;       //Jon 4-9-2001

  ShowPnlToPay(True);        //Jon 4-9-2001

{ // AMANDA 19/10/00 - use TableSelected below instead
  //AMANDA 12/10/00 - moved above ClearAlls as chkTable.State used from ClearAllOrderLines
  chkTable.State := cbUnChecked;
  cmdTable.Caption := '';
  FDelivery := '';}
  //AMANDA 19/10/00

  IniGoodyUI(Self, cmdGoody);

  ResetCashSale(False, True);       //Jon 09-01-2007
  NoSalePINEntered := False;
  PriceLevelChange(0, 3);
  SetView;

  //if not AppDetails.EnableFiscalPrinting then
  BuildAccountButtons;

  if (ShowingQty) and (AppDetails.AutoCloseNextQty) then begin
    ShowQty(False);
  end;

  OpenArch.CurrentQSPageID := AppDetails.CashSaleDefaultPageID;
  AllowActivateTesting := True;    //Jon 24-07-2003

  ClearStatusPage;
  if (NFManual and (not NFManualSecond)) then
    SetStatus(sQuickSalesINManualNFMode, False, True)
  else if (NFManual and (NFManualSecond)) then
    SetStatus(sQuickSalesINManualNFMode, False, False)
  else if NFEMode then
    SetStatus(sQuickSalesINNFEMode, False, NFManual)
  else
    SetStatus(sQuickSales, False, NFManual);

  //No refund functionality for Brazil
  cmdCPF.Enabled := False;
  cmdTable.Enabled := not AppDetails.DisableTableFeature;
  cmdTables.Enabled := not AppDetails.DisableTableFeature;

  //Only show 'RIP Expenses' button in tender line button area if buttons haven't already been repositioned
  //in FormCreate due to AppDetails.CashSaleRemoveTenderChange or AppDetails.CashSaleCashOnlyTenders > 0.
  if not ((AppDetails.CashSaleRemoveTenderChange) or (AppDetails.CashSaleCashOnlyTenders > 0)) and
    sExternalAccountIntegration.IsActive(eptRIP) then
  begin
    cmdTenderExternalAccRIP.Visible := True;
    cmdTenderSelectAccount.Visible := True;
    PositionControl(TControl(cmdTenderExternalAccRIP), 0, 0, 160, 48);
    PositionControl(TControl(cmdTenderSelectAccount), 160, 0, 80, 48);
    PositionControl(TControl(cmdTenderAndDestn), 0, 48, 80, 48);
    PositionControl(TControl(cmdToPayEFTPOS), 80, 48, 80, 48);
    PositionControl(TControl(cmdCancel), 160, 48, 80, 48);
    PositionControl(TControl(cmdToPayCash), 0, 96, 80, 48);
    PositionControl(TControl(cmdQuickCash), 80, 96, 160, 48);

    pnlTenderLineButtons.Height := 144;
    pnlTenderBottom.Height := pnlTenderLineButtons.Top + pnlTenderLineButtons.Height;
  end;

  //show 'External Accounts' button if there's at least one active external account other than RIP
  cmdExternalAccLookup.Visible := sExternalAccountIntegration.IsActive and
    (not (sExternalAccountIntegration.IsActive(eptRIP) and (sExternalAccountIntegration.FExternalProviders.Count = 1)));
  //show 'RIP Expenses' button in account area if RIP active
  cmdExternalAccRIP.Visible := sExternalAccountIntegration.IsActive(eptRIP);

  //reposition 'RIP Expense' button if 'External Accounts' button is not visible
  if (not cmdExternalAccLookup.Visible) and cmdExternalAccRIP.Visible then
    PositionControl(TControl(cmdExternalAccRIP), 392, 4, 128, 48);

  sExternalAccountIntegration.SetOrderChanged;

  CheckTaxCoreReadyForInvoice;
end;
{******************************************************************************}
procedure TformQuickSaleUser.FormActivate(Sender: TObject);
begin
  inherited;
  IsShowing := True;
  //  cmdFiscalMenu.Visible := False;
  //  cmdSelectNameClick(nil);
  if (AllowActivateTesting) then begin    //Jon 24-07-2003  So that this code only happens when we've just shown the screen, and not for other events
    //akm 04.12.01 - if force staff card swipe, don't need to show select staff prompt, just go straight in
    //if (AppDetails.ForceStaffCardSwipe) or (SelectName(True, True, AppDetails.ProveStaffCashSale)) then begin
    AllowActivateTesting := False;   //Jon 24-07-2003
    if (SelectName(True, True, AppDetails.ProveStaffCashSale)) then
    begin
      //InitialiseAccountInfo;
      DeselectAccount;
      CloseFromCashSales := False;
      AddStaffLoginButton;
      tmrQSInactivate.Enabled := False;
      FQSInactiveTime := Now;
      tmrQSInactivate.Enabled := (AppDetails.QSInactiveTimeout > 0);

      if (CSStartPhoneOrder) then
      begin
        if ((not DoPhoneOrder(True, True)) and (not OnTable)) then
        begin
          IsShowing := False;
          PostMessage(Self.Handle, WM_CLOSE, 0, 0);
          Exit;
        end;
      end
      else begin
        DoPhoneOrder(False, False);

        if ((AppDetails.StaffAutoParkSale > 0) and (SelectParkedSale(True))) then begin
          TryLoadParkedSale;
        end;
      end;

      if AppDetails.EnableFiscalPrinting then
      begin
        cmdRefund.Visible := False;
        cmdSecondControlRefund.Visible := False;
        if (not CloseFromCashSales) then
        begin
          if (CheckFiscalPrinterFunctions and ShowCPFNUmberInActivate and AppDetails.FiscalIsCFPRequired) then
          begin
            FCPFNumber := GetCPFNumber;
          end;
        end;
      end;
      FormActivated := True;
    end
    else begin
      //cmdHideClick(nil);      //Deleted by Jon 11-7-2001 cos it doesn't work for some reason. That's why I put a delay on it instead.
      CloseFromCashSales := True;
      StaffPromptTimeoutTimer.Interval := 50;
      StaffPromptTimeoutTimer.Enabled := True;
      StaffPromptTimeoutTimerActive := True;

      DoingServiceTransaction := False;
    end;
  end;
  ActiveControl := nil;
end;
{******************************************************************************}
procedure TformQuickSaleUser.FormClose(Sender: TObject;var Action: TCloseAction);
begin
  if Assigned(DiscCalculator) then
  begin
    DiscCalculator.ClearItemLevelDiscounts;
    DiscCalculator.ClearPrevTabItems;
  end;
  ClearItemLevelDiscounts(OLList);

  if Assigned(SubsidyCalc) then
  begin
    SubsidyCalc.ClearSubsidyDetails;
    ClearItemLevelSubsidy;
  end;

  //to speed up cash sale screen 7Dec99 AM
  tmrQSInactivate.Enabled := False;
  if (dm.qrCashSales.Active) then
  begin
    dm.qrCashSales.Close; //make inactive so not refreshed once out of cash sale
  end;

  if ((GlbTable.GroupID > 0) and (GlbTable.TableLocked)) then
  begin
    CheckTableLock(False, True);
  end;

  InCashSale := False;
  CSStartPhoneOrder := False;
  if (WBTables.RefreshNeeded) then
  begin
    ProcessEventQuery(dm.qrTables);
    WBTables.RefreshNeeded := False;
  end;

  StopStaffPromptTimeoutTimer;
  DoingServiceTransaction := False;

  ClearStatusPage;       //Jon 27-10-2003

  PhoneOrderPersistant := False;
  IsShowing := False;

  inherited;
end;

{******************************************************************************}
procedure TformQuickSaleUser.FormDeactivate(Sender: TObject);
begin
  ActiveControl := nil;
  inherited;
end;

procedure TformQuickSaleUser.FormDestroy(Sender: TObject);
begin
  IsShowing := False;
  ClearListObjects(OLList, True);
  ClearListObjects(VoidOLList, True);
  ClearListObjects(FExchangeOLs, True);

  TLList.Free;
  ClearAllSurcharges;
  SurchargeList.Free;

  ClearListObjects(StaffLoginsList, True);
  BackPagesList.Free;
  FAllCombos.Free;
  DiscCalculator.Free;
  SubsidyCalc.Free;
  ComboFreeItems.Free;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.ClearPayment;

  inherited;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQuickCashClick(Sender: TObject);
var
  TLType: TTLT;
  TLPayment: Currency;
  TLTip: Currency;
  TLChange: Currency;
  TLSurcharge: Currency;
  TLTender: Currency;
  TLRounding: Currency;
  TL: TTL;
  AddedAutoTenderLine: Boolean;
  FoundCashTenderLine: Boolean;
  FinishedSale: Boolean;
  I: Integer;
  TenderLineType: TTenderLineType;
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.cmdQuickCashClick begin');
  FinishedSale := False;

  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.cmdQuickCashClick Exit/1');
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001

  if ((AppDetails.CashSaleNoSaleDefault) and (TLList.Count = 0) and (OLList.Count = 0) and (not OnAccount)) then begin
    AttemptDefaultNoSale;
    LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.cmdQuickCashClick Exit/2');
    Exit;
  end;

  if (((AppDetails.CashSaleAutoTender) or (ItemAutoTenderActive)) and (not OnAccount) and (not (OnTable xor (GlbTable.SaleCategoryType = scDriveThru))) and
      (not DoingPhoneOrder) and (not DoingWaste)) then begin
    TLPayment := GetDue(True);
    FoundCashTenderLine := False;
    I := 0;    //so it's initialised and removes warning
    if ((TLPayment >= 0.005) or (TLPayment <= -0.005)) then begin // JEH 18/08/2001 Only do if no tend lines, if there are then treat as OK.
      for I := 0 to (TLList.Count - 1) do begin
        TL := TLList.Items[I];
        if ((TL.TLType = TLTCash) or (TLPayment < 0)) then begin
          FoundCashTenderLine := True;
          Break;
        end;
      end;

      TLType := TLTCash;
      TLTip := 0.00;
      if (TLPayment < 0) then begin
        TLChange := RoundToRounding(-TLPayment, RoundForTLType(TLTCash));
        TLTender := 0;
      end
      else begin
        TLChange := 0.00;
        TLTender := RoundToRounding(TLPayment, RoundForTLType(TLType));
      end;

      TenderLineType := GetTenderLineTypeRecord(TLType);
      if ((TenderLineType <> nil) and (TenderLineType.SurchargeItemID > 0)) then begin
        TLSurcharge := GetRoundedUpDown((((TLTender * TenderLineType.SurchargePercent) / 100) + TenderLineType.SurchargeFixed), 2);
        TLPayment := TLPayment + TLSurcharge;
      end
      else begin
        TLSurcharge := 0;
      end;

      TLRounding := TLPayment + TLChange - TLTender;

      if (FoundCashTenderLine) then begin
        TL := TLList.Items[I];
        TL.TLPayment := GetRoundedUpDown(TL.TLPayment + TLPayment, AppDetails.DecimalPlaces);
        TL.TLTender := GetRoundedUpDown(TL.TLTender + TLTender, AppDetails.DecimalPlaces);
        TL.TLRounding := GetRoundedUpDown(TL.TLRounding + TLRounding, AppDetails.DecimalPlaces);
        TL.TLChange := GetRoundedUpDown(TL.TLChange + TLChange, AppDetails.DecimalPlaces);
        TL.TLSurcharge := GetRoundedUpDown(TL.TLSurcharge + TLSurcharge, AppDetails.DecimalPlaces);

        LoadTLs(LAEdit);
      end
      else begin
        TL := TLList.NewTL;
        TL.TLTypeID := GetTLTID(TLType);
        TL.TLType := TLType;
        TL.TLNotes := '';

        TL.TLPayment := GetRoundedUpDown(TLPayment, AppDetails.DecimalPlaces);
        TL.TLTip := GetRoundedUpDown(TLTip, AppDetails.DecimalPlaces);
        TL.TLChange := GetRoundedUpDown(TLChange, AppDetails.DecimalPlaces);
        TL.TLSurcharge := GetRoundedUpDown(TLSurcharge, AppDetails.DecimalPlaces);
        TL.TLTender := GetRoundedUpDown(TLTender, AppDetails.DecimalPlaces);
        TL.TLRounding := GetRoundedUpDown(TLRounding, AppDetails.DecimalPlaces);

        if ((TenderLineType <> nil) and (TenderLineType.SurchargeItemID > 0)) then begin
          TL.SurchargeItemID := TenderLineType.SurchargeItemID;
          TL.SurchargeItemAbbrev := TenderLineType.SurchargeItemAbbrev;
          TL.SurchargePercent := TenderLineType.SurchargePercent;
          TL.SurchargeFixed := TenderLineType.SurchargeFixed;
          TL.SurchargeSalesTaxPercent := TenderLineType.SurchargeSalesTaxPercent;
          TL.SurchargeGLCode := TenderLineType.SurchargeGLCode;
          TL.SurchargeGLCode2 := TenderLineType.SurchargeGLCode2;
        end
        else begin
          TL.SurchargeItemID := 0;
        end;

        LoadTLs(LAAdd);
      end;
      AddedAutoTenderLine := True;
    end
    else begin
      AddedAutoTenderLine := False;
      TLTender := 0;
      TLChange := 0;
      TLSurcharge := 0;
      TLRounding := 0;
    end;

    if ((FDoingRefund) or (DoingWaste)) then begin
      FinishedSale := DoCashSaleRefund(True);
    end
    else begin
      FinishedSale := DoCashSale(True,1);
    end;
    if ((not FinishedSale) and (AddedAutoTenderLine or AppDetails.CashSaleRemoveTenderChange or (AppDetails.CashSaleCashOnlyTenders > 0))) then begin
      if ((FoundCashTenderLine) and (not AppDetails.CashSaleRemoveTenderChange)) then begin
        TL := TLList.Items[I];
        TL.TLPayment := GetRoundedUpDown(TL.TLPayment - TLPayment, AppDetails.DecimalPlaces);
        TL.TLTender := GetRoundedUpDown(TL.TLTender - TLTender, AppDetails.DecimalPlaces);
        TL.TLRounding := GetRoundedUpDown(TL.TLRounding - TLRounding, AppDetails.DecimalPlaces);
        TL.TLChange := GetRoundedUpDown(TL.TLChange - TLChange, AppDetails.DecimalPlaces);
        TL.TLSurcharge := GetRoundedUpDown(TL.TLSurcharge - TLSurcharge, AppDetails.DecimalPlaces);
        LoadTLs(LAEdit);
      end
      else begin
        DeleteTenderLine;     //Jon 5-10-2001
      end;
    end;
  end
  else begin
    if ((FDoingRefund) or (DoingWaste)) then begin
      DoCashSaleRefund(True);
    end
    else begin
      DoCashSale(True,2);
    end;
  end;

  if FinishedSale then
    ClearSelectedCustomerDetails;

  ResetStaffPromptTimeoutTimer;
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.cmdQuickCashClick end');
end;
{******************************************************************************}
function TformQuickSaleUser.ValidateGoodySale: Boolean;
const
  GoodyValFail = 'ValidateCashSale False/Goody - ';
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.ValidateGoodySale begin');
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
    //Check not doing a Refund
    if FDoingRefund then
    begin
      LogMessage(ltPaymentVerbose, GoodyValFail + sNoRefundForGoodyScan);
      ShowMsg(sNoRefundForGoodyScan);
      Exit;
    end;

    //Check don't have an Event selected
    if GlbEvent.EventID <> 0 then
    begin
      LogMessage(ltPaymentVerbose, GoodyValFail + sNoEventForGoodyScan);
      ShowMsg(sNoEventForGoodyScan);
      Exit;
    end;

    //Check not on a Tab
    if OnTable then
    begin
      LogMessage(ltPaymentVerbose, GoodyValFail + sNoGoodyScanWhileTabSelected);
      ShowMsg(sNoGoodyScanWhileTabSelected);
      Exit;
    end;

    //If in Goody Discounted Sale
    if InGoodyDiscountedSale then
    begin
      //check account discount equals Goody discount
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
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.ValidateGoodySale end, Result = True');
end;
{******************************************************************************}
function TformQuickSaleUser.ValidateCashSale(ShowTheError: Boolean): Boolean;
var
  I: Integer;
  PINEntered, CheckPayShortBy: Boolean;
  TL: TTL;
  TotalInvoice: Currency;
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.ValidateCashSale begin');
  Result := False;
  if NFManual and (not NFManualSecond) and (not NFEMode) then
  begin
    if not GetNFManualExtraDetails then
    begin
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/1 - Missing NF details');
      Exit;
    end;
  end;

  //if not a table order, cash sale must have a sale category
  //if (chkTable.State = cbUnChecked) and (TheSaleCategoryID = 0) then begin
  if ((not OnTable) and (GlbTable.SaleCategoryID = 0)) then begin
    if (ShowTheError) then begin
      ShowQuickMsg(sNoSaleCatSelected);
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/2 - No sale category');
    Exit;
  end;

  if (GlbEvent.EventID > 0) then
  begin
    SetGlbEvent(GlbEvent.EventID, True);

    if ((GlbEvent.Available - FHostSubsidy + FDiscountAmount) < 0) then
    begin
      ShowMsg(sEventSpendLimitReached);
      txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available - FHostSubsidy + FDiscountAmount);
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/3 - Event spend limit reached');
      Exit;
    end;
  end;

  if not ValidateGoodySale then
    Exit;

  //Moved this up higher so we trigger payment checks below RPC 13/11/13
  if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (LoyaltyReward.AvailableQty > 0)) then begin
    if (ShowConf(sFreeLoyaltyRewardNotUsed) = mrYes) then
    begin
      GlbAccount.BestLoyaltyRewardID := 0; //Clears reward
      CheckLoyaltyReward(True);
      CalculateDiscountSchemeItemDiscount(True);
      CalculateDiscount;// show discounts
      FOrigDiscountPercent := GlbAccount.AccountDiscount;
      CalculateToPay;
    end
    else begin;
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/4 - Reward not used');
      Exit;
    end;
  end;

  if (not (AppDetails.CashSaleAllowAccPayment and OnAccount)) then       //allow overpayment so it can be credited to the account
  begin
    if ((FStillDue < 0)) then begin
      if (ShowTheError) then begin
        ShowQuickMsg(Format(sPayOverBy, [FormatCurrencySign(Abs(FStillDue))]));
      end;
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/5 - Overpayment');
      Exit;
    //end else if (chkTable.State = cbUnChecked) and
    end;

    //unsure why we test this if FStillDue is >= 0,
    TotalInvoice := (FToPay - FDiscountAmount + FSurcharge);

    if ((GlbEvent.EventID > 0) and (GlbEvent.SubsidyID>0) and (GlbEvent.AccountID>0) and (not OnAccount)) then
      TotalInvoice := (FToPay + FSurcharge);

    if (TlList.TotalPayment > TotalInvoice) then
    begin
      if (ShowTheError) then begin
        ShowQuickMsg(Format(sPayOverBy, [FormatCurrencySign(Abs(TlList.TotalPayment - TotalInvoice))]));
      end;
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/6 - Overpayment');
      Exit;
    end;
  end;

  CheckPayShortBy := True;
  if (not FDoingRefund) then
  begin
    case GlbTable.SaleCategoryType of
      scStandard: CheckPayShortBy := (not OnTable);
      scDriveThru: CheckPayShortBy := ((OnTable) and (GlbTable.Uninvoiced > 0));
      scRunningTab: CheckPayShortBy := (AppDetails.OneBill) and ((TLList.Count > 0) or ((cmdOnAccount.Down) and (GlbTable.Uninvoiced > 0)));
    end;
  end;


  if (CheckPayShortBy and (FStillDue > 0) and (not OnAccount) and (not DoingWaste) and (not (DoingPhoneOrder and (TLList.Count <= 0)))) then
  begin
    if (ShowTheError) then
    begin
      ShowQuickMsg(Format(sPayShortBy, [FormatCurrencySign(FStillDue)]));
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/7 - Underpayment');
    Exit;
  end;

  {if (TLList.TenderTypeExists(TLTExternalProviders)) and (OnAccount and (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance))) then begin
    if (ShowTheError) then begin
      ShowPnlToPay(True);
      ShowQuickMsg(sExternalPaymentToBalance);
    end;
    Exit;
  end
  else }

  //AMANDA 19/10/00 - CANNOT have any TLs if only an order on an account
  //end else if (chkTable.State = cbChecked) and (TLList.Count > 0) then begin
  if ((OnTable xor (GlbTable.SaleCategoryType = scDriveThru)) and (not (GlbTable.SaleCategoryType = scRunningTab)) and (TLList.Count > 0)) then begin
    if (ShowTheError) then begin
      ShowPnlToPay(True);      //Jon 4-9-2001
      ShowMsg(sNoOrderAcctTillTenderRemove);
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/8 - Remove tender lines to pay on account');
    Exit;
  end;

  if ((not FDoingRefund) and (OnAccount) and (GlbAccount.CreditLimitActive)) then begin    //Jon 22-10-2003
    if (not (OnTable xor (GlbTable.SaleCategoryType = scDriveThru)) and (GlbTable.SaleCategoryType <> scRunningTab)) then begin
      if (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance)) then begin
        Result := False;
        ShowMsg(Format(sCannotChargeCust, [FormatCurrencySign(FStillDue), FormatCurrencySign(-GlbAccount.CurrentBalance), FormatCurrencySign(GlbAccount.CreditLimit)]));
        LogMessage(ltPaymentVerbose, 'ValidateCashSale False/9 - Credit limit exceeded');
        Exit;
      end;
    end;
  end;

  // Test for both order lines and surcharge items before throw no items error
  if ((OLList.Count = 0) and (SurchargeList.Count = 0) and (not ((TLList.TenderTypeExists(TLTExternalProviders) and (TLList.TotalPayment = 0)) or
     (AppDetails.CashSaleAllowAccPayment and OnAccount)))) then
  begin
    if (ShowTheError) then begin
      ShowPnlToPay(True);
      ShowQuickMsg(sNoSaleItems);
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/10 - No items selected');
    Exit;
  end;

  // jeh 4/07/2002 Moved from top of procedure
  //akm 26.11.01 - if ForceScan and staff has not yet scanned, cannot continue                              TformQuickSaleUser.cmdDiscountPercentClic
  if (AppDetails.ForceStaffCardSwipe and (not GlbScan.SwipedStaffCard)) then begin
    if (ShowTheError) then begin
      ShowQuickMsg(sSwipeStaffCard);
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/11 - Staff card not swiped');
    Exit;
  end;

  if (GlbLogin.TillID = 0) then begin
    if (ShowTheError) then begin
      ShowQuickMsg(sLogonToTill);
    end;
    LogMessage(ltPaymentVerbose, 'ValidateCashSale False/12 - Not logged on to till');
    Exit;
  end;

  PINEntered := False;

  //AMANDA 21.02.01
  if (AppDetails.ProveStaffCashSale) then
  begin
    if (ProvenStaff) then
    begin //if had to prove and have
      PINEntered := True;
    end
    else
    begin
      //if need to prove staff and haven't yet
      if (VerifyPinNo) then begin
        ProvenStaff := True;
        PINEntered := True;
      end
      else begin
        LogMessage(ltPaymentVerbose, 'ValidateCashSale False/13 - Not verified PIN');
        Exit;
      end;
    end;
  end;

  if (TLList.Count > 0) then begin
    //if any hospitality tender lines then ask for pinno
    for I := 0 to (TLList.Count - 1) do begin
      TL := TLList.Items[I];
      if (TL.TLType in [TLTVoucher, TLTOther, TLTGuestHosp, TLTStaffHosp, TLTManagerHosp]) then begin
        //check if have privilege
        if (not VerifyPriv(6)) then begin
          ShowMsg(sCannotCompleteCashSaleGetHelp);
          LogMessage(ltPaymentVerbose, 'ValidateCashSale False/14 - Need priviledge for hospitality');
          Exit;
        end;

        //verify pinno
        if (not GlbPrivs[6].AvoidPINCheck) then begin
          if (not PINEntered) then begin
            if (not VerifyPinNo) then begin
              LogMessage(ltPaymentVerbose, 'ValidateCashSale False/15 - Not verified PIN');
              Exit;
            end
            else begin
              PINEntered := True;
            end;
          end;
        end;

        Break;
      end;
    end;

    if (DoingWaste) then begin
      if (ShowTheError) then begin
        ShowQuickMsg(sTenderExistsClearAll);
      end;
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/16 - Must clear tenderlines for waste');
      Exit;
    end;
  end;

  //if ordering on a table account, check have right to access account
  if (OnTable) then begin
    if (not CheckTableAccess(PINEntered, True)) then begin
      LogMessage(ltPaymentVerbose, 'ValidateCashSale False/17 - Do not have priviledge to order on account');
      Exit;
    end;
  end;

  Result := True;
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.ValidateCashSale end, Result = True');
end;
{******************************************************************************}
function TformQuickSaleUser.DoCashSale(ShowTheError: Boolean; CalledFrom: Integer): Boolean;
//Returns True if passes, False if something didn't work out.
var
  ConfirmationResult: TModalResult;
  TransactionPassed, WriteEventLogError: Boolean;
  OrderType: TOrderType;
  LoginID: Integer;
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.DoCashSale begin, called from #' + IntToStr(CalledFrom));
  WriteEventLogError := True;
  Result := False;
  try
    DoingTransaction := True;
    if (not ValidateCashSale(True)) then begin
      LogMessage(ltPaymentVerbose, 'DoCashSale Exit/1');
      Exit;
    end;

    if ((AppDetails.CashSalePromptWhere) and (not PromptedWhere)) then begin
      cmdDestnClick(nil);
    end;

    if ((AppDetails.CashSaleEatInTakeawayPrompt) and (not DoingPhoneOrder) and (not OnTable)) then begin
      EatInTakeawayPromptValue := EatInTakeAwayPrompt(False, False);
      if (EatInTakeawayPromptValue = 0) then begin
        //Cancel button was pushed
        LogMessage(ltPaymentVerbose, 'DoCashSale Exit/2');
        Exit;
      end
      else if ((EatInTakeawayPromptValue = 3) and (AppDetails.CashSalePrintPhoneOrders = 2)) then begin
        PrintPhoneOrder := False;
        ConfirmationResult := ShowConfWithCancel(sPrintOrder);
        if (ConfirmationResult = mrCancel) then begin
          LogMessage(ltPaymentVerbose, 'DoCashSale Exit/3');
          Exit;
        end
        else if (ConfirmationResult = mrYes) then begin
          PrintPhoneOrder := True;
        end;
      end;
    end;

    if ((DoingPhoneOrder) and (AppDetails.PhoneOrderConfirmation)) then begin
      case EatInTakeawayPromptValue of
        1: OrderType := otPOEatIn;
        2, 4: OrderType := otPOPickup;
        5: OrderType := otPODelivery;
      else
        LogMessage(ltPaymentVerbose, 'DoCashSale Exit/4');
        Exit;
      end;
      // Copy Notes to Delivery to allow user to edit...
      if (OrderType <> otPODelivery) then
      begin
        GlbCustomer.DeliveryNotes := FTabNotes;
      end;
      if (ViewOrderLines(OrderType, GetItemCount, grdOLs) <> volaSend) then begin
        // Return notes back....
        if (OrderType <> otPODelivery) then
        begin
          FTabNotes := GlbCustomer.DeliveryNotes;
        end;
        LogMessage(ltPaymentVerbose, 'DoCashSale Exit/5');
        Exit;
      end;
      // Move Notes back and clear Delivery Notes
      if (OrderType <> otPODelivery) then
      begin
        FTabNotes := GlbCustomer.DeliveryNotes;
        GlbCustomer.DeliveryNotes := '';
      end;
    end;

    if ((DoingPhoneOrder) and (EatInTakeawayPromptValue = 5) and (FStillDue >= 0.005)) then begin
      //We're doing a delivery order, so we need the desired tendertype
      FormAlphaBlend.ShowAlphaBlend;
      IntendedTenderTypeID := SelectTenderType(sIntendedTender, False, False, True, IntendedTenderTypeProviderID);
      FormAlphaBlend.HideAlphaBlend;
      if (IntendedTenderTypeID = 0) then
      begin
        LogMessage(ltPaymentVerbose, 'DoCashSale Exit/6');
        Exit;
      end;
    end
    else begin
      IntendedTenderTypeID := 0;
      IntendedTenderTypeProviderID := 0;
    end;

    SurplusOnAccount := False;
    if ((AppDetails.PaymentAllowChangeOnAccount) and (FChange > 0) and (OnAccount) and (GlbAccount.AccountID <> 0) and (GlbAccount.AllowCredit)) then begin
      ConfirmationResult := ShowConfWithCancel(Format(sCreditChangeToAcct, [FormatCurrencySign(FChange)]));
      if (ConfirmationResult = mrCancel) then begin
        LogMessage(ltPaymentVerbose, 'DoCashSale Exit/7');
        Exit;
      end
      else if (ConfirmationResult = mrYes) then begin
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

    if (AppDetails.CashSaleCombineOLs) then begin
      CombineOrderLines;
    end;

    if ((DoingPhoneOrder) and (TestRemoteLocationOption(rlsoMaster)) and (AppDetails.PhoneOrderUseRemLoc)) then begin
      TransactionPassed := TryRemotePhoneOrder(True);
    end
    else begin
      TransactionPassed := TryCashSale(True);
      WriteEventLogError := False; //Don't write to event log if trycashsale returns true or false, as if false, already done
    end;

    if (TransactionPassed) then
    begin
      //Try finalise any wiGroup tenders
      if AppDetails.wiGroupEnabled then
        wiGroupPOSSrv.CheckAdviseFinaliseTLs;

      ShowPnlToPay(True);
      ClearSelectedCustomerDetails;

      //Do CashCamm Doofrey if needed
      if (AppDetails.CashCamEnabled) then begin
        QuickSaleCashCam;
      end;

      Result := True;
      ClearCDAMessage;
      ResetCashSale(True, False);

      if CDAServer.Connected then
      begin
        CDAServer.CDAData.onetapFunction := ofShowChange;
        CDAServer.CDAData.Change := StrToFloatDef(StripCommas(txtCChange.Caption), 0);
        CDAServer.SendCDAMessage;
      end;

      if (AppDetails.CashSaleStaffPromptTimeout = 0) or
          (AppDetails.CashSaleStaffPromptTimeout >= 86400) or
          (GlbLogin.AutoClockedOut) then
      begin
        LoginID := GlbLogin.LoginID;
        if (SelectName(True, True, AppDetails.ProveStaffCashSale)) then begin
          InitialiseAccountInfo;
          AddStaffLoginButton;
          if ((AppDetails.StaffAutoParkSale > 0) and (LoginID <> GlbLogin.LoginID) and (SelectParkedSale(True))) then begin
            TryLoadParkedSale;
          end;
          GlbLogin.AutoClockedOut := False;
        end
        else begin
          cmdHideClick(nil);
          LogMessage(ltPaymentVerbose, 'DoCashSale Exit/8 (Note: Not an error)');
          Exit; //This is not an error
        end;
      end
      else if (AppDetails.CashSaleStaffPromptTimeout = -1)  then
      begin
        InitialiseAccountInfo;
      end
      else
      begin
        StaffPromptTimeoutTimer.Interval := (AppDetails.CashSaleStaffPromptTimeout * 1000);
        StaffPromptTimeoutTimer.Enabled := True;
        StaffPromptTimeoutTimerActive := True;
      end;

      SetCurrentTime;
      DoPhoneOrder(PhoneOrderPersistant, PhoneOrderPersistant);

      if (GlbEvent.EventID > 0) then
      begin
        lblEventName.Caption := '  ' + GlbEvent.EventName;
        if (GlbEvent.AccountID > 0) and (not LoadEventAccount) then
        begin
          DeselectEvent;
        end;
      end;
      if (AppDetails.EnableFiscalPrinting and CheckFiscalPrinterFunctions and AppDetails.FiscalIsCFPRequired) then
      begin
        FCPFNumber := GetCPFNumber;
      end;
    end;
  finally
    DoingTransaction := False;
    if (not Result and WriteEventLogError) then
      Write2EventLog(eiDoCashSale, rsEDDoCashSale, 'DoCashSale Returned False');
    LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.DoCashSale end, Result = ' + BoolToStr(Result, True));
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.DoCashSaleRefund(ShowTheError: Boolean): Boolean;
//Returns True if passes, False if something didn't work out.
var
  LoginID: Integer;
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.DoCashSaleRefund begin');
  try
    DoingTransaction := True;

    //Do a priviledge thing here or somewhere
    //if login pin not already entered for account priviledges
    Result := False;

    if (DoingWaste) then begin
      //check if have privilege
      if (not VerifyPriv(2)) then begin
        ShowMsg(sGetHelpToVoid);
        Exit;
      end;
    end
    else begin
      //check if have privilege
      if (not VerifyPriv(17)) then begin
        ShowMsg(sGetHelpToRefundCashSale);
        Exit;
      end;
    end;

    //verify pinno
    if ((GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
      Exit;
    end;

    if (not ValidateCashSale(True)) then begin
      Exit;
    end;

    if (TryCashSaleRefund(True)) then
    begin
      ShowPnlToPay(True);

      //Do CashCamm Doofrey if needed
      if (AppDetails.CashCamEnabled) then begin
        //Deal with this to get it to do a refund instead of a purchase
        QuickSaleCashCam;
      end;

      Result := True;
      ResetCashSale(True, False);

      case (AppDetails.CashSaleStaffPromptTimeout) of
        -1: begin
          InitialiseAccountInfo;
        end;
        1..86400: begin
          StaffPromptTimeoutTimer.Interval := (AppDetails.CashSaleStaffPromptTimeout * 1000);
          StaffPromptTimeoutTimer.Enabled := True;
          StaffPromptTimeoutTimerActive := True;
        end;
        else begin
          LoginID := GlbLogin.LoginID;
          if SelectName(True, True, AppDetails.ProveStaffCashSale) then begin
            InitialiseAccountInfo;
            AddStaffLoginButton;
            if ((AppDetails.StaffAutoParkSale > 0) and (LoginID <> GlbLogin.LoginID) and (SelectParkedSale(True))) then begin
              TryLoadParkedSale;
            end;
          end
          else begin
            cmdHideClick(nil);
          end;
        end
      end;

      SetCurrentTime;
      DoPhoneOrder(PhoneOrderPersistant, PhoneOrderPersistant);
      if (AppDetails.EnableFiscalPrinting and CheckFiscalPrinterFunctions and ShowCPFNUmberInActivate and AppDetails.FiscalIsCFPRequired) then
      begin
        FCPFNumber := GetCPFNumber;
      end;
    end;
  finally
    DoingTransaction := False;
    LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.DoCashSaleRefund end, Result = ' + BoolToStr(Result, True));
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.AllowTendering: Boolean;
begin
  Result := (not ((DoingWaste) or (DoingPhoneOrder) or (((OnTable xor (GlbTable.SaleCategoryType = scDriveThru)) and (GlbTable.SaleCategoryType <> scRunningTab)))));
end;

{******************************************************************************}
function TformQuickSaleUser.TryCashSale(Print: Boolean): Boolean;
var
  I, J: Integer;
  aOL: TOrderLine;
  VoidOL: TOrderLine;
  ModOL: TOrderLine;
  TL: TTL;
  aSI: TSurchargeItem;
  TheVoidID: Integer;
  TheOrderID: Integer;
  ValueFound: Boolean;
  FoodExists: Boolean;
  TrickGuestCount: Boolean;

  TheInvoiceID, TheInvoiceNo: Integer;
  TheInvoiceDate: TDateTime;
  TheTenderID: Integer;

  SoFarDiscount, SoFarComboDiscount, ILDiscount: Currency;

  TheLine: Integer;

  TransCommitted: Boolean;
  PrevOnTable: Boolean;

  BalanceDelta: Currency;
  LibExists: Boolean;
  NZAExists: Boolean;
  EvolutionExists: Boolean;
  FidelioExists: Boolean;
  PhoenixExists: Boolean;
  Micros4700Exists: Boolean;
  AgoraExists: Boolean;
  RMSExists: Boolean;
  RoomChargeExists: Boolean;
  MainMealCount: Integer;
  iGuestCountUsed: Integer; //only set when edit_tableguests executed

  DriveThroughNewOLsExist: Boolean;
  DriveThroughPayment: Boolean;

  TheModalResult: TModalResult;
  ToWaste: Boolean;

  OrderPrinterList: TIntegerList;
  VoidPrinterList: TIntegerList;
  TheSP: TIB_StoredProc;
  GenTR, TheTR: TIB_Transaction;
  SPNumber: Integer;

  AddOnDiscount: Boolean;       // Used for when we need to add the discount back to the ILAmount because we not paying off full item
  StatusString: string;
  aPromoMessage: string; // for fiscal printing
  FiscalTLList: TTLList;
  TenderLine: TTenderLineType;
  aTotalTip: Currency;
  aTotalTender: Currency;
  NYHappyHourOL: TNYHappyHourOL;
  FiscalPaymentInitialized: Boolean;
  NFEItem: TNFEItem;
  aHash: string;
  CupomCOO: string;
  TEFReportData: TStringList;
  iOldTenderID, iRefundTenderID, iRefundInvoiceID: Integer;
  OrgInvCOO, aFiscalNumero: string;
  PrintOK, ResetFlags: Boolean;
  OrderLineOrder: Integer;
  aCompleteFiscal: Boolean;   //Used to do all of the fiscal commands here.
  extAccountsMsg: string;
  tmpItemGroup: TItemGroup;
begin
  LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.TryCashSale begin');

  FiscalPaymentInitialized := False;
  SFiscalPrinting.FiscalOpTryAgainCommand := False;

  SFiscalPrinting.SupressFiscalErrorMessage := False;
  if SWBEncryption.AuxiliaryData.TEFDone = '1' then
    SFiscalPrinting.SupressFiscalErrorMessage := True;

  OrderPrinterList := TIntegerList.Create;
  OrderPrinterList.AutoSort := False;
  OrderPrinterList.AllowDuplicates := False;
  VoidPrinterList := TIntegerList.Create;
  VoidPrinterList.AutoSort := False;
  VoidPrinterList.AllowDuplicates := False;
  FiscalTLList := TTLList.Create;

  if (AppDetails.OrdersLockWait) then begin
    SPNumber := 3;
  end
  else begin
    SPNumber := 1;
  end;

  TheSP := dm.GetSP(SPNumber);
  TheTR := dm.GetTR(SPNumber);

  GenTR := DM.qrGeneral.IB_Transaction;
  DM.qrGeneral.IB_Transaction := TheTR;

  DiscCalculator.SPNumber := SPNumber;
  DiscCalculator.TheSP := TheSP;
  DiscCalculator.TheTR := TheTR;

  TransCommitted := False;
  PrevOnTable := OnTable;
  Result := False;

  try
    if (not OnTable) then begin
      while (RSC.ReasonAlreadyOrdered and not UpdateSwipeCardAfterReplacement) do
      begin
        if ((not RSC.CancelPressed) and
            ((RSC.CardCode = '') or
             ((not RSC.AllowUpdate) and (ShowConf(sTryAnotherCardCode) = mrYes)))) then
        begin
          if (SelectReplacementSwipeCard(@RSC)) then begin
            if (not RSC.ReasonAlreadyOrdered) then
              OrderItem(RSC.ItemID);
            RSC.AllowUpdate := True;
            RSC.ReasonAlreadyOrdered := True;
          end;
        end
        else
        begin
          RSC.AllowUpdate := True;
          RSC.CancelPressed := False;
          ShowMsg(sMustEnterNewCardCodeToCompleteSale);
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/1');
          Exit;
        end;
      end;
    end;

    //Using BalanceDelta to deal with half payments of items, it also is the on account balance
    GlobalMenuList.ListNYHappyHourOL.Clear;
    BalanceDelta := 0.00;
    TheOrderID := 0;
    TheInvoiceID := 0;
    TheTenderID := 0;
    TheVoidID := 0;
    OrderLineOrder := 0;
    FoodExists := False;
    TrickGuestCount := False;

    DriveThroughNewOLsExist := False;
    DriveThroughPayment := False;
    DoSpecialNYHappyHour := True;
    aCompleteFiscal := False;

    if (GlbTable.SaleCategoryType >= scDriveThru) then
    begin
      if ((TLList.Count > 0) or (cmdOnAccount.Down and (GlbTable.Uninvoiced > 0))) then
      begin
        DriveThroughPayment := True;
        aCompleteFiscal := (AppDetails.EnableFiscalPrinting and (not (NFManual or NFEMode)) and (not SFiscalPrinting.OpenedFiscalInvoice) and (not DoingPhoneOrder));
      end;

      for I := 0 to (OLList.Count - 1) do begin
        if (not TOrderLine(OLList.Items[I]).AlreadyOrdered) then begin
          DriveThroughNewOLsExist := True;
          Break;
        end;
      end;
    end
    else
    begin
      // Check that we didn't need to check the account state, or number of Tender Lines like we did above.
      aCompleteFiscal := (AppDetails.EnableFiscalPrinting and (not AppDetails.FiscalLiveInvoice) and (not (NFManual or NFEMode)) and (not SFiscalPrinting.OpenedFiscalInvoice) and (not DoingPhoneOrder));
    end;

    //if ON A TABLE, if there is a limit, if have exceeded available, only continue with confirmation
    if ((OnTable) and (GlbTable.SpendLimitActive)) then
    begin
      if (FAvail < 0.00) then
      begin
        if (((AppDetails.PaymentPreAuthForceSpendLimit) and (AppDetails.EFTPOSEnablePreAuth) and (GlbTable.EFTPOSTxnRef <> '')) or
            (AppDetails.PaymentForceSpendLimit)) then
        begin
          ShowQuickMsg(sLimitExceededCannotContinue);
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/2');
          Exit;
        end
        else if (((AppDetails.PaymentSpendLimitPrompt = 1) or
                  ((AppDetails.PaymentSpendLimitPrompt = 2) and (GlbTable.Avail >= 0))) and
                 (ShowConf(sLimitExceededContinue) <> mrYes)) then
        begin
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/3');
          Exit;
        end;
      end;
    end;

    ToWaste := False;
    if (((DriveThroughPayment) or (GlbTable.SaleCategoryType = scRunningTab)) and (VoidOLList.Count > 0)) then
    begin
      TheModalResult := ShowWastage;
      if (TheModalResult = mrYes) then begin
        ToWaste := True;
      end
      else if (TheModalResult = mrNo) then begin
        ToWaste := False;
      end
      else begin
        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/4');
        Exit;
      end;
    end;

    NewGuestCount := 0;
    iGuestCountUsed := 0;
    if ((OnTable) and (GlbTable.RequireGuests)) then
    begin
      MainMealCount := 0;
      for I := 0 to (OLList.Count - 1) do begin
        aOL := OLList.Items[I];
        if ((aOL.CourseIsMain) and (aOL.ItemID <> 0)) then begin
          MainMealCount := MainMealCount + Ceil(aOL.Qty);
        end;
      end;
      if (MainMealCount > GlbTable.Guests) then begin
        NewGuestCount := MainMealCount;
        if (not GetNewGuests(NewGuestCount)) then begin
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/5');
          Exit;
        end;
      end;
    end
    else if (AppDetails.CashSaleGuestCount in [1, 2]) then begin
      if (AppDetails.CashSaleGuestCount = 1) then begin
        NewGuestCount := 1;
      end;
      if (not GetNewGuests(NewGuestCount)) then begin
        Result := False;
        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/6');
        Exit;
      end;
      if ((AppDetails.CashSaleGuestCount = 1) and (NewGuestCount <= 0)) then begin
        Result := False;
        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/7');
        Exit;
      end;
    end;

    if (AppDetails.CashSaleAllowZeroPriceInvoices) then begin
      ValueFound := True;
    end
    else
    begin
      //check that the invoice will be greater than zero, else don't create one
      ValueFound := False;
      if ((not OnTable) or (DriveThroughPayment)) then begin
        if (TLList.Count > 0) then begin
          ValueFound := True;
        end
        else begin
          for I := 0 to (OLList.Count - 1) do begin
            if (TOrderLine(OLList[I]).ToPay > 0) then begin
              ValueFound := True;
              Break;
            end;
          end;
        end;
      end;
    end;
    aFiscalNumero := '';
    if (AppDetails.EnableFiscalPrinting and NFManualSecond and (not InputQuery(sFiscalInvCaption, sFiscalInvPrompt, aFiscalNumero))) then
    begin
      Result := False;
      LogMessage(ltPaymentVerbose, 'TryCashSale Exit/8');
      Exit;
    end;

    //loop through order lines
    for I := 0 to (OLList.Count - 1) do begin
      aOL := OLList.Items[I];
      //for each order line loop through walist twice, first for printer, then for ccprinter
      if ((aOL.PrinterID <> 0) and (not aOL.ModsChangePrinter) and ((not aOL.AlreadyOrdered) or (aOL.Qty > aOL.AlreadyOrderedQty))) then begin
        AddPrinterToPrinterList(OrderPrinterList, aOL.PrinterID);

        //loop through walist, if repeat printer not found add
        if ((aOL.RepeatPrinterID > 0) and (not AppDetails.CashSaleDisableRepeatPrinter)) then begin
          AddPrinterToPrinterList(OrderPrinterList, aOL.RepeatPrinterID);
        end;
      end;
    end;
    AddCCPrintersToPrinterList(OrderPrinterList);

    //if not on table, will be printing, and no destination specified, set to WAName
    if ((not OnTable) and (OrderPrinterList.Count > 0) and (FDelivery = '')) then begin
      FDelivery := AppDetails.WAName;
    end;

    Result := True;
    SetStatus(sSaving);

    //if there are tender lines, kick cash draw ASAP
    if (TLList.OpenDrawerExists(not SurplusOnAccount)) or
        ((AppDetails.CashSaleAllowZeroPriceInvoices) and (AppDetails.CashSaleZPIOpenCashDrawer) and (TLList.Count = 0)) then
    begin
      if not AppDetails.EnableFiscalPrinting then
        KickCashDraw(GlbLogin.TillDriverName, GlbLogin.TillDrawKickString);
    end;

    try
      if (not dm.TRStartTest(SPNumber)) then begin
        Result := False; //trycashsale would have incorrectly returned true otherwise
        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/9');
        Exit;
      end;

      with TheSP do
      begin
        if (AppDetails.OrdersLockWait) then
        begin
          SetStoredProcName('SETORDERMUTEX', SPNumber);
          I := 0;
          repeat
            Inc(I);
            ExecProc;
          until((FieldByName('errorcode').IsNull) or (I >= 3));

          if (not FieldByName('errorcode').IsNull) then begin
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
          end;
        end;

        if (DoingPhoneOrder) then
        begin //We need to open it and order to that instead
          SetStoredProcName('OPENPHONEORDERTAB', SPNumber);
          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          case EatInTakeawayPromptValue of
            1: ParamByName('sectionid').AsInteger := AppDetails.EatInSectionID;
            4: ParamByName('sectionid').AsInteger := AppDetails.PickupSectionID;
            5: ParamByName('sectionid').AsInteger := AppDetails.DeliverySectionID;
          end;
          ParamNull(ParamByName('phoneordersection'));

          ParamByName('groupname').AsString := Copy(GlbCustomer.CustomerName, 1, 30);
          if (FTabNotes = '') then
          begin
            StringAsParam(ParamByName('notes'), FDelivery);
          end
          else
          begin
            StringAsParam(ParamByName('notes'), FTabNotes);
          end;
          ParamByName('perorgid').AsInteger := GlbCustomer.PerorgID;
          IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
          StringAsParam(ParamByName('deliveryname'), GlbCustomer.DeliveryName);
          DateAsParam(ParamByName('orderreadyfor'), GlbCustomer.DeliveryOrderReadyFor);
          StringAsParam(ParamByName('deliveryaddress1'), GlbCustomer.DeliveryAddress[1]);
          StringAsParam(ParamByName('deliveryaddress2'), GlbCustomer.DeliveryAddress[2]);
          StringAsParam(ParamByName('deliveryaddress3'), GlbCustomer.DeliveryAddress[3]);
          StringAsParam(ParamByName('deliveryphoneno'), GlbCustomer.DeliveryPhoneNo);
          StringAsParam(ParamByName('deliverynotes'), GlbCustomer.DeliveryNotes);
          StringAsParam(ParamByName('deliveryflatno'), GlbCustomer.DeliveryFlatNo);
          StringAsParam(ParamByName('deliverystreetno'), GlbCustomer.DeliveryStreetNo);
          IntegerAsParam(ParamByName('deladdressmaplocationid'), GlbCustomer.DeliveryAddressMapLocationID);
          IntegerAsParam(ParamByName('intendedtendertypeid'), IntendedTenderTypeID);
          IntegerAsParam(ParamByName('intendedtentypeproid'), IntendedTenderTypeProviderID);
          Execute;

          if (not FieldByName('errorcode').IsNull) then
          begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if ((AppDetails.EnableFiscalPrinting) and
                (not (NFManual or NFEMode)) and
                (not ((DoingPhoneOrder) or ((OnTable) and (GlbTable.GroupID > 0))))) then
            begin
              CancelCashSale(True);
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/10');
            Exit;
          end
          else
          begin
            GlbTable.GroupID := FieldByName('groupid').AsInteger;
            GlbTable.TableNo := FieldByName('tableno').AsInteger;
            OnTable := True;

            GlbTable.RequireGuests := False;

            if ((AppDetails.EnableFiscalPrinting) and (FieldByName('autoorderitemid').IsNotNull)) then
            begin
              SWBEncryption.Initialise;
              SWBEncryption.AddInt(FieldByName('autoorderitemid').AsInteger);
              SWBEncryption.AddStr(FieldByName('autoitemname').AsString);
              SWBEncryption.AddDouble(1);
              SWBEncryption.AddCurrency(FieldByName('unitprice').AsCurrency);
              SWBEncryption.AddCurrency(FieldByName('unitprice').AsFloat);
              SWBEncryption.AddDouble(0);
              aHash := SWBEncryption.GetHash;
              SFiscalPrinting.UpdateOrderLineChecksumToDb(aHash, FieldByName('orderlineid').AsInteger);
            end;
          end;
        end
        else if (((GlbTable.SaleCategoryType >= scDriveThru) or ((AppDetails.CashSaleGuestCount in [1, 2]) and (NewGuestCount > 0))) and (not OnTable)) then
        begin//We need to open it and order to that instead
          SetStoredProcName('OPENDRIVETHROUGHTAB', SPNumber);

          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;

          if (OLList.Count > 0) then
          begin
            ParamByName('groupname').AsString := Copy(Copy(TOrderLine(OLList.Items[0]).ItemString, 1, 20) + ' ' + FormatCurrencySign(FToPay), 1, 30);
          end
          else begin
            ParamNull(ParamByName('groupname'));
          end;

          if (FTabNotes = '') then
          begin
            StringAsParam(ParamByName('notes'), FDelivery);
          end
          else
          begin
            StringAsParam(ParamByName('notes'), FTabNotes);
          end;
          Execute;

          if (not FieldByName('errorcode').IsNull) then
          begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if ((AppDetails.EnableFiscalPrinting) and
                (not (NFManual or NFEMode)) and
                (not DoingPhoneOrder) and
                (not (OnTable and (GlbTable.GroupID > 0)))) then
            begin
              CancelCashSale(True);
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/11');
            Exit;
          end
          else
          begin
            GlbTable.GroupID := FieldByName('groupid').AsInteger;
            GlbTable.TableNo := FieldByName('tableno').AsInteger;
            OnTable := (GlbTable.SaleCategoryType >= scDriveThru);

            GlbTable.RequireGuests := ((AppDetails.CashSaleGuestCount in [1, 2])and (NewGuestCount > 0));
            TrickGuestCount := ((not OnTable) and ((AppDetails.CashSaleGuestCount in [1, 2])and (NewGuestCount > 0)));

            if ((AppDetails.EnableFiscalPrinting) and (FieldByName('autoorderitemid').IsNotNull)) then
            begin
              SWBEncryption.Initialise;
              SWBEncryption.AddInt(FieldByName('autoorderitemid').AsInteger);
              SWBEncryption.AddStr(FieldByName('autoitemname').AsString);
              SWBEncryption.AddDouble(1);
              SWBEncryption.AddCurrency(FieldByName('unitprice').AsCurrency);
              SWBEncryption.AddCurrency(FieldByName('unitprice').AsCurrency);
              SWBEncryption.AddDouble(0);
              aHash := SWBEncryption.GetHash;
              SFiscalPrinting.UpdateOrderLineChecksumToDb(aHash, FieldByName('orderlineid').AsInteger)
            end;
          end;
        end;

        //if (chkTable.State = cbChecked) then begin
        if (OnTable) then
        begin//check table still open
          SetStoredProcName('CHECK_CLOSED', SPNumber);  //check_closed
          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ExecProc;
          if (not FieldByName('whenclosed').IsNull) then begin
            TheTR.Rollback;
            ShowQuickMsg(sNoOrderSinceAcctClosed);
            if ((AppDetails.EnableFiscalPrinting) and
                (not (NFManual or NFEMode)) and
                (not DoingPhoneOrder) and
                (not (OnTable and (GlbTable.GroupID > 0)))) then
            begin
              CancelCashSale(True);
            end;
            Result := False;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/12');
            Exit;
          end;
        end;

        if (FDriveThroughGroupIDs.Count > 1) then
        begin
          //Merge all tabs to the current tab

          for I := 0 to (FDriveThroughGroupIDs.Count - 1) do
          begin
            if (FDriveThroughGroupIDs.Get(I) <> GlbTable.GroupID) then
            begin
              SetStoredProcName('MERGE_TABLE', SPNumber);
              ParamByName('fromgroupid').AsInteger := FDriveThroughGroupIDs.Get(I);
              ParamByName('togroupid').AsInteger := GlbTable.GroupID;
              ParamNull(ParamByName('newguests'));
              ParamByName('loginid').AsInteger := GlbLogin.LoginID;
              BoolAsParam(ParamByName('correction'), False);                                //Must stay as the same salecategoryid
              IntegerAsParam(ParamByName('fromsalecategoryid'), GlbTable.SaleCategoryID);   //Must stay as the same salecategoryid
              IntegerAsParam(ParamByName('tosalecategoryid'), GlbTable.SaleCategoryID);     //Must stay as the same salecategoryid
              BoolAsParam(ParamByName('printtransfers'), False);
              ParamNull(ParamByName('Notes'));
              ExecProc;

              if (not FieldByName('errorcode').IsNull) then begin
                Result := False;
                ShowErrorMsg(FieldByName('errorcode').AsInteger);
                TheTR.Rollback;
                if ((AppDetails.EnableFiscalPrinting) and
                    (not (NFManual or NFEMode)) and
                    (not DoingPhoneOrder) and
                    (not (OnTable and (GlbTable.GroupID > 0)))) then
                begin
                  CancelCashSale(True);
                end;
                LogMessage(ltPaymentVerbose, 'TryCashSale Exit/13');
                Exit;
              end;
            end;
          end;
        end;

        if ((OnTable or TrickGuestCount) and (GlbTable.RequireGuests) and (NewGuestCount > 0)) then
        begin
          SetStoredProcName('EDIT_TABLEGUESTS', SPNumber);

          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamByName('guests').AsInteger := NewGuestCount;
          iGuestCountUsed := NewGuestCount;
          ExecProc;
        end;

        if (AppDetails.SpecialNYHappyHour) then
        begin
          for I := 0 to (OLList.Count - 1) do begin
            aOL := OLList.Items[I];
            //if an order line as opposed to a modifier
            if ((aOL.ItemID > 0) and (not aOL.AlreadyOrdered)) then begin
              AddItemQtyToSpecialNYHappyHour(aOL.ItemID, aOL.Qty);
            end;
          end;

          //Checkes it not on account or ontable
          if ((OnAccount) or (OnTable)) then begin
            DoSpecialNYHappyHour := False;
          end
          else begin
            for I := 0 to (TLList.Count - 1) do begin
              if (TTL(TLList.Items[I]).TLTypeID <> GetTLTID(TLTCash)) then begin
                DoSpecialNYHappyHour := False;
                Break;
              end;
            end;
          end;

          if (DoSpecialNYHappyHour) then begin
            CombineOrderLines;
            for I := 0 to (OLList.Count - 1) do begin
              aOL := OLList.Items[I];
              if ((aOL.ItemID > 0) and ((aOL.Qty - aOL.LoyaltyFreeQty) > 0)) then begin
                NYHappyHourOL := TNYHappyHourOL.Create;
                NYHappyHourOL.Item := aOL.ItemString;
                NYHappyHourOL.Qty := aOL.Qty;
                NYHappyHourOL.UnitPrice := aOL.UnitPrice;
                NYHappyHourOL.Price := aOL.ToPay;
                NYHappyHourOL.ForB := aOL.FORB;
                NYHappyHourOL.SalesTaxPercent := aOL.SalesTaxPercent;
                GlobalMenuList.ListNYHappyHourOL.Add(NYHappyHourOL);
                LogSepicalNYHappyHour(aOL.ItemID, aOL.Qty, aOL.ToPay, aOL.FORB = 'F');
              end;
            end;

            GlbNYHappyHour.Total := FChange;
            PrintOrder(OrderPrinterList);
            if (AppDetails.PrintCashSale) then PrintInvoice;
            TheTR.Rollback; //don't save the transaction & Exit out of here
            //Here we show last receipt :) for JBM
            if (((not OnTable) or DriveThroughPayment) and (AppDetails.CashSaleShowLastReceipt)) then begin
              ShowLastReceipt;
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/14 Result: ' + BoolToStr(Result, True));
            Exit; //Note this returns true - assume OK for now.
          end; //DoSpecialNYHappyHour
        end; //SpecialNYHappyHour Enabled

        if ((not DriveThroughPayment) or (DriveThroughNewOLsExist) or (SurchargeList.Count > 0)) then
        begin
          //create order record
          SetStoredProcName('INSERT_ORDER', SPNumber);
          ParamByName('outletid').AsInteger := AppDetails.OutletID;

          IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, (OnTable or TrickGuestCount));

          ParamByName('rush').AsInteger := 0;
          StringAsParam(ParamByName('location'), FDelivery);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;

          //may be null from tables already open when Sale Categories introduced
          IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);

          //if (chkTable.State = cbChecked) then
          IntegerAsParamIfBool(ParamByName('tableno'), GlbTable.TableNo, OnTable);
          IntegerAsParamIfBool(ParamByName('eatintakeaway'), EatInTakeawayPromptValue, ((not OnTable) and (AppDetails.CashSaleEatInTakeawayPrompt or DoingPhoneOrder)));
          IntegerAsParam(ParamByName('heldgroupid'), FHeldGroupID);
          ParamByName('CPFNumber').AsString := Trim(FCPFNumber);
          ExecProc;

          TheOrderID := FieldByName('orderid').AsInteger;
          WBOrders.RefreshID := TheOrderID;
        end;

        //if a value found and not for a table
        //if ValueFound and (chkTable.State = cbUnChecked) then begin
        if (ValueFound and
           ((not OnTable) or (DriveThroughPayment) or
            (DoingPhoneOrder and ((TLList.Count > 0) or (OnAccount))))) then
        begin
          if (aCompleteFiscal) then
          begin
            if (not SFiscalPrinting.GetFiscalPrinterConfigurations) then
            begin
              TheTR.Rollback;
              Result := False;
              LogMessage(ltPaymentVerbose, 'TryCashSale Exit/15');
              Exit;
            end;

            FCreditCardDiscount := 0;
            ShowQuickMsgOnTop(sOpeningFiscalInvoice);
            if (not SFiscalPrinting.OpenFiscalPrinterInvoice(FCPFNumber, GlbAccount.PerorgName, GlbAccount.PerorgAddress)) then
            begin
              TheTR.Rollback;
              Result := False;
              LogMessage(ltPaymentVerbose, 'TryCashSale Exit/16');
              Exit;
            end;
            SFiscalPrinting.GetMovementDate;
            SWBEncryption.GenerateEncryptedAuxFile;
            SFiscalPrinting.GetCOOForInvoice; // retrive the last COO in SFiscalPrinting.FCOO . This has to be stored in invoice table
            SFiscalPrinting.GetCCFForInvoice;

            FInvCOO := SFiscalPrinting.COO;
            FInvCCF := SFiscalPrinting.CCF;
          end;

          //create invoice record
          SetStoredProcName('INSERT_INVOICE', SPNumber);  //insert_invoice

          ParamByName('discountrate').AsCurrency := FDiscountPercent;
          IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
          if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
            IntegerAsParam(ParamByName('accountid'), GlbEvent.AccountID);

          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, ((DriveThroughPayment) or (DoingPhoneOrder and (TLList.Count > 0)) or (TrickGuestCount)));

          //may be null from tables already open when Sale Categories introduced
          IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);
          ParamNull(ParamByName('refundnote'));
          ParamByName('invoicetype').AsString := 'I';
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamByName('salestax').AsCurrency := FSalesTax;
          IntegerAsParam(ParamByName('cashsaleorderid'), TheOrderID);
          StringAsParam(ParamByName('cpfnumber'), FCPFNumber);
          IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);

//          StringAsParam(ParamByName('REFERENCENO'), aZapaReferences);
//          StringAsParam(ParamByName('Notes'), aZapaNotes);
          if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          begin
            StringAsParam(ParamByName('FiscalCOO'), FInvCOO);
            StringAsParam(ParamByName('FiscalCCF'), FInvCCF);
          end;

          if AppDetails.EnableFiscalPrinting then
          begin
            SWBEncryption.Initialise;
            SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
            SWBEncryption.AddStr(FInvCOO);
            SWBEncryption.AddStr(FInvCCF);
            SWBEncryption.AddCurrency(FToPay - FDiscountAmount);
            SWBEncryption.AddCurrency(0);
            aHash := SWBEncryption.GetHash;
            StringAsParam(ParamByName('checksum'), aHash);
          end;

          ExecProc;
          if (not FieldByName('errorcode').IsNull) then
          begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if ((AppDetails.EnableFiscalPrinting) and
                 (not (NFManual or NFEMode)) and
                 (not DoingPhoneOrder) and
                 (not (OnTable and (GlbTable.GroupID > 0)))) then
            begin
              CancelCashSale(True);
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/17');
            Exit;
          end
          else
          begin
            TheInvoiceID := FieldByName('invoiceid').AsInteger;
            WBInvoices.RefreshID := TheInvoiceID;
            TheInvoiceNo := FieldByName('InvoiceNo').AsInteger;
            TheInvoiceDate := FieldByName('InvoiceDate').AsDateTime;
            if AppDetails.EnableFiscalPrinting then
            begin
              if ((aCompleteFiscal) or
                  ((not DoingPhoneOrder) and
                   (not (OnTable and (GlbTable.GroupID > 0))))) then
              begin
                SetStoredProcName('UPDATEFISCALDETAILS', SPNumber);
                ParamByName('MANUFACTURENO').AsString := GlbFiscal.ManufactureNo;
                ParamByName('ECFMODEL').AsString := GlbFiscal.ECFModel;
                ParamByName('ADDITIONALMF').AsString := GlbFiscal.MFAdditional;
                ParamByName('THEID').AsInteger := TheInvoiceID;
                ParamByName('TABLETYPE').AsInteger := Ord(fttInvoice); // INVOICE Table
                ParamByName('MANUALSERIES').AsString := '';
                ParamByName('MANUALSUBSERIES').AsString := '';
                ParamByName('FISCALINVNO').AsString := '';
                ParamByName('FISCALSEQNO').AsInteger := AppDetails.FiscalPrinterSeqNo;
                if NFManual or NFEMode then
                begin
                  ParamByName('MANUFACTURENO').AsString := '';
                  ParamByName('ECFMODEL').AsString := '';
                  ParamByName('ADDITIONALMF').AsString := '';
                end;
                if NFManual then
                begin
                  ParamByName('MANUALSERIES').AsString := FManualSeries;
                  ParamByName('MANUALSUBSERIES').AsString := FManualSubSeries;
                  ParamByName('FISCALINVNO').AsString := FManualInvNo;
                end;
                ParamByName('FISCALINVTYPE').AsString := '2D';
                if NFManual then
                  ParamByName('FISCALINVTYPE').AsString := '02'
                else if NFEMode then
                  ParamByName('FISCALINVTYPE').AsString := '55';

                ExecSQL;
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

            if ((AppDetails.ClearItemsVoid > 0) and (FVoidID > 0) and (TheInvoiceID > 0)) then
            begin
              with DM.qrGeneral do
              begin
                Close;
                SQL.Clear;
                SQL.Add('UPDATE VOIDLN SET');
                SQL.Add('       INVOICEID = :invoiceid');
                SQL.Add(' WHERE (VOIDID = :VOIDID)');
                ParamByName('invoiceid').AsInteger := TheInvoiceID;
                ParamByName('voidid').AsInteger := FVoidID;
                ExecSQL;
              end;
            end;
          end;
        end;

        //process External Accounts here
        try
          if (AllowTendering) then
          begin
            sExternalAccountIntegration.SPNumber := SPNumber;
            if (OnAccount and (TLList.TenderTypeExists(TLTExternalProviders) and (TLList.TotalPayment < 0))) then
            begin
              ShowMsg('Cannot Topup External Accounts with credit from onetap Customer Accounts');
              TheTR.Rollback;
              Result := False;
              LogMessage(ltPaymentVerbose, 'TryCashSale Exit/18');
              Exit;
            end
            else if (sExternalAccountIntegration.ActionPresentedCards(TheInvoiceID, TheInvoiceNo, OLList, SurchargeList, TLList)) then
            begin
              if (sExternalAccountIntegration.CanContinueSale) then
              begin
                //shall continue sale here as transaction to external accounts have been committed on their side, we just might not have a valid transaction number
                extAccountsMsg := sExternalAccountIntegration.ValidateProcessedCards;
                if ((extAccountsMsg <> '')) then
                begin
                  LogMessage(ltExternalAccounts, extAccountsMsg);
                end;
              end
              else
              begin
                sExternalAccountIntegration.CheckLoadedRedemptions;  //we failed completing transaction, ask to clear redemptions
                TheTR.Rollback;
                Result := False;
                LogMessage(ltPaymentVerbose, 'TryCashSale Exit/19');
                Exit;
              end;
            end
            else if (not sExternalAccountIntegration.CanContinueSale) then
            begin
              sExternalAccountIntegration.CheckLoadedRedemptions;  //we failed completing transaction, ask to clear redemptions
              TheTR.Rollback;
              Result := False;
              LogMessage(ltPaymentVerbose, 'TryCashSale Exit/20');
              Exit;
            end;
          end;
        except
          ShowMsg('Unexpected External Account Issue');
          LogMessage(ltExternalAccounts, 'Unexpected External Account Issue');
          TheTR.Rollback;
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/21');
          Exit;
        end;

        //for each cash sale line added
        //create order line, invoice line and the order line's modifier record
        SoFarDiscount := 0.000;
        SoFarComboDiscount := 0;

        if TheOrderID > 0 then
          UpdateOrderLines(TheOrderID, SPNumber, TheSP);

        //Allocate and apply wiGroup tender line discounts to OL list.
        //NB: Assumes no other discount is/will be applied to the selected order
        //lines to pay, EnableFiscalPrinting = False, no Event selected (no subsidy),
        //not used by a ROSS system, nor interfacing with other external parties
        //(e.g. Fidelio, Phoenix, Micros, NZA, Evolution, ICR Touch).
        if TLList.TenderTypeExists(TLTwiGroup) then
        begin
          if not (wiGroupPOSSrv.AllocateDiscount(OLList) and wiGroupPOSSrv.ApplyDiscount(OLList, FSalesTax)) then
          begin
            Result := False;
            ShowMsg(Format(swiGroupDiscAllocError, [swiGroupTL]));
            Exit;
          end
          else
          begin
            //NB: Update invoice with sales tax value once reduced by any wiGroup discount
            wiGroupPOSSrv.UpdInvSalesTax(TheInvoiceID, FSalesTax);
          end;
        end;

        if (DoingPhoneOrder or (GlbTable.GroupID > 0)) then
          UpdateTabDiscount;

        for I := 0 to (OLList.Count - 1) do
        begin
          aOL := OLList.Items[I];
          if (aOL.ItemID > 0) then begin
            //NB: if there's a wiGroup transaction, always allow zero price invoice lines, in case involved in a discount
            if (((aOL.ToPay > 0) or (AppDetails.CashSaleAllowZeroPriceInvoices) or (TLList.TenderTypeExists(TLTwiGroup)) or (sExternalAccountIntegration.OverrideDiscount)) and
              ((not OnTable) or (DriveThroughPayment) or (DoingPhoneOrder and (TLList.Count > 0)))) then begin
              if ((aOL.Qty - aOL.LoyaltyFreeQty) > 0) then begin
                //RPC 09/02/11 Using BalanceDelta for check if the everything been paid off, if still outstanding then we fix up records to match this (important for discounting)
                //This is a bit messy code, to explain if we have $4 item with 50% off, but if only $1 is to be paid we need to change the discount from $2 to $1 so we pay half of the item.
                if ((BalanceDelta <= FPayment) or  // till payment amount
                    (OnAccount) or                 // or if pay on account, do this for all
                    ((GlbEvent.EventID>0) and (GlbEvent.AccountID>0) and (GlbEvent.SubsidyID>0))) then // or if event, do this for all
                begin
                  SetStoredProcName('INSERT_INVOICELINE', SPNumber);
                  ParamByName('invoiceid').AsInteger := TheInvoiceID;
                  ParamByName('orderlineid').AsInteger := aOL.OrderLineID;
                  aOL.ILAmount := aOL.ToPay - aOL.LoyaltyFreeDiscount;

                  //NB: if wiGroup TLs exist, don't enter this loop, as aOL.ILDiscount has already been set above
                  if (not TLList.TenderTypeExists(TLTwiGroup)) and (not sExternalAccountIntegration.OverrideDiscount) then
                  begin
                    ILDiscount := 0.00;
                    //if (FDiscountAmount > 0) or (FComboDiscount > 0) then
                    begin
                      //if it's the last line, allocate whatever hasn't been pro rata-ed
                      if aOL.ComboID > 0 then
                      begin
                        if (aOL.LastComboItem) then
                        begin
                          ILDiscount := FComboDiscount - SoFarComboDiscount;
                        end
                        else
                        begin
                          if ((AppDetails.EnableFiscalPrinting) and (FUseDiscountAmount) and (aOL.ILAmount = aOL.AllowedDiscount)) then
                            aOL.AllowedDiscount := ((aOL.ILAmount * (FDiscountAmount - aOL.LoyaltyFreeDiscount)) / FToPay);

                          ILDiscount := aOL.AllowedDiscount;
                        end;

                        ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                        SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
                        SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
                      end
                      else if (FDiscountAmount- aOL.LoyaltyFreeDiscount > 0) then
                      begin
                        if ((aOL.LastNormalItem) and ((OnAccount) or (FStillDue = 0))) then begin
                          ILDiscount := FDiscountAmount - aOL.LoyaltyFreeDiscount - SoFarDiscount;
                          SoFarDiscount := SoFarDiscount + ILDiscount;
                          SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
                        end
                        else
                        begin
                          //pro rata discount and accumulate discount pro rata-ed so far
                          if ((AppDetails.EnableFiscalPrinting) and (FUseDiscountAmount) and (aOL.ILAmount = aOL.AllowedDiscount)) then
                          begin
                            aOL.AllowedDiscount := ((aOL.ILAmount * (FDiscountAmount- aOL.LoyaltyFreeDiscount)) / FToPay);
                            ILDiscount := aOL.AllowedDiscount;
                          end
                          else
                          begin
                            ILDiscount := aOL.AllowedDiscount - aOL.LoyaltyFreeDiscount;
                          end;

                          ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                          SoFarDiscount := SoFarDiscount + ILDiscount;
                          SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
                        end;
                      end;
                    end;
                    ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                    aOL.ILDiscount := ILDiscount;
                  end;

                  // If we don't have enough money to pay off the item in full we going recal it ilamount & discount.
                  //NB: if wiGroup TLs exist, don't recalc, as aOL.ILDiscount has already been set correctly above
                  if ((not (OnAccount or ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (GlbEvent.SubsidyID > 0)))) and
                      (FPayment <> 0) and
                      ((BalanceDelta + (aOL.ILAmount - aOL.ILDiscount) - FPayment) > 0.005) and
                      (not TLList.TenderTypeExists(TLTwiGroup)) and
                      (not sExternalAccountIntegration.OverrideDiscount)) then
                  begin
                    ParamByName('ilamount').AsCurrency := GetRoundedUpDown(FPayment - BalanceDelta + (aOL.ILDiscount * (FPayment - BalanceDelta)/(aOL.ILAmount - aOL.ILDiscount)), AppDetails.DecimalPlaces);
                    ParamByName('discountamount').AsCurrency := GetRoundedUpDown(aOL.ILDiscount * (FPayment - BalanceDelta)/(aOL.ILAmount - aOL.ILDiscount), AppDetails.DecimalPlaces);
                  end
                  else
                  begin
                    ParamByName('ilamount').AsCurrency := aOL.ILAmount;
                    ParamByName('discountamount').AsCurrency := aOL.ILDiscount;
                  end;
                  if (aOL.DiscountSchemeID > 0) then
                    IntegerAsParam(ParamByName('discountschemeid'), aOL.DiscountSchemeID)
                  else
                    IntegerAsParam(ParamByName('discountschemeid'), GlbAccount.DiscountSchemeID);

                  if (AppDetails.TaxExclusivePrices) then
                    BalanceDelta := BalanceDelta + GetRoundedUpDown((aOL.ILAmount - aOL.ILDiscount) * (100 + aOL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces)
                  else
                    BalanceDelta := BalanceDelta + GetRoundedUpDown(aOL.ILAmount - aOL.ILDiscount, AppDetails.DecimalPlaces);

                  if AppDetails.EnableFiscalPrinting then
                  begin
                    if (aCompleteFiscal) then
                    begin
                      if (not AddingComboItems) then
                      begin
                        SendOrderLineToFiscalPrinter(aOL, I);
                      end;
                    end;

                    SWBEncryption.Initialise;
                    SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
                    SWBEncryption.AddStr(FInvCOO);
                    SWBEncryption.AddStr(FInvCCF);
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
                    ParamByName('FISCALTAX').AsInteger := 1;
                    ParamByName('FISCALSEQNO').AsString := aOL.FiscalIndex;
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

              if (aOL.LoyaltyFreeQty > 0) then
              begin
                SetStoredProcName('INSERT_INVOICELINE', SPNumber);
                ParamByName('invoiceid').AsInteger := TheInvoiceID;
                ParamByName('orderlineid').AsInteger := aOL.OrderLineID;

                aOL.ILAmount := aOL.LoyaltyFreeDiscount;
                ParamByName('ilamount').AsFloat := aOL.ILAmount;

                aOL.ILDiscount := aOL.LoyaltyFreeDiscount;
                ParamByName('discountamount').AsCurrency := aOL.ILDiscount;

                ParamNull(ParamByName('discountschemeid'));

                if AppDetails.EnableFiscalPrinting then
                begin
                  SWBEncryption.Initialise;
                  SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
                  SWBEncryption.AddStr(FInvCOO);
                  SWBEncryption.AddStr(FInvCCF);
                  SWBEncryption.AddInt(aOL.ItemID);
                  SWBEncryption.AddStr(aOL.ItemString);
                  SWBEncryption.AddStr(aOL.TaxSituation);
                  SWBEncryption.AddStr(FloatToStr(aOL.SalesTaxPercent));
                  SWBEncryption.AddCurrency(aOL.Qty);
                  SWBEncryption.AddCurrency(aOL.UnitPrice);
                  SWBEncryption.AddCurrency(ParamByName('ilamount').AsFloat - ParamByName('discountamount').AsFloat);
                  SWBEncryption.AddCurrency(0);
                  aHash := SWBEncryption.GetHash;
                  ParamByName('CHECKSUM').AsString := aHash;
                  ParamByName('FISCALTAX').AsInteger := 1;
                  ParamByName('FISCALSEQNO').AsString := '';
                end;
                // for Fiscal Printing - Open an Invoice in the Fiscal Printer
                if ((aCompleteFiscal) or
                    ((AppDetails.EnableFiscalPrinting) and
                     (not (NFManual or NFEMode)) and
                     (not DoingPhoneOrder) and
                     (not (OnTable and (GlbTable.GroupID > 0))))) then
                begin
                  if (not SFiscalPrinting.OpenedFiscalInvoice) then
                  begin
                    TheTR.Rollback;
                    Result := False;
                    LogMessage(ltPaymentVerbose, 'TryCashSale Exit/22');
                    Exit;
                  end
                  else
                  begin
                    if (not AddingComboItems) then
                    begin
                      SendOrderLineToFiscalPrinter(aOL, grdOLs.Row);
                    end;
                  end;
                end;
                ExecProc;
                aOL.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;
                SoFarDiscount := SoFarDiscount + aOL.ILDiscount;
                SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);

                if (AppDetails.FiscalType = fiscalTaxCore) then
                begin
                  SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
                  ParamByName('invoicelineid').AsInteger := aOL.InvoiceLineID;
                  StringAsParam(ParamByName('taxlabel'), aOL.TaxCoreLabel);
                  ExecProc;
              end;
              end;

              //set foodexists
              if (aOL.FORB = 'F') then
              begin
                FoodExists := True;
              end;
            end;
          end;
        end;

        if (((not OnTable) or (DriveThroughPayment) or (DoingPhoneOrder and (TLList.Count > 0))) and (SurchargeList.Count > 0)) then
        begin
          for I := 0 to (SurchargeList.Count - 1) do begin
            aSI := SurchargeList.Items[I];
            if (aSI.Amount <> 0) then begin
              //create order line
              SetStoredProcName('INSERT_ORDERLINE', SPNumber);

              ParamNull(ParamByName('courseid'));
              ParamByName('qty').AsCurrency := 1;
              ParamNull(ParamByName('notes'));
              ParamByName('happyhour').AsInteger := 0;
              ParamNull(ParamByName('positions'));
              ParamNull(ParamByName('numpos'));
              ParamByName('itemid').AsInteger := aSI.ItemID;
              ParamByName('orderid').AsInteger := TheOrderID;
              ParamNull(ParamByName('printerid'));
              ParamNull(ParamByName('repeatprinterid'));
              ParamByName('held').AsInteger := 0;
              ParamByName('openprice').AsCurrency := aSI.Amount;
              ParamNull(ParamByName('openpriceitemabbrev'));
              ParamByName('pricelevel').AsInteger := 0;
              ParamByName('currenthour').AsInteger := -1;
              ParamByName('currentminute').AsInteger := -1;
              ParamByName('currentday').AsInteger := CurrentDay;
              ParamNull(ParamByName('orderlineorder')); //will add as last item
              ParamNull(ParamByName('comboid'));
              ParamNull(ParamByName('combogroup'));
              ParamNull(ParamByName('comboindex'));
              ParamNull(ParamByName('comboqty'));
              ParamNull(ParamByName('FixTheCombo'));
              ParamNull(ParamByName('FreeComboItem'));
              if AppDetails.EnableFiscalPrinting then
              begin
                SWBEncryption.Initialise;
                SWBEncryption.AddInt(aSI.ItemID);
                SWBEncryption.AddStr(aSI.ItemAbbrev);
                SWBEncryption.AddStr('T');
                SWBEncryption.AddStr(FloatToStr(aSI.SalesTaxPercent));
                SWBEncryption.AddCurrency(1);
                SWBEncryption.AddCurrency(aSI.Amount);
                SWBEncryption.AddCurrency(aSI.Amount);
                SWBEncryption.AddCurrency(0);
                aHash := SWBEncryption.GetHash;
                ParamByName('CHECKSUM').AsString := aHash;
                ParamByName('FISCALTAX').AsInteger := 1;
              end;

              ExecProc;
              aSI.OrderLineID := FieldByName('orderlineid').AsInteger;

              SetStoredProcName('INSERT_INVOICELINE', SPNumber);
              ParamByName('ilamount').AsCurrency := aSI.Amount;
              ParamByName('discountamount').AsCurrency := aSI.Discount;
              ParamByName('invoiceid').AsInteger := TheInvoiceID;
              ParamByName('orderlineid').AsInteger := aSI.OrderLineID;
              ParamNull(ParamByName('discountschemeid'));
              ParamByName('FISCALSEQNO').AsString := '';
              if AppDetails.EnableFiscalPrinting then
              begin
                SWBEncryption.Initialise;
                SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
                SWBEncryption.AddStr(FInvCOO);
                SWBEncryption.AddStr(FInvCCF);
                SWBEncryption.AddInt(aSI.ItemID);
                SWBEncryption.AddStr(aSI.ItemAbbrev);
                SWBEncryption.AddStr('T');
                SWBEncryption.AddStr(FloatToStr(aSI.SalesTaxPercent));
                SWBEncryption.AddCurrency(1);
                SWBEncryption.AddCurrency(aSI.Amount);
                SWBEncryption.AddCurrency(aSI.Amount);
                SWBEncryption.AddCurrency(0);
                aHash := SWBEncryption.GetHash;
                ParamByName('CHECKSUM').AsString := aHash;
                ParamByName('FISCALTAX').AsInteger := 1;
              end;

              // for Fiscal Printing - Open an Invoice in the Fiscal Printer
              if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode)))then
              begin

              end;
              if (AppDetails.TaxExclusivePrices) then begin
                BalanceDelta := BalanceDelta + GetRoundedUpDown(aSI.Amount * (100 + aSI.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
              end
              else begin
                BalanceDelta := BalanceDelta + GetRoundedUpDown(aSI.Amount, AppDetails.DecimalPlaces);
              end;
              ExecProc;
              aSI.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;
              if (AppDetails.FiscalType = fiscalTaxCore) then
              begin
                SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
                ParamByName('invoicelineid').AsInteger := aSI.InvoiceLineID;
                tmpItemGroup := GlobalMenuList.GetItemsItemGroup(aSI.ItemID);
                if (Assigned(tmpItemGroup)) then
                  StringAsParam(ParamByName('taxlabel'), tmpItemGroup.TaxCoreLabel)
                else
                  ParamNull(ParamByName('taxlabel'));
                ExecProc;
            end;
          end;
        end;
        end;

        aTotalTip := 0;
        FCreditCardDiscount := 0;
        aTotalTender := 0;

        if (TLList.Count > 0) then
        begin
          if (ValueFound and ((not OnTable) or (DriveThroughPayment) or (DoingPhoneOrder))) then
          begin
            //create tender record
            SetStoredProcName('INSERT_TENDER', SPNumber);
            ParamByName('tendertype').AsString := 'P';
            IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);

            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            ParamNull(ParamByName('nosaletypeid'));
            ParamNull(ParamByName('nosalenotes'));

            BoolAsParam(ParamByName('printed'), AppDetails.PrintCashSale);

            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, ((DriveThroughPayment) or (DoingPhoneOrder and (TLList.Count > 0)) or (TrickGuestCount)));

            LibExists := ((AppDetails.PMSType = pmsLibica) and (TLList.TenderTypeExists(TLTLibica)));
            StringAsParamIfBool(ParamByName('libaccount'), LibAccount, LibExists);
            StringAsParamIfBool(ParamByName('libroom'), LibRoom, LibExists);
            StringAsParamIfBool(ParamByName('libname'), LibName, LibExists);
            StringAsParamIfBool(ParamByName('libstatus'), LibStatus, LibExists);

            NZAExists := ((AppDetails.PMSType = pmsNZAGold) and (TLList.TenderTypeExists(TLTNZAGold)));
            IntegerAsParamIfBool(ParamByName('nzaaccount'), NZAAccount, NZAExists);
            StringAsParamIfBool(ParamByName('nzaakey'), NZAAkey, NZAExists);
            StringAsParamIfBool(ParamByName('nzaname'), Copy(NZAName, 1, 30), NZAExists);

            EvolutionExists := ((AppDetails.PMSType = pmsEvolution) and (TLList.TenderTypeExists(TLTEvolution)));
            IntegerAsParamIfBool(ParamByName('evolutionaccountno'), EvolutionAccountNo, EvolutionExists);
            StringAsParamIfBool(ParamByName('evolutionaccount'), Copy(EvolutionAccount, 1, 30), EvolutionExists);
            StringAsParamIfBool(ParamByName('evolutionname'), Copy(EvolutionName, 1, 60), EvolutionExists);
            if ((EvolutionExists) and (AppDetails.EvolutionAccExtraField <> '')) then begin
              if (AppDetails.EvolutionAccExtraFieldSortInt) then begin
                IntegerAsParam(ParamByName('nzaaccount'), StrToIntDef(EvolutionAccExtra, 0));
              end
              else begin
                StringAsParam(ParamByName('nzaname'), Copy(EvolutionAccExtra, 1, 30));
              end;
            end;

            FidelioExists := ((AppDetails.PMSType = pmsFidelio) and (TLList.TenderTypeExists(TLTFidelio) or TLList.TenderTypeExists(TLTRoomCharge)));
            PhoenixExists := ((AppDetails.PMSType = pmsPhoenix) and (TLList.TenderTypeExists(TLTPhoenix)));
            Micros4700Exists := ((AppDetails.PMSType = pmsMicros4700) and (TLList.TenderTypeExists(TLTMicros4700) or TLList.TenderTypeExists(TLTMicros4700Account)));
            AgoraExists := ((AppDetails.PMSType = pmsAgora) and (TLList.TenderTypeExists(TLTAgora)));
            RMSExists := ((AppDetails.PMSType = pmsRMS) and (TLList.TenderTypeExists(TLTRMS)));
            RoomChargeExists := (TLList.TenderTypeExists(TLTRoomCharge));

            if (PhoenixExists) then begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbPhoenixAccount.GuestNumber, PhoenixExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbPhoenixAccount.RoomNumber, PhoenixExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbPhoenixAccount.GuestName, PhoenixExists);
            end
            else if (Micros4700Exists) then begin
              ParamNull(ParamByName('fidelioguestnumber'));
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbMicros4700Account.RoomNumber, Micros4700Exists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbMicros4700Account.SubFolio, Micros4700Exists);
            end
            else if (AgoraExists) then
            begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), StrToIntDef(GlbAgoraAccount.FolioNumber, 0), AgoraExists);
              StringAsParamIfBool(ParamByName('folioid'), GlbAgoraAccount.FolioId, AgoraExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbAgoraAccount.RoomNumber, AgoraExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbAgoraAccount.DisplayName, AgoraExists);
            end
            else if (RMSExists) then
            begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbRMSAccount.AccountId, RMSExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbRMSAccount.RoomNumber, RMSExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbRMSAccount.DisplayName, RMSExists);
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
            if ((aCompleteFiscal) or
                ((AppDetails.EnableFiscalPrinting) and
                 (not (NFManual or NFEMode)) and
                 (not DoingPhoneOrder) and
                 (not (OnTable and (GlbTable.GroupID > 0))))) then
            begin
              if (not SFiscalPrinting.OpenedFiscalInvoice) then
              begin
                TheTR.Rollback;
                if AppDetails.EnableFiscalPrinting and (not (NFManual or NFEMode))then
                begin
                  CancelCashSale(True);
                end;
                Result := False;
                LogMessage(ltPaymentVerbose, 'TryCashSale Exit/23');
                Exit;
              end
              else
              begin
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

                ShowQuickMsgOnTop(sPrintingDontDisturb);
                SFiscalPrinting.SupressFiscalErrorMessage := True;
                repeat
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
                    FreeQuickMsg;
                    if ShowConf(sConfirmCashPayWait) = mrNO then
                    begin
                      FreeQuickMsg;
                      TheTR.Rollback;
                      if (not SFiscalPrinting.FiscalCommError) then
                        SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                      Result := False;
                      SFiscalPrinting.SupressFiscalErrorMessage := False;
                      LogMessage(ltPaymentVerbose, 'TryCashSale Exit/24');
                      Exit;
                    end;
                  end;
                until(PrintOK);

                SFiscalPrinting.SupressFiscalErrorMessage := False;

                FiscalPaymentInitialized := True;
              end;
            end;
            ParamByName('COO').AsString := FInvCOO;
            ParamByName('GNF').AsString := '';
            SWBEncryption.Initialise;
            SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
            SWBEncryption.AddStr(FInvCOO);
            SWBEncryption.AddStr(FInvCCF); //CCF
            // CDC
            // GNF
            aHash := SWBEncryption.GetHash;
            ParamByName('CHECKSUM').AsString := aHash;
            IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);
            ExecProc;
            TheTenderID := FieldByName('tenderid').AsInteger;
            WBReceipts.RefreshID := TheTenderID;
            if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode)) and (not DoingPhoneOrder)) then
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
              ParamByName('FISCALINVTYPE').AsString := '';
              ParamByName('FISCALINVNO').AsString := '';
              if NFManual then
              begin
                ParamByName('MANUFACTURENO').AsString := '';
                ParamByName('ECFMODEL').AsString := '';
                ParamByName('ADDITIONALMF').AsString := '';
              end;
              Execute;
            end;

            //create tender lines
            FiscalTLList.Copy(TLList);
            for I := 0 to (TLList.Count - 1) do
            begin
              TL := TLList.Items[I];

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
                begin
                  aTotalTender := aTotalTender + TL.TLPayment;
                end;

                SetStoredProcName('INSERT_TENDERLINE', SPNumber);
                ParamByName('tenderid').AsInteger := TheTenderID;
                ParamByName('tenderlinetypeid').AsInteger := TL.TLTypeID;
                ParamByName('tenderlineamount').AsCurrency := TL.TLTender;
                ParamByName('tenderlinetip').AsCurrency := TL.TLTip;
                //ParamByName('roundingamount').AsFloat := 0;
                ParamByName('roundingamount').AsCurrency := TL.TLRounding;
                BoolAsParam(ParamByName('changeamount'), False);
                if (SurplusOnAccount) then begin
                  ParamByName('tenderlinechange').AsFloat := 0;
                end
                else begin
                  ParamByName('tenderlinechange').AsFloat := TL.TLChange;
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

                BalanceDelta := BalanceDelta - GetRoundedUpDown(TL.TLTender - TL.TLTip + TL.TLRounding, AppDetails.DecimalPlaces);
                if AppDetails.EnableFiscalPrinting then
                begin
                  StringAsParam(ParamByName('COO'), FInvCOO);
                  ParamNull(ParamByName('GNF'));
                  StringAsParam(ParamByName('CCF'), FInvCCF);

                  SWBEncryption.Initialise;
                  SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
                  SWBEncryption.AddStr(FInvCOO);
                  SWBEncryption.AddStr(FInvCCF); //CCF
                  StringAsParam(ParamByName('CHECKSUM'), SWBEncryption.GetHash);
                end
                else
                begin
                  ParamNull(ParamByName('COO'));
                  ParamNull(ParamByName('GNF'));
                  ParamNull(ParamByName('CCF'));
                  ParamNull(ParamByName('CHECKSUM'));
                end;
                TL.COO := FInvCOO;
                TL.CCF := FInvCCF;
                IntegerAsParam(ParamByName('tenlinetypeproviderid'), TL.TLProviderID);
                IntegerAsParam(ParamByName('instalments'), TL.TLInstalments);
                ExecProc;
                TL.TLID := FieldByName('tenderlineid').AsInteger;
              end;
            end;

            // for Fiscal Printing - Open an Invoice in the Fiscal Printer
            if ((aCompleteFiscal) or
                ((AppDetails.EnableFiscalPrinting) and
                 (not (NFManual or NFEMode)) and
                 (not DoingPhoneOrder) and
                 (not (OnTable and (GlbTable.GroupID > 0))))) then
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

                if (not SFiscalPrinting.OpenedFiscalInvoice) then
                begin
                  TheTR.Rollback;
                  Result := False;
                  LogMessage(ltPaymentVerbose, 'TryCashSale Exit/25');
                  Exit;
                end
                else
                begin
                  TenderLine := GetTenderLineTypeRecord(GetTLT(TL.TLTypeID));

                  if (Assigned(TenderLine)) then
                  begin
                    ShowQuickMsgOnTop(sPrintingDontDisturb);
                    SFiscalPrinting.SupressFiscalErrorMessage := True;
                    repeat
                      PrintOK := SFiscalPrinting.AddFiscalPrinterPayment(TenderLine.TenderLineType, TL.TLTender - TL.TEFCreditCardDiscount, TL.TLTypeID = GetTLTID(TLTCash));
                      if (not PrintOK) then
                      begin
                        FreeQuickMsg;
                        if ShowConf(sConfirmCashPayWait) = mrNO then
                        begin
                          FreeQuickMsg;
                          TheTR.Rollback;
                          if (not SFiscalPrinting.FiscalCommError) then
                            SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                          Result := False;
                          SFiscalPrinting.SupressFiscalErrorMessage := False;
                          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/26');
                          Exit;
                        end;
                      end;

                    until(PrintOK);
                    SFiscalPrinting.SupressFiscalErrorMessage := False;
                  end;
                end;
              end;
            end;

            if AppDetails.EnableFiscalPrinting then
            begin
              SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '1';
              SWBEncryption.GenerateEncryptedAuxFile;
            end;

            //create a tender line for the change
            if ((FChange > 0) and (not SurplusOnAccount)) then
            begin
              SetStoredProcName('INSERT_TENDERLINE', SPNumber);
              ParamByName('tenderlinetypeid').AsInteger := 4; //cash
              ParamByName('tenderlineamount').AsCurrency := -FChange; //total change
              ParamByName('tenderlinetip').AsCurrency := 0;
              ParamByName('roundingamount').AsCurrency := 0;
              BoolAsParam(ParamByName('changeamount'), True);
              ParamByName('tenderlinechange').AsCurrency := 0;
              ParamNull(ParamByName('tenderlinenotes'));
              ParamByName('tenderid').AsInteger := WBReceipts.RefreshID;
              ParamNull(ParamByName('eftpostxnref'));
              ParamNull(ParamByName('eftposcardref'));
              BalanceDelta := BalanceDelta - GetRoundedUpDown(ParamByName('tenderlineamount').AsCurrency - ParamByName('tenderlinetip').AsCurrency + ParamByName('roundingamount').AsCurrency, AppDetails.DecimalPlaces);

              SWBEncryption.Initialise;
              SWBEncryption.AddStr(GlbFiscal.ManufactureNo);
              SWBEncryption.AddStr(FInvCOO);
              SWBEncryption.AddStr(FInvCCF); //CCF
              if AppDetails.EnableFiscalPrinting then
              begin
                StringAsParam(ParamByName('COO'), FInvCOO);
                ParamNull(ParamByName('GNF'));
                StringAsParam(ParamByName('CCF'), FInvCCF);
                StringAsParam(ParamByName('CHECKSUM'), SWBEncryption.GetHash);
              end
              else
              begin
                ParamNull(ParamByName('COO'));
                ParamNull(ParamByName('GNF'));
                ParamNull(ParamByName('CCF'));
                ParamNull(ParamByName('CHECKSUM'));
              end;
              ParamNull(ParamByName('tenlinetypeproviderid'));
              ParamNull(ParamByName('instalments'));
              ExecProc;
            end;
          end;
        end;

        //Save wiGroup transaction database records
        if TLList.TenderTypeExists(TLTwiGroup) then
          wiGroupPOSSrv.SaveTransRecords(TheSP, SPNumber, OLList);

        //For fiscal Printing
        if ((TheInvoiceID > 0) and (AppDetails.EnableFiscalPrinting)) then
        begin
          if NFEMode then
          begin
            if not GetNFEDetails then
            begin
              Result := False;
              TheTR.Rollback;
              LogMessage(ltPaymentVerbose, 'TryCashSale Exit/27');
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
                for I := 0 to (OLList.Count - 1) do
                begin
                  aOL := OLList.Items[I];
                  if (aOL.ItemID > 0) then
                  begin
                    NFEItem := TNFEItem.Create;

                    NFEItem.ItemID := aOL.ItemID;
                    NFEItem.ItemName := aOL.ItemString;
                    NFEItem.Qty := aOL.Qty;
                    NFEItem.CFOP := GlbNFEData.CFOP;
                    NFEItem.FiscalSeq := aOL.FiscalIndex;
                    NFEItem.WeightedItem := aOL.WeighedItem;
                    NFEItem.UnitPrice := aOL.UnitPrice;
                    NFEItem.Discount := aOL.ILDiscount;
                    NFEItem.Total := aOL.Qty * aOL.UnitPrice;
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
                  ShowQuickMsg(sErrorInSendingNFEData);
                  Result := False;
                  TheTR.Rollback;
                  LogMessage(ltPaymentVerbose, 'TryCashSale Exit/28');
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
            ParamByName('BOXQTY').AsCurrency := GlbNFEData.BoxQty;
            ParamByName('LOTNUMBER').AsCurrency := GlbNFEData.LotNumber;
            ParamByName('GROSSWEIGHT').AsCurrency := GlbNFEData.GrossWeight;
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

        if (((DriveThroughPayment) or (GlbTable.SaleCategoryType = scRunningTab)) and (VoidOLList.Count > 0)) then
        begin
          SetStoredProcName('INSERT_VOID', SPNumber);
          IntegerAsFindParam(FindParam('outletid'), AppDetails.OutletID);
          IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, (OnTable or TrickGuestCount));
          ParamByName('voidreasonid').AsInteger := AppDetails.DriveThroughVoidReasonID;
          ParamNull(ParamByName('voidexplanation1'));
          ParamNull(ParamByName('voidexplanation2'));
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
          TheVoidID := FieldByName('voidid').AsInteger;

          for I := 0 to (VoidOLList.Count - 1) do
          begin
            VoidOL := VoidOLList.Items[I];
            if ((VoidOL.ItemID > 0) and (VoidOL.AlreadyOrdered)) then begin //only void items that have already been ordered before
              SetStoredProcName('VOID_ORDERLINE', SPNumber);

              ParamByName('voidid').AsInteger := TheVoidID;
              ParamByName('orderlineid').AsInteger := VoidOL.OrderLineID;
              ParamByName('qtyvoided').AsCurrency := VoidOL.Qty;

              if (ToWaste) then begin
                ParamByName('qtywasted').AsCurrency := VoidOL.Qty;
              end
              else begin
                ParamByName('qtywasted').AsCurrency := 0;
              end;

              ParamByName('loginid').AsInteger := GlbLogin.LoginID;
              ExecProc;
            end
          end;

          if (AppDetails.CashSalePrintVoids) then
          begin
            //loop through Void order lines
            for I := 0 to (VoidOLList.Count - 1) do begin
              if (TOrderLine(VoidOLList[I]).PrinterID <> 0) then begin
                AddPrinterToPrinterList(VoidPrinterList, TOrderLine(VoidOLList[I]).PrinterID);

                //loop through walist, if repeat printer not found add
                if ((TOrderLine(VoidOLList[I]).RepeatPrinterID > 0) and (not AppDetails.CashSaleDisableRepeatPrinter)) then begin
                  AddPrinterToPrinterList(OrderPrinterList, TOrderLine(VoidOLList[I]).RepeatPrinterID);
                end;
              end;
            end;
            AddCCPrintersToPrinterList(VoidPrinterList);
          end;
        end;

        VoidClearedOL(SPNumber, False); //lets void those cleared items

        SFiscalPrinting.FiscalInvoiceAmount := 0;
        SFiscalPrinting.UpdateInvoiceChecksum(TheSP, TheInvoiceID, False, SPNumber);

        if (TheOrderID > 0) then begin
          SetStoredProcName('DECREMENT_STOCKORDER', SPNumber);
          IntegerAsParam(ParamByName('orderid'), TheOrderID);
          IntegerAsParamIfBool(ParamByName('terminalid'), AppDetails.TerminalID, AppDetails.StockEnabled);
          BoolAsParam(ParamByName('decrementavailqty'), True);
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
            begin
              CancelCashSale(True);
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/29');
            Exit;
          end;
        end;

        if (TheVoidID > 0) then begin
          SetStoredProcName('RESTORE_STOCKVOID', SPNumber);
          IntegerAsParam(ParamByName('voidid'), TheVoidID);
          IntegerAsParamIfBool(ParamByName('terminalid'), AppDetails.TerminalID, AppDetails.StockEnabled);
          BoolAsParam(ParamByName('restoreavailqty'), True);
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            if ((AppDetails.EnableFiscalPrinting) and
                 (not (NFManual or NFEMode)) and
                 (not DoingPhoneOrder) and
                 (not (OnTable and (GlbTable.GroupID > 0)))) then
            begin
              sExternalAccountIntegration.FinalisePresentedCards; //since ResetCashSale will clear presented cards
              CancelCashSale(True);
            end;
            LogMessage(ltPaymentVerbose, 'TryCashSale Exit/30');
            Exit;
          end;
        end;

        if ((AppDetails.FiscalType = fiscalNone) and (TheInvoiceID > 0) and
            ((AppDetails.PrintCashSale) or
             ((Appdetails.PaymentTenderOverRidePrints) and (TLList.PrintCopiesExists)) or
             ((Appdetails.PaymentAccountPrints = 1) and ((FDiscountAmount > 0) or (FLoyaltyFreeItemDiscount > 0))) or
             ((Appdetails.PaymentAccountPrints = 2) and (GlbAccount.AccountID > 0))) and
            ((not DoingPhoneOrder) or (AppDetails.PhoneOrderCloseTab))) then
        begin
          //insert print job
          InsertPrintJob('Invoice', TheInvoiceID, AppDetails.PrinterID, SPNumber);
        end;

        if ((not AppDetails.EnableFiscalPrinting) and ((AppDetails.CashSalePrintVoids) and (TheVoidID > 0))) then
        begin
          for I := 0 to (VoidPrinterList.Count - 1) do
          begin
            if (DoingWaste) then
              InsertPrintJob('Waste', TheVoidID, VoidPrinterList.Get(I), SPNumber)
            else
              InsertPrintJob('Void', TheVoidID, VoidPrinterList.Get(I), SPNumber);
          end;
        end;

        if (((not AppDetails.CashSaleEatInTakeawayPrompt) or
             (AppDetails.CashSalePrintPhoneOrders = 1) or
             ((AppDetails.CashSalePrintPhoneOrders = 2) and (PrintPhoneOrder)) or
             (EatInTakeawayPromptValue <> 3))) then
        begin
          for I := 0 to (OrderPrinterList.Count - 1) do
          begin
            if (FHeldGroupID > 0) then
            begin
              InsertPrintJob('Held Order', TheOrderID, OrderPrinterList.Get(I), SPNumber);
            end
            else begin
              InsertPrintJob('Order', TheOrderID, OrderPrinterList.Get(I), SPNumber);
            end;
          end;
        end;

        if (((not AppDetails.EnableFiscalPrinting) or
             (AppDetails.EnableFiscalPrinting and
              ((AppDetails.CashSalePrintPhoneOrders = 1) or
               ((AppDetails.CashSalePrintPhoneOrders = 2) and PrintPhoneOrder)))) and
            AppDetails.CopyFood and FoodExists) then
        begin
          InsertPrintJob('CF Order', TheOrderID, AppDetails.PrinterID, SPNumber);
        end;

        if (DoingPhoneOrder) then
        begin
          case AppDetails.PhoneOrderSummaryPrinterID of
            0:
              if AppDetails.EnableFiscalPrinting then
              begin
                PrintSummary(False, SPNumber);
              end
              else
              begin
                InsertPrintJob('Summary', GlbTable.GroupID, AppDetails.PrinterID, SPNumber);
              end;
            1..999:
              if AppDetails.EnableFiscalPrinting then
              begin
                PrintSummary(False, SPNumber);
              end
              else
              begin
                InsertPrintJob('Summary', GlbTable.GroupID, AppDetails.PhoneOrderSummaryPrinterID, SPNumber);
              end;
          end;
        end
        else if ((AppDetails.PrintSummaryAfterOrder and $02) > 0) and (OnTable) then
        begin
          if AppDetails.EnableFiscalPrinting then
            PrintSummary(False, SPNumber)
          else
            InsertPrintJob('Summary', GlbTable.GroupID, AppDetails.PrinterID, SPNumber);
        end;
        //AM 23Dec99 Account Balance
        //totalinvoices = insert_invoiceline: add sum(ilamount-discountamount)
        //totaltenders = insert_tenderline: less sum(tenderlineamount-tenderlinetip+roundingamount)
        //account balance delta = totalinvoices - totaltenders (+ Host Subsidy if event active)

        if (GlbAccount.AccountID > 0) then
        begin // if going on an account, set account balance
          SetStoredProcName('INSERTACCOUNTINVOICE', SPNumber);

          ParamByName('accountid').AsInteger := GlbAccount.AccountID;
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          IntegerAsParam(ParamByName('tenderid'), TheTenderID);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;

          if (GlbEvent.EventID > 0) then
          begin
            CurrencyAsParamIfBool(ParamByName('currentbalancechange'),
                        GetRoundedUpDown((BalanceDelta), AppDetails.DecimalPlaces), True, False);

          end
          else
            CurrencyAsParamIfBool(ParamByName('currentbalancechange'),
                        GetRoundedUpDown((BalanceDelta), AppDetails.DecimalPlaces), OnAccount, False);

          // for Fiscal Printing - Open an Invoice in the Fiscal Printer
          if BalanceDelta > 0.005 then
          begin
            if ((aCompleteFiscal) or
                ((AppDetails.EnableFiscalPrinting) and
                 (not (NFManual or NFEMode)) and
                 (not DoingPhoneOrder) and
                 (not (OnTable and (GlbTable.GroupID > 0))))) then
            begin
              if (not SFiscalPrinting.OpenedFiscalInvoice) then
              begin
                TheTR.Rollback;
                Result := False;
                LogMessage(ltPaymentVerbose, 'TryCashSale Exit/31');
                Exit;
              end
              else
              begin
                SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '0';
                SWBEncryption.GenerateEncryptedAuxFile;
                if (not FiscalPaymentInitialized) then
                begin
                  ShowQuickMsgOnTop(sPrintingDontDisturb);
                  SFiscalPrinting.SupressFiscalErrorMessage := True;
                  repeat
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
                      FreeQuickMsg;
                      if ShowConf(sConfirmCashPayWait) = mrNO then
                      begin
                        FreeQuickMsg;
                        TheTR.Rollback;
                        if (not SFiscalPrinting.FiscalCommError) then
                          SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                        Result := False;
                        SFiscalPrinting.SupressFiscalErrorMessage := False;
                        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/32');
                        Exit;
                      end;
                    end;
                  until(PrintOK);
                  SFiscalPrinting.SupressFiscalErrorMessage := False;
                end;

                if (Trim(AppDetails.FiscalAccountPaymentMethod) <> '') then
                begin
                  ShowQuickMsgOnTop(sPrintingDontDisturb);
                  SFiscalPrinting.SupressFiscalErrorMessage := True;
                  repeat
                    PrintOK := SFiscalPrinting.AddFiscalPrinterPayment(AppDetails.FiscalAccountPaymentMethod, BalanceDelta);

                    if not PrintOK then
                    begin
                      FreeQuickMsg;
                      if ShowConf(sConfirmCashPayWait) = mrNO then
                      begin
                        FreeQuickMsg;
                        TheTR.Rollback;
                        if (not SFiscalPrinting.FiscalCommError) then
                          SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
                        Result := False;
                        SFiscalPrinting.SupressFiscalErrorMessage := False;
                        LogMessage(ltPaymentVerbose, 'TryCashSale Exit/33');
                        Exit;
                      end;
                    end;
                  until(PrintOK);
                  SFiscalPrinting.SupressFiscalErrorMessage := False;

                end;
                SWBEncryption.AuxiliaryData.FiscalInvoiceDone := '1';
                SWBEncryption.GenerateEncryptedAuxFile;
              end;
            end;
          end;
          ExecProc;

          if ((LoyaltyReward.LoyaltyRewardID > 0) and (not LoyaltyReward.Processed) and (TheInvoiceID > 0)) then begin
            //If we didn't create an invoice, then we didn't sell anything, so a reward couldn't of been given yet.
            SetStoredProcName('REDEEMLOYALTYPOINTS', SPNumber);      //redeemloyaltypoints
            ParamByName('accountid').AsInteger := GlbAccount.AccountID;
            ParamByName('loyaltyrewardid').AsInteger := LoyaltyReward.LoyaltyRewardID;
            IntegerAsParam(ParamByName('invoiceid'), TheInvoiceID);
            IntegerAsParam(ParamByName('tenderid'), TheTenderID);
            IntegerAsParamIfBool(ParamByName('loyaltypoints1change'), LoyaltyReward.DecrementLoyalty1Points, ((LoyaltyReward.RewardOffer = roVariableDiscount) and (LoyaltyReward.DecrementLoyalty1Points > 0)));
            IntegerAsParamIfBool(ParamByName('loyaltypoints2change'), LoyaltyReward.DecrementLoyalty2Points, ((LoyaltyReward.RewardOffer = roVariableDiscount) and (LoyaltyReward.DecrementLoyalty2Points > 0)));
            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            ExecProc;
          end;
        end;

        if ((DriveThroughPayment) or (TrickGuestCount) or
            ((AppDetails.PhoneOrderCloseTab) and (FStillDue = 0))) then begin
          SetStoredProcName('CLOSE_TABLE', SPNumber);    //close_table

          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
        end;
      end;
      if ((FExchangeInvoiceID > 0) and (FExchangeOLs.Count > 0)) then
      begin
        if (not TryExchangeRefund(TheSP, SPNumber)) then
        begin
          TheTR.Rollback;
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/34');
          Exit;
        end;
      end;
      if ((not OnTable) or (DriveThroughPayment)) then
      begin
        if (((AppDetails.PMSType = pmsLibica) and (TLList.TenderTypeExists(TLTLibica)) and
             (not SendLibicaTransactions(SPNumber))) or

            ((AppDetails.PMSType = pmsFidelio) and
             ((AppDetails.FidelioAllTransactions = 1) or
              ((AppDetails.FidelioExtendedTender > 0) and (AppDetails.FidelioAllTransactions < 2) and (TLList.Count > 0)) or
              (TLList.TenderTypeExists(TLTFidelio))) and
             (not SendFidelioTransactions(SPNumber))) or

            ((AppDetails.PMSType = pmsPhoenix) and
             ((AppDetails.PhoenixAllTransactions) or
              ((AppDetails.PhoenixExtendedTender) and (TLList.Count > 0)) or
              (TLList.TenderTypeExists(TLTPhoenix))) and
             (not SendPhoenixTransactions(SPNumber))) or

            ((AppDetails.PMSType = pmsMicros4700) and
             ((AppDetails.Micros4700AllTransactions = 1) or
              ((AppDetails.Micros4700ExtendedTender) and (AppDetails.Micros4700AllTransactions < 1) and (TLList.Count > 0)) or
              (TLList.TenderTypeExists(TLTMicros4700) or TLList.TenderTypeExists(TLTMicros4700Account))) and
             (not SendMicros4700Transactions(SPNumber))) or

            ((AppDetails.PMSType = pmsNZAGold) and (TLList.TenderTypeExists(TLTNZAGold)) and
             (not SendNZATransactions(SPNumber))) or

            ((AppDetails.PMSType = pmsICRTouch) and (TLList.TenderTypeExists(TLTRoomCharge)) and
             (not SendICRTouchTransactions(SPNumber))) or

            ((AppDetails.PMSType = pmsEvolution) and (TLList.TenderTypeExists(TLTEvolution)) and
             (not SendEvolutionTransactions(iGuestCountUsed, SPNumber))) or

            ((AppDetails.PMSType = pmsMews) and ((AppDetails.MewsAddOutletBills) or (TLList.TenderTypeExists(TLTMews))) and
             (not SendMewsTransactions(SPNumber, TheTenderID))) or

            ((AppDetails.PMSType = pmsAgora) and ((AppDetails.AgoraAllTransactions = 1) or (TLList.TenderTypeExists(TLTAgora))) and
             (not SendAgoraTransactions(SPNumber, TheTenderID))) or

             ((AppDetails.PMSType = pmsRMS) and ((AppDetails.RMSAllTransactions = 1) or (TLList.TenderTypeExists(TLTRMS))) and
             (not SendRMSTransactions(SPNumber, TheTenderID)))) then
        begin
          //Something wrong happened while putting in the Transactions.
          TheTR.Rollback;
          if ((aCompleteFiscal) or
              ((AppDetails.EnableFiscalPrinting) and
               (not (NFManual or NFEMode)) and
               (not DoingPhoneOrder) and
               (not (OnTable and (GlbTable.GroupID > 0))))) then
          begin
            sExternalAccountIntegration.FinalisePresentedCards; //since ResetCashSale will clear presented cards
            CancelCashSale(True);
          end;
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/35');
          Exit;
        end;
      end;
      if (AppDetails.PDEnabled) then
      begin
        if (TLList.Count > 1) then begin
          DMComponents.PDOutputItemLine('TENDERED', GetRoundedUpDown(FPayment + FChange, AppDetails.DecimalPlaces), 0);
        end
        else if (TLList.Count = 1) then begin
          TL := TLList.Items[0];
          DMComponents.PDOutputItemLine(GetTLTType(TL.TLType), GetRoundedUpDown(TL.TLTender, AppDetails.DecimalPlaces), 0);
        end;
        if ((OnAccount) and (GlbAccount.PoleDisplayShow) and (GlbAccount.AllowCredit)) then begin
          DMComponents.pdOutputCreditDebitLine('BALANCE', GetRoundedUpDown((-GlbAccount.CurrentBalance) - BalanceDelta, AppDetails.DecimalPlaces), 1, False);
        end
        else begin
          DMComponents.PDOutputItemLine('CHANGE', GetRoundedUpDown(FChange, AppDetails.DecimalPlaces), 1);
        end;
      end;


      if (AppDetails.FiscalType = fiscalTaxCore) then
      begin
        if (SendTaxCoreTransactions) and TaxCoreTransactionData.SaveInvoice(False, SPNumber) and TaxCoreTransactionData.InsertPrintJob(SPNumber) then
        begin
          if (TaxCoreTransactionData.Messages <> '') then
            ShowQuickMsg('Tax Core Transaction' + #13#10 + TaxCoreTransactionData.Messages);
          TaxCoreTransactionData.Clear;
        end
        else begin
          // transaction failed
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
      if ((aCompleteFiscal) or
          ((AppDetails.EnableFiscalPrinting) and
           (not (NFManual or NFEMode)) and
           (not DoingPhoneOrder) and
           (not (OnTable and (GlbTable.GroupID > 0))))) then
      begin
        if (not SFiscalPrinting.OpenedFiscalInvoice) then
        begin
          TheTR.Rollback;
          sExternalAccountIntegration.FinalisePresentedCards; //since ResetCashSale will clear presented cards
          CancelCashSale(True);
          Result := False;
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/36');
          Exit;
        end
        else
        begin
          if (OnTable and (GlbTable.GroupID > 0)) then
          begin
            aPromoMessage := SFiscalPrinting.GetFiscalInvoiceFooter(OLList, Self, aFiscalNumero, GlbTable.GroupID, TheInvoiceID);
          end
          else
          begin
            aPromoMessage := SFiscalPrinting.GetFiscalInvoiceFooter(OLList, Self, aFiscalNumero, 0, TheInvoiceID);
          end;
          repeat
            SFiscalPrinting.SupressFiscalErrorMessage := True;
            PrintOK := SFiscalPrinting.CloseFiscalPrinterPayment(aPromoMessage, FTotalNCMTax);
            if not PrintOK then
            begin
              FreeQuickMsg;
              if ShowConf(sConfirmCashPayWait) = mrNO then
              begin
                FreeQuickMsg;
                TheTR.Rollback;
                sExternalAccountIntegration.FinalisePresentedCards; //since ResetCashSale will clear presented cards
                CancelCashSale(True);
                Result := False;
                SFiscalPrinting.SupressFiscalErrorMessage := False;
                LogMessage(ltPaymentVerbose, 'TryCashSale Exit/37');
                Exit;
              end
              else
              begin
                PrintOK := SFiscalPrinting.FiscalPrinterEnabled;
                if PrintOk then
                begin
                  ShowQuickMsgOnTop(sPrintingDontDisturb);
                  SFiscalPrinting.SupressFiscalErrorMessage := True;
                  PrintOK := SFiscalPrinting.CloseFiscalPrinterPayment(aPromoMessage, FTotalNCMTax);
                  SFiscalPrinting.SupressFiscalErrorMessage := False;
                end
                else
                begin
                  FreeQuickMsg;
                end;
              end;
            end;
          until (PrintOK);
          SFiscalPrinting.SupressFiscalErrorMessage := False;
        end;
      end;

      TheTR.Commit;
      TransCommitted := True;
      LogMessage(ltPaymentVerbose, 'TryCashSale Transaction Committed');

      if (OnTable) then
        STableManagementIntegration.OrderPlaced;

      cmdTable.Enabled := True;
      formEFTPOSIntegration.ClearTransactionID;

      if (GlbEvent.EventID > 0) then
      begin
        SetGlbEvent(GlbEvent.EventID, True);

        if (GlbEvent.EventEndTime <= Now) then
        begin
          ShowMsg(sEventEndTimeIsOverDeselect);
          DeselectEvent;
        end
        else if (not GlbEvent.EventActive) then
        begin
          ShowMsg(sEventNoLongerActiveDeselect);
          DeselectEvent;
        end
        else if (GlbEvent.Available <= 0) then
        begin
          ShowMsg(sEventSpendLimitReachedDeselect);
          DeselectEvent;
        end;
      end;

      LogMessage(ltPaymentVerbose, 'TryCashSale Total Tender : ' + FloatToStr(aTotalTender));
      // send paymark loyalty amount to eftpos machine again
      if ((AppDetails.EFTPOSIntegrated and AppDetails.EFTPOSPaymarkLoyaltyEnabled) and
          (aTotalTender > 0) and
          (ShowConf(sIsLoyaltyCardAvailable) = mrYes)) then
      begin
        formEFTPOSIntegration.EFTPOSTLType := TLTLoyaltyPoints;
        formEFTPOSIntegration.EFTPOSTxnRef := ''; // empty reference for loyalty points
        formEFTPOSIntegration.EFTPOSTxnAuthCode := '';
        formEFTPOSIntegration.AllowTipAuthorisation := False;
        formEFTPOSIntegration.DoEftposTrans(aTotalTender, 0, ettEftpos);
      end;

      if AppDetails.EnableFiscalPrinting then
      begin
        SWBEncryption.AuxiliaryData.DBInvoiceDone := '1';
        SWBEncryption.GenerateEncryptedAuxFile;
      end;
      ResetFlags := True;
      //dm.tr.SavePoint;
      SFiscalPrinting.FiscalOpTryAgainCommand := False;

      SFiscalPrinting.SupressFiscalErrorMessage := True;
      if ((AppDetails.EnableFiscalPrinting) and (AppDetails.EFTPOSIntegrated) and (EFTPOSTransExists)) then
      begin
        if (not SFiscalPrinting.PrintAllCDCs(TLList)) then
        begin
          SFiscalPrinting.LockKeyBoardWhilePrinting(False);
          FreeQuickMsg;
          SFiscalPrinting.CancelNonInvoiceTEF(True);
          SFiscalPrinting.SupressFiscalErrorMessage := True;
          PrintOK := SFiscalPrinting.FiscalPrinterEnabled;
          SFiscalPrinting.SupressFiscalErrorMessage := False;

          if (PrintOK and (not SFiscalPrinting.OpenedRG)) then
          begin
            SFiscalPrinting.RefundPOSSale(iOldTenderID, iRefundTenderID, iRefundInvoiceID, OrgInvCOO);
            SFiscalPrinting.CancelFiscalPrinterInvoice(SWBEncryption.AuxiliaryData.InvoiceCPF, SWBEncryption.AuxiliaryData.InvoiceName, SWBEncryption.AuxiliaryData.InvoiceAddress, True);
          end;
          sExternalAccountIntegration.FinalisePresentedCards; //since ResetCashSale will clear presented cards
          ResetCashSale(False, False);
          DoPhoneOrder(PhoneOrderPersistant, PhoneOrderPersistant);

          // Here lets start the timer again!
          if (AppDetails.CashSaleStaffPromptTimeout > 0) then
          begin
            StaffPromptTimeoutTimer.Interval := (AppDetails.CashSaleStaffPromptTimeout * 1000);
            StaffPromptTimeoutTimer.Enabled := True;
            StaffPromptTimeoutTimerActive := True;
          end;
          Result := True;  // Note - exiting with result true
          LogMessage(ltPaymentVerbose, 'TryCashSale Exit/38');
          Exit;
        end;
        SFiscalPrinting.LockKeyBoardWhilePrinting(False);
      end;
      FreeQuickMsg;
      if (TLList.OpenDrawerExists(not SurplusOnAccount)) then
      begin
        if ((aCompleteFiscal) or
            ((AppDetails.EnableFiscalPrinting) and
             (not (NFManual or NFEMode)) and
             (not DoingPhoneOrder) and
             (not (OnTable and (GlbTable.GroupID > 0))))) then
        begin
          SFiscalPrinting.KickDrawer;
        end;
      end;
      SFiscalPrinting.SupressFiscalErrorMessage := False;

      if ResetFlags then
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

      if ((BalanceDelta > 0.005) and (AppDetails.EnableFiscalPrinting)) then
        SFiscalPrinting.PrintAccountPaymentReport(sPayOnAccount, BalanceDelta, FCPFNumber);

      SFiscalPrinting.COO := '';
      SFiscalPrinting.CCF := '';
      FInvCOO := '';
      FInvCCF := '';
      FComboDiscount := 0;

    except
      on e: exception do begin
        SFiscalPrinting.FiscalOpTryAgainCommand := False;
        SFiscalPrinting.SupressFiscalErrorMessage := False;
        // JEH 9/1/19 Note - code below will return trycashsale TRUE in the case where TheTR.InTransaction false, ie it has committed trans
        if ((aCompleteFiscal) or
            ((AppDetails.EnableFiscalPrinting) and
             (not (NFManual or NFEMode)) and
             (not DoingPhoneOrder) and
             (not (OnTable and (GlbTable.GroupID > 0))))) then
        begin
          Result := False;
          SFiscalPrinting.CancelFiscalPrinterInvoice; // Cancel Opened invoice
          sExternalAccountIntegration.CanContinueSale := False;
        end
        else if TheTR.InTransaction then // roll back in case of non fiscal, where exception occurred before commit
        begin
          Result := False;
          TheTR.Rollback;
          sExternalAccountIntegration.CanContinueSale := False;
        end;

        SelectFirsts;
        LogException(e, Self.ClassName+':1', True);
        Exit;
      end;
    end;

    try
      if (AppDetails.PrintTicket) then PrintTicket;

      BackPagesList.Clear;
      SetDoingRefund(False, True);
      SetDoingExchange(False, True);
      SetDoingWaste(False, True);
      PriceLevelChange(0, 2);
      CourseChange(0, '', 2, False);    //Jon 28-02-2002
      NextItemButtonStateChange(1);   //Jon 22-04-2002
      ModifierFunctionChange(1);               //Jon 14-8-2001 returns mod function to +

      //Here we show last receipt :) for JBM
      if ((Result) and ((not OnTable) or DriveThroughPayment) and (AppDetails.CashSaleShowLastReceipt)) then
      begin
        ShowLastReceipt;
      end;
    except
      on e: exception do begin
        LogException(e, Self.ClassName+':2', True);
      end;
    end;
  finally
    sExternalAccountIntegration.FinalisePresentedCards;

    SFiscalPrinting.FiscalOpTryAgainCommand := False;
    if Assigned(GlbNFEData) then
      GlbNFEData.Free;

    ClearStatus;
    OrderPrinterList.Free;
    VoidPrinterList.Free;
    FiscalTLList.Free;
    DM.qrGeneral.IB_Transaction := GenTR;
    SetNextQty(1);

    if (not TransCommitted) then
    begin
      //Rollback a few other things that may have been changed
      OnTable := PrevOnTable;
    end;
    if (not Result) then
      Write2EventLog(eiTryCashSale, rsEDTryCashSale, 'TryCashSale Returned False');
    LogMessage(ltPaymentVerbose, 'TformQuickSaleUser.TryCashSale end, Result = ' + BoolToStr(Result, True));
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.TryRemotePhoneOrder(Print: Boolean): Boolean;
var
  I, J: Integer;
  aOL: TOrderLine;
  TL: TTL;
  aSI: TSurchargeItem;
  ROSSRemoteOrderLine: TROSSRemoteOrderLine;
  ValueFound: Boolean;
  BalanceDelta: Currency;
  FoodExists: Boolean;

  SoFarDiscount, ILDiscount: Currency;

  ROSSDataRemoteOrder: TROSSDataRemoteOrder;
  ROSSModifier: POModifier;
  ROSSTenderLine: ^TOTenderLine;
  ROSSTransMasIDs: TROSSTransMasIDs;

begin
  ROSSDataRemoteOrder := TROSSDataRemoteOrder.Create;
  ROSSTransMasIDs := TROSSTransMasIDs.Create;
  try
    //AM 23Dec99 Account Balance
    BalanceDelta := 0;
    SoFarDiscount := 0.000;
    FoodExists := False;

    if (OnTable) then begin
      ShowMsg(sUnableToPlaceOrdToRemoteTable);
      Result := False;
      Exit;
    end;

    if (not DoingPhoneOrder) then
    begin
      ShowMsg(sRemoteOrdSysUsefulToSendPhOrd);
      Result := False;
      Exit;
    end;

    if (AppDetails.CashSaleAllowZeroPriceInvoices) then begin
      ValueFound := True;
    end
    else begin
      //check that the invoice will be greater than zero, else don't create one
      ValueFound := False;
      //if chkTable.State = cbUnChecked then begin //AMANDA 19/10/00 - removed chkTable

      if (TLList.Count > 0) then begin
        ValueFound := True;
      end
      else begin
        for I := 0 to (OLList.Count - 1) do begin
          if (TOrderLine(OLList[I]).ToPay > 0) then begin
            ValueFound := True;
            Break;
          end;
        end;
      end;
    end;

    //AMANDA - if not on table, will be printing, and no destination specified, set to WAName
    //if (chkTable.State = cbUnChecked) and (WAList.Count > 0) and (FDelivery = '') then begin
    if (FDelivery = '') then begin
      FDelivery := AppDetails.WAName;
    end;

    Result := True;

    SetStatus(sSending);

    //if there are tender lines, kick cash draw ASAP
    if (TLList.OpenDrawerExists(not SurplusOnAccount)) then
    begin
      if ((AppDetails.EnableFiscalPrinting) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0)))) then
      begin
        SFiscalPrinting.KickDrawer;
      end
      else
      begin
        KickCashDraw(GlbLogin.TillDriverName, GlbLogin.TillDrawKickString);
      end;
    end;

    //General
    ROSSDataRemoteOrder.General.TerminalID := AppDetails.TerminalID;
    if (GlbRemoteLocation.TerminalID > 0) then begin
      ROSSDataRemoteOrder.General.TerminalID := GlbRemoteLocation.TerminalID;
    end;

    ROSSDataRemoteOrder.General.StaffID := GlbLogin.StaffID;
    ROSSDataRemoteOrder.General.AccountID := GlbAccount.AccountID;
    ROSSDataRemoteOrder.General.AccountBalanceDelta := 0.00;
    ROSSDataRemoteOrder.General.LoyaltyRewardID := LoyaltyReward.LoyaltyRewardID;
    ROSSDataRemoteOrder.General.EatInTakeAwayPromptValue := EatInTakeawayPromptValue;
    ROSSDataRemoteOrder.General.IntendedTenderTypeID := IntendedTenderTypeID;
    ROSSDataRemoteOrder.General.IntendedTenderTypeProviderID := IntendedTenderTypeProviderID;
    ROSSDataRemoteOrder.General.TabNotes := FTabNotes;
    ROSSDataRemoteOrder.General.DeliveryName := GlbCustomer.DeliveryName;
    ROSSDataRemoteOrder.General.DeliveryAddress[1] := GlbCustomer.DeliveryAddress[1];
    ROSSDataRemoteOrder.General.DeliveryAddress[2] := GlbCustomer.DeliveryAddress[2];
    ROSSDataRemoteOrder.General.DeliveryAddress[3] := GlbCustomer.DeliveryAddress[3];
    ROSSDataRemoteOrder.General.DeliveryPhoneNo := GlbCustomer.DeliveryPhoneNo;
    ROSSDataRemoteOrder.General.DeliveryNotes := GlbCustomer.DeliveryNotes;
    ROSSDataRemoteOrder.General.DeliveryOrderReadyFor := GlbCustomer.DeliveryOrderReadyFor;
    ROSSDataRemoteOrder.General.DelAddressMapLocationID := GlbCustomer.DeliveryAddressMapLocationID;
    ROSSDataRemoteOrder.General.OrderRush := False;
    ROSSDataRemoteOrder.General.OrderLocation := FDelivery;
    ROSSDataRemoteOrder.General.OrderSaleCategoryID := GlbTable.SaleCategoryID;
    ROSSDataRemoteOrder.General.OrderCurrentHour := CurrentHour;
    ROSSDataRemoteOrder.General.OrderCurrentMinute := CurrentMin;
    ROSSDataRemoteOrder.General.InvoiceInsert := (ValueFound and ((TLList.Count > 0) or (OnAccount)));
    ROSSDataRemoteOrder.General.InvoiceDiscountPercent := FDiscountPercent;
    ROSSDataRemoteOrder.General.InvoiceSaleTax := FSalesTax;
    ROSSDataRemoteOrder.General.PrintJobInvoice := False;
    ROSSDataRemoteOrder.General.PrintJobSummary := True;

    //Customer
    ROSSDataRemoteOrder.Customer.PerorgID := GlbCustomer.PerorgID;
    ROSSDataRemoteOrder.Customer.PerOrgType := StrToChar('P');
    ROSSDataRemoteOrder.Customer.TaxNumber := GlbCustomer.TaxNumber;

    ROSSDataRemoteOrder.Customer.Surname := GlbCustomer.Surname;
    ROSSDataRemoteOrder.Customer.FirstName := GlbCustomer.FirstName;
    ROSSDataRemoteOrder.Customer.MiddleName := GlbCustomer.MiddleName;
    ROSSDataRemoteOrder.Customer.Title := GlbCustomer.Title;

    ROSSDataRemoteOrder.Customer.Salutation := GlbCustomer.Salutation;
    ROSSDataRemoteOrder.Customer.CustomerName := GlbCustomer.CustomerName;

    ROSSDataRemoteOrder.Customer.Address1 := GlbCustomer.Address1;
    ROSSDataRemoteOrder.Customer.Address2 := GlbCustomer.Address2;
    ROSSDataRemoteOrder.Customer.Address3 := GlbCustomer.Address3;
    ROSSDataRemoteOrder.Customer.PostCode := GlbCustomer.PostCode;
    ROSSDataRemoteOrder.Customer.Country := GlbCustomer.Country;
    ROSSDataRemoteOrder.Customer.AddressMapLocationID := GlbCustomer.AddressMapLocationID;

    ROSSDataRemoteOrder.Customer.POAddress1 := GlbCustomer.POAddress1;
    ROSSDataRemoteOrder.Customer.POAddress2 := GlbCustomer.POAddress2;
    ROSSDataRemoteOrder.Customer.POAddress3 := GlbCustomer.POAddress3;
    ROSSDataRemoteOrder.Customer.POPostCode := GlbCustomer.POPostCode;
    ROSSDataRemoteOrder.Customer.POCountry := GlbCustomer.POCountry;

    ROSSDataRemoteOrder.Customer.HomeNo := GlbCustomer.HomeNo;
    ROSSDataRemoteOrder.Customer.MobileNo := GlbCustomer.MobileNo;
    ROSSDataRemoteOrder.Customer.WorkNo := GlbCustomer.WorkNo;
    ROSSDataRemoteOrder.Customer.FaxNo := GlbCustomer.FaxNo;

    ROSSDataRemoteOrder.Customer.DOB := GlbCustomer.DOB;
    ROSSDataRemoteOrder.Customer.BirthDay := GlbCustomer.BirthDay;
    ROSSDataRemoteOrder.Customer.BirthMonth := GlbCustomer.BirthMonth;

    ROSSDataRemoteOrder.Customer.Anniversary := GlbCustomer.Anniversary;
    ROSSDataRemoteOrder.Customer.AnniversaryDay := GlbCustomer.AnniversaryDay;
    ROSSDataRemoteOrder.Customer.AnniversaryMonth := GlbCustomer.AnniversaryMonth;

    ROSSDataRemoteOrder.Customer.EMail := GlbCustomer.EMail;
    ROSSDataRemoteOrder.Customer.Notes := GlbCustomer.Notes;
    ROSSDataRemoteOrder.Customer.Customer := GlbCustomer.Customer;
    ROSSDataRemoteOrder.Customer.VIP := GlbCustomer.VIP;

    ROSSDataRemoteOrder.Customer.Gender := StrToChar(GlbCustomer.Gender);
    ROSSDataRemoteOrder.Customer.FavoriteItem := GlbCustomer.FavoriteItem;
    ROSSDataRemoteOrder.Customer.Occupation := GlbCustomer.Occupation;
    ROSSDataRemoteOrder.Customer.Nationality := GlbCustomer.Nationality;
    ROSSDataRemoteOrder.Customer.OptOut := GlbCustomer.OptOut;

    if (GlbCustomer.Photo.Size > 0) then begin
      GlbCustomer.Photo.Position := 0;
      with ROSSDataRemoteOrder.BlobList.NewBlobObject do begin
        Blob.LoadFromStream(GlbCustomer.Photo);
        ROSSDataRemoteOrder.Customer.PhotoBOID := BlobID;
      end;
    end;
    ROSSDataRemoteOrder.Customer.LinkCode := GlbCustomer.LinkCode;

//OrderLines
    I := 0;
    while (I < OLList.Count) do begin // JEH 18/08/2001 1 -> 0
      aOL := OLList.Items[I];
      if (aOL.FORB = 'F') then begin
        FoodExists := True;
      end;
      ROSSRemoteOrderLine := TROSSRemoteOrderLine.Create;
      ROSSDataRemoteOrder.ListOLs.Add(ROSSRemoteOrderLine);
      ROSSRemoteOrderLine.ItemID := aOL.ItemID;
      ROSSRemoteOrderLine.CourseID := aOL.CourseID;
      ROSSRemoteOrderLine.Qty := aOL.Qty;
      ROSSRemoteOrderLine.IsSurcharge := False;
      ROSSRemoteOrderLine.OpenPrice := aOL.OpenPrice;
      if ((aOL.OpenPrice) and (AppDetails.OrderOpenPriceNameChange and (aOL.ItemString <> ''))) then begin
        ROSSRemoteOrderLine.OpenPriceItemAbbrev := aOL.ItemString;
      end
      else begin
        ROSSRemoteOrderLine.OpenPriceItemAbbrev := '';
      end;
      ROSSRemoteOrderLine.UnitPrice := aOL.UnitPrice;
      ROSSRemoteOrderLine.PriceLevel := aOL.OLPriceLevel;

      ROSSRemoteOrderLine.ILAmount := aOL.ToPay;
      if (aOL.DiscountSchemeID > 0) then
        ROSSRemoteOrderLine.ILDiscountSchemeID := aOL.DiscountSchemeID
      else ROSSRemoteOrderLine.ILDiscountSchemeID := GlbAccount.DiscountSchemeID;

      ROSSRemoteOrderLine.ComboID := aOL.ComboID;
      ROSSRemoteOrderLine.ComboGroup := aOL.ComboGroupID;

      if (FDiscountAmount <= 0) then begin
        ROSSRemoteOrderLine.ILDiscountAmount := 0.00;
      end
      else begin
        //if it's the last line, allocate whatever hasn't been pro rata-ed
        if (I = (OLList.Count - 1)) then begin
          ROSSRemoteOrderLine.ILDiscountAmount := FDiscountAmount - SoFarDiscount;
        end
        else
        begin
          //pro rata discount and accumulate discount pro rata-ed so far
          ILDiscount := aOL.AllowedDiscount;
          ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          SoFarDiscount := SoFarDiscount + ILDiscount;
          SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
          ROSSRemoteOrderLine.ILDiscountAmount := ILDiscount;
        end;
      end;

      //AM 23Dec99 Account Balance
      if (AppDetails.TaxExclusivePrices) then begin   //Jon 18-03-2002
        BalanceDelta := BalanceDelta + GetRoundedUpDown(((ROSSRemoteOrderLine.ILAmount - ROSSRemoteOrderLine.ILDiscountAmount) * (100 + aOL.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
      end
      else begin
        BalanceDelta := BalanceDelta + (ROSSRemoteOrderLine.ILAmount - ROSSRemoteOrderLine.ILDiscountAmount);
      end;
      BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
      ROSSRemoteOrderLine.ILLoyaltyFreeQty := aOL.LoyaltyFreeQty;
      ROSSRemoteOrderLine.ILLoyaltyFreeDiscount := aOL.LoyaltyFreeDiscount;

      Inc(I);
      while ((I < OLList.Count) and (TOrderLine(OLList.Items[I]).ItemID = 0)) do begin
        aOL := OLList.Items[I];

        ROSSModifier := ROSSRemoteOrderLine.NewModifier;
        ROSSModifier^.ModifierID := aOL.ModifierID;
        ROSSModifier^.Modifier := aOL.ItemString;
        ROSSModifier^.ModGroupID := aOL.ModGroupID;
        if (AppDetails.EnableModifierFunction) then begin
          ROSSModifier^.ModifierFunction := aOL.ModifierFunction;
        end
        else begin
          ROSSModifier^.ModifierFunction := 0;
        end;
        ROSSModifier^.InsertModPrice := ((AppDetails.SmartModPricing <> 0) or (AppDetails.ZeroPriceItemsStayZero and (aOL.ToPay = 0)));
        ROSSModifier^.ModPrice := aOL.ModPrice;
        ROSSModifier^.ModQtyOption := aOL.ModQtyOption;
        ROSSModifier^.ModQtyOrdered := aOL.ModQtyOrdered;
        ROSSModifier^.ModQtyIncluded := aOL.ModQtyIncluded;

        Inc(I);
      end;
    end;

//Surcharges
    if (((TLList.Count > 0) or OnAccount) and (SurchargeList.Count > 0)) then begin
      for I := 0 to (SurchargeList.Count - 1) do begin
        aSI := SurchargeList.Items[I];
        if (aSI.Amount <> 0) then begin
          ROSSRemoteOrderLine := TROSSRemoteOrderLine.Create;
          ROSSDataRemoteOrder.ListOLs.Add(ROSSRemoteOrderLine);
          ROSSRemoteOrderLine.IsSurcharge := True;
          ROSSRemoteOrderLine.ItemID := aSI.ItemID;
          ROSSRemoteOrderLine.Qty := 1;
          ROSSRemoteOrderLine.UnitPrice := aSI.Amount;

          ROSSRemoteOrderLine.ILAmount := aSI.Amount;
          ROSSRemoteOrderLine.ILDiscountAmount := 0;

          //For the rest the defaults will cover it

          if (AppDetails.TaxExclusivePrices) then begin   //Jon 18-03-2002
            BalanceDelta := BalanceDelta + (aSI.Amount * (100 + aSI.SalesTaxPercent)) / 100;
          end
          else begin
            BalanceDelta := BalanceDelta + (aSI.Amount);
          end;
          BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
        end;
      end;
    end;

//Tender Lines
    for I := 0 to (TLList.Count - 1) do begin
      New(ROSSTenderLine);

      TL := TLList.Items[I];

      ROSSTenderLine^.TenderLineTypeID := TL.TLTypeID;
      ROSSTenderLine^.TenderLineAmount := TL.TLTender;
      ROSSTenderLine^.TenderLineTip := TL.TLTip;
      ROSSTenderLine^.RoundingAmount := TL.TLRounding;
      ROSSTenderLine^.ChangeAmount := False;
      ROSSTenderLine^.TenderLineChange := TL.TLChange;
      ROSSTenderLine^.TenderLineNotes := TL.TLNotes;

      ROSSDataRemoteOrder.ListTLs.Add(ROSSTenderLine);

      BalanceDelta := BalanceDelta - (ROSSTenderLine^.TenderLineAmount - ROSSTenderLine^.TenderLineTip + ROSSTenderLine^.RoundingAmount);
      BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
    end;

    //create a tender line for the change
    if ((FChange > 0) and (not SurplusOnAccount)) then begin
      New(ROSSTenderLine);

      ROSSTenderLine^.TenderLineTypeID := 4;   //Cash
      ROSSTenderLine^.TenderLineAmount := -FChange;
      ROSSTenderLine^.TenderLineTip := 0;
      ROSSTenderLine^.RoundingAmount := 0;
      ROSSTenderLine^.ChangeAmount := True;
      ROSSTenderLine^.TenderLineChange := 0;
      ROSSTenderLine^.TenderLineNotes := '';

      ROSSDataRemoteOrder.ListTLs.Add(ROSSTenderLine);

      BalanceDelta := BalanceDelta - (ROSSTenderLine^.TenderLineAmount - ROSSTenderLine^.TenderLineTip + ROSSTenderLine^.RoundingAmount);
      BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
    end;

    if ((TestRemoteLocationOption(rlsoRemote)) and (AppDetails.RemoteLocationLevel = 2)) then begin
      if ((not ROSSTransMasIDs.TransTerminalID(ROSSDataRemoteOrder.General.TerminalID, ROSSDataRemoteOrder.General.TerminalID, False)) or
          (not ROSSTransMasIDs.TransStaffID(ROSSDataRemoteOrder.General.StaffID, ROSSDataRemoteOrder.General.StaffID, False)) or
          (not ROSSTransMasIDs.TransAccountID(ROSSDataRemoteOrder.General.AccountID, ROSSDataRemoteOrder.General.AccountID, True)) or
          (not ROSSTransMasIDs.TransLoyaltyRewardID(ROSSDataRemoteOrder.General.LoyaltyRewardID, ROSSDataRemoteOrder.General.LoyaltyRewardID, True)) or
          (not ROSSTransMasIDs.TransAddressMapLocationID(ROSSDataRemoteOrder.General.DelAddressMapLocationID, ROSSDataRemoteOrder.General.DelAddressMapLocationID, True)) or
          (not ROSSTransMasIDs.TransSaleCategoryID(ROSSDataRemoteOrder.General.OrderSaleCategoryID, ROSSDataRemoteOrder.General.OrderSaleCategoryID, False)) or

          (not ROSSTransMasIDs.TransPerorgID(ROSSDataRemoteOrder.Customer.PerorgID, ROSSDataRemoteOrder.Customer.PerorgID, False)) or
          (not ROSSTransMasIDs.TransAddressMapLocationID(ROSSDataRemoteOrder.Customer.AddressMapLocationID, ROSSDataRemoteOrder.Customer.AddressMapLocationID, True))) then begin
        ShowMsg(sSysNeedsToSync);
        Result := False;
        Exit;
      end;

      for I := 0 to (ROSSDataRemoteOrder.ListOLs.Count - 1) do begin
        ROSSRemoteOrderLine := ROSSDataRemoteOrder.ListOLs.Items[I];
        if ((not ROSSTransMasIDs.TransItemID(ROSSRemoteOrderLine.ItemID, ROSSRemoteOrderLine.ItemID, False)) or
            (not ROSSTransMasIDs.TransCourseID(ROSSRemoteOrderLine.CourseID, ROSSRemoteOrderLine.CourseID, True))) then begin
          ShowMsg(sSysNeedsToHaveItemsToSync);
          Result := False;
          Exit;
        end;
        for J := 0 to (ROSSRemoteOrderLine.ListModifiers.Count - 1) do begin
          ROSSModifier := ROSSRemoteOrderLine.ListModifiers.Items[J];
          if ((not ROSSTransMasIDs.TransModifierID(ROSSModifier.ModifierID, ROSSModifier.ModifierID, True)) or
              (not ROSSTransMasIDs.TransModGroupID(ROSSModifier.ModGroupID, ROSSModifier.ModGroupID, True))) then begin
            ShowMsg(sSysNeedsToHaveModifiersToSync);
            Result := False;
            Exit;
          end;
        end;
      end;
    end;

    if (AppDetails.PDEnabled) then begin
      if (TLList.Count > 1) then begin
        DMComponents.PDOutputItemLine('TENDERED', GetRoundedUpDown(FPayment + FChange, AppDetails.DecimalPlaces), 0);
      end
      else if (TLList.Count = 1) then begin
        TL := TLList.Items[0];
        DMComponents.PDOutputItemLine(GetTLTType(TL.TLType), GetRoundedUpDown(TL.TLTender, AppDetails.DecimalPlaces), 0);
      end;
      DMComponents.PDOutputItemLine('CHANGE', GetRoundedUpDown(FChange, AppDetails.DecimalPlaces), 1);
    end;

    ROSSDataRemoteOrder.General.PrintJobSummary := True;
    ROSSDataRemoteOrder.General.PrintJobCFOrder := (AppDetails.CopyFood and FoodExists);
    ROSSDataRemoteOrder.General.AccountBalanceDelta := BalanceDelta;
    ROSSDataRemoteOrder.General.AccountOnAccount := OnAccount;

    //    Send the Order Doofrey here

    if (not SendROSSOrderTransaction(ROSSDataRemoteOrder)) then begin
      Result := False;
      Exit;
    end;

    ClearStatus;
    BackPagesList.Clear;
    SetDoingRefund(False, True);
    SetDoingExchange(False, True);
    SetDoingWaste(False, True);
    PriceLevelChange(0, 2);
    CourseChange(0, '', 2, False);    //Jon 28-02-2002
    NextItemButtonStateChange(1);   //Jon 22-04-2002

    ModifierFunctionChange(1);               //Jon 14-8-2001 returns mod function to +
    SetNextQty(1);
  finally
    ROSSDataRemoteOrder.Free;
    ROSSTransMasIDs.Free;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.ResetCashSale(HoldChangeWindow: Boolean; ResetPhoneOrder: Boolean): Boolean;
var
  strBalDue: string;
  strStillDue: string;
  strChange: string;
begin
  ClearStatus;
  cmdExchange.Enabled := True;
  BackPagesList.Clear;
  SetDoingRefund(False, True);
  SetDoingExchange(False, True);
  SetDoingWaste(False, True);
  PriceLevelChange(0, 2);
  CourseChange(0, '', 2, False);    //Jon 28-02-2002
  NextItemButtonStateChange(1);   //Jon 22-04-2002

  ModifierFunctionChange(1);               //Jon 14-8-2001 returns mod function to +
  SetNextQty(1);


  //AMANDA 27.01.01 - moved above ClearOrderLines
  TableSelected(False);
  FDelivery := '';
  FDeliverySetByUser := False;
  FTabNotes := '';
  ChangeHeldGroup(0, '');     //Jon 12-10-2004
  PromptedWhere := False;      //Jon 12-01-2006

  // JEH 18/08/2001 Save so can redisplay when done
  strBalDue := txtBalDue.Caption;
  strStillDue := txtStillDue.Caption;
  strChange := txtCChange.Caption;

  sExternalAccountIntegration.ClearPresentedCards;   //do before CalculateToPay (called in ClearAllOrderLines) so cmdExternalAcc is updated

  ClearAllOrderLines;
  ClearAllTenderLines;
  ClearAllSurcharges;

  ClearListObjects(FExchangeOLs);

  ResetGlobalVariables;

  DoPhoneOrder(False, (PhoneOrderPersistant and (not ResetPhoneOrder)));
  GlbScan.SwipedStaffCard := False; //akm 26.11.01
  ConfirmedAccountDiscountSelection := False;
  ConfirmComboDiscountSelection := False;
  CheckStaffLoginButtons;
  if (GlbEvent.EventID = 0) then
    DeselectAccount
  else if (GlbEvent.AccountID > 0) then
    LoadEventAccount;      //to ensure we setup the Discount Calculator again that is cleared in ResetGlobalVariables above
  //InitialiseAccountInfo;

  if (HoldChangeWindow) then begin
    // JEH 18/08/2001 Redisplay saved values
    txtBalDue.Caption := strBalDue;
    txtStillDue.Caption := strStillDue;
    if (not SurplusOnAccount) then
      txtCChange.Caption := strChange;
  end;

  SetCurrentTime;   //Jon 25-06-2002
  //AKM 4.3.1
  if ((AppDetails.QSSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.QSSaleCategoryID))) then begin
    if ((AppDetails.CashSaleCategory = '') or (not SelectSaleCategory(AppDetails.CashSaleCategory))) then begin
      if ((AppDetails.DefaultSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.DefaultSaleCategoryID))) then begin
        //Well we didn't find one
      end;
    end;
  end;

//  Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, ' OK');
  Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, ' OK');

  //if (AppDetails.PMSType in [pmsFidelio, pmsPhoenix]) then begin
  if ((AppDetails.PMSType <> pmsNone) and (AppDetails.CashSaleAutoComplete)) then begin
    cmdToPayPMS.Visible := True;
    cmdQuickCash.Visible := False;
  end;

  DoingServiceTransaction := False;

  ItemAutoTenderActive := False;

  TheVoidOrderID := 0;
  FVoidID := 0;
  ClearComboDiscounts;

  FCPFNumber := '';
  FInvCOO := '';
  FInvCCF := '';
  FTotalNCMTax := 0;
  FAccountCPFNumberChanged := False;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.ClearPayment;

  FDriveThroughGroupIDs.Clear;

  FToWaste := False;
  FVoidReasonID := 0;
  FExp1 := '';
  FExp2 := '';

  if AppDetails.EnableFiscalPrinting then
  begin
    ForceCPFNumber := False;
    cmdCPF.Enabled := True;
    cmdCPF.Visible := True;
    cmdCNPJ.Visible := True;
    cmdCNPJ.Enabled := True;
    FCPFNumber := '';
//    FIsFiscalOpened := False; //Might need to cancel if SFiscalPrinting.OpenedFiscalInvoice is True
  end;
  cmdSelectAccount.Enabled := True;
  cmdAccountNoLookup.Enabled := True;
  cmdDeselectAccount.Enabled := True;
  cmdDiscountPercent.Enabled := False;
  cmdDiscountAmount.Enabled := False;

 IniGoodyTrans;

  if (pnlToPay.Visible and grdOLs.Enabled) then
  begin
    grdOLs.SetFocus;  //7-10-2010
  end;
  Result := True;
end;
{******************************************************************************}
function TformQuickSaleUser.TryCashSaleRefund(Print: Boolean): Boolean;
type
  TOrdLn = record
    OrderLineID: Integer;
    Qty: Currency;
  end;

var
  I, J, K: Integer;
  ValueFound: Boolean;

  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  TheLine: Integer;

  //AM 23Dec99 Account Balance
  BalanceDelta: Currency;

  TheVoidID: Integer;
  TheOrderID: Integer;

  TheTenderID: Integer;
  TheInvoiceID: Integer;
  TheInvoiceNo: Integer;

  OrdLnList: TList;
  OrdLn: ^TOrdLn;
  LibExists: Boolean;
  NZAExists: Boolean;
  FidelioExists: Boolean;  //Jon 23-08-2004
  PhoenixExists: Boolean;
  Micros4700Exists: Boolean;
  AgoraExists: Boolean;
  RMSExists: Boolean;
  RoomChargeExists: Boolean;

  OrderLine: TOrderLine;
  ModOL: TOrderLine;
  TL: TTL;
  aSI: TSurchargeItem;
  CreatingInvoice: Boolean;

  PrinterList: TIntegerList;
  TheSP: TIB_StoredProc;
  TheTR: TIB_Transaction;
  SPNumber: Integer;

  OrderLineOrder, OrderLineModifierOrder: Integer;
  OnTableForWastage: Boolean;
  OrderedModQty: Integer;
  extAccountsMsg: string;
  tmpItemGroup: TItemGroup;
begin
  Result := False;
  OnTableForWastage := False;
  PrinterList := TIntegerList.Create;
  PrinterList.AutoSort := False;
  PrinterList.AllowDuplicates := False;
  OrdLnList := TList.Create;

  if (AppDetails.OrdersLockWait) then begin
    SPNumber := 3;
  end
  else begin
    SPNumber := 1;
  end;

  TheSP := dm.GetSP(SPNumber);
  TheTR := dm.GetTR(SPNumber);

  try
    //AM 23Dec99 Account Balance
    BalanceDelta := 0.00;
    TheInvoiceID := 0;
    TheInvoiceNo := 0;
    TheTenderID := 0;
    TheVoidID := 0;
    OrderLineOrder := 0;

    if (AppDetails.CashSaleAllowZeroPriceInvoices) then begin
      ValueFound := True;
    end
    else begin
      //check that the invoice will be greater than zero, else don't create one
      ValueFound := False;
      //if chkTable.State = cbUnChecked then begin //AMANDA 19/10/00 - removed chkTable
      if (not OnTable) then begin
        if (TLList.Count > 0) then begin
          ValueFound := True;
        end
        else begin
          for I := 0 to OLList.Count - 1 do begin
            if TOrderLine(OLList[I]).ToPay > 0 then begin
              ValueFound := True;
              Break;
            end;
          end;
        end;
      end;
    end;

    //loop through order lines
    for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
      OrderLine := OLList.Items[I];
      //for each order line loop through walist twice, first for printer, then for ccprinter
       // JEH 18/08/2001 1 -> 0
      if ((OrderLine.PrinterID <> 0) and (not OrderLine.ModsChangePrinter)) then begin
        AddPrinterToPrinterList(PrinterList, OrderLine.PrinterID);

        //loop through walist, if repeat printer not found add
        if ((OrderLine.RepeatPrinterID > 0) and (not AppDetails.CashSaleDisableRepeatPrinter)) then begin
          AddPrinterToPrinterList(PrinterList, OrderLine.RepeatPrinterID);
        end;
      end;
    end;
    AddCCPrintersToPrinterList(PrinterList);

    //AMANDA - if not on table, will be printing, and no destination specified, set to WAName
    if ((not OnTable) and (PrinterList.Count > 0) and (FDelivery = '')) then begin
      FDelivery := AppDetails.WAName;
    end;

    Result := True;

    SetStatus(sSaving);

    //if there are tender lines, kick cash draw ASAP
    if (TLList.OpenDrawerExists(not SurplusOnAccount)) then
    begin
      if ((AppDetails.EnableFiscalPrinting) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0)))) then
      begin
        SFiscalPrinting.KickDrawer;
      end
      else
      begin
        KickCashDraw(GlbLogin.TillDriverName, GlbLogin.TillDrawKickString);
      end;
    end;

    try
      if (not dm.TRStartTest(SPNumber)) then begin
        Exit;
      end;
      with TheSP do begin
        if (AppDetails.OrdersLockWait) then begin
          SetStoredProcName('SETORDERMUTEX', SPNumber);  ///setordermutex
          I := 0;
          repeat
            Inc(I);
            ExecProc;
          until((FieldByName('errorcode').IsNull) or (I >= 3));

          if (not FieldByName('errorcode').IsNull) then begin
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
          end;
        end;

        //if (chkTable.State = cbChecked) then begin
        if (OnTable) then begin
          //check table still open
          SetStoredProcName('CHECK_CLOSED', SPNumber); //check_closed

          ParamByName('groupid').AsInteger := GlbTable.GroupID;
          ExecProc;
          if (not FieldByName('whenclosed').IsNull) then begin
            TheTR.Rollback;   //Jon 10-07-2003
            ShowQuickMsg(sNoOrderSinceAcctClosed);
            Result := False;
            Exit;
          end;
        end;

        if ((DoingWaste) and (AppDetails.PrintSummaryForWastageMode) and (AppDetails.DefaultWastageTab > 0)) then
        begin
          if not OpenDefaultTabForWastage then
          begin
            TheTR.Rollback;
            Result := False;
            Exit;
          end
          else
            OnTableForWastage := True;
        end;

        //create order record
        SetStoredProcName('INSERT_ORDER', SPNumber);   //insert_order

        ParamByName('outletid').AsInteger := AppDetails.OutletID;

        //if (chkTable.State = cbChecked) then
        IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, (OnTable or OnTableForWastage));
        ParamByName('rush').AsInteger := 0;
        StringAsParam(ParamByName('location'), FDelivery);
        ParamByName('loginid').AsInteger := GlbLogin.LoginID;

        //may be null from tables already open when Sale Categories introduced
        IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);

        //if (chkTable.State = cbChecked) then
        IntegerAsParamIfBool(ParamByName('tableno'), GlbTable.TableNo, (OnTable or OnTableForWastage));

        ParamNull(ParamByName('eatintakeaway'));
        IntegerAsParam(ParamByName('heldgroupid'), FHeldGroupID);
        ParamByName('CPFNumber').AsString := Trim(FCPFNumber);
        ExecProc;
        TheOrderID := FieldByName('orderid').AsInteger;
        WBOrders.RefreshID := TheOrderID;

        //if a value found and not for a table
        //if ValueFound and (chkTable.State = cbUnChecked) then begin
        CreatingInvoice := ((ValueFound) and (not OnTable) and (not DoingWaste));

        if (CreatingInvoice) then
        begin
          //create invoice record
          SetStoredProcName('INSERT_INVOICE', SPNumber);          //insert_invoice

          ParamByName('discountrate').AsCurrency := FDiscountPercent;
          IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          ParamNull(ParamByName('groupid'));

          //may be null from tables already open when Sale Categories introduced
          IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);
          ParamNull(ParamByName('refundnote'));
          ParamByName('invoicetype').AsString := 'C';
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamByName('salestax').AsCurrency := -FSalesTax;    //Jon 30-5-2001
          ParamByName('cashsaleorderid').AsInteger := TheOrderID;   //Jon 14-11-2001
          StringAsParam(ParamByName('cpfnumber'), FCPFNumber);
          IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);
          if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          begin
            StringAsParam(ParamByName('FiscalCOO'), SFiscalPrinting.COO);
            StringAsParam(ParamByName('FiscalCCF'), SFiscalPrinting.CCF);
          end;
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;   //Jon 10-07-2003
            Exit;
          end
          else begin
            TheInvoiceID := FieldByName('invoiceid').AsInteger;
            TheInvoiceNo := FieldByName('invoiceno').AsInteger;
            WBCreditNotes.RefreshID := TheInvoiceID;

            if ((AppDetails.ClearItemsVoid > 0) and (FVoidID > 0) and (TheInvoiceID > 0)) then
            begin
              with DM.qrGeneral do
              begin
                Close;
                SQL.Clear;
                SQL.Add('UPDATE VOIDLN SET');
                SQL.Add('       INVOICEID = :invoiceid');
                SQL.Add(' WHERE (VOIDID = :VOIDID)');
                ParamByName('invoiceid').AsInteger := TheInvoiceID;
                ParamByName('voidid').AsInteger := FVoidID;
                ExecSQL;
              end;
            end;
          end;
        end;

        //process External Accounts here
        try
          if (AllowTendering) then
          begin
            sExternalAccountIntegration.SPNumber := SPNumber;
            if (sExternalAccountIntegration.ActionPresentedCards(TheInvoiceID, TheInvoiceNo, OLList, SurchargeList, TLList, True)) then
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
                Result := False;
                TheTR.Rollback;
                Exit;
              end;
            end
            else if (not sExternalAccountIntegration.CanContinueSale) then
            begin
              Result := False;
              TheTR.Rollback;
              Exit;
            end
          end;
        except
          ShowMsg('Unexpected External Accounts Issue');
          LogMessage(ltExternalAccounts, 'Unexpected External Accounts Issue');
          Result := False;
          TheTR.Rollback;
          Exit;
        end;

        //for each cash sale line added
        //create order line, invoice line and the order line's modifier record
        SoFarDiscount := 0.000;
        SoFarComboDiscount := 0;
        for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
          OrderLine := OLList.Items[I];

          //if an order line as opposed to a modifier
          if (OrderLine.ItemID > 0) then begin // JEH 18/08/2001 1 -> 0
            Inc(OrderLineOrder);
            //create order line
            SetStoredProcName('INSERT_ORDERLINE', SPNumber);   //insert_orderline
            IntegerAsParam(ParamByName('courseid'), OrderLine.CourseID);    //Jon 28-02-2002
            if (DoingWaste) then
            begin
              ParamByName('qty').AsCurrency := OrderLine.Qty;               //Jon 24-9-2001
            end
            else begin
              ParamByName('qty').AsCurrency := 0;        //Jon 24-9-2001
            end;
            //        create a list and record the orderlineid and the qty in it. Then later on enter in the voidlines
            //        with the orderlineid and the qty/wastage from this list.
            ParamNull(ParamByName('notes'));
            ParamByName('happyhour').AsInteger := 0;
            ParamNull(ParamByName('positions'));
            ParamByName('itemid').AsInteger := OrderLine.ItemID; // JEH 18/08/2001 1 -> 0 + below...
            ParamByName('orderid').AsInteger := TheOrderID;
            //          ParamByName('printerid').AsInteger := TOL(OLList[I-0]^).PrinterID;
            //          IntegerAsParam(ParamByName('repeatprinterid'), TOL(OLList[I]^).RepeatPrinterID);
            if (OrderLine.ModsChangePrinter) then begin
              J := I + 1;
              while ((J < OLList.Count) and (TOrderLine(OLList[J]).PrinterID = 0) and (TOrderLine(OLList[J]).ItemID = 0)) do begin
                Inc(J);
              end;
              if ((J < OLList.Count) and (TOrderLine(OLList[J]).PrinterID > 0) and (TOrderLine(OLList[J]).ItemID = 0)) then begin
                ModOL := OLList.Items[J];
                ParamByName('printerid').AsInteger := ModOL.PrinterID;
                IntegerAsParam(ParamByName('repeatprinterid'), ModOL.RepeatPrinterID);
              end
              else begin
                OrderLine.ModsChangePrinter := False;
              end;
            end;

            if (not OrderLine.ModsChangePrinter) then begin
              IntegerAsParam(ParamByName('printerid'), OrderLine.PrinterID);
              IntegerAsParam(ParamByName('repeatprinterid'), OrderLine.RepeatPrinterID);
            end;

            BoolAsParam(ParamByName('held'), False);

            if (OrderLine.OpenPrice) then begin
              ParamByName('openprice').AsCurrency := OrderLine.UnitPrice;
              StringAsParamIfBool(ParamByName('openpriceitemabbrev'), OrderLine.ItemString, (AppDetails.OrderOpenPriceNameChange and (OrderLine.ItemString <> '')));
            end
            else begin
              ParamNull(ParamByName('openprice'));
              ParamNull(ParamByName('openpriceitemabbrev'));
            end;

            if ((OrderLine.OLPriceLevel >= 1) and (OrderLine.OLPriceLevel <= 6)) then begin
              ParamByName('pricelevel').AsInteger := OrderLine.OLPriceLevel;
            end
            else begin
              ParamByName('pricelevel').AsInteger := 0;
            end;

            ParamByName('currenthour').AsInteger := CurrentHour;   //Jon 24-06-2002
            ParamByName('currentminute').AsInteger := CurrentMin;  //Jon 24-06-2002
            ParamByName('currentday').AsInteger := CurrentDay;     //RPC 28-09-2010
            IntegerAsParam(ParamByName('orderlineorder'), orderlineorder);
            IntegerAsParam(ParamByName('comboid'), OrderLine.ComboID);
            IntegerAsParam(ParamByName('combogroup'), OrderLine.ComboGroupID);
            IntegerAsParam(ParamByName('comboindex'), OrderLine.ComboIndex);
            ParamByName('comboqty').AsCurrency := OrderLine.ComboQty;
            BoolAsParam(ParamByName('fixthecombo'), OrderLine.FixThisComboPart);
            BoolAsParam(ParamByName('freecomboitem'), OrderLine.FreeComboItem);
            ExecProc;
            OrderLine.OrderLineID := FieldByName('orderlineid').AsInteger;
            New(OrdLn);
            OrdLn^.OrderLineID := OrderLine.OrderLineID;
            OrdLn^.Qty := OrderLine.Qty;
            OrdLnList.Add(OrdLn);
          end;
        end;

        for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
          OrderLine := OLList.Items[I];

          //if an order line as opposed to a modifier
          if (OrderLine.ItemID > 0) then begin // JEH 18/08/2001 1 -> 0
            //loop through any mods for this ol, until hit an ol or the last line
            TheLine := I;  // JEH 18/08/2001 1 -> 0
            //while not on the last line, try the next line for a mod, if not break loop
            OrderLineModifierOrder := 0;
            while (TheLine <> (OLList.Count - 1)) do begin  // JEH 18/08/2001 take off the +1
              Inc(TheLine);
              ModOL := OLList.Items[TheLine];
              if ((ModOL.ItemID = 0) and (ModOL.ModifierID <> 0)) then begin
                //create any modifiers for order line
                SetStoredProcName('INSERT_ORDERLINEMODIFIER', SPNumber);   //insert_orderlinemodifier

                ParamByName('orderlineid').AsInteger := OrderLine.OrderLineID;
                IntegerAsParamIfBool(ParamByName('modifierid'), ModOL.ModifierID, (ModOL.ModifierID > 0));
                StringAsParam(ParamByName('modifier'), ModOL.ItemString);
                IntegerAsParam(ParamByName('modgroupid'), ModOL.ModGroupID);
                IntegerAsParamIfBool(ParamByName('modfunction'), ModOL.ModifierFunction, AppDetails.EnableModifierFunction);

                if (ModOL.ModifierFunction = 2) then
                begin
                  ParamByName('modprice').AsCurrency := -ModOL.ModPrice;
                end
                else begin
                  ParamByName('modprice').AsCurrency := ModOL.ModPrice;
                end;

                IntegerAsParam(ParamByName('orderlinemodifierorder'), orderlinemodifierorder);

                if (ModOL.ModQtyOption = 0) then
                  OrderedModQty := Round(ModOL.ModQtyOrdered-ModOL.ModQtyIncluded)
                else
                  OrderedModQty := 1;

                for K := 1 to OrderedModQty do
                begin
                  Inc(OrderLineModifierOrder);
                  IntegerAsParam(ParamByName('orderlinemodifierorder'), OrderLineModifierOrder);
                  ExecProc;
                end;

                OrderedModQty := Round(ModOL.ModQtyIncluded);
                ParamByName('modprice').AsCurrency := 0;
                for K := 1 to OrderedModQty do
                begin
                  Inc(OrderLineModifierOrder);
                  IntegerAsParam(ParamByName('orderlinemodifierorder'), OrderLineModifierOrder);
                  ExecProc;
                end;
              end
              else begin
                Break;
              end;
            end;
          end;
        end;

        for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
          OrderLine := OLList.Items[I];

          //if an order line as opposed to a modifier
          if (OrderLine.ItemID > 0) then begin // JEH 18/08/2001 1 -> 0
            //if line value found and not for a table
            //create invoice line
            //if (TOL(OLList[I-1]^).Price > 0) and (chkTable.State = cbUnChecked) then begin
            if (((OrderLine.ToPay > 0) or (AppDetails.CashSaleAllowZeroPriceInvoices)) and (CreatingInvoice) and (TheInvoiceID > 0)) then begin
              SetStoredProcName('INSERT_INVOICELINE', SPNumber);

              OrderLine.ILAmount := -OrderLine.ToPay;
              ParamByName('ilamount').AsFloat := OrderLine.ILAmount;

              //if IsNormalLoyalty then//check
              begin
                if OrderLine.ComboID > 0 then
                begin
                  if OrderLine.LastComboItem then
                  begin
                    ILDiscount := FComboDiscount - SoFarComboDiscount;
                  end
                  else
                  begin
                    ILDiscount := OrderLine.AllowedDiscount;
                  end;

                  ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                  SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
                  SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
                end
                else
                begin
                  //if it's the last line, allocate whatever hasn't been pro rata-ed
                  if OrderLine.LastNormalItem then
                  begin
                    ILDiscount := FDiscountAmount - SoFarDiscount;
                  end
                  else
                  begin              //pro rata discount and accumulate discount pro rata-ed so far
                    ILDiscount := OrderLine.AllowedDiscount;
                    ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                    SoFarDiscount := SoFarDiscount + ILDiscount;
                    SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
                  end;
                end;

                ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                OrderLine.ILDiscount := ILDiscount;
              end;
              ParamByName('discountamount').AsCurrency := -OrderLine.ILDiscount;
              ParamByName('invoiceid').AsInteger := TheInvoiceID;
              ParamByName('orderlineid').AsInteger := OrderLine.OrderLineID;
              if (OrderLine.DiscountSchemeID > 0) then
              begin
                IntegerAsParam(ParamByName('discountschemeid'), OrderLine.DiscountSchemeID);
              end
              else
              begin
                IntegerAsParam(ParamByName('discountschemeid'), GlbAccount.DiscountSchemeID);
              end;
              ParamByName('FISCALSEQNO').AsString := '';
              if AppDetails.EnableFiscalPrinting then
                ParamByName('FISCALTAX').AsInteger := 1;
              //AM 23Dec99 Account Balance
              if (AppDetails.TaxExclusivePrices) then begin   //Jon 18-03-2002
                BalanceDelta := BalanceDelta + ((OrderLine.ILAmount + OrderLine.ILDiscount) * (100 + OrderLine.SalesTaxPercent)) / 100;
              end
              else begin
                BalanceDelta := BalanceDelta + (OrderLine.ILAmount + OrderLine.ILDiscount);
              end;
              BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
              ExecProc;
              OrderLine.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

              if (AppDetails.FiscalType = fiscalTaxCore) then
              begin
                SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
                ParamByName('invoicelineid').AsInteger := OrderLine.InvoiceLineID;
                StringAsParam(ParamByName('taxlabel'), OrderLine.TaxCoreLabel);
                ExecProc;
            end;
          end;
        end;
        end;

        if ((SurchargeList.Count > 0) and (CreatingInvoice)) then begin
          for I := 0 to (SurchargeList.Count - 1) do begin
            aSI := SurchargeList.Items[I];
            if (aSI.Amount <> 0) then
            begin
              //create order line
              SetStoredProcName('INSERT_ORDERLINE', SPNumber);      //insert_orderline
              ParamNull(ParamByName('courseid'));
              ParamByName('qty').AsCurrency := 0;
              ParamNull(ParamByName('notes'));
              ParamByName('happyhour').AsInteger := 0;
              ParamNull(ParamByName('positions'));
              ParamNull(ParamByName('numpos'));
              ParamByName('itemid').AsInteger := aSI.ItemID;
              ParamByName('orderid').AsInteger := TheOrderID;
              ParamNull(ParamByName('printerid'));
              ParamNull(ParamByName('repeatprinterid'));
              ParamByName('held').AsInteger := 0;
              ParamByName('openprice').AsCurrency := aSI.Amount;
              ParamNull(ParamByName('openpriceitemabbrev'));
              ParamByName('pricelevel').AsInteger := 0;
              ParamByName('currenthour').AsInteger := -1;   //Jon 24-06-2002
              ParamByName('currentminute').AsInteger := -1;  //Jon 24-06-2002
              ParamByName('currentday').AsInteger := CurrentDay;     //RPC 28-09-2010
              ParamNull(ParamByName('orderlineorder'));
              ParamNull(ParamByName('comboid'));
              ParamNull(ParamByName('combogroup'));
              ParamNull(ParamByName('comboindex'));
              ParamNull(ParamByName('fixthecombo'));
              ParamNull(ParamByName('freecomboitem'));

              ExecProc;
              aSI.OrderLineID := FieldByName('orderlineid').AsInteger;

              SetStoredProcName('INSERT_INVOICELINE', SPNumber);
              ParamByName('ilamount').AsCurrency := -aSI.Amount;
              ParamByName('discountamount').AsCurrency := 0.000;
              ParamByName('invoiceid').AsInteger := TheInvoiceID;
              ParamByName('orderlineid').AsInteger := aSI.OrderLineID;
              ParamNull(ParamByName('discountschemeid'));
              ParamByName('FISCALSEQNO').AsString := '';
              if AppDetails.EnableFiscalPrinting then
                ParamByName('FISCALTAX').AsInteger := 1;
              //AM 23Dec99 Account Balance
              if (AppDetails.TaxExclusivePrices) then begin   //Jon 18-03-2002
                BalanceDelta := BalanceDelta - GetRoundedUpDown((aSI.Amount * (100 + aSI.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
              end
              else begin
                BalanceDelta := BalanceDelta - (aSI.Amount);
              end;
              BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
              ExecProc;
              aSI.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

              if (AppDetails.FiscalType = fiscalTaxCore) then
              begin
                SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
                ParamByName('invoicelineid').AsInteger := aSI.InvoiceLineID;
                tmpItemGroup := GlobalMenuList.GetItemsItemGroup(aSI.ItemID);
                if (Assigned(tmpItemGroup)) then
                  StringAsParam(ParamByName('taxlabel'), tmpItemGroup.TaxCoreLabel)
                else
                  ParamNull(ParamByName('taxlabel'));
                ExecProc;
            end;
          end;
        end;
        end;

        if ((TLList.Count > 0) and (CreatingInvoice)) then
        begin
          //if ValueFound and (chkTable.State = cbUnChecked) then begin
          if (ValueFound and (not OnTable)) then begin
            //create tender record
            SetStoredProcName('INSERT_TENDER', SPNumber);

            ParamByName('tendertype').AsString := 'R';

            IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);

            ParamByName('invoiceid').AsInteger := TheInvoiceID;
            ParamNull(ParamByName('nosaletypeid'));
            ParamNull(ParamByName('nosalenotes'));

            BoolAsParam(ParamByName('printed'), AppDetails.PrintCashSale);

            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, (OnTable or OnTableForWastage));

            LibExists := ((AppDetails.PMSType = pmsLibica) and (TLList.TenderTypeExists(TLTLibica)));
            StringAsParamIfBool(ParamByName('libaccount'), LibAccount, LibExists);   //Jon 03-10-2002
            StringAsParamIfBool(ParamByName('libroom'), LibRoom, LibExists);         //Jon 03-10-2002
            StringAsParamIfBool(ParamByName('libname'), LibName, LibExists);         //Jon 03-10-2002
            StringAsParamIfBool(ParamByName('libstatus'), LibStatus, LibExists);     //Jon 03-10-2002

            NZAExists := ((AppDetails.PMSType = pmsNZAGold) and (TLList.TenderTypeExists(TLTNZAGold)));
            IntegerAsParamIfBool(ParamByName('nzaaccount'), NZAAccount, NZAExists);       //Jon 01-02-2003
            StringAsParamIfBool(ParamByName('nzaakey'), NZAAkey, NZAExists);              //Jon 01-02-2003
            StringAsParamIfBool(ParamByName('nzaname'), Copy(NZAName, 1, 30), NZAExists); //Jon 01-02-2003

{            EvolutionExists := ((AppDetails.PMSType = pmsEvolution) and (TenderTypeExists(TLTEvolution)));
            IntegerAsParamIfBool(ParamByName('evolutionaccountno'), EvolutionAccountNo, EvolutionExists);
            StringAsParamIfBool(ParamByName('evolutionaccount'), EvolutionAccount, EvolutionExists);
            StringAsParamIfBool(ParamByName('evolutionname'), Copy(EvolutionName, 1, 30), EvolutionExists);}

            FidelioExists := ((AppDetails.PMSType = pmsFidelio) and (TLList.TenderTypeExists(TLTFidelio) or TLList.TenderTypeExists(TLTRoomCharge)));
            PhoenixExists := ((AppDetails.PMSType = pmsPhoenix) and (TLList.TenderTypeExists(TLTPhoenix)));
            Micros4700Exists := ((AppDetails.PMSType = pmsMicros4700) and (TLList.TenderTypeExists(TLTMicros4700) or TLList.TenderTypeExists(TLTMicros4700Account)));
            AgoraExists := ((AppDetails.PMSType = pmsAgora) and (TLList.TenderTypeExists(TLTAgora)));
            RMSExists := ((AppDetails.PMSType = pmsRMS) and (TLList.TenderTypeExists(TLTRMS)));
            RoomChargeExists := (TLList.TenderTypeExists(TLTRoomCharge));
            if (PhoenixExists) then begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbPhoenixAccount.GuestNumber, PhoenixExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbPhoenixAccount.RoomNumber, PhoenixExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbPhoenixAccount.GuestName, PhoenixExists);
            end
            else if (Micros4700Exists) then begin
              ParamNull(ParamByName('fidelioguestnumber'));
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbMicros4700Account.RoomNumber, Micros4700Exists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbMicros4700Account.SubFolio, Micros4700Exists);
            end
            else if (AgoraExists) then
            begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), StrToIntDef(GlbAgoraAccount.FolioNumber, 0), AgoraExists);
              StringAsParamIfBool(ParamByName('folioid'), GlbAgoraAccount.FolioId, AgoraExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbAgoraAccount.RoomNumber, AgoraExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbAgoraAccount.DisplayName, AgoraExists);
            end
            else if (RMSExists) then
            begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbRMSAccount.AccountId, RMSExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbRMSAccount.RoomNumber, RMSExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbRMSAccount.DisplayName, RMSExists);
            end
            else if RoomChargeExists then
            begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbTable.FidelioGuestNumber, RoomChargeExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbTable.FidelioRoomNumber, RoomChargeExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbTable.FidelioGuestName, RoomChargeExists);
            end
            else begin
              IntegerAsParamIfBool(ParamByName('fidelioguestnumber'), GlbFidelioAccount.GuestNumber, FidelioExists);
              StringAsParamIfBool(ParamByName('fidelioroomnumber'), GlbFidelioAccount.RoomNumber, FidelioExists);
              StringAsParamIfBool(ParamByName('fidelioguestname'), GlbFidelioAccount.GuestName, FidelioExists);
            end;
            IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);

            ExecProc;
            WBReceipts.RefreshID := FieldByName('tenderid').AsInteger;
            TheTenderID := WBReceipts.RefreshID;

            //create tender lines
            for I := 0 to (TLList.Count - 1) do
            begin
              TL := TLList.Items[I]; //only create a record if amount > 0
              if (TL.TLTender > 0) then
              begin
                SetStoredProcName('INSERT_TENDERLINE', SPNumber);
                ParamByName('tenderid').AsInteger := WBReceipts.RefreshID;
                ParamByName('tenderlinetypeid').AsInteger := TL.TLTypeID;
                ParamByName('tenderlineamount').AsCurrency := -TL.TLTender;
                ParamByName('tenderlinetip').AsCurrency := -TL.TLTip;
  //            ParamByName('roundingamount').AsFloat := 0;
                ParamByName('roundingamount').AsCurrency := -TL.TLRounding;    //Jon 30-3-2001
                BoolAsParam(ParamByName('changeamount'), False);
                ParamByName('tenderlinechange').AsCurrency := -TL.TLChange;    //akm 01.12.01
                StringAsParam(ParamByName('tenderlinenotes'), TL.TLNotes);    //Jon 14-04-2003
                StringAsParam(ParamByName('eftpostxnref'), TL.EFTPOSTxnRef);
                StringAsParam(ParamByName('eftpostxnauthcode'), TL.EFTPOSTxnAuthCode);
                StringAsParam(ParamByName('eftposcardref'), TL.EFTPOSCardRef);
                //AM 23Dec99 Account Balance
                BalanceDelta := BalanceDelta + (TL.TLTender - TL.TLTip + TL.TLRounding);
                BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);

                ParamNull(ParamByName('COO'));
                ParamNull(ParamByName('GNF'));
                ParamNull(ParamByName('CCF'));
                ParamNull(ParamByName('checksum'));
                IntegerAsParam(ParamByName('tenlinetypeproviderid'), TL.TLProviderID);
                IntegerAsParam(ParamByName('instalments'), TL.TLInstalments);
                ExecProc;
                TL.TLID := FieldByName('TenderLineId').AsInteger;
              end;
            end;

            //create a tender line for the change
            if (FChange > 0) then begin
              SetStoredProcName('INSERT_TENDERLINE', SPNumber);

              ParamByName('tenderid').AsInteger := WBReceipts.RefreshID;
              ParamByName('tenderlinetypeid').AsInteger := 4; //cash
              ParamByName('tenderlineamount').AsCurrency := FChange; //total change
              ParamByName('tenderlinetip').AsCurrency := 0;
              ParamByName('roundingamount').AsCurrency := 0; // JEH 24/11/01 Removed -FRounding
              BoolAsParam(ParamByName('changeamount'), True);
              ParamByName('tenderlinechange').AsCurrency := 0; //akm 01.12.01
              ParamNull(ParamByName('tenderlinenotes'));
              ParamNull(ParamByName('eftpostxnref'));
              ParamNull(ParamByName('eftposcardref'));
              //AM 23Dec99 Account Balance
              BalanceDelta := BalanceDelta - FChange;
              BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);

              ParamNull(ParamByName('COO'));
              ParamNull(ParamByName('GNF'));
              ParamNull(ParamByName('CCF'));
              ParamNull(ParamByName('checksum'));
              ParamNull(ParamByName('tenlinetypeproviderid'));
              ParamNull(ParamByName('instalments'));
              ExecProc;
            end;
          end;
        end;

        if (OrdLnList.Count > 0) then
        begin
          SetStoredProcName('INSERT_VOID', SPNumber);  //insert_void

          IntegerAsFindParam(FindParam('outletid'), AppDetails.OutletID);
          IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, OnTableForWastage);
          ParamByName('voidreasonid').AsInteger := FVoidReasonID;
          StringAsParam(ParamByName('voidexplanation1'), FExp1);
          StringAsParam(ParamByName('voidexplanation2'), FExp2);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
          TheVoidID := FieldByName('voidid').AsInteger;

          I := 0;
          while (I <= (OrdLnList.Count - 1)) do begin
            SetStoredProcName('VOID_ORDERLINE', SPNumber);

            ParamByName('voidid').AsInteger := TheVoidID;
            OrdLn := OrdLnList.Items[I];
            ParamByName('orderlineid').AsInteger := OrdLn^.OrderLineID;
            ParamByName('qtyvoided').AsCurrency := OrdLn^.Qty;
            if (FToWaste) then begin
              ParamByName('qtywasted').AsCurrency := OrdLn^.Qty;
            end
            else begin
              ParamByName('qtywasted').AsCurrency := 0;
            end;
            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            ExecProc;
            Inc(I);
          end;

          if (not FToWaste) then begin
            SetStoredProcName('RESTORE_STOCKVOID', SPNumber); //restore_stockvoid
            ParamByName('voidid').AsInteger := TheVoidID;
            IntegerAsParamIfBool(ParamByName('terminalid'), AppDetails.TerminalID, AppDetails.StockEnabled);   //Jon 18-02-2002
            BoolAsParam(ParamByName('restoreavailqty'), True);
            ExecProc;
            if (not FieldByName('errorcode').IsNull) then begin
              Result := False;
              ShowErrorMsg(FieldByName('errorcode').AsInteger);
              TheTR.Rollback;
              Exit;
            end;
          end;
        end;

        if (DoingWaste) then begin
          SetStoredProcName('DECREMENT_STOCKORDER', SPNumber);   //decrement_stockorder
          IntegerAsParam(ParamByName('orderid'), TheOrderID);
          IntegerAsParamIfBool(ParamByName('terminalid'), AppDetails.TerminalID, AppDetails.StockEnabled);
          BoolAsParam(ParamByName('decrementavailqty'), True);
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then begin
            Result := False;
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            Exit;
          end;

          if OnTableForWastage then
          begin
            SetStoredProcName('CLOSE_TABLE', SPNumber);   //close_table
            ParamByName('groupid').AsInteger := GlbTable.GroupID;
            ParamByName('loginid').AsInteger := GlbLogin.LoginID;
            ExecProc;
            if (not FieldByName('errorcode').IsNull) then
            begin
              Result := False;
              ShowErrorMsg(FieldByName('errorcode').AsInteger);
              TheTR.Rollback;
              Exit;
            end;

            with dm.sp do begin
              UnPrepare;
              StoredProcName := 'insert_printjob';
              Prepare;
              ParamByName('printjobtype').AsString := 'Summary';
              ParamByName('theid').AsInteger := GlbTable.GroupID;
              ParamByName('printerid').AsInteger := AppDetails.PrinterID;
              //IntegerAsParamIfBool(ParamByName('periodendid'), SplitBy, (SplitBy > 0));
              ExecProc;
            end;
          end;
        end;

        if ((CreatingInvoice) and (AppDetails.PrintRefund) and (not AppDetails.EnableFiscalPrinting)) then begin
          //insert print job
          InsertPrintJob('Credit Note', TheInvoiceID, AppDetails.PrinterID);
        end;

        if ((AppDetails.CashSalePrintVoids) and (TheVoidID > 0)) then begin       //Jon 19-03-2004
          for I := 0 to (PrinterList.Count - 1) do begin
            if (DoingWaste) then
              InsertPrintJob('Waste', TheVoidID, PrinterList.Get(I))
            else
              InsertPrintJob('Void', TheVoidID, PrinterList.Get(I));
          end;
        end;

        //AM 23Dec99 Account Balance
        //totalinvoices = insert_invoiceline: add sum(ilamount-discountamount)
        //totaltenders = insert_tenderline: less sum(tenderlineamount-tenderlinetip+roundingamount)
        //account balance delta = totalinvoices - totaltenders
        if ((GlbAccount.AccountID > 0) and (not DoingWaste)) then begin // if going on an account, set account balance
          SetStoredProcName('INSERTACCOUNTINVOICE', SPNumber);          //insertaccountinvoice

          ParamByName('accountid').AsInteger := GlbAccount.AccountID;
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          IntegerAsParam(ParamByName('tenderid'), TheTenderID);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          CurrencyAsParam(ParamByName('currentbalancechange'), BalanceDelta);
          ExecProc;

        end;
      end;

      if ((not DoingWaste) and (not OnTable)) then begin
        if ((AppDetails.PMSType = pmsFidelio) and
            (AppDetails.FidelioSendRefunds) and
            ((AppDetails.FidelioAllTransactions = 1) or
             ((AppDetails.FidelioExtendedTender > 0) and (AppDetails.FidelioAllTransactions < 2) and (TLList.Count > 0)) or
             (TLList.TenderTypeExists(TLTFidelio)))) then begin
          if (not SendFidelioTransactions(SPNumber)) then begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
        else if ((AppDetails.PMSType = pmsPhoenix) and (AppDetails.PhoenixSendRefunds) and ((AppDetails.PhoenixAllTransactions) or ((AppDetails.PhoenixExtendedTender) and (TLList.Count > 0)) or (TLList.TenderTypeExists(TLTPhoenix)))) then begin
          if (not SendPhoenixTransactions(SPNumber)) then begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
        else if ((AppDetails.PMSType = pmsMicros4700) and ((AppDetails.Micros4700AllTransactions = 1) or
                 ((AppDetails.Micros4700ExtendedTender) and (AppDetails.Micros4700AllTransactions < 2) and (TLList.Count > 0)) or(TLList.TenderTypeExists(TLTMicros4700) or TLList.TenderTypeExists(TLTMicros4700Account)))) then begin
          if (not SendMicros4700Transactions(SPNumber)) then
          begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
        else if ((AppDetails.PMSType = pmsICRTouch) and (TLList.TenderTypeExists(TLTRoomCharge))) then begin
          if (not SendICRTouchTransactions(SPNumber)) then begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
        else if ((AppDetails.PMSType = pmsMews) and ((AppDetails.MewsAddOutletBills) or (TLList.TenderTypeExists(TLTMews)))) then begin
          if (not SendMewsTransactions(SPNumber, TheTenderID)) then
          begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
         else if ((AppDetails.PMSType = pmsAgora) and ((AppDetails.AgoraAllTransactions = 1) or (TLList.TenderTypeExists(TLTAgora)))) then begin
          if (not SendAgoraTransactions(SPNumber, TheTenderID)) then
          begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end
        else if ((AppDetails.PMSType = pmsRMS) and ((AppDetails.RMSAllTransactions = 1) or (TLList.TenderTypeExists(TLTRMS)))) then begin
          if (not SendRMSTransactions(SPNumber, TheTenderID)) then
          begin
            //Something wrong happened while putting in the Transactions.

            TheTR.Rollback;
            Result := False;
            Exit;
          end;
        end;
      end;

      if (AppDetails.PDEnabled) then begin
        if (TLList.Count > 1) then begin
          DMComponents.PDOutputItemLine('REFUNDED', GetRoundedUpDown(FPayment + FChange, AppDetails.DecimalPlaces), 0);
        end
        else if (TLList.Count = 1) then begin
          TL := TLList.Items[0];
          DMComponents.PDOutputItemLine(GetTLTType(TL.TLType), GetRoundedUpDown(TL.TLTender, AppDetails.DecimalPlaces), 0);
        end;
        DMComponents.PDOutputItemLine('CHANGE', GetRoundedUpDown(FChange, AppDetails.DecimalPlaces), 1);
      end;

      TheTR.Commit;   //Jon 10-07-2003

      cmdTable.Enabled := True;
      formEFTPOSIntegration.ClearTransactionID;

      SFiscalPrinting.COO := '';
      SFiscalPrinting.CCF := '';
      FComboDiscount := 0;
    except
      on e: exception do begin
        Result := False;
        ShowMessage(e.message);
        TheTR.Rollback;
        SelectFirsts;
      end;
    end;
    BackPagesList.Clear;  //Jon 13-8-2001
    SetDoingRefund(False, True);
    SetDoingExchange(False, True);
    SetDoingWaste(False, True);
    PriceLevelChange(0, 2);
    CourseChange(0, '', 2, False);                    //Jon 28-02-2002
    NextItemButtonStateChange(1);   //Jon 22-04-2002

    ModifierFunctionChange(1);          //Jon 14-8-2001 returns mod function to +
  finally
    sExternalAccountIntegration.FinalisePresentedCards;

    PrinterList.Free;

    for I := (OrdLnList.Count - 1) downto 0 do begin
      OrdLn := OrdLnList.Items[I];
      Dispose(OrdLn);
      OrdLnList.Delete(I);
    end;
    OrdLnList.Free;
    ClearStatus;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.SendLibicaTransactions(SPNumber: Integer): Boolean;
var
  LibFood, LibBev, LibTip: Currency;
  TotalFood, TotalBev, LibAmt: Currency;
  LibDesc: string;
  LibGLCodeF, LibGLCodeB, LibGLCodeTip: string;
  ListCount: Integer;
  I: Integer;
  TL: TTL;
  OrderLine: TOrderLine;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  LibicaSourceList: TList;

begin
  TL := TLList.GetTLOfTypeID(GetTLTID(TLTLibica));
  if (not Assigned(TL)) then
  begin
  //This should never happen cos we should only come here if libicatransexist
    Result := False;
    Exit;
  end;

  LibAmt := TL.TLPayment;
  LibTip := TL.TLTip;

  ListCount := OLList.Count;
  TotalFood := 0;
  TotalBev := 0;
  SoFarDiscount := 0.00;
  SoFarComboDiscount := 0.00;
  for I := 0 to (ListCount - 1) do
  begin
    OrderLine := OLList.Items[I];
    if ((FDiscountAmount = 0) and (FComboDiscount = 0)) then
    begin
      ILDiscount := 0.00;
    end
    else if OrderLine.ComboID > 0 then
    begin
      if OrderLine.LastComboItem then
        ILDiscount := FComboDiscount - SoFarComboDiscount
      else
        ILDiscount := OrderLine.AllowedDiscount;

      ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
      SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
      SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
    end
    else
    begin
      if OrderLine.LastNormalItem then
        ILDiscount := FDiscountAmount - SoFarDiscount
      else
      begin
        ILDiscount := OrderLine.AllowedDiscount;
        SoFarDiscount := SoFarDiscount + ILDiscount;
        SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
      end;
    end;
    if (OrderLine.ForB = 'F') then begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalFood := TotalFood + ((OrderLine.ToPay - ILDiscount) * (100 + OrderLine.SalesTaxPercent)) / 100;
      end
      else begin
        TotalFood := TotalFood + OrderLine.ToPay - ILDiscount;
      end;
      //TotalFood := GetRoundedUpDown(TotalFood, AppDetails.DecimalPlaces);
    end
    else begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalBev := TotalBev + ((OrderLine.ToPay - ILDiscount) * (100 + OrderLine.SalesTaxPercent)) / 100;
      end
      else begin
        TotalBev := TotalBev + OrderLine.ToPay - ILDiscount;
      end;
      //TotalBev := GetRoundedUpDown(TotalBev, AppDetails.DecimalPlaces);
    end;
  end;

  try
    //We are already in a Transaction so another one does not need to be started
    with dm.GetSP(SPNumber) do begin
      SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := CurrentDay;
      Execute;

      if (FieldByName('salecategoryid').IsNull) then begin
        dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
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
      dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
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
  ClearLibicaSourceDataList(LibicaSourceList);
  LibicaSourceList.Free;
end;
{******************************************************************************}
function TformQuickSaleUser.SendFidelioTransactions(SPNumber: Integer): Boolean;
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
      LocalSub := GetRoundedUpDown(aSub[J] * Ratio, AppDetails.DecimalPlaces);
      LocalTax := GetRoundedUpDown(aTax[J] * Ratio, AppDetails.DecimalPlaces);
      FidelioTransaction.Sub[J] := FidelioTransaction.Sub[J] + LocalSub;
      FidelioTransaction.Tax[J] := FidelioTransaction.Tax[J] + LocalTax;
      FidelioTransaction.Disc[J] := GetRoundedUpDown(LocalSub + LocalTax - (aTotal[J] * Ratio), AppDetails.DecimalPlaces);
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
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTFidelio));
    if ((not Assigned(TL)) and (AppDetails.FidelioExtendedTender = 0) and (AppDetails.FidelioAllTransactions <> 1)) then begin
    //This should never happen cos we should only come here if Fideliotransexist
      Result := False;
      Exit;
    end
    else begin
      if ((AppDetails.FidelioBreakdownType = 0) and (Assigned(TL))) then
      begin
        FidelioTransaction := TFidelioTransaction.Create;

        FidelioTransaction.GuestNumber := GlbFidelioAccount.GuestNumber;
        FidelioTransaction.RoomNumber := GlbFidelioAccount.RoomNumber;
        FidelioTransaction.GuestName := GlbFidelioAccount.GuestName;
        FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
        FidelioTransaction.Covers := 0;

        FidTotal := TL.TLPayment + TL.TLTip;
        if ((FToPay + FSurcharge - FDiscountAmount) <> 0) then
        begin
          FidTax := FSalesTax * (TL.TLPayment) / (FToPay + FSurcharge - FDiscountAmount);
        end
        else
        begin
          FidTax := 0;
        end;
        FidTax := GetRoundedUpDown(FidTax, AppDetails.DecimalPlaces);
        FidSubtotal := FidTotal - FidTax;

        FidelioTransaction.FoodSub := FidSubtotal;
        FidelioTransaction.FoodTax := FidTax;
        FidelioTransaction.BreakdownType := 0;
        FidelioTransaction.Refund := FDoingRefund;
        ListFT.Add(FidelioTransaction);
      end
      else if (AppDetails.FidelioBreakdownType > 0) then
      begin
      //Do this right then remove the same code where it exists below
        ListCount := (OLList.Count - 1);

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
          OL := OLList.Items[I];

          if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
          begin
            ILDiscount := 0;
          end
          else if OL.ComboID > 0 then
          begin
            if OL.LastComboItem then
              ILDiscount := FComboDiscount - SoFarComboDiscount
            else
              ILDiscount := OL.AllowedDiscount;

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

          if (AppDetails.TaxExclusivePrices) then
          begin
            LineSub := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);
            LineTax := GetRoundedUpDown((OL.ToPay - ILDiscount) * OL.SalesTaxPercent / 100, AppDetails.DecimalPlaces);
            LineDisc := ILDiscount;
            LineTotal := LineSub + LineTax - LineDisc;
          end
          else
          begin
            LineSub := GetRoundedUpDown((OL.ToPay * 100) / (100 + OL.SalesTaxPercent), AppDetails.DecimalPlaces);
            LineTotal := (OL.ToPay - ILDiscount);
            LineTax := GetRoundedUpDown(LineTotal * ((OL.SalesTaxPercent) / (100 + OL.SalesTaxPercent)), AppDetails.DecimalPlaces);
            LineDisc := ((ILDiscount * 100) / (100 + OL.SalesTaxPercent));
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
        for I := 0 to (ListCount) do
        begin
          SurchargeItem := SurchargeList.Items[I];

          if (AppDetails.TaxExclusivePrices) then
          begin
            LineSub := GetRoundedUpDown(SurchargeItem.Amount, AppDetails.DecimalPlaces);
            LineTax := GetRoundedUpDown(SurchargeItem.Amount * SurchargeItem.SalesTaxPercent / 100, AppDetails.DecimalPlaces);
            LineDisc := 0;
            LineTotal := LineSub + LineTax;
          end
          else
          begin
            LineSub := GetRoundedUpDown((SurchargeItem.Amount * 100) / (100 + SurchargeItem.SalesTaxPercent), AppDetails.DecimalPlaces);
            LineTotal := SurchargeItem.Amount;
            LineTax := GetRoundedUpDown(LineTotal * ((SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent)), AppDetails.DecimalPlaces);
            LineDisc := 0;
          end;

          aSub[3] := aSub[3] + LineSub;
          aTax[3] := aTax[3] + LineTax;
          aDisc[3] := aDisc[3] + LineDisc;
          aTotal[3] := aTotal[3] + LineTotal;
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

            if (FieldByName('salecategoryid').IsNull) then
            begin
              dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
              ShowQuickMsg(sSaleCatNotFound);
              Result := False;
              Exit;
            end;

            ServingTime := FieldAsInt(FieldByName('fidservingtime'));

            if ((FieldAsInt(FieldByName('tpfidservingtime'))) > 0) then
            begin
              ServingTime := FieldAsInt(FieldByName('tpfidservingtime'));
            end;
          end;
        except
          on e: exception do begin
            dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
            Result := False;
            ShowMessage(e.message);
            Exit;
          end;
        end;

        if (Assigned(TL)) then
        begin
          FidelioTransaction := TFidelioTransaction.Create;

          FidelioTransaction.GuestNumber := GlbFidelioAccount.GuestNumber;
          FidelioTransaction.RoomNumber := GlbFidelioAccount.RoomNumber;
          FidelioTransaction.GuestName := GlbFidelioAccount.GuestName;
          FidelioTransaction.ServingTime := ServingTime;
          FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
          FidelioTransaction.Covers := 0;       //Cos all cash sales have a 0 cover.

          FidTotal := TL.TLPayment;

          AddFidelioValues;

          FidelioTransaction.Tip := TL.TLTip;

          FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
          FidelioTransaction.Refund := FDoingRefund;

          ListFT.Add(FidelioTransaction);
        end;

        if ((AppDetails.FidelioExtendedTender > 0) and
            ((AppDetails.FidelioAllTransactions < 2) or
             ((AppDetails.FidelioAllTransactions = 2) and (TLList.TenderTypeExists(TLTFidelio))))) then
        begin
          ListCount := (TLList.Count - 1);
          for I := 0 to ListCount do begin
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
              FidelioTransaction.Covers := 0;   //Cos all cash sales have a 0 cover.
              FidelioTransaction.PaymentMethod := TenderLineType.MiniName;

              FidTotal := TL.TLPayment;

              AddFidelioValues;

              FidelioTransaction.Tip := TL.TLTip;

              FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
              FidelioTransaction.Refund := FDoingRefund;

              ListFT.Add(FidelioTransaction);
            end;
          end;
        end;

        if ((AppDetails.FidelioAllTransactions = 1) or
            ((AppDetails.FidelioAllTransactions = 2) and (TLList.TenderTypeExists(TLTFidelio))) and (ListFT.Count <= 0)) then
        begin
          FidelioTransaction := TFidelioTransaction.Create;
          TenderLineType := GetTenderLineTypeRecord(TLTCash);

          FidelioTransaction.GuestNumber := StrToIntDef(TenderLineType.LibGLCode, AppDetails.FidelioLIBGLCode);
          FidelioTransaction.RoomNumber := '';
          FidelioTransaction.GuestName := '';
          FidelioTransaction.ServingTime := ServingTime;
          FidelioTransaction.InvoiceID := WBInvoices.RefreshID;
          FidelioTransaction.Covers := 0;       //Cos all cash sales have a 0 cover.
          FidelioTransaction.PaymentMethod := TenderLineType.MiniName;

          FidTotal := 0;
          AddFidelioValues;

          FidelioTransaction.Tip := 0;

          FidelioTransaction.BreakdownType := AppDetails.FidelioBreakdownType;
          FidelioTransaction.Refund := FDoingRefund;

          ListFT.Add(FidelioTransaction);
        end;
      end;
    end;

    Result := DoFidelioTransactionList(ListFT);
    //RPC Here if we failed, we removed the room transfer tender to force to re enter room details
    if (not Result) then
    begin
      ListCount := TLList.Count;
      I := 0;
      while (I < ListCount) do
      begin
        if (TTL(TLList.Items[I]).TLTypeID = GetTLTID(TLTFidelio)) then
        begin
          grdTLs.Row := I + 1;
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
function TformQuickSaleUser.SendPhoenixTransactions(SPNumber: Integer): Boolean;
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

      Value := GetRoundedUpDown(ATotal * Ratio, AppDetails.DecimalPlaces);
      if (Value <> 0) then begin
        PhoenixTransaction.AddPhoenixCategory(ACategory, Value, True);
      end;

      PhxTotal := PhxTotal - (ATotal * Ratio);
    end;

    procedure AllocateGLOLsAgainstPhxTotal(ForB: string; ATotal: Currency);
    var
      iIdx: Integer;
      aAmtToAlloc: Currency;
    begin
      //if there is a GL Food/Bev total, it is distributed across the order
      //lines below, where there is a GLCode, and the ForB param matches
      if ATotal > 0 then
      begin
        //loop through each order line
        ListCount := (OLList.Count - 1);
        for iIdx := 0 to ListCount do
        begin
          OL := OLList.Items[iIdx];

          //if find an order line with a GLCode (from item's itemgroup)
          if (OL.GLCode <> '') and (OL.FORB = ForB) then
          begin
            //allocate the net amount to pay on the orderline (or less, if remaining total is smaller)
            if OL.NetToPay <= ATotal then
              aAmtToAlloc := OL.NetToPay
            else
              aAmtToAlloc := ATotal;
            AllocateAgainstPhxTotal(StrToIntDef(OL.GLCode, 0), aAmtToAlloc);

            //remove amount just allocated from remaining total
            ATotal := ATotal - aAmtToAlloc;
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
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTPhoenix));
    if ((not Assigned(TL)) and (not (AppDetails.PhoenixExtendedTender or AppDetails.PhoenixAllTransactions))) then begin
      //This should never happen cos we should only come here if Phoenixtransexist
      Result := False;
      Exit;
    end;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.GetSP(SPNumber) do begin
        SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := Currentday;
        Execute;

        if (FieldByName('salecategoryid').IsNull) then begin
          dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
          ShowQuickMsg(sSaleCatNotFound);
          Result := False;
          Exit;
        end;

        FoodCat := StrToIntDef(FieldAsString(FieldByName('libglf')), 0);
        BevCat := StrToIntDef(FieldAsString(FieldByName('libglb')), 0);
        MiscCat := StrToIntDef(FieldAsString(FieldByName('libglmisc')), 0);
        TipCat := StrToIntDef(FieldAsString(FieldByName('libgltip')), 0);

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
        dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
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
      if (FDoingRefund) then
        PhoenixTransaction.InvoiceID := WBCreditNotes.RefreshID
      else
        PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
      if (AppDetails.CashSaleGuestCount > 0) and (NewGuestCount > 0) then
        PhoenixTransaction.Covers := NewGuestCount
      else
        PhoenixTransaction.Covers := 0;

      PhxTotal := TL.TLPayment + TL.TLTip;

      PhoenixTransaction.AddPhoenixCategory(FoodCat, PhxTotal);

      PhoenixTransaction.BreakdownType := 0;
      PhoenixTransaction.Refund := FDoingRefund;
      ListPT.Add(PhoenixTransaction);
    end
    else if (AppDetails.PhoenixBreakdownType = 1) then begin
    //Do this right then remove the same code where it exists below
      ListCount := (OLList.Count - 1);

      GLFoodTotal := 0;
      GLBevTotal := 0;
      FoodTotal := 0;
      BevTotal := 0;
      MiscTotal := 0;
      TipTotal := 0;
      SoFarDiscount := 0;
      SoFarComboDiscount := 0.00;

      for I := 0 to ListCount do begin
        OL := OLList.Items[I];

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

      PhoenixTransaction := nil;

      ListCount := (SurchargeList.Count - 1);
      for I := 0 to (ListCount) do begin
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
        if (FDoingRefund) then
          PhoenixTransaction.InvoiceID := WBCreditNotes.RefreshID
        else
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;

        if (AppDetails.CashSaleGuestCount > 0) and (NewGuestCount > 0) then
          PhoenixTransaction.Covers := NewGuestCount
        else
          PhoenixTransaction.Covers := 0;

        PhxTotal := TL.TLPayment;
        TipTotal := TL.TLTip;

        PhoenixTransaction.BreakdownType := 1;
        PhoenixTransaction.Refund := FDoingRefund;

        ListPT.Add(PhoenixTransaction);
      end
      else if (AppDetails.PhoenixExtendedTender) then
      begin
        if (not Assigned(PhoenixTransaction)) then
          PhoenixTransaction := TPhoenixTransaction.Create;

        PhoenixTransaction.GuestNumber := AppDetails.PhoenixTenGuestNumber;
        PhoenixTransaction.RoomNumber := AppDetails.PhoenixTenRoomNumber;
        PhoenixTransaction.GuestName := '';
        if (FDoingRefund) then
          PhoenixTransaction.InvoiceID := WBCreditNotes.RefreshID
        else
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;

        if (AppDetails.CashSaleGuestCount > 0) and (NewGuestCount > 0) then
          PhoenixTransaction.Covers := NewGuestCount
        else
          PhoenixTransaction.Covers := 0;

        PhoenixTransaction.BreakdownType := 1;
        PhoenixTransaction.Refund := FDoingRefund;

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
        if (FDoingRefund) then
          PhoenixTransaction.InvoiceID := WBCreditNotes.RefreshID
        else
          PhoenixTransaction.InvoiceID := WBInvoices.RefreshID;
        if (AppDetails.CashSaleGuestCount > 0) and (NewGuestCount > 0) then
          PhoenixTransaction.Covers := NewGuestCount
        else
          PhoenixTransaction.Covers := 0;

        PhoenixTransaction.BreakdownType := 1;
        PhoenixTransaction.Refund := FDoingRefund;

        ListPT.Add(PhoenixTransaction);
      end;
      if (Assigned(PhoenixTransaction)) then begin
        if (PhoenixTransaction.ListPhoenixCategorys.Count = 0) then begin
          PhoenixTransaction.AddPhoenixCategory(FoodCat, 0);
        end;
      end;
    end;

    Result := DoPhoenixTransactionList(ListPT);
  finally
    ClearListObjects(ListPT, True);
  end;
end;
{*******************************************************************************
Calculates and send the transactions to the Micros 4700 spec system.

Change History
#00338# 23/08/2012 JBB Breakdown into subtotals with tax and discounts.
*******************************************************************************}
function TformQuickSaleUser.SendMicros4700Transactions(SPNumber: Integer): Boolean;
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
        MaxI := (OLList.Count - 1);
        for I := 0 to MaxI do
        begin
          OL := OLList.Items[I];

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
  Result := False;
  MT := TMicros4700Transaction.Create;
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
    if (FDoingRefund) then
      MT.InvoiceID := WBCreditNotes.RefreshID
    else
      MT.InvoiceID := WBInvoices.RefreshID;

    if Assigned(TL) then
    begin
      MT.PostingValue := TL.TLPayment + TL.TLTip;
      MT.TipValue := TL.TLTip;
      MT.SurchargeValue := TL.TLSurcharge;

      TenderLineType := GetTenderLineTypeRecord(TL.TLType);
      MT.Payment := StrToIntDef(TenderLineType.LibGLCode, 0)
    end;

    MT.FoodValue := 0;
    MT.BevValue := 0;
    MT.MiscValue := 0;

    MT.BreakdownType := AppDetails.Micros4700BreakdownType;
    MT.RevenueCenter := AppDetails.Micros4700RevenueCenter;
    MT.Refund := FDoingRefund;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.GetSP(SPNumber) do begin
        SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := currenttime;
        ParamByName('currentday').AsInteger := Currentday;
        Execute;

        if (FieldByName('salecategoryid').IsNull) then begin
          dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
          ShowQuickMsg(sSaleCatNotFound);
          Exit;
        end;

        MT.ServingPeriod := FieldAsInt(FieldByName('fidservingtime'));
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
        dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;

    if (AppDetails.Micros4700BreakdownType = 1) then
    begin
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
      SoFarComboDiscount := 0.00;
      MaxI := (OLList.Count - 1);
      for I := 0 to MaxI do
      begin
        OL := OLList.Items[I];

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
          LineTax := GetRoundedUpDown(((OL.ToPay - ILDiscount) * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineTaxBefore := GetRoundedUpDown((OL.ToPay * OL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineDisc := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          LineDiscBefore := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
          LineSub := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);
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

        if (AppDetails.TaxExclusivePrices) then begin
          LineSub := GetRoundedUpDown(SurchargeItem.Amount, AppDetails.DecimalPlaces);
          LineTax := GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
          LineTaxBefore := LineTax;
          LineDisc := 0;
          LineDiscBefore := 0;
          LineTotal := LineSub + LineTax;
        end
        else
        begin
          LineSub := GetRoundedUpDown((SurchargeItem.Amount * 100) / (100 + SurchargeItem.SalesTaxPercent), AppDetails.DecimalPlaces);
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
      MaxI := (OLList.Count - 1);

      GLFoodTotal := 0;
      GLBevTotal := 0;
      FoodTotal := 0;
      BevTotal := 0;
      MiscTotal := 0;
      SoFarDiscount := 0;
      SoFarComboDiscount := 0.00;

      for I := 0 to MaxI do begin
        OL := OLList.Items[I];

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

      if (not Assigned(TL)) and ((AppDetails.Micros4700ExtendedTender) or (AppDetails.Micros4700AllTransactions = 1)) then
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
      with dm.GetSP(SPNumber) do
      begin
        try
          SetStoredProcName('INSERT_INVOICEMICROS4700', SPNumber);
          ParamByName('invoiceid').AsInteger := MT.InvoiceID;
          BoolAsParam(ParamByName('bulkpost'), False);
          Execute;

        except
          on e: exception do begin
            dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
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
function TformQuickSaleUser.SendNZATransactions(SPNumber: Integer): Boolean;
var
  NZAFood, NZABev, NZATip: Currency;
  NZAFoodTax, NZABevTax, NZATipTax: Currency;
  TotalFood, TotalBev, NZAAmt: Currency;
  TotalFoodTax, TotalBevTax: Currency;
  NZADesc: string;
  NZAStockF, NZAStockB, NZAStockTip: string;
  ListCount: Integer;
  I: Integer;
  TL: TTL;
  OrderLine: TOrderLine;
  SoFarComboDiscount, SoFarDiscount, ILDiscount: Currency;
  NZAInvoiceData: TNZAInvoiceData;
  NZAInvoiceLineList: TList;
begin
  TL := TLList.GetTLOfTypeID(GetTLTID(TLTNZAGold));

  if (not Assigned(TL)) then begin
    //This should never happen cos we should only come here if nzatransexist
    Result := False;
    Exit;
  end;

  NZAAmt := TL.TLPayment;
  NZATip := TL.TLTip;

  ListCount := OLList.Count;

  TotalFood := 0;
  TotalBev := 0;
  TotalFoodTax := 0;
  TotalBevTax := 0;
  SoFarDiscount := 0;
  SoFarComboDiscount := 0;
  for I := 0 to (ListCount - 1) do
  begin
    OrderLine := OLList.Items[I];

    if ((FDiscountAmount <= 0) and (FComboDiscount <= 0)) then
    begin
      ILDiscount := 0;
    end
    else if OrderLine.ComboID > 0 then
    begin
      if OrderLine.LastComboItem then
        ILDiscount := FComboDiscount - SoFarComboDiscount
      else
        ILDiscount := OrderLine.AllowedDiscount;

      ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
      SoFarComboDiscount := SoFarComboDiscount + ILDiscount;
      SoFarComboDiscount := GetRoundedUpDown(SoFarComboDiscount, AppDetails.DecimalPlaces);
    end
    else
    begin
      //if it's the last line, allocate whatever hasn't been pro rata-ed
      if OrderLine.LastNormalItem then
        ILDiscount := FDiscountAmount - SoFarDiscount
      else
      begin              //pro rata discount and accumulate discount pro rata-ed so far
        ILDiscount := OrderLine.AllowedDiscount;
        ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
        SoFarDiscount := SoFarDiscount + ILDiscount;
        SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
      end;
    end;

    if (OrderLine.ForB = 'F') then begin
      if (AppDetails.TaxExclusivePrices) then begin
        TotalFood := TotalFood + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (100 + OrderLine.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
        TotalFoodTax := TotalFoodTax + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (OrderLine.SalesTaxPercent)) / 100, AppDetails.DecimalPlaces);
      end
      else begin
        TotalFood := TotalFood + GetRoundedUpDown(OrderLine.ToPay - ILDiscount, AppDetails.DecimalPlaces);
        TotalFoodTax := TotalFoodTax + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (OrderLine.SalesTaxPercent)) / (100 + OrderLine.SalesTaxPercent), AppDetails.DecimalPlaces);
      end;
    end
    else
    begin
      if (AppDetails.TaxExclusivePrices) then
      begin
        TotalBev := TotalBev + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (100 + OrderLine.SalesTaxPercent) / 100), AppDetails.DecimalPlaces);
        TotalBevTax := TotalBevTax + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (OrderLine.SalesTaxPercent) / 100), AppDetails.DecimalPlaces);
      end
      else
      begin
        TotalBev := TotalBev + GetRoundedUpDown(OrderLine.ToPay - ILDiscount, AppDetails.DecimalPlaces);
        TotalBevTax := TotalBevTax + GetRoundedUpDown(((OrderLine.ToPay - ILDiscount) * (OrderLine.SalesTaxPercent)) / (100 + OrderLine.SalesTaxPercent), AppDetails.DecimalPlaces);
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
  NZATipTax := 0;

  if (TotalFood > 0) then begin
    NZAFoodTax := (NZAFood * TotalFoodTax) / TotalFood;
  end;
  if (TotalBev > 0) then begin
    NZABevTax := (NZABev * TotalBevTax) / TotalBev;
  end;

  try
    //We are already in a Transaction so another one does not need to be started
    with dm.GetSP(SPNumber) do begin
      SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := Currentday;
      Execute;

      if (FieldByName('salecategoryid').IsNull) then begin
        dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
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
      dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
      Result := False;
      ShowMessage(e.message);
      Exit;
    end;
  end;

  NZADesc := AppDetails.TerminalName + ' Ref# ' + IntToStr(WBInvoices.RefreshID);

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
  NZAInvoiceData.IntRef := IntToStr(WBInvoices.RefreshID);   //Jon 02-10-2003

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
function TformQuickSaleUser.SendEvolutionTransactions(AGuestCount: Integer; SPNumber: Integer): Boolean;
var
  iLineNo: Integer;

  varTL: TTL;
  iTLIdx: Integer;
  eTotTLAmt: Extended; //total tendered
  eEvoTLAmt: Extended; //total tendered to Evo
  eEvoTLTip: Extended; //total tip to Evo
  eEvoTLProp: Extended; //proportion of Evo tender to Total tender

  varOL: TOrderLine;
  iOLIdx: Integer;
  eILQty: Extended;  //proportional line qty
  eILDisc: Extended; //proportional line disc
  eILAmt: Extended;  //proportional line amount
  eILDiscPerc: Extended;

  varSC: TSurchargeItem;
  iSCIdx: Integer;

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
    with dm.GetSP(SPNumber) do begin
      SetStoredProcName('GETSALECATEGORYINFO', SPNumber);
      ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
      ParamByName('currenttime').AsInteger := CurrentTime;
      ParamByName('currentday').AsInteger := Currentday;
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
  for iTLIdx := 0 to Pred(TLList.Count) do begin
    varTL := TLList.Items[iTLIdx];
    eTotTLAmt := eTotTLAmt + varTL.TLPayment;

    if (varTL.TLTypeID = GetTLTID(TLTEvolution)) then begin
      eEvoTLAmt := eEvoTLAmt + varTL.TLPayment;
      eEvoTLTip := eEvoTLTip + varTL.TLTip;
    end;
  end;
  eEvoTLProp := (eEvoTLAmt / eTotTLAmt);

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
    if not GlbEvoSCItems.FindEvoSCItem(NZAStockF, varEvoItemF) then
      aCodesNotFound := aCodesNotFound + NZAStockF + ', ';
    if not GlbEvoSCItems.FindEvoSCItem(NZAStockB, varEvoItemB) then
      aCodesNotFound := aCodesNotFound + NZAStockB + ', ';
  end;

  //get Evo Tip stock item information, if need to save a tip invoice line
  if (eEvoTLTip <> 0) then begin
    if (not GlbEvoSCItems.FindEvoSCItem(NZAStockTip, varEvoItemTip)) then begin
      aCodesNotFound := aCodesNotFound + NZAStockTip + ', ';
    end;
  end;

  //if any of the needed stock codes are not found in Evolution, then error and Exit
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
  for iOLIdx := 0 to Pred(OLList.Count) do begin
    varOL := TOrderLine(OLList[iOLIdx]);
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
      else begin    //increment F/B totals
        if (varOL.ForB = 'F') then begin
          eITDisc_Food := eITDisc_Food + eILDisc;
          eITAmt_Food := eITAmt_Food + eILAmt;
        end
        else begin  //varOL.ForB = 'B'
          eITDisc_Bev := eITDisc_Bev + eILDisc;
          eITAmt_Bev := eITAmt_Bev + eILAmt;
        end;
      end;
    end;
  end;

  //go through each surcharge item, add itemised Evo invoice lines or increment totals
  for iSCIdx := 0 to Pred(SurchargeList.Count) do begin
    varSC := TSurchargeItem(SurchargeList.Items[iSCIdx]);
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
      if (eITAmt_Food + eITAmt_Bev <> 0) then begin
        eEvoFoodProp := eITAmt_Food / (eITAmt_Food + eITAmt_Bev)
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

  SetEvoInvHdr(AGuestCount, WBInvoices.RefreshID, aInvoiceDescr); //AGuestCount = 0, forced to clear table account
  Result := EvoSaveInvoice;
end;
{******************************************************************************}
function TformQuickSaleUser.SendICRTouchTransactions(SPNumber: Integer): Boolean;
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

    for I := 0 to OLList.Count - 1 do
    begin
      OL := OLList.Items[I];
      if (OL.ItemID > 0) then
      begin
        lOLList.Add(OL);
      end;
    end;

    SoFarDiscount := 0;
    SoFarComboDiscount := 0.00;

    lOLList.Sort(@SortOLList);

    lICRPosting.DoingRefund := FDoingRefund;
    lICRPosting.TerminalID := AppDetails.TerminalID;
    lICRPosting.TerminalName := AppDetails.TerminalName;
    lICRPosting.RoomNumber := GlbICRTouchFolio.RoomNumber;
    lICRPosting.FolioNumber := GlbICRTouchFolio.FolioNumber;

    try
      //We are already in a Transaction so another one does not need to be started
      with dm.GetSP(SPNumber) do
      begin
        SetStoredProcName('GETSALECATEGORYINFO', SPNumber);

        ParamByName('psalecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := Currentday;
        Execute;

        if (FieldByName('salecategoryid').IsNull) then begin
          dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
          ShowQuickMsg(sSaleCatNotFound);
          Exit;
        end;

        lICRPosting.ShiftNumber := FieldAsInt(FieldByName('fidservingtime'));
        if ((FieldAsInt(FieldByName('tpfidservingtime'))) > 0) then begin
          lICRPosting.ShiftNumber := FieldAsInt(FieldByName('tpfidservingtime'));
        end;
      end;
    except
      on e: exception do begin
        dm.GetTR(SPNumber).Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;

    if (FDoingRefund) then
    begin
      lICRPosting.ConseqNumber := WBCreditNotes.RefreshID;
      lICRPosting.CheckNumber := WBCreditNotes.RefreshID;
    end
    else
    begin
      lICRPosting.ConseqNumber := WBInvoices.RefreshID;
      lICRPosting.CheckNumber := WBInvoices.RefreshID;
    end;
//    lICRPosting.TabNumber := 23;    Cash Sale
//    lICRPosting.Covers := 2;        Cash Sale
    if (PriceLevel in [1..6]) then
    begin
      lICRPosting.PriceLevel := PriceLevel;
    end
    else
    begin
      lICRPosting.PriceLevel := GlbTable.PriceLevel;
    end;
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
function TformQuickSaleUser.SendMewsTransactions(SPNumber, TheTenderID: Integer): Boolean;
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
     //This should never happen cos we should only come here if TransExist
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
      CustomerId := GlbMewsAccount.CustomerId;
      DisplayName := GlbMewsAccount.DisplayName;
      RoomNumber := GlbMewsAccount.RoomNumber;
    end
    else if (AppDetails.MewsAddOutletBills) then
    begin
      Transaction := TransactionList.NewMewsTransaction;
      Transaction.ServiceId := ServiceId;
      Transaction.OutletId := OutletId;
      CustomerId := '';
      DisplayName := '';
      RoomNumber := '';
    end;

    if (Assigned(Transaction)) then
    begin
      Transaction.Refund := FDoingRefund;
      if (FDoingRefund) then
        Transaction.InvoiceID := WBCreditNotes.RefreshID
      else
        Transaction.InvoiceID := WBInvoices.RefreshID;

      for I:= 0 to (OLList.Count -1) do
      begin
        OL := OLList.Items[I];
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
{******************************************************************************}
function TformQuickSaleUser.SendAgoraTransactions(SPNumber, TheTenderID: Integer): Boolean;
var
  I: Integer;
  TL: TTL;
  Transaction: TAgoraTransaction;
  FoodCat, BevCat, MiscCat, TipCat: string;
  OL: TOrderLine;
  FFolioNumber, FFolioID, FRoomNumber: String;
  TenderLineType: TTenderLineType;
begin
  Transaction := TAgoraTransaction.Create;
  Result := False;
  try
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTRMS));
    if ((not Assigned(TL)) and (not (AppDetails.AgoraAllTransactions <> 1))) then
    begin
     //This should never happen cos we should only come here if TransExist
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

        FoodCat := FieldAsString(FieldByName('libglf'));
        BevCat := FieldAsString(FieldByName('libglb'));
        MiscCat := FieldAsString(FieldByName('libglmisc'));
        TipCat := FieldAsString(FieldByName('libgltip'));

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
      FFolioNumber := GlbAgoraAccount.FolioNumber;
      FFolioID := GlbAgoraAccount.FolioID;
      FRoomNumber := GlbAgoraAccount.RoomNumber;
    end
    else
    begin
      FFolioNumber := AppDetails.AgoraFolioNumber;
      FFolioID := AppDetails.AgoraFolioID;
      FRoomNumber := AppDetails.AgoraRoomNumber;
    end;

    with Transaction.Posting do
    begin
      FolioNumber := FFolioNumber;
      RoomNumber := FRoomNumber;
      if (FDoingRefund) then
        ReceiptNumber := 'INV-' +  Format('%.*d',[8, WBCreditNotes.RefreshID])
      else
        ReceiptNumber := 'INV-' +  Format('%.*d',[8, WBInvoices.RefreshID]);

      for I:= 0 to (OLList.Count -1) do
      begin
        OL := OLList.Items[I];
        if (OL.ItemID > 0) then
        begin
          with Tenders.newTender do
          begin
            FolioId :=  FFolioId;
            CurrencyCode := AppDetails.AgoraCurrencyCode;
            Amount := FloatToStr(OL.ILAmount - OL.ILDiscount);
            Description := OL.ItemString;
            if (OL.GLCode <> '') then
              RevenueCategoryCode := OL.GLCode
            else if (OL.ForB = 'F') then
              RevenueCategoryCode := FoodCat
            else
              RevenueCategoryCode := BevCat;
          end;
        end;
      end;

      for I:= 0 to (TLList.Count - 1) do
      begin
        TL := TLList.Items[I];
        if (TL.TLTypeID <> GetTLTID(TLTRMS)) then
        begin
          TenderLineType := GetTenderLineTypeRecord(TL.TLType);
          if (TL.TLTip > 0) then
          with Tenders.newTender do
          begin
            FolioId :=  FFolioId;
            CurrencyCode := AppDetails.AgoraCurrencyCode;
            Amount := FloatToStr(TL.TLTip);
            Description := 'Tips';
            RevenueCategoryCode := TipCat;
          end;
        end;
      end;

      TotalPostingAmount;
    end;

    Result := DoAgoraTransaction(Transaction);

    try
      with dm.GetSP(SPNumber) do
      begin
        SetStoredProcName('EDIT_INVOICEAGORA', SPNumber);
        if (FDoingRefund) then
          ParamByName('invoiceid').AsInteger := WBCreditNotes.RefreshID
        else
          ParamByName('invoiceid').AsInteger := WBInvoices.RefreshID;
        StringAsParam(ParamByName('postingid'), Transaction.Posting.PostingId);
        StringAsParam(ParamByName('venueid'), Transaction.Posting.VenueId);
        StringAsParam(ParamByName('receiptnumber'), Transaction.Posting.ReceiptNumber);
        StringAsParam(ParamByName('transactionid'), Transaction.Posting.TransactionId);
        BoolAsParam(ParamByName('bulkpost'), False);
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

  finally
    Transaction.Free;
  end;
end;                                                                                                                                                                           ['{1341FCEF-3140-4EE0-9FA8-78307EBB7344}']
{******************************************************************************}
function TformQuickSaleUser.SendRMSTransactions(SPNumber, TheTenderID: Integer): Boolean;
var
  I: Integer;
  TL: TTL;
  Transaction: TRMSTransaction;
  FoodCat, BevCat, MiscCat, TipCat: string;
  RefundFoodCat, RefundBevCat, RefundMiscCat, RefundTipCat: string;
  OL: TOrderLine;
  FRoomCharge: Boolean;
  ChargeRequest: TRMSChargeRequest;
  ReceiptRequest: TRMSReceiptRequest;
  TenderLineType: TTenderLineType;
  GLCode: Integer;
begin
  Transaction := TRMSTransaction.Create;
  Result := False;
  try
  //Build the room transfer first
    TL := TLList.GetTLOfTypeID(GetTLTID(TLTRMS));
    if ((not Assigned(TL)) and (AppDetails.RMSAllTransactions <> 1)) then
    begin
     //This should never happen cos we should only come here if TransExist
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

        FoodCat := FieldAsString(FieldByName('libglf'));
        BevCat := FieldAsString(FieldByName('libglb'));
        MiscCat := FieldAsString(FieldByName('libglmisc'));
        TipCat := FieldAsString(FieldByName('libgltip'));

        RefundFoodCat := FieldAsString(FieldByName('glcode2f'));
        RefundBevCat := FieldAsString(FieldByName('glcode2b'));
        RefundMiscCat := FieldAsString(FieldByName('glcode2misc'));
        RefundTipCat := FieldAsString(FieldByName('glcode2tip'));
      end;
    except
      on e: exception do begin
        dm.tr.Rollback;   //Jon 14-07-2003
        ShowMessage(e.message);
        Exit;
      end;
    end;

    FRoomCharge:= Assigned(TL);

    if (FDoingRefund) then
    begin
      for I:= 0 to (OLList.Count -1) do
      begin
        OL := OLList.Items[I];
        if (OL.ItemID > 0) then
        begin
          if (StrToIntDef(OL.GLCode2, 0) > 0) then
            GLCode := StrToIntDef(OL.GLCode2, 0)
          else if (OL.ForB = 'F') then
            GLCode := StrToIntDef(RefundFoodCat, 0)
          else
            GLCode := StrToIntDef(RefundBevCat, 0);
          with Transaction.CreditNoteList.NewCharge(GLCode) do
          begin
            accountId := GlbRMSAccount.AccountId;
            amount := amount + (OL.ILAmount - OL.ILDiscount);
            comment := 'INV ' + WBCreditNotes.RefreshID.ToString;
            if not AppDetails.RMSGroupSundries then
              comment := comment + ' - ' + OL.ItemString;
            dateOfTransaction := now;
            description := '';
            overrideExchangeRate := 0;
            source := AppDetails.RMSReceiptSource;
            sundryId := GLCode;
            thirdPartyClientId := 0;
            useRmsAccountingDateForPostingDate := True;
            useSecondaryCurrency := 'default';
            InvoiceLineID.Add(OL.InvoiceLineID);
            RoomTransfer := FRoomCharge;
          end;
        end;
      end;
    end
    else
    begin
      for I:= 0 to (OLList.Count -1) do
      begin
        OL := OLList.Items[I];
        if (OL.ItemID > 0) then
        begin
           if (StrToIntDef(OL.GLCode, 0) > 0) then
            GLCode := StrToIntDef(OL.GLCode, 0)
          else if (OL.ForB = 'F') then
            GLCode := StrToIntDef(FoodCat, 0)
          else
            GLCode := StrToIntDef(BevCat, 0);
          with Transaction.ChargeRequestList.NewCharge(GLCode) do
          begin
            accountId := GlbRMSAccount.AccountId;
            amount := amount + (OL.ILAmount - OL.ILDiscount);
            comment := 'INV ' + WBInvoices.RefreshID.ToString;
            if not AppDetails.RMSGroupSundries then
              comment := comment + ' - ' + OL.ItemString;
            dateOfTransaction := now;
            description := '';
            overrideExchangeRate := 0;
            source := AppDetails.RMSReceiptSource;
            sundryId := GLCode;
            thirdPartyClientId := 0;
            useRmsAccountingDateForPostingDate := True;
            useSecondaryCurrency := 'default';
            InvoiceLineID.Add(OL.InvoiceLineID);
            RoomTransfer := FRoomCharge;
          end;
        end;
      end;
    end;

    for I:= 0 to (TLList.Count - 1) do
    begin
      TL := TLList.Items[I];
      if (TL.TLTypeID <> GetTLTID(TLTRMS)) then
      begin
        TenderLineType := GetTenderLineTypeRecord(TL.TLType);
        if (TL.TLTip > 0) then
        begin
          GLCode := StrToIntDef(TipCat, 0);
          with Transaction.ChargeRequestList.NewCharge(GLCode) do
          begin
            accountId := GlbRMSAccount.AccountId;
            amount := amount + TL.TLTip;
            comment := 'INV ' + WBInvoices.RefreshID.ToString;
            if not AppDetails.RMSGroupSundries then
              comment := comment + ' - Tip';
            dateOfTransaction := now;
            description := '';
            overrideExchangeRate := 0;
            source := AppDetails.RMSReceiptSource;

            thirdPartyClientId := 0;
            useRmsAccountingDateForPostingDate := True;
            useSecondaryCurrency := 'default';
            RoomTransfer := FRoomCharge;
          end;
        end;

        if (TL.SurchargeItemID > 0) and (TL.TLSurcharge > 0) then
        begin
          if (StrToIntDef(TL.SurchargeGLCode, 0) > 0) then
            GLCode := StrToIntDef(TL.SurchargeGLCode, 0)
          else
            GLCode := StrToIntDef(MiscCat, 0);
          with Transaction.ChargeRequestList.NewCharge(GLCode) do
          begin
            accountId := GlbRMSAccount.AccountId;
            amount := amount + TL.TLSurcharge;
            comment := 'INV ' + WBInvoices.RefreshID.ToString;
            if not AppDetails.RMSGroupSundries then
              comment := comment + ' - ' + TL.SurchargeItemAbbrev;
            dateOfTransaction := now;
            description := '';
            overrideExchangeRate := 0;
            source := AppDetails.RMSReceiptSource;
            sundryId := GLCode;
            thirdPartyClientId := 0;
            useRmsAccountingDateForPostingDate := True;
            useSecondaryCurrency := 'default';
            RoomTransfer := FRoomCharge;
          end;
        end;

        if (Not FDoingRefund) then

        with Transaction.ReceiptRequestList.NewReceipt do
        begin
          accountId := GlbRMSAccount.AccountId;
          if (FDoingRefund) then
          begin
            amount := amount - TL.TLPayment - TL.TLTip;
            comment := 'INV ' + WBCreditNotes.RefreshID.ToString + ' - ' + TL.TLTypeName;
          end
          else
          begin
            amount := amount + TL.TLPayment - TL.TLTip;
            comment := 'INV ' + WBInvoices.RefreshID.ToString + ' - ' + TL.TLTypeName;
          end;
          dateOfTransaction := now;
          description := '';
          receiptType :=  TenderLineType.LibGLCode;
          source := AppDetails.RMSReceiptSource;
          useRmsAccountingDateForPostingDate := True;
          useSecondaryCurrency := 'default';
          RoomTransfer := FRoomCharge;
          TenderLineIDList.Add(TL.TLID);
        end;
      end;
    end;

    Result := DoRMSTransaction(Transaction);

    try
      with dm.GetSP(SPNumber) do
      begin
        SetStoredProcName('EDIT_INVOICERMS', SPNumber);
        if (FDoingRefund) then
          ParamByName('invoiceid').AsInteger := WBCreditNotes.RefreshID
        else
          ParamByName('invoiceid').AsInteger := WBInvoices.RefreshID;
        IntegerAsParamIfBool(ParamByName('accountid'), GlbRMSAccount.AccountId, FRoomCharge);
        ExecProc;

        SetStoredProcName('EDIT_INVLINERMS', SPNumber);
        for ChargeRequest in Transaction.ChargeRequestList do
        begin
          for I := 0 to (ChargeRequest.InvoiceLineID.Count -1) do
          begin
            ParamByName('invoicelineid').AsInteger := ChargeRequest.InvoiceLineID.Get(I);
            IntegerAsParam(ParamByName('transactionid'), ChargeRequest.ChargeResponse.id);
            BoolAsParam(ParamByName('bulkpost'), False);
            ExecProc;
          end;
        end;

        SetStoredProcName('EDIT_TENLINERMS', SPNumber);
        for ReceiptRequest in Transaction.ReceiptRequestList do
        begin
          for I := 0 to (ReceiptRequest.TenderLineIDList.Count -1) do
          begin
            ParamByName('tenderlineid').AsInteger := ReceiptRequest.TenderLineIDList.Get(I);
            IntegerAsParam(ParamByName('transactionid'), ReceiptRequest.ReceiptResponse.id);
            BoolAsParam(ParamByName('bulkpost'), False);
            ExecProc;
          end;
        end;
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

  finally
    Transaction.Free;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.SendTaxCoreTransactions: Boolean;
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
    for I := 0 to (OLList.Count - 1) do
    begin
      OL := OLList.Items[I];
      if (OL.ItemID > 0) then
      begin
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
{******************************************************************************}
procedure TformQuickSaleUser.AddPrinterToPrinterList(List: TIntegerList; PrinterID: Integer);
begin
  if (PrinterID = AppDetails.PrinterID) then begin
    Exit;
  end;
  List.Add(PrinterID)
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddCCPrintersToPrinterList(List: TIntegerList);
var
  I: Integer;
  ListCount: Integer;

begin
  //All Current Printers are either Printer or Repeat printer
  //We need to add the respective CCPrinter if one exists
  ListCount := (List.Count - 1);

  if (not dm.qrPrinters.Active) then begin
    dm.qrPrinters.Open;
  end;

  //loop through list, if ccprinter not found add
  for I := 0 to ListCount do begin
    dm.qrPrinters.KeyFields.ByName('printerid').AsInteger := List.Get(I);
    //printer valid and has a ccprinterid add to OrderPrinterList, else next order line
    if (dm.qrPrinters.LookUpKeyForFields) and (not dm.qrPrinters.FieldByName('ccprinterid').IsNull) then
    begin
      AddPrinterToPrinterList(List, dm.qrPrinters.FieldByName('ccprinterid').AsInteger);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdButtonsClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  ShowButtons := True;
  SetButtons;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoSelectName; //akm 01.10.01
var
  LoginID, ParkSale: Integer;
  SwipedStaffCard: Boolean;

begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin
    Exit;
  end;

  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  StopInactiveTimer;
  LoginID := GlbLogin.LoginID;

  CancelLoginSession;
  if (SelectName(True, True, AppDetails.ProveStaffCashSale)) then
  begin
    GlbLogin.LoginPIN := 0;       //Jon 07-06-2005
    NoSalePINEntered := False;
    AddStaffLoginButton;

    if ((AppDetails.StaffAutoParkSale > 0) and (LoginID <> GlbLogin.LoginID)) then begin
      if (((OLList.Count > 0) or (TLList.Count > 0)) and (LoginID > 0)) then begin
        if (not TryParkSale(ParkSale, LoginID)) then
        begin
          if Self.Visible and Self.Enabled then
            Self.SetFocus;
          StartInactiveTimer;
          Exit;
        end;
        SwipedStaffCard := GlbScan.SwipedStaffCard;
        ResetCashSale(False, True);
        GlbScan.SwipedStaffCard := SwipedStaffCard;    //Because the reset cash sale was resetting this when we didn't want it to
      end;
      if (SelectParkedSale(True)) then begin
        TryLoadParkedSale;
      end;
    end
    else if ((OLList.Count = 0) and (TLList.Count = 0) and (VoidOLList.Count = 0)) then begin
      SetCurrentTime;
    end;
  end
  else if GlbLogin.LoginID = 0 then
  begin
    if OLList.Count > 0 then
    begin
      if (AppDetails.ClearItemsVoid > 0) or
         (EFTPOSTransExists or
          ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard)))) then //has eftpos /zapa/smartcard
      begin
        ShowQuickMsg(sStaffLogoutMiddleOfSale);
      end
      else if ShowConf(sStaffLogoutMiddleOfSaleConfirm) = mrYes then
      begin
        CancelCashSale;
      end;

      DoSelectName;
      Exit;
    end
    else
    begin
      CancelCashSale;
      Close;
    end;
  end;


  if Self.Visible and Self.Enabled then
    Self.SetFocus;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoSelectNameProximityCode(ProximityCard: string); //Jon 16-06-2003
var
  LoginID, ParkSale: Integer;
  SwipedStaffCard: Boolean;

begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin
    Exit;
  end;

  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  LoginID := GlbLogin.LoginID;

  if (SelectNameProximityCode(ProximityCard)) then begin
    GlbLogin.LoginPIN := 0;       //Jon 07-06-2005
    NoSalePINEntered := False;
    AddStaffLoginButton;

    if ((AppDetails.StaffAutoParkSale >  0) and (LoginID <> GlbLogin.LoginID)) then begin
      if (((OLList.Count > 0) or (TLList.Count > 0)) and (LoginID > 0)) then begin
        if (not TryParkSale(ParkSale, LoginID)) then
        begin
          if Self.Enabled then
            Self.SetFocus;
          Exit;
        end;
        SwipedStaffCard := GlbScan.SwipedStaffCard;
        ResetCashSale(False, True);
        GlbScan.SwipedStaffCard := SwipedStaffCard;    //Because the reset cash sale was resetting this when we didn't want it to
      end;
      if (SelectParkedSale(True)) then begin
        TryLoadParkedSale;
      end;
    end
    else if ((OLList.Count = 0) and (TLList.Count = 0) and (VoidOLList.Count = 0)) then begin
      SetCurrentTime;
    end;
  end;
  if Self.Enabled then
    Self.SetFocus;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoSelectNameStaffID(StaffID: Integer; ProveStaff: Boolean = False);
var
  LoginID, ParkSale: Integer;
  CurrentStaffID: Integer;
  SwipedStaffCard: Boolean;
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin
    Exit;
  end;

  StopStaffPromptTimeoutTimer;
  LoginID := GlbLogin.LoginID;
  CurrentStaffID := GlbLogin.StaffID;

  if (SelectNameStaffID(StaffID, ProveStaff)) then
  begin
    GlbLogin.LoginPIN := 0;
    AddStaffLoginButton;
    PINNotRqd := False;
    NoSalePINEntered := False;

    if ((AppDetails.StaffAutoParkSale > 0) and (LoginID <> GlbLogin.LoginID)) then begin
      if (((OLList.Count > 0) or (TLList.Count > 0)) and (LoginID > 0)) then begin
        if (not TryParkSale(ParkSale, LoginID)) then
        begin
          if Self.Enabled then
            Self.SetFocus;
          Exit;
        end;
        SwipedStaffCard := GlbScan.SwipedStaffCard;
        ResetCashSale(False, True);
        GlbScan.SwipedStaffCard := SwipedStaffCard;    //Because the reset cash sale was resetting this when we didn't want it to
      end;
      if (SelectParkedSale(True)) then begin
        TryLoadParkedSale;
      end;
    end
    else if ((OLList.Count = 0) and (TLList.Count = 0) and (VoidOLList.Count = 0)) then begin
      SetCurrentTime;
    end;
  end
  else
  begin
    dm.qrTerminalLogins.KeyFieldByName('staffid').AsInteger := CurrentStaffID;
    dm.qrTerminalLogins.LookupKeyForFields;
  end;
  if Self.Enabled then
    Self.SetFocus;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdSelectNameClick(Sender: TObject);
begin
  StopInactiveTimer;
  DoSelectName; //akm 01.10.01
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdFUpClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdItems.Handle, WM_VSCROLL, SB_PAGEUP, 0);
  StartInactiveTimer;
end;
procedure TformQuickSaleUser.cmdGoodyDesClick(Sender: TObject);
begin
  inherited;

end;

{******************************************************************************}
procedure TformQuickSaleUser.cmdFDownClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdItems.Handle, WM_VSCROLL, SB_PAGEDOWN, 0);
  if grdItems.DataSource.DataSet.Fields[0].IsNull then
    grdItems.DataSource.DataSet.Last;
  StartInactiveTimer;
end;

procedure TformQuickSaleUser.UPDPOBCOLOR(var Msg: TMessage);
begin
  if (AppDetails.CallerIDEnabled and AppDetails.CallerIDIncomingCaller) then
    cmdPhoneOrder.Color := clYellow
  else
    cmdPhoneOrder.Color := clWhite;
end;

{******************************************************************************}
function TformQuickSaleUser.TryLookupCashSaleItem(BarCode: string; PriceCode: string): Boolean;
begin
  Result := False;
  if ((CurrentSelectedTEdit <> nil) and (CurrentSelectedTEdit.Focused)) then begin
    CurrentSelectedTEdit.Text := CurrentSelectedTEditText;
  end;
  try
    if (not dm.TestConnection) then begin
      Exit;
    end;

    with dm.sp do begin
      SetStoredProcName('GETBARCODEITEM');       //getbarcodeitem

      ParamByName('barcode').AsString := BarCode;
      ExecProc;
      if (not FieldByName('errorcode').IsNull) then begin
        //ShowErrorMsg(FieldByName('errorcode').AsInteger);
        Exit;
      end
      else if (not FieldByName('itemid').IsNull) then begin
        BarCodePriceCode := PriceCode;
        FAddingDuplicateItem := False;
        ComboFreeItems.Clear;
        Result := True;  //we found the item so dont tell user not found if OrderItem returns false.
        OrderItem(FieldByName('itemid').AsInteger);
        AddFreeComboItemsToOrderList;
        BarCodePriceCode := '';
      end;
    end;
  except on e: exception do begin
    ShowMessage(e.message);
  end; end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.grdItemsCellClick(Sender: TObject; ACol, ARow: Integer; AButton: TMouseButton; AShift: TShiftState);
var
  aSelectedItemID: Integer;

begin
  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;
  
  StopInactiveTimer;

  if AppDetails.PromptItemSizeSelection then
  begin
    if (Assigned(GlobalMenuList.ListParentItems.Find(grdItems.DataSource.Dataset.FieldByName('itemid').AsInteger))) then // means parent item selected
    begin
      if (not PromptAndGetDifferentSizeItem(
            grdItems.DataSource.Dataset.FieldByName('itemid').AsInteger,
            grdItems.DataSource.Dataset.FieldByName('ItemAbbrev').AsString,
            CurrentTime, CurrentDay, aSelectedItemID)) then
      begin
        Exit;
      end;
      if (ItemView = ivBev) then
      begin
        dm.qrOBItems.Locate('itemid', aSelectedItemID, []);
      end
      else if (ItemView = ivFood) then
      begin
        dm.qrOFItems.Locate('itemid', aSelectedItemID, []);
      end;
    end;
  end;
  ComboFreeItems.Clear;
  OrderItemST(grdItems.DataSource.Dataset);
  AddFreeComboItemsToOrderList;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.OpenArchButtonClick(Sender: TObject);
var
  OpenArchButton: TOpenArchButton;
  aItemID, aSelectedItemID: Integer;
  aItem: TItem;

begin
  StopInactiveTimer;
  try
    if (Sender is TOpenArchButton) then begin
      OpenArchButton := Sender as TOpenArchButton;

      if (OpenArchButton.WPrivID > 0) then begin
        if (not VerifyPriv(OpenArchButton.WPrivID)) then begin
          ShowMsg(sCannotUseThisBtn);
          OpenArchButton.Down := False;
          Exit;
        end;
        //verify pinno
        if ((GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
          OpenArchButton.Down := False;
          Exit;
        end;
      end;

      FAddingDuplicateItem := False;

      case OpenArchButton.QSButtonAction of
        qsbaItem:
          begin
            ComboFreeItems.Clear;
            if ((NextItemButtonState = 2) and (OpenArchButton.AssociatedItem2 > 0)) then
            begin
              aItemID := OpenArchButton.AssociatedItem2;
            end
            else
            begin
              aItemID := OpenArchButton.AssociatedItem;
            end;
            aItem := GlobalMenuList.ListItems.Find(aItemID);
            if (not Assigned(aItem)) then
            begin
              Exit;
            end;

            aSelectedItemID := aItemID;
            if AppDetails.PromptItemSizeSelection and (Assigned(GlobalMenuList.ListParentItems.Find(aItemID))) then
            begin
              if (not PromptAndGetDifferentSizeItem(aItemID, aItem.ItemAbbrev, CurrentTime, CurrentDay, aSelectedItemID)) then
              begin
                Exit;
              end;
            end;
            OrderItem(aSelectedItemID);
            AddFreeComboItemsToOrderList;
          end;
        qsbaModifier:
          begin
            OrderModifier(OpenArchButton.AssociatedItem, OpenArchButton.AssociatedItem2);
          end;
        qsbaPage:
          begin
            BackPagesList.Add(OpenArch.CurrentQSPageID);
            PostMessage(formMain.Handle, WM_QSCHANGEPAGE, OpenArchButton.AssociatedItem, 0);
          end;
        qsbaSaleCategory:
          begin
            if (not OnTable) or (GlbTable.GroupID <= 0) then
            begin
              SetSaleCategory(OpenArchButton.AssociatedItem);
            end;
          end;
        qsbaModFunction:
          begin
            if ((FModifierFunction >= 2) and (OpenArchButton.AssociatedItem = FModifierFunction)) then begin
              ModifierFunctionChange(1);
            end
            else begin
              ModifierFunctionChange(OpenArchButton.AssociatedItem);       //Jon 06-06-2003
            end;
          end;
        qsbaPriceLevel:
          begin
            if ((OpenArchButton.AssociatedItem >= 1) and (OpenArchButton.AssociatedItem <= 6)) then begin
              if ((OpenArchButton.AssociatedItem = PriceLevel) and (OpenArchButton.AssociatedItem2 = PriceLevelType)) then begin
                PriceLevelChange(0, OpenArchButton.AssociatedItem2);
              end
              else begin
                PriceLevelChange(OpenArchButton.AssociatedItem, OpenArchButton.AssociatedItem2);
              end;
            end;
          end;
        qsbaCourse:
          begin
            OrderCourse(OpenArchButton.AssociatedItem, OpenArchButton.AssociatedItem2);
          end;
        qsbaAssociatedItem:
          begin
            if (NextItemButtonState = OpenArchButton.AssociatedItem) then begin
              NextItemButtonStateChange(1);
            end
            else begin
              NextItemButtonStateChange(OpenArchButton.AssociatedItem);
            end;
          end;
          qsbaTask: begin
            case OpenArchButton.AssociatedItem of
              1:
                //Manual barcode entry
                ManualBarCodeEntry;
               //Load Applications 2 to 5
              2: if (ProcessExecute(AppDetails.RunMusic, SW_SHOW)<>0) then
                    ProcessExecute(AppDetails.AppData + '\' + AppDetails.RunMusic, SW_SHOW);
              3: if (ProcessExecute(AppDetails.RunReports, SW_SHOW) <> 0) then
                    ProcessExecute(AppDetails.AppData + '\' + AppDetails.RunReports, SW_SHOW);
              4: if (ProcessExecute(AppDetails.RunQSDesigner, SW_SHOW) <> 0) then
                    ProcessExecute(AppDetails.AppData + '\' + AppDetails.RunQSDesigner, SW_SHOW);
              5: if (ProcessExecute(AppDetails.RunSecurity, SW_SHOW) <> 0) then
                    ProcessExecute(AppDetails.AppData + '\' + AppDetails.RunSecurity, SW_SHOW);
                //Cashups 6 - Cashup, 7 - Reading, 8 - Stats
              6: DoCashup(True);
              7: DoCashup(False);
              8: DoStats;
              9: GetSingleItemNote;
            end;
          end;
        qsbaTenderType:
          begin
            AddTenderLine(GetTLT(OpenArchButton.AssociatedItem));
          end;
        qsbaAccount:
          begin
            if (OpenArchButton.AssociatedItem2 > 0) then begin
              case OpenArchButton.AssociatedItem of
                1: cmdSelectAccountClick(self);
                2: cmdAccountNoLookupClick(self);
                3:
                  begin
                    if (OnTable) then
                    begin
                      ShowMsg(sTabHasSelected);
                    end
                    else if (cmdWaste.Down or cmdRefund.Down or cmdPhoneOrder.Down) then
                    begin
                      ShowMsg(sNoReplacementSwipeCard);
                    end
                    else
                    begin
                      ReplaceSwipeCardQS;
                    end;
                  end;
              end;
            end
            else begin

              //if in a Goody discounted sale, disallow loading a different account
              if InGoodyDiscountedSale then
              begin
                ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
                Exit;
              end;

              if (CanSelectAccount) then begin
                GlbAccount.LookupAccountID := OpenArchButton.AssociatedItem;
                LoadAccount(GlbAccount.LookupAccountID);
              end;
            end;
          end;
        qsbaCombo:
          begin
            OrderCombo(OpenArchButton.AssociatedItem);
          end;
        qsbaCreateAccount:
          begin
            QSCreateAccount(OpenArchButton.AssociatedItem);
          end;
        qsbaExternalAccount:
          begin
             DoExternalAccountButtonClick(False, OpenArchButton.AssociatedItem);
          end;
      end;
    end;
  finally
    StartInactiveTimer;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.OpenArchButtonHeld(Sender: TObject);
var
  OpenArchButton: TOpenArchButton;
begin
  if (Sender is TOpenArchButton) then begin
    OpenArchButton := Sender as TOpenArchButton;

    if (OpenArchButton.QSButtonAction = qsbaItem) then begin
      if ((NextItemButtonState = 2) and (OpenArchButton.AssociatedItem2 > 0)) then begin
        ShowRecipe(OpenArchButton.AssociatedItem2);
      end
      else begin
        ShowRecipe(OpenArchButton.AssociatedItem);
      end;
    end;
  end;
end;

{******************************************************************************}
function TformQuickSaleUser.OrderItem(ItemID: Integer; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
var
  Item: TItem;
  ItemsQty: Currency;
  BarcodeOpenPrice: Boolean;
  aOL: TOrderLine;
  PLevel: Integer;
  I: Integer;
  SaleCategory: TSaleCategory;
  Course: TCourse;
  lComboLink: TComboGroupLink;
  lCombo: TCombo;

begin
  Result := False;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;
        
  BarcodeOpenPrice := False;
  if not AddingFreeComboItem then
    ComboFreeItems.Clear;

  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  if StillAddingAnItem then
    Exit;

  if ((OLList.Count = 0) {and (GlbAccount.AccountID > 0)}) then
  begin
    DiscCalculator.CurrentTime := CurrentTime;
    DiscCalculator.CurrentDay := CurrentDay;
  end;

  StillAddingAnItem := True;
  try
    if ((AppDetails.EnableFiscalPrinting) and
        (AppDetails.FiscalLiveInvoice) and
        (not DoingPhoneOrder) and
        (not (OnTable and (GlbTable.GroupID > 0))) and
        (GlbTable.SaleCategoryType <= scStandard)) then
    begin
      if ((not (NFManual or NFEMode)) and
          ((not SFiscalPrinting.FiscalPrinterEnabled) or (not SFiscalPrinting.GetFiscalPrinterDetails))) then
      begin
        StartInactiveTimer;
        StillAddingAnItem := False;
        Exit;
      end;
    end;

    ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
    if (not SaleCategorySelected) then begin
      StartInactiveTimer;
      StillAddingAnItem := False;
      Exit; //AKM 3.3.1
    end;
    //Reset the current time & day as this is best point to define start time of sale
    if ((not SFiscalPrinting.OpenedFiscalInvoice) and (OLList.Count = 0)) then
    begin
      SetCurrentTime;
      SetSaleCategory(GlbTable.SaleCategoryID);
      if ((AppDetails.EnableFiscalPrinting) and
          (AppDetails.FiscalLiveInvoice) and
          (not (NFManual or NFEMode)) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0))) and
          (GlbTable.SaleCategoryType <= scStandard)) then
      begin
        try
          if (not SFiscalPrinting.GetFiscalPrinterConfigurations) then
          begin
            //ShowQuickMsg(sPrinterSystemDateMismatch);
            PostMessage(Self.Handle, WM_CLOSE, 0, 0);
            Result := False;
            Exit;
          end;

          FCreditCardDiscount := 0;
          ShowQuickMsgOnTop(sOpeningFiscalInvoice);
          if (not SFiscalPrinting.OpenFiscalPrinterInvoice(FCPFNumber, GlbAccount.PerorgName, GlbAccount.PerorgAddress)) then
          begin
            StartInactiveTimer;
            Exit;
          end;
          SFiscalPrinting.GetMovementDate;
          SWBEncryption.GenerateEncryptedAuxFile;
          SFiscalPrinting.GetCOOForInvoice; // retrive the last COO in SFiscalPrinting.FCOO . This has to be stored in invoice table
          SFiscalPrinting.GetCCFForInvoice;

          FInvCOO := SFiscalPrinting.COO;
          FInvCCF := SFiscalPrinting.CCF;

          cmdTable.Enabled := False;
          cmdPhoneOrder.Enabled := False;
        finally
          FreeQuickMsg;
        end;
      end;
    end;

    ModifierFunctionChange(1);       //Reset Modifier function to +
    if ((AppDetails.DocketOverrideSCExist) or (AppDetails.CashSaleAllowSpecialPrices)) then begin
    //We don't want to handle it.
    //Get the details direct from the DB
    //Mainly for alternate printer
      StillAddingAnItem := False;
      Result := OrderItemSP(ItemID, AddingFreeComboItem, aComboID, aComboGroupID, aComboIndex);
      StartInactiveTimer;
      Exit;
    end;

    Item := GlobalMenuList.ListItems.Find(ItemID);
    if (not Assigned(Item)) then
    begin
      StillAddingAnItem := False;
      StartInactiveTimer;
      Exit;
    end;
    GlbItem.Item := Item.ItemAbbrev;
    GlbItem.ItemID := Item.ItemID;
    GlbItem.ItemGroupID := Item.ItemGroupID;
    GlbItemModifiers.ForB := Item.FORB;
    //GlbItem.TaxType := Item.TaxType;
    GlbItem.TaxSituation := Item.TaxSituation;
    GlbItem.CSOSN := Item.CSOSN;
    GlbItem.CST_ICMS := Item.Cst_Icms;
    GlbItem.NCMProductCode := Item.NCMProductCode;
    GlbItem.Barcode := Item.BarCode;
    GlbItem.ProductOrigin := 0;
    if Length(GlbItem.CST_ICMS) >= 3 then
    begin
      GlbItem.ProductOrigin := StrToIntDef(Copy(GlbItem.CST_ICMS, 1, 1), 0);
      GlbItem.CST_ICMS := Copy(GlbItem.CST_ICMS, 2, Length(GlbItem.CST_ICMS) - 1);
    end;
    GlbItem.CFOP := Item.CFOP;
    GlbItem.CST_PIS := Item.CST_PIS;
    GlbItem.CST_COFFINS := Item.CST_COFFINS;
    GlbItem.GTIN := Item.GTIN;

    DiscCalculator.AddingFreeComboItems := True;

    if ((PriceLevel >= 1) and (PriceLevel <= 6)) then
    begin
      PLevel := PriceLevel;        //Local Price Level for item
    end
    else begin
      PLevel := GlbTable.PriceLevel;     //Price level as set by sales category
      if (Item.IGSaleCategoryID > 0) then begin
        SaleCategory := GlobalMenuList.ListSaleCategorys.Find(Item.IGSaleCategoryID);
        if (Assigned(SaleCategory)) then begin
          PLevel := SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay);
        end;
      end;
    end;
    GlbItem.ThePrice := Item.ItemPrice[PLevel];

    if ((Item.OpenPrice) and (GlbTimePeriod.TimePeriodID = 0)) then begin
      //check if have privilege
      if (not VerifyPriv(24)) then begin
        ShowMsg(sAdminOpenPrice);
        StillAddingAnItem := False;
        Exit;
      end;
      //verify pinno
      if ((not GlbPrivs[24].AvoidPINCheck) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
        StartInactiveTimer;
        StillAddingAnItem := False;
        Exit;
      end;

      if (not GetOpenPrice(GlbItem.ThePrice, Item.OpenPriceMin, Item.OpenPriceMax)) then begin
        StartInactiveTimer;
        StillAddingAnItem := False;
        Exit;
      end;

      if (AppDetails.OrderOpenPriceNameChange) then begin
        if (not GetSingleNote(GlbItem.Item, 25, sItemName, sItemNameWithColon, tiPieSlice, True)) then begin
          StartInactiveTimer;
          StillAddingAnItem := False;
          Exit;
        end;
      end;
    end;

  if ((not FAddingDuplicateItem) and ((not AddingComboItems) or ForcedNextQty)) then
  begin

    if ( (NextQty > 1)) then
    begin
      OrderingQty := 1;
    end
    else
      OrderingQty := NextQty;
  end
  else
    OrderingQty := 1;

  if (BarCodePriceCode = '') then begin
    if (Item.EnterQty > 0) then begin
      OrderingQty := Item.EnterQty;
      if (not GetQuantity(OrderingQty, gqQuantity)) then begin
        StartInactiveTimer;
        StillAddingAnItem := False;
        Exit;
      end;
    end
    else if (Item.WeighedItem) then begin
      if (not GetQuantity(OrderingQty, gqWeight)) then begin    // OrderingQty always comes in grams from quantity form
        StartInactiveTimer;
        StillAddingAnItem := False;
        Exit;
      end;
    end;
  end
  else begin
    if (Item.WeighedItem) then begin
      OrderingQty := StrToIntDef(BarCodePriceCode, 0) / 1000;
    end
    else begin
      OrderingQty := 1;
      BarCodeOpenPrice := True;
      GlbItem.ThePrice := (StrToIntDef(BarCodePriceCode, 0)) / 100;
    end;
    if (OrderingQty = 0) then begin
      StartInactiveTimer;
      StillAddingAnItem := False;
      Exit;
    end;
  end;

  OrderingQty := CRoundTo(OrderingQty, 3);

  AvailQtyTested := False;   //Jon 14-05-2003
  if (AppDetails.OrderDecrementItemQty > 0) then begin
    ItemsQty := 0;
    for I := (OLList.Count - 1) downto 0 do begin
      aOL := OLList.Items[I];
      if (aOL.ItemID = Item.ItemID) then begin
        if (aOL.AvailQtyOK) then begin
          AvailQtyTested := True;
        end;
        ItemsQty := ItemsQty + aOL.Qty;
      end;
    end;
    if ((AppDetails.OrderDecrementItemQty in [2, 3]) or (not AvailQtyTested)) then begin
      if (not CheckItemQty(OrderingQty + ItemsQty, Item.ItemID, AvailQtyTested)) then begin
        StartInactiveTimer;
        Exit;
      end;
    end;
  end;

//  if ((not FAddingDuplicateItem) and (not AddingComboItems)) then
  if (not AddingComboItems) then
  begin
    GlbItemModifiers.AddingItem := True;
    LoadMods;
  end;

  if ((((not AppDetails.CashSaleBevModsCompulsory) and (Item.ForB = 'B')) or (GlbTimePeriod.TimePeriodID <> 0))  or
      ((FAddingDuplicateItem) or (not FAddingDuplicateItem) and (AddingComboItems or formItemModifiers.GetMods))) then
  begin
    // JEH 18/08/2001 1st clear change
    if (OLList.Count = 0) then begin
      txtCChange.Caption := '0.00';
    end;
    //add to OLList
    aOL := TOrderLine.Create;
    with aOL do
    begin
      OrderLineOrder := OLList.Count + 1;
      FixThisComboPart := AddingComboItems;
      AllowEdit := True;
      ItemID := Item.ItemID;
      ItemGroupID := Item.ItemGroupID;
      ItemString := GlbItem.Item;
      ItemGrpType := (GlbItem.ItemGrpType);

      // Fiscal Fields
      TaxSituation := GlbItem.TaxSituation;
      CSOSN := GlbItem.CSOSN;
      CSTICMS := GlbItem.CST_ICMS;
      NCMProductCode := GlbItem.NCMProductCode;
      CFOP := GlbItem.CFOP;
      ProductOrigin := GlbItem.ProductOrigin;
      BarCode := GlbItem.Barcode;
      CST_PIS := GlbItem.CST_PIS;
      CST_COFFINS := GlbItem.CST_COFFINS;

      Qty := OrderingQty;       //Jon 12-03-2004
      OriginalQty := OrderingQty;
      FORB := Item.ForB;
      PrinterID := Item.PrinterID;
      RepeatPrinterID := Item.RepeatPrinterID;
      OpenPrice := Item.OpenPrice or BarcodeOpenPrice;         //Jon 20-7-2001
      OpenPriceMin := Item.OpenPriceMin;   //Jon 20-7-2001
      OpenPriceMax := Item.OpenPriceMax;   //Jon 20-7-2001
      WeighedItem := Item.WeighedItem;

      LastNormalItem := False;
      if ((PriceLevel >= 1) and (PriceLevel <= 6)) then begin
        PLevel := PriceLevel;        //Local Price Level for item
        OLPriceLevel := PriceLevel;       //Choose this price level
      end
      else begin
        PLevel := GlbTable.PriceLevel;     //Price level as set by sales category
        OLPriceLevel := 0;                //Choose the salecategory price level

        if (Item.IGSaleCategoryID > 0) then begin   //Jon 29-09-2003
          SaleCategory := GlobalMenuList.ListSaleCategorys.Find(Item.IGSaleCategoryID);
          if (Assigned(SaleCategory)) then begin
            PLevel := SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay);
          end;
        end;
      end;
      OriginalUnitPrice := Item.ItemPrice[PLevel];   //AKM 3.3.1

      for I := 1 to 6 do
      begin
        ItemPrices[I] := Item.ItemPrice[I];
      end;

      PriceExcludesTax := Item.PriceExcludesTax;     //Jon 29-5-2001
      SalesTaxPercent := Item.SalesTaxPercent;       //Jon 29-5-2001

      if (OpenPrice) then begin           //Jon 23-7-2001
        OriginalUnitPrice := GlbItem.ThePrice;
      end;

      if ((AppDetails.TaxExclusivePrices) and (not PriceExcludesTax)) then begin
        OriginalUnitPrice := ((OriginalUnitPrice * 100) / (100 + SalesTaxPercent));
      end
      else if ((not AppDetails.TaxExclusivePrices) and (PriceExcludesTax)) then begin
        OriginalUnitPrice := ((OriginalUnitPrice * (100 + SalesTaxPercent)) / 100);
      end;

      //if item.WeighedItem then
        //OriginalUnitPrice := (OriginalUnitPrice / 10); // to get the price in grams since we store price for 10grams in db for fiscal version}

      UnitPrice := OriginalUnitPrice;     //Jon 19-7-2001
      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
      begin
        UnitPrice := TruncateTo(UnitPrice, 2);
        Qty := TruncateTo(Qty, 3);
      end;
      ToPay := GetRoundedUpDown(UnitPrice * Qty, AppDetails.DecimalPlaces);                     //Jon 11-02-2005
      Course := GlobalMenuList.ListCourses.Find(Item.CourseID);
      if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
        ToPay := TruncateTo(UnitPrice * Qty, 2);                     //Jon 11-02-2005

      if ((CurrentCourseID <= 0) and (AppDetails.CashSaleUseCourses) and (Assigned(Course))) then
      begin
        CourseID := Course.CourseID;
        CourseName := Course.Course;
        CourseIsMain := Course.IsMain;
      end
      else
      begin
        CourseID := CurrentCourseID;         //Jon 28-02-2002
        CourseName := CurrentCourseName;
        CourseIsMain := CurrentCourseIsMain; //Jon 07-05-2003
      end;
      ModsChangePrinter := False;      //Jon 03-06-2003
      AvailQtyOK := AvailQtyTested;    //Jon 14-05-2003
      AlreadyOrdered := False;         //Jon 10-03-2004
      AlreadyOrderedQty := 0;

      if ((AppDetails.EnableFiscalPrinting) and (GlbAccount.AccountID <= 0)) then
      begin
        aOL.ILAmount := aOL.ToPay;
        aOL.ILDiscount := 0;
        aOL.DiscountedAmount := aOL.ToPay;
        //aOL.IsFiscalDiscountDone := False;
      end;

      ApplyDiscountType := adtAllItems;
      if AddingComboItems then
      begin
        lCombo := FAllCombos.GetCombo(aComboID);
        if Assigned(lCombo) then
        begin
          lComboLink := lCombo.GetComboGroupLink(aComboGroupID);
          if Assigned(lComboLink) then
          begin
            ApplyDiscountType := lComboLink.ApplyDiscountType;
            FreeComboItem := (lComboLink.ComboGroup.ComboGroupType = cgtFree);
          end;
        end;
        ComboID := aComboID;
        ComboGroupID := aComboGroupID;
        ComboQty := Qty;
        FreeComboQty := 0;
        if FreeComboItem then
          FreeComboQty := Qty;
        ComboTempQty := Qty;
        ComboIndex := aComboIndex;
        IsFiscalDiscountDone := True;
      end;
      if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem)) then begin
        LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, ItemID);
        AllocateItemLoyalty(aOL);
      end;

      if (AppDetails.CashSaleShowLoyaltyPoints) then begin
        LoyaltyPoints1RatioNum := Item.LoyaltyPoints1RatioNum;
        LoyaltyPoints1RatioDen := Item.LoyaltyPoints1RatioDen;
        LoyaltyPoints1Qty := Item.LoyaltyPoints1Qty;
        LoyaltyPoints2RatioNum := Item.LoyaltyPoints2RatioNum;
        LoyaltyPoints2RatioDen := Item.LoyaltyPoints2RatioDen;
        LoyaltyPoints2Qty := Item.LoyaltyPoints2Qty;
      end;
      GLCode := Item.GLCode;
      GLCode2 := Item.GLCode2;
      GTIN := Item.GTIN;
      TaxCoreLabel := Item.TaxCoreLabel;
      JustAdded := True;
    end;

    if ((AppDetails.EnableFiscalPrinting) and
        (not (NFManual or NFEMode)) and
        (not DoingPhoneOrder) and
        (not (OnTable and (GlbTable.GroupID > 0)))) then
    begin
      if ((aOL.ItemID <= 0) or ((aOL.Qty - aOL.LoyaltyFreeQty) <= 0) or (aOL.UnitPrice <= 0)) then
      begin
        ShowQuickMsg(sInvalidFiscalItem);
        if Assigned(aOL) then
        begin
          aOL.Free;
        end;
        StartInactiveTimer;
        Exit;
      end;
    end;

    OLList.Add(aOL);

    Result := True;
    //add to grid
    with grdOLs do begin
      if (OLList.Count > 1) then begin
        RowCount := RowCount + 1;
      end;

      //select OL row
      Row := (RowCount - 1);
      UpdateOLGridRow(Row, aOL);
      FAddedFiscalRow := Row;
    end;
    FAddingFiscalItem := True;

    //add any mods from the item modifier form
    //if not AddingFreeComboItem then
    AddMods;
    AddFiscalItemNote;
    PriceLevelChange(0, 1);
    CourseChange(0, '', 1, False);
    NextItemButtonStateChange(1);   //Jon 22-04-2002

    SetNextQty(1);
    if (ShowingQty) and (AppDetails.AutoCloseNextQty) then begin
      ShowQty(False);
    end;

    //BuildItemDiscounts;
    if (not AddingFreeComboItem) and (not AddingComboItems) then
      CalculateToPay;

    Application.ProcessMessages;
    //select the OL added, just in case added mods and on last mod row
    I := FindOLJustAddedIndex;
    if (I >= 0) then
    begin
      grdOLs.Row := I;
      aOL := OLList.Items[I];
      aOL.JustAdded := False;
    end;
    UpdateOLGridRow(grdOLs.Row, aOL);
    if ((AppDetails.EnableFiscalPrinting) and
        (not (NFManual or NFEMode)) and
        (not DoingPhoneOrder) and
        (not (OnTable and (GlbTable.GroupID > 0)))) then
    begin
      Application.ProcessMessages;

      if aOL.DiscountedAmount <= 0 then
      begin
        ShowQuickMsg(sInvalidFiscalItem);
        aOL.Free;
        OLList.Delete(OLList.Count-1); //delete the last item added if the final amount after discount is <= 0
        UpdateOLGridRow(grdOLs.Row, nil);
        grdOLs.RowCount := grdOLs.RowCount - 1; // delete the last row added;

        StartInactiveTimer;
        Exit;
      end;

      if ((aOL.ItemID > 0) and ((aOL.Qty - aOL.LoyaltyFreeQty) > 0) and (aOL.UnitPrice > 0)) then
      begin
        if (not AddingComboItems) then
        begin
          SendOrderLineToFiscalPrinter(aOL, grdOLs.Row);
        end;
      end;
    end;
    //Pole Display Jon 29-11-2001
    if (AppDetails.PDEnabled) then begin
      DMComponents.PDOutputItemLine(aOL.ItemString, GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces), 0, False);
      DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
    end;
    TemporarilyDeactiveButtons;
  end;
  if ((Item.Happy) and (AppDetails.CashSaleItemAutoTender)) then begin
    ItemAutoTenderActive := True;
    ButtonsDeactivated := False;
    cmdQuickCashClick(self);
    StartInactiveTimer;
  end;

  if ((not FAddingDuplicateItem) ) then
  begin
    FAddingDuplicateItem := False;
    StartInactiveTimer;
    Exit;
  end;

  if not FAddingDuplicateItem then
  begin
    if not AddingComboItems then
      DisposeMods;

    GlbItemModifiers.AddingItem := False;
  end;

  cmdCPF.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdCNPJ.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  ShowCPFNUmberInActivate := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdSelectAccount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdAccountNoLookup.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdDeselectAccount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdDiscountPercent.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
  cmdDiscountAmount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);

  FAddingFiscalItem := False;
  finally
    DisposeMods;
    CancelPOSInvoiceIfFiscalInvCancelled;
    DiscCalculator.AddingFreeComboItems := False;
    StillAddingAnItem := False;
    BroadcastCDAMessage;
    StartInactiveTimer;
  end;
end;

{******************************************************************************}
function TformQuickSaleUser.OrderItemSP(ItemID: Integer; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
begin
  StopInactiveTimer;
  Result := False;
  with dm.sp2 do
  begin
    if (not dm.TestConnection) then
      Exit;

    if StillAddingAnItem then
      Exit;

    StillAddingAnItem := True;
    try
      if ((OLList.Count = 0) and (TLList.Count = 0) and (VoidOLList.Count = 0)) then
      begin
        if ((AppDetails.EnableFiscalPrinting) and
            (AppDetails.FiscalLiveInvoice) and
            (not (NFManual or NFEMode)) and
            (not DoingPhoneOrder) and
            (not (OnTable and (GlbTable.GroupID > 0))) and
            (GlbTable.SaleCategoryType <= scStandard)) then
        begin
          try
            if (not SFiscalPrinting.OpenedFiscalInvoice) then
            begin
              if (not SFiscalPrinting.GetFiscalPrinterConfigurations) then
              begin
                //ShowQuickMsg(sPrinterSystemDateMismatch);
                PostMessage(Self.Handle, WM_CLOSE, 0, 0);
                Result := False;
                Exit;
              end;

              ShowQuickMsgOnTop(sOpeningFiscalInvoice);
              if (not SFiscalPrinting.OpenFiscalPrinterInvoice(FCPFNumber, GlbAccount.PerorgName, GlbAccount.PerorgAddress)) then
              begin
                StartInactiveTimer;
                Exit;
              end;
              SFiscalPrinting.GetCOOForInvoice; // retrive the last COO in SFiscalPrinting.FCOO . This has to be stored in invoice table
              SFiscalPrinting.GetCCFForInvoice;
              if AppDetails.EnableFiscalPrinting then
              begin
                cmdTable.Enabled := ((not AppDetails.DisableTableFeature) and (not SFiscalPrinting.OpenedFiscalInvoice));
                cmdPhoneOrder.Enabled := (not SFiscalPrinting.OpenedFiscalInvoice);
              end;
            end;
          finally
            FreeQuickMsg;
          end;
        end;
      end;
      SetStoredProcName('GET_OAITEM', 2);

      ParamByName('pitemid').AsInteger := ItemID;
      ParamByName('terminalid').AsInteger := AppDetails.TerminalID;
      ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;

      ExecProc;

      StillAddingAnItem := False;
//      ComboFreeItems.Clear;
      Result := OrderItemST(dm.sp2, AddingFreeComboItem, aComboID, aComboGroupID, aComboIndex);
{      if (not AddingFreeComboItem) then
      begin
        AddFreeComboItemsToOrderList;
      end;}
    finally
      StillAddingAnItem := False;
      StartInactiveTimer;
    end;
  end;
end;
{******************************************************************************}
function TFormQuickSaleUser.OrderItemST(Statement: TIB_Statement; AddingFreeComboItem: Boolean = False; aComboID: Integer = 0; aComboGroupID: Integer = 0; aComboIndex: Integer = 0): Boolean;
var
  PLevel: Integer;
  ItemsQty: Currency;  //Jon 14-05-2003
  J: Integer;         //Jon 14-05-2003
  aOL: TOrderLine;
  RowToSelect: Integer;
  IGSaleCategoryID: Integer;
  SaleCategory: TSaleCategory;

  ItemCourseID: Integer;
  ItemCourse: string;
  ItemCourseIsMain: Boolean;   //Jon 05-05-2003
  ItemFORB: string;
  ItemItemID: Integer;
  ItemItemGroupID: Integer;
  ItemItemAbbrev: string;
  ItemCurrentPrinter: Integer;
  ItemDefaultPrinter: Integer;
  ItemCurrentRepeatPrinter: Integer;
  ItemDefaultRepeatPrinter: Integer;
  ItemUnitPrice: Currency;
  ItemPrice: Currency;
  ItemExcludePrice: Currency;      //Jon 28-5-2001
  ItemIncludePrice: Currency;      //Jon 28-5-2001
  ItemPriceExcludesTax: Boolean;   //Jon 28-5-2001
  ItemSalesTaxPercent: Real;       //Jon 28-5-2001
  ItemOpenPrice: Boolean;           //Jon 19-7-2001
  ItemOpenPriceMin: Currency;       //Jon 19-7-2001
  ItemOpenPriceMax: Currency;       //Jon 19-7-2001
  ItemWeighedItem: Boolean;
  ItemPositionPrompt: Integer;      //Jon 08-04-2002
  ItemPriceLevel: Integer;          //Jon 29-09-2003
  ItemLoyaltyPoints1RatioNum: Currency;
  ItemLoyaltyPoints1RatioDen: Currency;
  ItemLoyaltyPoints1Qty: Currency;
  ItemLoyaltyPoints2RatioNum: Currency;
  ItemLoyaltyPoints2RatioDen: Currency;
  ItemLoyaltyPoints2Qty: Currency;
  tmpItemGrpType: TItemGrpType;
  ItemBarCode, ItemGTIN, ItemCST_PIS, ItemCST_COFFINS: string;
  ItemProductOrigin, ItemCFOP, ItemCEST: Integer;

  ItemNCMProductCode, ItemTaxSituation, ItemCSOSN, ItemCSTICMS: string;
  ItemTaxCoreLabel: string;
  ItemGLCode, ItemGLCode2: string;
  ItemHappy: Boolean;             //RPC 5/3/10 used to determine if auto complete sale
  I, iCount: Integer;
  tmpItem: TComboGroupItem;
  OL: TOrderLine;
  lCombo: TCombo;
  lComboLink: TComboGroupLink;

begin
  Result := False;
  StopInactiveTimer;
  if not AddingFreeComboItem then
    ComboFreeItems.Clear;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  Update;
  if (not SaleCategorySelected) then begin
    Exit; //AKM 3.3.1
  end;
  if ((OLList.Count = 0){ and (GlbAccount.AccountID > 0)}) then
  begin
    DiscCalculator.CurrentTime := CurrentTime;
    DiscCalculator.CurrentDay := CurrentDay;
  end;

  //AMANDA 18/10/00 - only prompt mods if not (beverage AND optional)
  try
    if StillAddingAnItem then
      Exit;
    StillAddingAnItem := True;
    //Reset the current time & day as this is best point to define start time of sale
    if ((OLList.Count = 0) and (TLList.Count = 0) and (VoidOLList.Count = 0)) then
    begin
      SetCurrentTime;
      SetSaleCategory(GlbTable.SaleCategoryID);
      if ((AppDetails.EnableFiscalPrinting) and
          (AppDetails.FiscalLiveInvoice) and
          (not (NFManual or NFEMode)) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0))) and
          (GlbTable.SaleCategoryType <= scStandard)) then
      begin
        try
          if (not SFiscalPrinting.OpenedFiscalInvoice) then
          begin
            FCreditCardDiscount := 0;
            if (not SFiscalPrinting.GetFiscalPrinterConfigurations) then
            begin
              //ShowQuickMsg(sPrinterSystemDateMismatch);
              PostMessage(Self.Handle, WM_CLOSE, 0, 0);
              Result := False;
              Exit;
            end;

            ShowQuickMsgOnTop(sOpeningFiscalInvoice);
            if not SFiscalPrinting.OpenFiscalPrinterInvoice(FCPFNumber, GlbAccount.PerorgName, GlbAccount.PerorgAddress) then
            begin
              StartInactiveTimer;
              Exit;
            end;
            SFiscalPrinting.GetMovementDate;
            SWBEncryption.GenerateEncryptedAuxFile;

            SFiscalPrinting.GetCOOForInvoice; // retrive the last COO in SFiscalPrinting.FCOO . This has to be stored in invoice table
            SFiscalPrinting.GetCCFForInvoice;

            if AppDetails.EnableFiscalPrinting then
            begin
              cmdTable.Enabled := ((not AppDetails.DisableTableFeature) and (not SFiscalPrinting.OpenedFiscalInvoice));
              cmdPhoneOrder.Enabled := (not SFiscalPrinting.OpenedFiscalInvoice);
            end;
          end;
        finally
          FreeQuickMsg;
        end;
      end;
    end;

    with (Statement) do begin
      if (Fields[0].IsNull) then
      begin
        StartInactiveTimer;
        Exit;
      end;


      GlbItemModifiers.ForB := FieldByName('forb').AsString;
      //GlbItem.ThePrice := FieldByName('price').AsCurrency; //AMANDA 08.02.01
      if ((PriceLevel >= 1) and (PriceLevel <= 6)) then begin
        PLevel := PriceLevel;        //Local Price Level for item
        ItemPriceLevel := PLevel;   //Jon 29-09-2003
      end
      else begin
        PLevel := GlbTable.PriceLevel;     //Price level as set by sales category
        ItemPriceLevel := 0;        //Jon 29-09-2003

        IGSaleCategoryID := FieldByName('igsalecategoryid').AsInteger;
        if (IGSaleCategoryID > 0) then begin
          SaleCategory := GlobalMenuList.ListSaleCategorys.Find(IGSaleCategoryID);
          if (Assigned(SaleCategory)) then begin
            PLevel := SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay);
          end;
        end;
      end;
      ItemUnitPrice := GetItemPrice(Statement, PLevel, AppDetails.CashSaleAllowSpecialPrices); //AKM 3.3.1
      ItemPriceExcludesTax := FieldAsBool(FieldByName('priceexcludestax'));   //Jon 28-5-2001
      ItemSalesTaxPercent := FieldByName('salestaxpercent').AsFloat;          //Jon 28-5-2001

      ItemCourseID := FieldByName('courseid').AsInteger;
      ItemCourse := FieldByName('course').AsString;
      ItemCourseIsMain := FieldAsBool(FieldByName('courseismain'));   //Jon 07-05-2003
      ItemFORB := FieldByName('forb').AsString;
      ItemItemID := FieldByName('itemid').AsInteger;
      ItemItemGroupID := FieldByName('itemgroupid').AsInteger;
      ItemItemAbbrev := FieldByName('itemabbrev').AsString;
      ItemCurrentPrinter := FieldAsInt(FieldByName('currentprinter'));
      ItemDefaultPrinter := FieldAsInt(FieldByName('defaultprinter'));
      ItemCurrentRepeatPrinter := FieldAsInt(FieldByName('currentrepeatprinter'));
      ItemDefaultRepeatPrinter := FieldAsInt(FieldByName('defaultrepeatprinter'));

      ItemOpenPrice := FieldAsBool(FieldByName('openprice'));   //Jon 23-7-2001
      ItemOpenPriceMin := FieldAsCurrency(FieldByName('openpricemin'));
      ItemOpenPriceMax := FieldAsCurrency(FieldByName('openpricemax'));
      ItemWeighedItem := FieldAsBool(FieldByName('weigheditem'));
      tmpItemGrpType := TItemGrpType(FieldAsInt(FieldByName('ItemGrpType')));
      ItemTaxSituation := FieldByName('TaxSituation').AsString;
      DiscCalculator.AddingFreeComboItems := True;
      ItemCSOSN := FieldByName('CSOSN').AsString;
      ItemCSTICMS := FieldByName('CST_ICMS').AsString;
      ItemNCMProductCode := FieldByName('NCMPRODUCTCODE').AsString;
      ItemBarCode := FieldByName('Barcode').AsString;
      ItemGTIN := FieldByName('gtin').AsString;
      ItemCEST := FieldAsInt(FieldByName('cest'));
      ItemTaxCoreLabel := FieldAsString(FieldByName('taxcorelabel'));
      ItemProductOrigin := 0;
      if Length(ItemCSTICMS) >= 3 then
      begin
        ItemProductOrigin := StrToIntDef(Copy(ItemCSTICMS, 1, 1), 0);
        ItemCSTICMS := Copy(ItemCSTICMS, 2, Length(ItemCSTICMS)-1);
      end;

      ItemCFOP := FieldByName('CFOP').AsInteger;
      ItemCST_PIS := FieldByName('CST_PIS').AsString;
      ItemCST_COFFINS := FieldByName('CST_COFFINS').AsString;

      if (AppDetails.CashSaleShowLoyaltyPoints) then begin
        ItemLoyaltyPoints1RatioNum := FieldAsCurrency(FieldByName('loyaltypoints1rationum'), 0);
        ItemLoyaltyPoints1RatioDen := FieldAsCurrency(FieldByName('loyaltypoints1ratioden'), 1);
        ItemLoyaltyPoints1Qty := FieldAsCurrency(FieldByName('loyaltypoints1qty'), 0);
        ItemLoyaltyPoints2RatioNum := FieldAsCurrency(FieldByName('loyaltypoints2rationum'), 0);
        ItemLoyaltyPoints2RatioDen := FieldAsCurrency(FieldByName('loyaltypoints2ratioden'), 1);
        ItemLoyaltyPoints2Qty := FieldAsCurrency(FieldByName('loyaltypoints2qty'), 0);
      end
      else begin
        ItemLoyaltyPoints1RatioNum := 0;
        ItemLoyaltyPoints1RatioDen := 1;
        ItemLoyaltyPoints1Qty := 0;
        ItemLoyaltyPoints2RatioNum := 0;
        ItemLoyaltyPoints2RatioDen := 1;
        ItemLoyaltyPoints2Qty := 0;
      end;

      ItemGLCode := FieldAsString(FieldByName('glcode'));
      ItemGLCode2 := FieldAsString(FieldByName('glcode2'));
      ItemHappy := FieldAsBool(FieldByName('happy'));                  //RPC 5/3/2010

      if ((ItemOpenPrice) and (GlbTimePeriod.TimePeriodID = 0)) then begin      //Jon 23-7-2001
        //check if have privilege
        if (not VerifyPriv(24)) then
        begin
          ShowMsg(sAdminOpenPrice);
          StartInactiveTimer;
          Exit;
        end;
        //verify pinno
        if ((not GlbPrivs[24].AvoidPINCheck) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
          StartInactiveTimer;
          Exit;
        end;

        if (not GetOpenPrice(ItemUnitPrice, ItemOpenPriceMin, ItemOpenPriceMax)) then begin
          StartInactiveTimer;
          Exit;
        end;

        if (AppDetails.OrderOpenPriceNameChange) then begin
          if (not GetSingleNote(ItemItemAbbrev, 25, sItemName, sItemNameWithColon, tiPieSlice, True)) then begin
            StartInactiveTimer;
            Exit;
          end;
        end;
      end;

      OrderingQty := NextQty;
      if (BarCodePriceCode = '') then begin
        if (FieldByName('enterqty').AsCurrency > 0) then begin
          OrderingQty := FieldByName('enterqty').AsCurrency;
          if (not GetQuantity(OrderingQty, gqQuantity)) then begin
            StartInactiveTimer;
            Exit;
          end;
        end
        else if (FieldAsBool(FieldByName('weigheditem'))) then begin
          if (not GetQuantity(OrderingQty, gqWeight)) then begin
            StartInactiveTimer;
            Exit;
          end;
        end;
      end
      else begin
        if (FieldAsBool(FieldByName('weigheditem'))) then begin
          OrderingQty := StrToIntDef(BarCodePriceCode, 0) / 1000;
        end
        else begin
          OrderingQty := 1;
          ItemOpenPrice := True;
          ItemUnitPrice := (StrToIntDef(BarCodePriceCode, 0)) / 100;
        end;
        if (OrderingQty = 0) then begin
          StartInactiveTimer;
          Exit;
        end;
      end;
    end;

    if ((AppDetails.TaxExclusivePrices) and (not ItemPriceExcludesTax)) then begin    //Jon 26-6-2001
      ItemUnitPrice := ((ItemUnitPrice * 100) / (100 + ItemSalesTaxPercent)); //Jon 26-6-2001
    end
    else if ((not AppDetails.TaxExclusivePrices) and (ItemPriceExcludesTax)) then begin   //Jon 26-6-2001
      ItemUnitPrice := (RoundCurr(ItemUnitPrice * (100 + ItemSalesTaxPercent)) / 100); //Jon 26-6-2001
    end;

    ItemPrice := ItemUnitPrice;                                //AKM 3.3.1
    OrderingQty := CRoundTo(OrderingQty, 3);
    if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    begin
      OrderingQty := TruncateTo(OrderingQty, 3);
    end;

    AvailQtyTested := False;   //Jon 14-05-2003
    if (AppDetails.OrderDecrementItemQty > 0) then begin
      ItemsQty := 0;
      for J := (OLList.Count - 1) downto 0 do begin
        aOL := OLList.Items[J];
        if (aOL.ItemID = ItemItemID) then begin
          if (aOL.AvailQtyOK) then begin
            AvailQtyTested := True;
          end;
          ItemsQty := ItemsQty + aOL.Qty;
        end;
      end;
      if ((AppDetails.OrderDecrementItemQty in [2, 3]) or (not AvailQtyTested)) then begin
        if (not CheckItemQty(OrderingQty + ItemsQty, ItemItemID, AvailQtyTested)) then begin
          StartInactiveTimer;
          Exit;
        end;
      end;
    end;

    GlbItemModifiers.AddingItem := True;
    GlbItem.ItemID := ItemItemID;
    GlbItem.ItemGroupID := ItemItemGroupID;
    GlbItem.Item := ItemItemAbbrev;

    if (not AddingComboItems) then
    begin
      GlbItemModifiers.AddingItem := True;
      LoadMods;
    end;

    if ((((not AppDetails.CashSaleBevModsCompulsory) and (ItemFORB = 'B')) or (GlbTimePeriod.TimePeriodID <> 0)) or
        (AddingComboItems or formItemModifiers.GetMods)) then begin
      // JEH 18/08/2001 1st clear change
      if OLList.Count = 0 then txtCChange.Caption := '0.00';
      //prepare values for adding OL

      //add to OLList
      aOL := TOrderLine.Create;
      with aOL do begin
        FixThisComboPart := AddingComboItems;
        AllowEdit := True;
        Qty := OrderingQty;     //Jon 12-03-2004
        OriginalQty := OrderingQty;
        FORB := ItemFORB;
        ItemID := ItemItemID;
        ItemGroupID := ItemItemGroupID;
        ItemString := ItemItemAbbrev;
        NCMProductCode := ItemNCMProductCode;
        if (ItemCurrentPrinter > 0) then begin
          PrinterID := ItemCurrentPrinter;
          RepeatPrinterID := ItemCurrentRepeatPrinter;
        end
        else begin
          PrinterID := ItemDefaultPrinter;
          RepeatPrinterID := ItemDefaultRepeatPrinter;
        end;

        OriginalUnitPrice := ItemUnitPrice;
        UnitPrice := ItemUnitPrice;
        ToPay := GetRoundedUpDown(UnitPrice * Qty, AppDetails.DecimalPlaces);                     //Jon 11-02-2005
        OLPriceLevel := ItemPriceLevel;
        PriceExcludesTax := ItemPriceExcludesTax; //Jon 29-5-2001
        SalesTaxPercent := ItemSalesTaxPercent;   //Jon 29-5-2001
        OpenPrice := ItemOpenPrice;               //Jon 19-7-2001
        OpenPriceMin := ItemOpenPriceMin;         //Jon 19-7-2001
        OpenPriceMax := ItemOpenPriceMax;         //Jon 19-7-2001
        WeighedItem := ItemWeighedItem;

        ItemPrices[1] := Statement.FieldByName('itemprice').AsCurrency;    //Jon 07-10-2003
        ItemPrices[2] := Statement.FieldByName('itemprice2').AsCurrency;   //Jon 07-10-2003
        ItemPrices[3] := Statement.FieldByName('itemprice3').AsCurrency;   //Jon 07-10-2003
        ItemPrices[4] := Statement.FieldByName('itemprice4').AsCurrency;   //Jon 07-10-2003
        ItemPrices[5] := Statement.FieldByName('itemprice5').AsCurrency;   //Jon 07-10-2003
        ItemPrices[6] := Statement.FieldByName('itemprice6').AsCurrency;   //Jon 07-10-2003

        ItemGrpType := tmpItemGrpType;
        TaxSituation := ItemTaxSituation;

        CSOSN := ItemCSOSN;
        CSTICMS := ItemCSTICMS;
        CFOP := ItemCFOP;
        BarCode := ItemBarCode;
        GTIN := ItemGTIN;
        CST_PIS := ItemCST_PIS;
        CST_COFFINS := ItemCST_COFFINS;
        CEST := ItemCEST;
        ProductOrigin := ItemProductOrigin;
        TaxCoreLabel := ItemTaxCoreLabel;

        if ((CurrentCourseID > 0) or (not AppDetails.CashSaleUseCourses)) then begin
          CourseID := CurrentCourseID;         //Jon 28-02-2002
          CourseName := CurrentCourseName;
          CourseIsMain := CurrentCourseIsMain;
        end
        else begin
          CourseID := ItemCourseID;
          CourseName := ItemCourse;
          CourseIsMain := ItemCourseIsMain;
        end;
        AvailQtyOK := AvailQtyTested;    //Jon 14-05-2003
        ApplyDiscountType := adtAllItems;

        if AddingComboItems then
        begin
          lCombo := FAllCombos.GetCombo(aComboID);
          if Assigned(lCombo) then
          begin
            lComboLink := lCombo.GetComboGroupLink(aComboGroupID);
            if Assigned(lComboLink) then
            begin
              ApplyDiscountType := lComboLink.ApplyDiscountType;
              FreeComboItem := (lComboLink.ComboGroup.ComboGroupType = cgtFree);
            end;
          end;
          ComboID := aComboID;
          ComboGroupID := aComboGroupID;
          ComboQty := Qty;
          FreeComboQty := 0;
          if FreeComboItem then
            FreeComboQty := Qty;
          ComboTempQty := Qty;
          ComboIndex := aComboIndex;
          IsFiscalDiscountDone := True;
        end;

        if ((AppDetails.EnableFiscalPrinting) and (GlbAccount.AccountID <= 0)) then
        begin
          aOL.ILAmount := aOL.ToPay;
          aOL.ILDiscount := 0;
          aOL.DiscountedAmount := aOL.ToPay;
          //aOL.IsFiscalDiscountDone := False;
        end;

        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem)) then begin
          LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, ItemID);
          AllocateItemLoyalty(aOL);
        end;

        LoyaltyPoints1RatioNum := ItemLoyaltyPoints1RatioNum;
        LoyaltyPoints1RatioDen := ItemLoyaltyPoints1RatioDen;
        LoyaltyPoints1Qty := ItemLoyaltyPoints1Qty;
        LoyaltyPoints2RatioNum := ItemLoyaltyPoints2RatioNum;
        LoyaltyPoints2RatioDen := ItemLoyaltyPoints2RatioDen;
        LoyaltyPoints2Qty := ItemLoyaltyPoints2Qty;
        GLCode := ItemGLCode;
        GLCode2 := ItemGLCode2;
        JustAdded := True;
        NetToPay := 0;
      end;

      if ((AppDetails.EnableFiscalPrinting) and
          (not (NFManual or NFEMode)) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0)))) then
      begin
        if ((aOL.ItemID <= 0) or (aOL.Qty <= 0) or (aOL.UnitPrice <= 0)) then
        begin
          ShowQuickMsg(sInvalidFiscalItem);
          if Assigned(aOL) then
          begin
            aOL.Free;
          end;
          StartInactiveTimer;
          Exit;
        end;
      end;

      OLList.Add(aOL);
      Result := True;
      //add to grid
      with grdOLs do begin
        if (OLList.Count > 1) then begin
          RowCount := RowCount + 1;
        end;

        //select OL row
        Row := RowCount - 1;
        UpdateOLGridRow(Row, aOL);

        RowToSelect := Row;
        FAddedFiscalRow := RowToSelect;
      end;

      //add any mods from the item modifier form
      AddMods;
      AddFiscalItemNote;
      //BuildItemDiscounts;

      if (not AddingFreeComboItem)  then
        CalculateToPay;

      //select the OL added, just in case added mods and on last mod row
      I := FindOLJustAddedIndex;
      if (I >= 0) then
      begin
        grdOLs.Row := I;
        aOL := OLList.Items[I];
        aOL.JustAdded := False;
      end;
      Application.ProcessMessages;

      UpdateOLGridRow(grdOLs.Row, aOL);

      if ((AppDetails.EnableFiscalPrinting) and (AppDetails.FiscalLiveInvoice) and 
          (not (NFManual or NFEMode)) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0)))) then
      begin
        if aOL.DiscountedAmount <= 0 then
        begin
          ShowQuickMsg(sInvalidFiscalItem);
          if Assigned(aOL) then
          begin
            aOL.Free;
          end;
          OLList.Delete(OLList.Count-1);   //delete the last item added if the final amount after discount is <= 0
          UpdateOLGridRow(grdOLs.Row, nil);
          grdOLs.RowCount := grdOLs.RowCount - 1; // delete the last row added;
          StartInactiveTimer;
          Exit;
        end;

        if ((aOL.ItemID > 0) and (aOL.Qty > 0) and (aOL.UnitPrice > 0)) then
        begin
          if (not AddingComboItems) then
            SendOrderLineToFiscalPrinter(aOL, grdOLs.Row);
        end;
      end;

      //Pole Display Jon 29-11-2001
      if (AppDetails.PDEnabled) then begin
        DMComponents.PDOutputItemLine(aOL.ItemString, RoundCurrency(aOL.ToPay, 2), 0, False);
        DMComponents.PDOutputItemLine('SUB TOTAL', RoundCurrency(FStillDue, 2), 1, False);
      end;
      TemporarilyDeactiveButtons;
    end;

    cmdCPF.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdCNPJ.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    ShowCPFNUmberInActivate := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdSelectAccount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdAccountNoLookup.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdDeselectAccount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdDiscountPercent.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);
    cmdDiscountAmount.Enabled := ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice) or NFManual);

    DisposeMods;
    GlbItemModifiers.AddingItem := False;       //Jon 24-12-2003
    PriceLevelChange(0, 1);
    CourseChange(0, '', 1, False);
    NextItemButtonStateChange(1);   //Jon 22-04-2002
    SetNextQty(1);

    if (ShowingQty) and (AppDetails.AutoCloseNextQty) then begin
      ShowQty(False);
    end;
    if ((ItemHappy) and (appdetails.CashSaleItemAutoTender)) then begin
      ItemAutoTenderActive := True;
      ButtonsDeactivated := False;
      cmdQuickCashClick(self);
    end;
  finally
    DiscCalculator.AddingFreeComboItems := False;
    CancelPOSInvoiceIfFiscalInvCancelled;
    StartInactiveTimer;
    StillAddingAnItem := False;
    BroadcastCDAMessage;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.FindOLJustAddedIndex: Integer;
var
  I: Integer;

begin
  for I := (OLList.Count - 1) downto 0 do
  begin
    if (TOrderLine(OLList.Items[I]).JustAdded) then
    begin
      Result := I;
      Exit;
    end;
  end;
  Result := -1;
end;
{******************************************************************************}
procedure TformQuickSaleUser.OrderModifier(ModifierID: Integer; ModGroupID: Integer);
var
  RowToSelect: Integer;
  J: Integer;
  Modifier: TModifier;
  AddTheModifier: Boolean;
  aMod: TMod;
  aOL: TOrderLine;

begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001

  Modifier := GlobalMenuList.ListModifiers.Find(ModifierID);

  if (not Assigned(Modifier)) then begin
    Exit;
  end;

  //if there are any ols
  if (OLList.Count > 0) then begin
    //if an OL selected
    SelectModifiersItemInGrid;
    aOL := OLList.Items[grdOLs.Row];

    if ((aOL.ItemID > 0) and (not aOL.AlreadyOrdered)) then begin
      Update;

      RowToSelect := grdOLs.Row;

      GlbItemModifiers.AddingItem := False;

      //AMANDA 17/10/00
      LoadMods;
      if (Modifier.ForB <> aOL.ForB) then begin
        ShowQuickMsg(Format(sCannotHaveModifierWithItem, [ForBToString(Modifier.ForB), ForBToString(aOL.ForB)]));
      end
      else begin
        AddTheModifier := True;
        for J := 0 to (Mods.Count - 1) do
        begin
          aMod := TMod(Mods.Items[J]);
          if (aMod.ModID = Modifier.ModifierID) and (aMod.ModGroupID = ModGroupID) then
          begin
            if (aMod.ModQty <= 1) then
            begin
              ShowQuickMsg(sModifierAlreadyExists);
            end
            else if (aMod.ModQtyOrdered >= aMod.ModQty) then
            begin
              ShowQuickMsg(sModifierLimitReached, False);
            end
            else
            begin
              aMod.ModQtyOrdered := aMod.ModQtyOrdered + 1;
              ModifierFunctionChange(1);
              ClearMods;
              AddMods;
            end;
            AddTheModifier := False;
            Break;
          end;
        end;
        if (AddTheModifier) then begin
          aMod := TMod.Create;
          aMod.ModID := Modifier.ModifierID;
          aMod.ModGroupID := ModGroupID;
          aMod.Modifier := Modifier.Modifier;
          aMod.ModGroupOrder := 0;
          case FModifierFunction of
            1: aMod.OriginalModPrice := Modifier.ModPrice;
            2: aMod.OriginalModPrice := -Modifier.ModPrice;
            3: aMod.OriginalModPrice := 0;        //Jon 17-06-2003
          else
            aMod.OriginalModPrice := Modifier.ModPrice;
          end;
          aMod.ModPrice := aMod.OriginalModPrice;      //Jon 10-9-2001
          aMod.ModFunction := FModifierFunction;   //Jon 9-8-2001
          aMod.PrinterID := Modifier.PrinterID;                //Jon 03-06-2003
          aMod.RepeatPrinterID := Modifier.RepeatPrinterID;    //Jon 03-06-2003
          aMod.ModQty := Modifier.ModQty;
          aMod.ModQtyOption := Modifier.ModQtyOption;
          aMod.ModQtyOrdered := 1;
          Mods.Add(aMod);
          ModifierFunctionChange(1);

          ClearMods;
          AddMods;
        end;
      end;
      DisposeMods;
      //select the OL added
      grdOLs.Row := RowToSelect;
      aOL := OLList.Items[grdOLs.Row];
      CalculateToPay;
      if (AppDetails.PDEnabled) then begin
        DMComponents.PDOutputItemLine(aOL.ItemString, RoundCurrency(aOL.ToPay, 2), 0, False);
        DMComponents.PDOutputItemLine('SUB TOTAL', RoundCurrency(FStillDue, 2), 1, False);
      end;
      BroadcastCDAMessage;
    end
    else if (aOL.AlreadyOrdered) then begin
      ShowQuickMsg(sItemAlreadyOrdered);
    end
    else begin
      OrderModifier(ModifierID, ModGroupID);
      Exit;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.OrderCombo(aComboID: Integer);
var
  lCombo: TCombo;
  SelectCombo: TSelectCombo;
  K, I, J, lComboIndex: Integer;
  tmpItem, tmpNewItem: TComboGroupItem;
  OL: TOrderLine;
  NextComboQty: Integer;
  NewComboMod, ComboMod: TComboModifier;
  tmpList: TObjectList;
  aMod: TMod;
begin
  StopInactiveTimer;
  SelectCombo := TSelectCombo.Create;
  tmpList := TObjectList.Create;
  NextComboQty := NextQty;
  lComboIndex := 0;
  try
    lCombo := FAllCombos.GetCombo(aComboID);
    if (Assigned(lCombo) and lCombo.IsTimeInComboPeriod(CurrentTime, CurrentDay)) then
    begin
      SelectCombo.SelectComboItems(lCombo);

      if SelectCombo.SelectedComboItems.Count > 0 then
      begin
        AddingComboItems := True;
        DisposeMods;
        Mods := TList.Create;
        tmpItem := nil;
        for I := 0 to SelectCombo.SelectedComboItems.Count - 1 do
        begin
          tmpItem := TComboGroupItem(SelectCombo.SelectedComboItems.Items[I]);
          tmpNewItem := TComboGroupItem.Create;
          tmpNewItem.Copy(tmpItem);
          tmpList.Add(tmpNewItem);
        end;
        lComboIndex := (GetMaxComboIndex(aComboID, GlbTable.GroupID, OLList));

        for I := 0 to tmpList.Count - 1 do
        begin
          DisposeMods;
          if ((Assigned(tmpItem)) and (Assigned(tmpItem.Mods)) and (tmpItem.Mods.Count > 0)) then
          begin
            Mods := TList.Create;
            for J := 0 to tmpItem.Mods.Count  - 1 do
            begin
              aMod := TMod.Create;
              ComboMod := TComboModifier(tmpItem.Mods[J]);
              aMod.ModID := ComboMod.ModID;
              aMod.ModGroupID := ComboMod.ModGroupID;
              aMod.Modifier := ComboMod.Modifier;
              aMod.ModGroupOrder := ComboMod.ModGroupOrder;
              aMod.ModPrice := ComboMod.ModPrice;
              aMod.OriginalModPrice := ComboMod.OriginalModPrice;
              aMod.PrinterID := ComboMod.PrinterID;
              aMod.RepeatPrinterID := ComboMod.RepeatPrinterID;
              aMod.ModFunction := ComboMod.ModFunction;
              aMod.ModQty := ComboMod.ModQty;
              aMod.ModQtyOption := ComboMod.ModQtyOption;
              aMod.ModQtyOrdered := ComboMod.ModQtyOrdered;
              aMod.ModQtyIncluded := ComboMod.ModQtyIncluded;
              Mods.Add(aMod);
            end;
          end;

          ForcedNextQty := (tmpItem.QtySelected > 1);
          NextQty := Ceil(tmpItem.QtySelected);
          if NextQty <= 0 then
            NextQty := 1;

          if (not OrderItem(tmpItem.ItemID, True, aComboID, tmpItem.ComboGroupID, lComboIndex)) then
          begin
            ClearCombo(OLList, lCombo.ComboID, lComboIndex);
            Exit;
          end;
          AddFreeComboItemsToOrderList;
          tmpItem := TComboGroupItem(tmpList.Items[I]);
          if (Assigned(Mods) and (Mods.Count > 0)) then   // if mods are selected for combo items save them along with each order line, we need it for editing the combo
          begin
            OL := nil;
            for K := OLList.Count-1 downto 0 do
            begin
              OL := OLList.Items[K];
              if OL.ItemID > 0 then
                Break;
            end;

            if Assigned(OL) then
            begin
              if Assigned(OL.ComboMods) then
                OL.ComboMods.Clear;

              for J := 0 to Mods.Count  - 1 do
              begin
                aMod := Mods.Items[J];
                NewComboMod := TComboModifier.Create;
                NewComboMod.ModID := aMod.ModID;//ComboMod.ModID;
                NewComboMod.ModGroupID := aMod.ModGroupID; //ComboMod.ModGroupID;
                NewComboMod.Modifier := aMod.Modifier;//ComboMod.Modifier;
                NewComboMod.ModGroupOrder := aMod.ModGroupOrder;//ComboMod.ModGroupOrder;
                NewComboMod.ModPrice := aMod.ModPrice;//ComboMod.ModPrice;
                NewComboMod.OriginalModPrice := aMod.OriginalModPrice;//ComboMod.OriginalModPrice;
                NewComboMod.PrinterID := aMod.PrinterID;//ComboMod.PrinterID;
                NewComboMod.RepeatPrinterID := aMod.RepeatPrinterID;//ComboMod.RepeatPrinterID;
                NewComboMod.ModFunction := aMod.ModFunction;
                NewComboMod.ModQty := aMod.ModQty;
                NewComboMod.ModQtyOption := aMod.ModQtyOption;
                NewComboMod.ModQtyOrdered := aMod.ModQtyOrdered;
                NewComboMod.ModQtyIncluded := aMod.ModQtyIncluded;
                OL.ComboMods.Add(NewComboMod);
              end;
            end;
          end;
        end;

        CalculateToPay;

        OL := nil;
        for I := OLList.Count-1 downto 0 do
        begin
          OL := OLList.Items[I];
          if OL.ItemID > 0 then
            Break;
        end;

        if (NextComboQty > 1) and Assigned(OL) then
        for I := 1+1 to NextComboQty do
          AddSameCombo(aComboID, OL.ComboIndex);
      end;
    end
    else
    begin
      ShowQuickMsg(sCheckComboIsValid);
    end;
  finally
    if (AppDetails.EnableFiscalPrinting and (lComboIndex > 0)) then
    begin
      for I := 0 to OLList.Count-1 do
      begin
        OL := OLList.Items[I];
        if ((OL.ItemID > 0) and (OL.ComboID = aComboID) and (OL.ComboIndex = lComboIndex)) then
        begin
          SendOrderLineToFiscalPrinter(OL, I);
        end;
      end;
    end;
    {if AppDetails.EnableFiscalPrinting then
      CheckForFiscalDiscountChanges;}

    grdOLs.Repaint;
    DisposeMods;
    AddingComboItems := False;
    ForcedNextQty := False;
    tmpList.Clear;
    tmpList.Free;
    SelectCombo.Free;
    BroadcastCDAMessage;
    StartInactiveTimer;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.OrderCourse(CourseID: Integer; CourseType: Integer);
var
  Course: TCourse;
  aOL: TOrderLine;
begin
  if (CourseType = 4) then begin
    if ((OLList.Count = 0) or (courseid > 2)) then begin
      Exit;
    end;
    if (CourseID = 1) then begin
      aOL := OLList.Items[grdOLs.Row];

      if (aOL.ItemID > 0) then begin
        with aOL do begin
          CourseID := 0;
          CourseName := '';
          CourseIsMain := False;
        end;
        UpdateOLGridRow(grdOLs.Row, aOL);
      end;
    end
    else begin
      aOL := OLList.Items[grdOLs.Row];

      if ((aOL.ItemID > 0) and (SelectCourse(True, aOL.CourseID))) then begin
        with aOL do begin
          CourseID := GlbCourse.CourseID;
          CourseName := GlbCourse.Course;
          CourseIsMain := GlbCourse.IsMain;   //Jon 07-05-2003
        end;
        UpdateOLGridRow(grdOLs.Row, aOL);
      end;
    end;
  end
  else begin
    Course := GlobalMenuList.ListCourses.Find(CourseID);
    if (not Assigned(Course)) then begin
      Exit;
    end;

    if (CurrentCourseID = Course.CourseID) then begin
      CourseChange(0, '', 2, False);
    end
    else begin
      CourseChange(Course.CourseID, Course.Course, CourseType, Course.IsMain);
    end;
  end;
end;

{******************************************************************************}
procedure TformQuickSaleUser.cmdVoidDesClick(Sender: TObject);
var
  aOL: TOrderLine;

begin
  RemoveOrderLine := True;
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (OLList.Count <= 0) then
  begin
    StartInactiveTimer;
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then
  begin
    StartInactiveTimer;
    Exit;
  end;

  DeletingComboItems := False;

  aOL := OLList.Items[grdOLs.Row];
  ComboIDToDelete := 0;
  if ((aOL.ItemID > 0) and (aOL.FixThisComboPart) and (not DeletingComboItems)) then
  begin
    ComboIDToDelete := aOL.ComboID;
    FComboIndexToDelete := aOL.ComboIndex;
  end;

  if ((aOL.ItemID > 0) and (ComboIDToDelete > 0) and (FComboIndexToDelete > 0) and (not DeletingComboItems)) then
  begin
    DeleteCombos(grdOLs.Row);
    DeletingComboItems := False;
    RemoveOrderLine := False;
    BroadcastCDAMessage;
    Exit;
  end;

  VoidLine;
  if (OLList.Count = 0) then
  begin
    if ((AppDetails.CashSaleCancelOnClear = 1) or ((AppDetails.CashSaleCancelOnClear = 2) and (ShowConf(sCancelSale) = mrYes))) then
    begin
      CancelCashSale;

      if Assigned(DiscCalculator) then
      begin
        DiscCalculator.ClearItemLevelDiscounts;
        DiscCalculator.ClearPrevTabItems;
      end;
      ClearItemLevelDiscounts(OLList);
      if Assigned(SubsidyCalc) then
      begin
        SubsidyCalc.ClearSubsidyDetails;
        ClearItemLevelSubsidy;
      end;

      if AppDetails.EnableFiscalPrinting then
      begin
        cmdTable.Enabled := ((not AppDetails.DisableTableFeature) and (not SFiscalPrinting.OpenedFiscalInvoice));
        cmdPhoneOrder.Enabled := (not SFiscalPrinting.OpenedFiscalInvoice);
      end;
      if (AppDetails.EnableFiscalPrinting and CheckFiscalPrinterFunctions and ShowCPFNUmberInActivate and AppDetails.FiscalIsCFPRequired) then
      begin
        FCPFNumber := GetCPFNumber;
      end;
    end
    else
      CalculateToPay;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
  RemoveOrderLine := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdUpClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdItems.Handle, WM_VSCROLL, SB_PAGEUP, 0);
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDownClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdItems.Handle, WM_VSCROLL, SB_PAGEDOWN, 0);
  if grdItems.DataSource.DataSet.Fields[0].IsNull then begin
    grdItems.DataSource.DataSet.Last;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ItemGroupClick(Sender: TObject);
begin
  setgrdItemsToItemGroup(TfcCustomImageBtn(Sender).Tag);
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetGrdItemsToItemGroup(ItemGroupID: Integer);
var
  I: Integer;
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  {NEW WAY - see notes in same procedure in UNewOrder}
  if ((AppDetails.DisplayPriceWhenOrdering) or (AppDetails.UseInactiveItems > 0)) then
    FilterItemListByItemGroupID(TIB_Query(grdItems.DataSource.Dataset), ItemGroupID)
  else
  begin
    if (ItemView = ivBev) then
    begin
      with dm.qrOBItems do
      begin
        KeyFieldByName('itemgroupid').AsInteger := ItemGroupID;
        for I := 1 to 10 do
        begin
          KeyFieldByName('itemorder').AsInteger := I;
          if LookupKeyForFields then
          begin
            grdItems.TopRowNum := RowNum;    //Jon 08-07-2003
            Break;
          end;
        end;
      end;
    end
    else if (ItemView = ivFood) then
    begin
      with dm.qrOFItems do
      begin
        KeyFieldByName('itemgroupid').AsInteger := ItemGroupID;
        for I := 1 to 10 do
        begin
          KeyFieldByName('itemorder').AsInteger := I;
          if LookupKeyForFields then
          begin
            grdItems.TopRowNum := RowNum;    //Jon 08-07-2003
            Break;
          end;
        end;
      end;
    end;
  end;

  //if need to then shrink scroll boxes
  if (cmdAllGroups.Down) then
  begin
    cmdAllGroups.Down := False;
    cmdAllGroupsClick(nil);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ShowLastReceipt;
var
  InvoiceID: Integer;
  InvoiceType: string;
begin
  if (AppDetails.CashSaleStaffPromptTimeout <> 0) then
  begin
    if (SelectLastCashSale(InvoiceID, InvoiceType, True)) then
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
        ShowQuickMsg(sCashSalePrinted);
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
  else
  begin
    GlbLogin.ShowLastReceipt := True;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.BuildItemGroups;
const
  BUTTONWIDTH  = 48;
  BUTTONHEIGHT = 48;
var
  FORB: string;
  cmdItemGroup: TfcCustomImageBtn;
  BPR: Integer;       //Buttons Per Row
  F, B: Integer;
  FLine, BLine: Integer;
begin
  F := 0;
  B := 0;
  FLine := 0;
  BLine := 0;

//this may cause problems - in to avoid might not have been initialised hint
  cmdItemGroup := nil;

  BPR := ((Screen.Width - pnlFunctions.Width - 20) div BUTTONWIDTH);
  //set scroll boxs width
  pnlFood.Width := (BUTTONWIDTH * BPR); //44 * 7;
  pnlBev.Width := (BUTTONWIDTH * BPR); //44 * 7;
  scrF.Width := (BUTTONWIDTH * BPR); //44 * 7;
  scrB.Width := (BUTTONWIDTH * BPR); //44 * 7;

  //make sure panels are alNone, top = 0, left = 0, so that panel height can >
  pnlFood.Align := alNone;
  pnlFood.Top := 0;
  pnlFood.Left := 0;
  pnlBev.Align := alNone;
  pnlBev.Top := 0;
  pnlBev.Left := 0;

  with dm.qrOItemGroups do begin
    First;
    while (not EOF) do begin
      FORB := FieldByName('forb').AsString;

      if (FORB = 'F') then begin
        Skin.CreateNewButton(cmdItemGroup, TComponent(pnlFood), TWinControl(pnlFood), bstGen48);
      end
      else if (FORB = 'B') then begin
        Skin.CreateNewButton(cmdItemGroup, TComponent(pnlBev), TWinControl(pnlBev), bstGen48);
      end;

      cmdItemGroup.Font.Name := 'Arial';
      cmdItemGroup.Font.Style := [fsBold];
      cmdItemGroup.Font.Size := 8;

      cmdItemGroup.Caption := FieldByName('itemgroupabbrev').AsString;
      cmdItemGroup.Tag := FieldByName('itemgroupid').AsInteger;
      cmdItemGroup.OnClick := ItemGroupClick;
      cmdItemGroup.Width := BUTTONWIDTH;
      cmdItemGroup.Height := BUTTONHEIGHT;

      if (FORB = 'F') then begin
        cmdItemGroup.Top := (FLine * BUTTONHEIGHT);
        cmdItemGroup.Left := (F mod BPR) * BUTTONWIDTH;
        pnlFood.Height := ((FLine + 1) * BUTTONHEIGHT);

        Inc(F);
        if ((F mod BPR) = 0) then begin
          Inc(FLine);
        end;
      end
      else if (FORB = 'B') then begin
        cmdItemGroup.Top := (BLine * BUTTONHEIGHT);
        cmdItemGroup.Left := (B mod BPR) * BUTTONWIDTH;
        pnlBev.Height := ((BLine + 1) * BUTTONHEIGHT);

        Inc(B);
        if ((B mod BPR) = 0) then begin
          Inc(BLine);
        end;
      end;

      Next;
    end;
  end;

  //set the scroll height to two rows
  if (FLine = 0) then begin
    scrF.Height := BUTTONHEIGHT;
  end
  else begin
    scrF.Height := (2 * BUTTONHEIGHT);
  end;
  if (BLine = 0) then begin
    scrB.Height := BUTTONHEIGHT;
  end
  else begin
    scrB.Height := (2 * BUTTONHEIGHT);
  end;
{
{  //WHEN HAVE TIME - CHECK THAT THIS WORKS AND THEN???
  //with all controls in scroll delete
  if FORB = 'F' then begin
    for I := 0 to pnlFood.ControlCount-1 do
      pnlFood.Controls[2].Destroy;
  end else if FORB = 'B' then begin
    for I := 0 to pnlBev.ControlCount-1 do
      pnlBev.Controls[2].Destroy;
  end;
        if FieldByName('itemgroupcolor').IsNull then
        cmdItemGroup.Color := clBlack
      else
        cmdItemGroup.Color := StrToInt(FieldByName('itemgroupcolor').AsString);

      if FieldByName('itemgroupfontcolor').IsNull then
        cmdItemGroup.Font.Color := clWhite
      else
        cmdItemGroup.Font.Color := StrToInt(FieldByName('itemgroupfontcolor').AsString);

      cmdItemGroup.BevelWidth := 2;
}
end;
{******************************************************************************}
procedure TformQuickSaleUser.BuildItems;
begin
  OpenArch := TOpenArch.Create(pnlButtons);
  OpenArch.Parent := pnlButtons;
  OpenArch.Visible := False;
  OpenArch.Align := alClient;
  if (not AppDetails.CashSaleUseCourses) then
    OpenArch.QSButtonActions := AllQSButtonActions - [qsbaCourse];
  OpenArch.SetQSPages;
  OpenArch.OnOpenArchButtonClick := OpenArchButtonClick;
  OpenArch.OnOpenArchButtonHeld := OpenArchButtonHeld;
  OpenArch.HoldTimeout := AppDetails.RecipeTimeout;
  OpenArch.CurrentQSPageID := AppDetails.CashSaleDefaultPageID;
  OpenArch.Visible := True;
end;
{******************************************************************************}
function TformQuickSaleUser.SetSaleCategory(SCID: Integer): Boolean;
var
  NewPriceLevel: Integer;
  SaleCategory: TSaleCategory;
  SC: string;
  TheSelectedItemID, TheTopRow: Integer;

begin
  Result := False;
  SC := '';
  //AKM 3.3.1 - if order being placed, check that changing sale category won't change price level
  if (OLList.Count > 0) then begin
    //determine what new price level will be
    NewPriceLevel := -1;   //just in case can't find it, ensure get error msg (as will be diff to current)
    SaleCategory := GlobalMenuList.ListSaleCategorys.Find(SCID);
    if (Assigned(SaleCategory)) then begin
      NewPriceLevel := SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay);
      SC := SaleCategory.SaleCategory;
    end;

    //if not sys has selected SC (ie -1) and if new price level will be diff to current, give error
    if ((NewPriceLevel <> -1) and (NewPriceLevel <> GlbTable.PriceLevel)) then begin
      if ((AppDetails.CashSaleAllowPriceLevelChange) and
          (ShowConf(sConfirmChangePriceLvl) = mrYes)) then begin
        ChangeCurrentOrdersPriceLevel(NewPriceLevel);
      end
      else begin
        if (not AppDetails.CashSaleAllowPriceLevelChange) then begin
          ShowMsg(sNoChangePriceLvlDuringOrd);
        end;
        RevertedSaleCategory := True; //AKM 4.3.1
        SelectSaleCategory(GlbTable.SaleCategory); //revert to current sale category, not one user has just clicked
        Exit;
      end;
    end;
  end;

  if ((GlbTable.SaleCategoryID <> SCID) or (AppDetails.DisplayPriceWhenOrdering) or
      (AppDetails.UseInactiveItems > 0)) then begin
    GlbTable.SaleCategoryID := SCID;
    GlbTable.SaleCategory := SC;
    if (not ShowButtons) then begin
      //RPC here need to save the current position in the list otherwise it will lost
      TheTopRow := grdItems.TopRowNum;

      with dm.qrOFItems do
      begin
        TheSelectedItemID := FieldByName('itemid').AsInteger;
        Close;
        ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        BoolAsParam(ParamByName('useinactiveitems'), AppDetails.UseInactiveItems > 0);
        Open;
        if (TheSelectedItemID > 0) then begin
          grdItems.TopRowNum := TheTopRow;
          Locate('itemid', TheSelectedItemID, []);
        end;
      end;

      with dm.qrOBItems do
      begin
        TheSelectedItemID := FieldByName('itemid').AsInteger;
        Close;
        ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        BoolAsParam(ParamByName('useinactiveitems'), AppDetails.UseInactiveItems > 0);
        Open;
        if (TheSelectedItemID > 0) then begin
          grdItems.TopRowNum := TheTopRow;
          Locate('itemid', TheSelectedItemID, []);
        end;
      end;
    end;
  end;


  //AKM 2.3.1 - find price level in SaleCategoryList
  SaleCategory := GlobalMenuList.ListSaleCategorys.Find(GlbTable.SaleCategoryID);
  if (Assigned(SaleCategory)) then begin
    GlbTable.SaleCategory := SaleCategory.SaleCategory;
    GlbTable.SaleCategoryType := SaleCategory.SaleCategoryType;     //Jon 08-03-2004

    ChangeQuickCashButtonCaption;

    GlbTable.SaleCategorySurchargeItemID := SaleCategory.SurchargeItemID;           //Jon 26-05-2003
    GlbTable.SaleCategorySurchargeItemAbbrev := SaleCategory.SurchargeItemAbbrev;
    GlbTable.SaleCategorySurchargePercent := SaleCategory.SurchargePercent;         //Jon 26-05-2003
    GlbTable.SaleCategorySurchargeFixed := SaleCategory.SurchargeFixed;             //Jon 26-05-2003
    GlbTable.SaleCategorySurchargeSTRate := SaleCategory.SurchargeSalesTaxPercent;  //Jon 26-05-2003
    SetPriceLevel(SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay));
  end;
  OpenArch.ButtonsDown(qsbaSaleCategory, GlbTable.SaleCategoryID);
  CalculateToPay;
  Result := True;
end;
{******************************************************************************}
function TformQuickSaleUser.SelectSaleCategory(SaleCategoryName: string; const SaleCategoryID: Integer = 0): Boolean;
//Returns True when something changes
//Set SaleCategory string to '' and use the SaleCategoryID if wanted
var
  I: Integer;
  SaleCategory: TSaleCategory;
begin
  //initialise
  SetTheSaleCategoryID(0);
  GlbTable.SaleCategory := '';

  SaleCategory := nil;
  if (SaleCategoryID <> 0) then begin
    SaleCategory := GlobalMenuList.ListSaleCategorys.Find(SaleCategoryID);
  end;

  if ((not Assigned(SaleCategory)) and (SaleCategoryName <> '')) then begin
    for I := (GlobalMenuList.ListSaleCategorys.Count - 1) downto 0 do begin
      SaleCategory := GlobalMenuList.ListSaleCategorys.Items[I];
      if (SaleCategory.SaleCategory = SaleCategoryName) then begin
        Break;
      end;
      SaleCategory := nil;
    end;
  end;

  if (Assigned(SaleCategory)) then
  begin
    GlbTable.SaleCategoryID := SaleCategory.SaleCategoryID;   // moved out of below if
    if ((AppDetails.DisplayPriceWhenOrdering) or  {GlbTable.SaleCategoryID has been set to 0 above (in SetTheSaleCategoryID) thus no point to checking <> SaleCategory.SaleCategoryID since will alway be true}
      (AppDetails.UseInactiveItems > 0)) and (not ShowButtons) then      //update these queries to ensure correct price shown, and inactive items are handled
    begin
       with dm.qrOFItems do
       begin
         Close;
         ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
         ParamByName('currenttime').AsInteger := CurrentTime;
         ParamByName('currentday').AsInteger := CurrentDay;
         Open;
       end;
       with dm.qrOBItems do
       begin
         Close;
         ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
         ParamByName('currenttime').AsInteger := CurrentTime;
         ParamByName('currentday').AsInteger := CurrentDay;
         Open;
       end;
    end;
    GlbTable.SaleCategoryType := SaleCategory.SaleCategoryType;     //Jon 08-03-2004
    if ((GlbTable.SaleCategoryType >= scDriveThru) xor (OnTable)) then begin
  //    Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
      Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
      cmdQuickCash.Tag := 0;
    end
    else if (cmdQuickCash.Tag <> 1) then begin
//      Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
      Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
      cmdQuickCash.Tag := 1;
    end;
    GlbTable.SaleCategory := SaleCategory.SaleCategory;
    GlbTable.SaleCategorySurchargeItemID := SaleCategory.SurchargeItemID;
    GlbTable.SaleCategorySurchargeItemAbbrev := SaleCategory.SurchargeItemAbbrev;
    GlbTable.SaleCategorySurchargePercent := SaleCategory.SurchargePercent;
    GlbTable.SaleCategorySurchargeFixed := SaleCategory.SurchargeFixed;
    GlbTable.SaleCategorySurchargeSTRate := SaleCategory.SurchargeSalesTaxPercent;
    SetPriceLevel(SaleCategory.CurrentPriceLevel(CurrentTime, CurrentDay));
  end;

  Result := (Assigned(SaleCategory));

  OpenArch.ButtonsDown(qsbaSaleCategory, GlbTable.SaleCategoryID);
end;
{******************************************************************************}
procedure TformQuickSaleUser.InsertLine;
var
  I: Integer;
begin
  with grdOLs do begin
    //add 1 to rowcount
    RowCount := RowCount + 1;

    for I := (RowCount - 1) downto Row do begin
      Cells[0, I] := Cells[0, I - 1];
      Cells[1, I] := Cells[1, I - 1];
      Cells[2, I] := Cells[2, I - 1];
      Cells[3, I] := Cells[3, I - 1];
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddLine;
//var
//  I: Integer;
begin
  with grdOLs do begin
    //add 1 to rowcount
    RowCount := RowCount + 1;

    Row := RowCount - 1;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetView;
begin
  if (ShowButtons) then
    Exit;

  if (ItemView = ivBev) then begin
    cmdBev.Down := True;
    grdItems.DataSource := dm.dsOBItems;

    scrF.Visible := False;
    scrB.Visible := True;
  end
  else if (ItemView = ivFood) then begin
    cmdFood.Down := True;
    grdItems.DataSource := dm.dsOFItems;

    scrF.Visible := True;
    scrB.Visible := False;
  end;

  if ((AppDetails.DisplayPriceWhenOrdering) or (AppDetails.UseInactiveItems > 0)) then
    FilterItemListByItemGroupID(TIB_Query(grdItems.DataSource.Dataset), TIB_Query(grdItems.DataSource.Dataset).FieldByName('itemgroupid').AsInteger);

end;
{******************************************************************************}
procedure TformQuickSaleUser.SetButtons;
var
  itemgroupid: Integer;
begin
  if ShowButtons then
  begin
    cmdButtons.Visible := False;
    cmdList.Visible := True;
    cmdList.Left := 0;
    pnlList.Visible := False;
    pnlButtons.Visible := True;
    pnlButtons.Align := alClient;
  end
  else
  begin
    //we need to make sure the list is loaded up correctly
    if ((AppDetails.DisplayPriceWhenOrdering) or (AppDetails.UseInactiveItems > 0)) then
    begin
      with dm.qrOFItems do
      begin
        First;
        itemgroupid := FieldByName('itemgroupid').AsInteger;
        Close;
        ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        BoolAsParam(ParamByName('useinactiveitems'), AppDetails.UseInactiveItems > 0);
        FilterItemListByItemGroupID(dm.qrOFItems, itemgroupid);
        Open;
      end;

      with dm.qrOBItems do
      begin
        First;
        itemgroupid := FieldByName('itemgroupid').AsInteger;
        Close;
        ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
        ParamByName('currenttime').AsInteger := CurrentTime;
        ParamByName('currentday').AsInteger := CurrentDay;
        BoolAsParam(ParamByName('useinactiveitems'), AppDetails.UseInactiveItems > 0);
        FilterItemListByItemGroupID(dm.qrOBItems, itemgroupid);
        Open;
      end;
    end;
    cmdButtons.Visible := True;
    cmdButtons.Left := 0;
    cmdList.Visible := False;
    pnlButtons.Visible := False;
    pnlList.Visible := True;
    pnlList.Align := alClient;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTenderAndDestnDesClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
//  pcDetail.ActivePage := tsTender;      //Jon 4-9-2001
  ShowPnlToPay(False);       //Jon 4-9-2001
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdReturnToSaleClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
//  pcDetail.ActivePage := tsToPay;       //Jon 4-9-2001
  ShowPnlToPay(True);        //Jon 4-9-2001
end;
{******************************************************************************}
procedure TformQuickSaleUser.CalculateToPay;
var
  I: Integer;
  SalesTaxPercent: Real;     //Jon 29-5-2001
  SurchargeItem: TSurchargeItem;   //Jon 26-05-2003
  aOL: TOrderLine;
  aDiscPercToUse: Double;
  aToPay: Currency;
  RecalculateAll: Boolean;
begin
  {RPC: 17-12-2010 (working progress)
  Ok discounts and loyalty is a mess....let try clean up the code
  It appears we can let..Caluclate to Pay to call the Discount Procedures, cal to pay is called all the time...
  At the moment will leave 3 discount procedures in place..
  CalDiscountScheme := called to work out discount by item
  CalDiscountPercent := called to work out total discount as a percent
  CalDiscount := Called to display amount and discount from above.....

  There are times we need to reset the discount, this is if someone changes the percent or selects a different account...
  my thought here is to clear all discounts first, then let CaltoPay re do the discount

  Loyalty, in the past loyalty is built into CalDisPercent, this needs to also be built in CalDiscScheme!!
  Again if loyalty is cleared or changed...reset all discounts back to 0 and then let CaltoPay recalulate it

  Finally so if anything ever goes wrong clear discounts and let caltopay re do it!! Remember not to clear the account discounts!}

  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  FToPay := 0;
  FSalesTax := 0;     //Jon 29-5-2001
  FSalesTaxCalculated := False;
  FLoyaltyFreeItemDiscount := 0;
  FLoyaltyPoints1 := 0;
  FLoyaltyPoints2 := 0;
  FAvail := 0.00;
  FFoodTotal := 0;
  FBevTotal := 0;
  FGuestSubsidy := 0.00;
  FHostSubsidy := 0.00;

  if (OLList.Count > 0) then begin
    for I := 0 to (OLList.Count - 1) do
    begin
      aOL := OLList.Items[I];
      if (aOL.ItemID > 0) then
      begin
        aToPay := aOL.ToPay;//, AppDetails.DecimalPlaces;

        FToPay := FToPay + aToPay;

        if aOL.FORB = 'F' then
          FFoodTotal := FFoodTotal + aToPay
        else
          FBevTotal := FBevTotal + aToPay;

        FLoyaltyFreeItemDiscount := GetRoundedUpDown(FLoyaltyFreeItemDiscount + aOL.LoyaltyFreeDiscount, AppDetails.DecimalPlaces);
        SalesTaxPercent := aOL.SalesTaxPercent;     //Jon 29-5-2001

        if (AppDetails.TaxExclusivePrices) then      //Jon 19-7-2001
          FSalesTax := FSalesTax + (((aToPay - aOL.LoyaltyFreeDiscount) * SalesTaxPercent) / 100)
        else                                      //Jon 19-7-2001
          FSalesTax := FSalesTax + (((aToPay - aOL.LoyaltyFreeDiscount) * SalesTaxPercent) / (100 + SalesTaxPercent));

        FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);

        if (AppDetails.CashSaleShowLoyaltyPoints1) then begin
          FLoyaltyPoints1 := FLoyaltyPoints1 + ((aToPay * aOL.LoyaltyPoints1RatioNum) / aOL.LoyaltyPoints1RatioDen) + (aOL.Qty * aOL.LoyaltyPoints1Qty);
        end;
        if (AppDetails.CashSaleShowLoyaltyPoints2) then begin
          FLoyaltyPoints2 := FLoyaltyPoints2 + ((aToPay * aOL.LoyaltyPoints2RatioNum) / aOL.LoyaltyPoints2RatioDen) + (aOL.Qty * aOL.LoyaltyPoints2Qty);
        end;
        FLoyaltyPoints1 := GetRoundedUpDown(FLoyaltyPoints1, AppDetails.DecimalPlaces);
        FLoyaltyPoints2 := GetRoundedUpDown(FLoyaltyPoints2, AppDetails.DecimalPlaces);
      end;
    end;
  end;
  FLoyaltyFreeItemDiscount := 0;
  DoingServiceTransaction := ((OLList.Count > 0) or (TLList.Count > 0));   //So we can decide if we want to test if main server is online again, when we're in offline mode

  RecalculateAll := ((not FUseItemDiscounts) and (not FUseDiscountPeriods));

  CalculateSubsidy(RecalculateAll);
  CalculateDiscountSchemeItemDiscount(RecalculateAll);
  CalculateDiscount;// show discounts

  //when using account type discounting, FDiscountPercent is zero, so we need
  //to temporarily calculate a percentage, in order to calculate the sales tax
  if ((FUseItemDiscounts) or (FUseDiscountPeriods) or (FComboDiscount > 0)) then
  begin
    if ((FToPay - FLoyaltyFreeItemDiscount) <> 0) then begin
      aDiscPercToUse := ((FDiscountAmount + FComboDiscount) / (FToPay - FLoyaltyFreeItemDiscount)) * 100;
    end
    else begin
      aDiscPercToUse := 0;
    end;
  end
  else
  begin
    aDiscPercToUse := FDiscountPercent;
  end;

  if AppDetails.EnableFiscalPrinting then
  begin
    if (OLList.Count > 0) then
    begin
      for I := 0 to (OLList.Count - 1) do
      begin
        aOL := OLList.Items[I];
        if (aOL.ItemID > 0) then
          FSalesTax := FSalesTax + GetRoundedUpDown(((aOL.ToPay - (aOL.ToPay * aDiscPercToUse / 100)) * aOL.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
      end;
    end;
  end
  else
  begin
    FSalesTax := FSalesTax * ((100 - aDiscPercToUse) / 100);
  end;

  FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);
  for I := 0 to (SurchargeList.Count - 1) do
  begin
    SurchargeItem := SurchargeList.Items[I];
    if AppDetails.EnableFiscalPrinting then
    begin
      FSalesTax := FSalesTax + GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
    end
    else
    begin
      if (AppDetails.TaxExclusivePrices) then
      begin
        FSalesTax := FSalesTax + GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / 100, AppDetails.DecimalPlaces);
      end
      else
      begin
        FSalesTax := FSalesTax + GetRoundedUpDown((SurchargeItem.Amount * SurchargeItem.SalesTaxPercent) / (100 + SurchargeItem.SalesTaxPercent), AppDetails.DecimalPlaces);
      end;
    end;
  end;

  //Jon 29-5-2001  to get the FSalesTax to 2 dp
  FSalesTax := GetRoundedUpDown(FSalesTax, AppDetails.DecimalPlaces);

  FSalesTaxCalculated := True;
  //Jon 30-5-2001  to get the FToPay to 2 dp
  FToPay := GetRoundedUpDown(FToPay, AppDetails.DecimalPlaces);

  //AMANDA 4/10/00
  //if ON A TABLE, only set avail if there is a limit
  //if (chkTable.State = cbChecked) and (TheLimit > 0.00) then begin
  lblAcctName.Caption := DoubleAmpersand(GlbAccount.AccountName);

  cmdEvent.Color := clWhite;

  if (OnTable) and (GlbTable.SaleCategoryType <> scDriveThru) and (GlbTable.SpendLimitActive) then
  begin
    EnableAvailBox(True);
    lblAvail.Caption := sAvailable;
    if GlbAccount.AccountID > 0 then
      lblAcctName.Caption := ' ' + DoubleAmpersand(GlbAccount.AccountName);

    FAvail := (GlbTable.Avail - FToPay);
    txtAvail.Text := FormatCurrencyNoSign(FAvail);
    txtAvail.Font.Color := clBlack;
    //AMANDA 12/10/00
    if (FAvail < 0.00) then begin
      txtAvail.Color := $00FF80FF; //light fuchsia
    end
    else begin
      txtAvail.Color := clBtnFace;
    end;

    lblTitle.Visible := False;
    pnlAvail.Visible := True;
    pnlLoyalty.Visible := (GlbAccount.AccountID > 0);
  end
  else if (GlbAccount.AllowCredit) then
  begin
    EnableAvailBox(True);
    lblAcctName.Caption := '';
    if (GlbEvent.EventID <= 0) then
    begin
      lblAvail.Caption := DoubleAmpersand(GlbAccount.AccountName);
      txtAvail.Text := FormatCurrencyNoSign(Abs(GlbAccount.CurrentBalance));
      //AMANDA 12/10/00
      if ((GlbAccount.CurrentBalance) > 0.00) then
      begin
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
    lblTitle.Visible := False;    
    pnlAvail.Visible := True;
    pnlLoyalty.Visible := (GlbEvent.EventID <= 0);
  end
  else
  begin
    if GlbAccount.AccountID > 0 then
    begin
      EnableAvailBox(False);
      lblAcctName.Caption := DoubleAmpersand(GlbAccount.AccountName);
      lblTitle.Visible := False;
      pnlAvail.Visible := True;
      pnlLoyalty.Visible := True;
    end
    else
    begin
      lblTitle.Visible := True;    
      pnlAvail.Visible := False;
      pnlLoyalty.Visible := False;
    end;
  end;
end;

{******************************************************************************}
procedure TformQuickSaleUser.CalculateDiscount;
var
  tmpDiscountAmount: Currency;
  I: Integer;       //Jon 26-05-2003
  Balance: Currency;
  aSI: TSurchargeItem;

begin
  //if not Zapa / Goody sale, check discount limit and change % / $ if necessary
  if (FLimitMaximum) and (not InGoodySale) then
  begin
    if ((FDiscountPercent < FOrigDiscountPercent) and (FUseDiscountAmount)) then
    begin
      tmpDiscountAmount := GetRoundedUpDown(((FToPay - FLoyaltyFreeItemDiscount) * FOrigDiscountPercent) / 100, AppDetails.DecimalPlaces);

      if (tmpDiscountAmount < FMaxDiscount) then begin
        FDiscountPercent := FOrigDiscountPercent;
        FDiscountAmount := tmpDiscountAmount;
        FUseDiscountAmount := False;
      end;
    end;
    if (FDiscountAmount > FMaxDiscount) then begin
      ShowQuickMsg(Format(sAcctLimit, [FormatCurrencySign(FMaxDiscount)]));
      FDiscountAmount := FMaxDiscount;
      FUseDiscountAmount := True;
    end;
  end
  //if Goody sale, check discount limit and warn if necessary
  else if FLimitMaximum and InGoodySale then
  begin
    //NB: Code should never get here as a scanned discount that exceeds the limit is rejected.
    //Assumes account selected is Goody Discount Account. Always want FUseDiscountAmount = True.
    if FDiscountAmount > FMaxDiscount then
      ShowMsg(Format(sGoodyDiscExceedsAcctLimit, [FormatCurrencySign(FMaxDiscount)]));
  end;

  {if (FUseDiscountAmount) then
  begin
    if ((FToPay - FLoyaltyFreeItemDiscount) <= 0) then begin
      FDiscountPercent := 0.00;
    end
    else begin
      FDiscountPercent := (((FDiscountAmount/(FToPay - FLoyaltyFreeItemDiscount)) * 10000)) / 100;
    end;
  end;}
  FLoyaltyFreeItemDiscount := GetRoundedUpDown(FLoyaltyFreeItemDiscount, AppDetails.DecimalPlaces);

  txtDiscountPercent.Text := FormatFloat('##0.00', FDiscountPercent);
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
    SaleCategorySurchargeItem.Amount := SaleCategorySurchargeItem.Fixed + GetRoundedUpDown(((FToPay - FLoyaltyFreeItemDiscount - FDiscountAmount - FComboDiscount) * SaleCategorySurchargeItem.Percent) / 100, AppDetails.DecimalPlaces);
  end
  else if (SaleCategorySurchargeItem <> nil) then begin
    for I := (SurchargeList.Count - 1) downto 0 do begin
      aSI := SurchargeList.Items[I];
      if (aSI.SurchargeType = stSaleCategory) then begin
        SaleCategorySurchargeItem.Free;
        SaleCategorySurchargeItem := nil;
        SurchargeList.Delete(I);
      end;
    end;
  end;

  // Note: We do this even if no order lines.  You can have a surcharge and no order lines if have topped up a transactor gift card.
  FSurcharge := 0;
  for I := 0 to (SurchargeList.Count - 1) do
  begin
    aSI := SurchargeList.Items[I];
    FSurcharge := FSurcharge + GetRoundedUpDown(aSI.Amount, AppDetails.DecimalPlaces);
  end;

  if (OnTable and (GlbTable.SaleCategoryType = scStandard)) then  // normal tab order, no need to display discount, no tenders, so just display topay
    Balance := (GetRoundedUpDown(FToPay, AppDetails.DecimalPlaces))
  else if not DoingWaste then
  begin
    if (AppDetails.TaxExclusivePrices) then begin      //Jon 30-5-2001
      if (FSalesTaxCalculated) then begin   //Jon 30-5-2001
        if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount) and (GlbEvent.SubsidyID > 0)) then
          Balance := GetRoundedUpDown(FGuestSubsidy + FSurcharge + FSalesTax, AppDetails.DecimalPlaces)
        else
          Balance := GetRoundedUpDown(FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount + FSalesTax, AppDetails.DecimalPlaces);    //Jon 30-5-2001
      end
      else begin
        if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount) and (GlbEvent.SubsidyID > 0)) then
          Balance := GetRoundedUpDown(FGuestSubsidy + FSurcharge + FSalesTax, AppDetails.DecimalPlaces)
        else
          Balance := GetRoundedUpDown(FToPay + FSurcharge - FDiscountAmount - FComboDiscount - FLoyaltyFreeItemDiscount + ((FSalesTax * (100 - FDiscountPercent)) / 100), AppDetails.DecimalPlaces);   //Jon 30-5-2001
      end;
    end
    else
    begin
      if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount) and (GlbEvent.SubsidyID > 0)) then
        Balance := GetRoundedUpDown(FGuestSubsidy + FSurcharge, AppDetails.DecimalPlaces)
      else
        Balance := GetRoundedUpDown(FToPay + FSurcharge - FDiscountAmount- FComboDiscount - FLoyaltyFreeItemDiscount, AppDetails.DecimalPlaces);
    end;
  end
  else
    Balance :=0;

  txtBalDue.Caption := FormatCurrencyNoSign(Balance);

  txtLoyaltyPoints1.Caption := FormatFloat('###,##0', Trunc(FLoyaltyPoints1));
  txtLoyaltyPoints2.Caption := FormatFloat('###,##0', Trunc(FLoyaltyPoints2));
  if (OnTable and (GlbTable.SaleCategoryType = scStandard)) then  // normal tab order, no need to display discount, no tenders, so just display topay
    FStillDue := FToPay
  else if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount) and (GlbEvent.SubsidyID > 0)) then
    FStillDue := CalculateStillDue(FGuestSubsidy + FSurcharge, 0, FPayment, FSalesTax)
  else if (FSalesTaxCalculated) then begin   //Jon 30-5-2001
    FStillDue := CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FDiscountAmount + FLoyaltyFreeItemDiscount, FPayment, FSalesTax);
  end
  else begin
    FStillDue := CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FDiscountAmount + FLoyaltyFreeItemDiscount, FPayment, GetRoundedUpDown((FSalesTax * (100 - FDiscountPercent)) / 100, AppDetails.DecimalPlaces));
  end;

  FStillDue := GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces);

  if ((GlbAccount.AccountID > 0) and (OnAccount) and (not FDoingRefund)) then
  begin
    if ((GlbAccount.CreditLimitActive) and (FStillDue > (GlbAccount.CreditLimit - GlbAccount.CurrentBalance))) then
    begin
      FRequired := (FStillDue - (GlbAccount.CreditLimit - GlbAccount.CurrentBalance));
    end
    else begin
      FRequired := 0;
    end;
  end
  else begin
    FRequired := FStillDue;
  end;

  //TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding);

  if not DoingWaste then
  begin
    if (AppDetails.CashSaleSwapStillDueForTenders) then begin   //Jon 21-08-2003
      txtStillDue.Caption := FormatCurrencyNoSign(FTender);
    end
    else begin
      txtStillDue.Caption := FormatCurrencyNoSign(FRequired);
    end;
  end
  else
    txtStillDue.Caption := FormatCurrencyNoSign(0);
end;

{******************************************************************************}
procedure TformQuickSaleUser.CalculateDiscountSchemeItemDiscount(RecalculateAll: Boolean);
begin
  PrevOLCount := OLList.Count;

  CalculateComboAndAccountDiscounts;

  txtComboDisc.Visible := (FComboDiscount > 0);
  lblComboDisc.Visible := (FComboDiscount > 0);
  if FComboDiscount > 0 then
    txtComboDisc.Text := FormatFloat(PRICEFORMAT, FComboDiscount);
  cmdComboIndex.Enabled := ((AppDetails.EnableCombo) and (DiscCalculator.SaleHasCombos));
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCashClick(Sender: TObject);
begin
  if ((AppDetails.CashSaleRemoveTenderChange) and (TLList.Count > 0)) then begin
    EditTenderLine(True);
  end
  else begin
    AddTenderLine(TLTCash);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdEFTPOSClick(Sender: TObject);
begin
  AddTenderLine(TLTEFTPOS);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdVisaClick(Sender: TObject);
begin
  AddTenderLine(TLTVisa);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDinersClick(Sender: TObject);
begin
  AddTenderLine(TLTDiners);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdAMEXClick(Sender: TObject);
begin
  AddTenderLine(TLTAMEX);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdMasterCardClick(Sender: TObject);
begin
  AddTenderLine(TLTMasterCard);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdOtherDesClick(Sender: TObject);
begin
  AddTenderLine(TLTOther);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdChequeClick(Sender: TObject);
begin
  if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '')) then
    AddTenderLine(TLTPocketVoucher)
  else if AppDetails.wiGroupEnabled then
    AddTenderLine(TLTwiGroup)
  else
    AddTenderLine(TLTCheque);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdVoucherClick(Sender: TObject);
begin
  AddTenderLine(TLTVoucher);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdPMSClick(Sender: TObject);
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;

  if ((DoingWaste) or (FStillDue = 0)) then begin
    cmdQuickCashClick(Sender);
    Exit;
  end;

  case AppDetails.PMSType of
    pmsLibica: AddTenderLine(TLTLibica);
    pmsFidelio, pmsPhoenix, pmsMews, pmsAgora, pmsRMS: begin
      if ((GetDue(True) > 0) and (not (OnTable xor (GlbTable.SaleCategoryType = scDriveThru))) and (not DoingPhoneOrder) and (not DoingWaste)) then begin
        if (AppDetails.PMSType = pmsPhoenix) then begin
          AddTenderLine(TLTPhoenix);
        end
        else if (AppDetails.PMSType = pmsMews) then
        begin
          AddTenderLine(TLTMews);
        end
        else if (AppDetails.PMSType = pmsAgora) then
        begin
          AddTenderLine(TLTAgora);
        end
        else if (AppDetails.PMSType = pmsRMS) then
        begin
          AddTenderLine(TLTRMS);
        end
        else begin
          AddTenderLine(TLTFidelio);
        end;
      end
      else begin
        cmdQuickCashClick(Sender);
      end;
    end;
    pmsMicros4700: begin
      if ((GlbAccount.AccountID > 0) and (GlbAccount.Folio <> '') and (ShowConf(format(sMicrosTransfer, [GlbAccount.Folio, GlbAccount.SubFolio])) = mrYes)) then
        AddTenderLine(TLTMicros4700Account)
      else
        AddTenderLine(TLTMicros4700);
    end;
    pmsNZAGold: AddTenderLine(TLTNZAGold);
    pmsEvolution: AddTenderLine(TLTEvolution);
    pmsMiniBar, PMSICRTouch: AddTenderLine(TLTRoomCharge);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DeleteTenderLine;
var
  TLType: TTLT;
  TL: TTL;
  Tender: Currency;
  Surcharge: Currency;
begin
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

  Surcharge := TL.TLSurcharge;
  //if EFTPOS integrated and is EFTPOS line, undo trans, if doesn't work showmsg & Exit

  if (AppDetails.EftposIntegrated) then begin
    TLType := TL.TLType;
    if (TLType in [TLTEFTPOS, TLTVisaEFTPOS, TLTAmexEFTPOS, TLTDinersEFTPOS, TLTMasterCardEFTPOS, TLTBarterCard]) then
    begin
      Tender := TL.TLTender;
      //undoing a payment, thus a refund is made to them
      //or undoing a refund, thus a payment is made from them
      //NOTE: Even for a refund, the EFTPOS value must be a positive value
      formEFTPOSIntegration.EFTPOSTLType := TL.TLType;
      formEFTPOSIntegration.EFTPOSTxnRef := TL.EFTPOSTxnRef;
      formEFTPOSIntegration.EFTPOSTxnAuthCode := TL.EFTPOSTxnAuthCode;
      if AppDetails.EFTPOSType in [ecTEFIPDaruma] then
      begin
        formEFTPOSIntegration.DoEftposTrans(0, 0, ettFinalize, '');
        SWBEncryption.AuxiliaryData.TEFDone := '0';
        SWBEncryption.GenerateEncryptedAuxFile;
      end
      else
      begin
        if (((not FDoingRefund) and (not formEFTPOSIntegration.DoEftposTrans(Tender, 0.00, ettRefund))) or
            ((FDoingRefund) and (not formEFTPOSIntegration.DoEftposTrans(Tender, 0.00, ettEftpos)))) then
        begin
          ShowMsg(sCannotClearEFtPosTender);
          Exit;
        end;
        if AppDetails.EFTPOSType in [ecDPS, ecSektor, ecSektorVault, ecSyncroPlus, ecSmartpaySmartLink, ecSmartLinkLiteWS, ecPaymentSenseWS] then
          formEFTPOSIntegration.RemoveTransactionID(TL.EFTPOSTxnRef);
      end;
    end;
  end;
  if (AppDetails.SmartCardInterface > 0) then begin
    if (TL.TLType = TLTSmartCard) then begin
      Tender := TL.TLTender;
      //undoing a payment, thus a refund is made to them
      //or undoing a refund, thus a payment is made from them
      if (((not FDoingRefund) and (not DoSmartCardTransaction(-Tender))) or
          ((FDoingRefund) and (not DoSmartCardTransaction(Tender)))) then begin
        ShowMsg(sCannotClearSmartCardTender);
        Exit;
      end;
    end;
  end;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.CheckReverseTransaction(TL);

  TLList.DisposeTL(grdTLs.Row - 1);
  LoadTLs(LADelete);
  if (Surcharge <> 0) then begin
    CalculateToPay;
  end;

  if AppDetails.wiGroupEnabled then
    wiGroupPOSSrv.CheckClearToPay(TLList);

  BroadcastCDAMessage;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDetailsClick(Sender: TObject);
var
  Notes: string;
  TL: TTL;

begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (TLList.Count > 0) then begin
    TL := TLList.Items[grdTLs.Row-1];
    Notes := TL.TLNotes;

    if (GetSingleNote(Notes, 40, sTenderLineDetail, sTenderLineDetailCol, tiMoneyBag, True)) then begin
      TL.TLNotes := Notes;
      LoadTLs(LAEdit);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTLClearClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if TLList.Count > 0 then
  begin
    if (ShowConf(sClearAll) = mrYes) then
    begin
      if (EFTPOSTransExists and (AppDetails.EFTPOSIntegrated)) then
      begin
        ShowMsg(sEftPosTenderLineFound);
        Exit;
      end;
      if ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard))) then
      begin
        ShowMsg(sClearSmartCardTenderLines);
        Exit;
      end;

      if AppDetails.wiGroupEnabled and TLList.TenderTypeExists(TLTwiGroup) then
      begin
        ShowMsg(Format(sClearwiGroupTenderLines, [swiGroupTL, swiGroupTL]));
        Exit;
      end;

      ClearAllTenderLines;
      CalculateToPay;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTLDeleteClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001

  DeleteTenderLine;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetOLCols;
begin
  with grdOLs do begin
    ColCount := 4;
    if (appdetails.CashSaleUseCourses) then begin
     ColWidths[0] := 60;
     ColWidths[1] := 40; //JEH 18/08/2001
     ColWidths[2] := 145;
     ColWidths[3] := 60;
    end
    else begin
      ColWidths[0] := -1;
      ColWidths[1] := 60; //JEH 18/08/2001
      ColWidths[2] := 185;
      ColWidths[3] := 60;
    end;
  end;
  ClearAllOrderLines;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDestnClick(Sender: TObject);
var
  HeldGroupID: Integer;
  HeldGroupName: string;
begin
  if ((Sender is TfcCustomImageBtn) and (DoingTransaction)) then begin     //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (AppDetails.CashSaleHoldOrder) then begin
    if (SelectHeldGroup(HeldGroupID, HeldGroupName)) then begin
      ChangeHeldGroup(HeldGroupID, HeldGroupName);
    end;
  end
  else begin
    if (((DoingPhoneOrder) or ((GlbTable.SaleCategoryType >= scDriveThru) and (not OnTable))) and (AppDetails.CashSaleUseTabNotes)) then begin
      GetSingleNoteMemo(FTabNotes, 500, sPhoneNote, sNoteColon, tiPencil);
    end
    else begin
      FDeliverySetByUser := GetSingleNote(FDelivery, 60, sDestination, sDeliverToColon, tiPencil, True);
    end;
  end;
  TemporarilyDeactiveButtons;
  PromptedWhere := True;       //Jon 12-01-2006
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdFoodClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  ItemView := ivFood;
  SetView;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdBevClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  ItemView := ivBev;
  SetView;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdListClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  ShowButtons := False;
  SetView;
  SetButtons;
end;
{******************************************************************************}
function TformQuickSaleUser.OkToClose: Boolean;
var
  ParkSale: Integer;
  aConfResult: TModalResult;
begin
  StopInactiveTimer;
  Result := True;

  //if doing Goody sale, confirm clear all Goody details first
  if Result and InGoodySale then
  begin
    if (ShowConf(sConfirmClearGoodyDetails) <> mrYes) then
      Result := False
    else
      IniGoodyTrans;
  end;

  if Result and ((OLList.Count > 0) or (TLList.Count > 0) or ((AppDetails.ClearItemsVoid > 0) and (VoidOLList.Count > 0))) then
  begin
    if (EFTPOSTransExists) then
    begin
      ShowMsg(sEftPosTenderLineFound);
      Result := False;
    end
    else if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
      Result := False
    else if ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard))) then
    begin
      ShowMsg(sClearSmartCardTenderLines);
      Result := False;
    end
    else if (ConfirmBeforeCancel) then
    begin
      if (AppDetails.CashSaleRemoveParkSale) then
      begin
        aConfResult := ShowConf(sCloseWithoutCashSale);
      end
      else
      begin
        aConfResult := ShowConfWithExtra(sCloseWithoutCashSale, sParkSale, gstParkSale);
      end;
      if (aConfResult = mrAll) then
      begin
        if (TryParkSale(ParkSale, GlbLogin.LoginID)) then
        begin
          ResetCashSale(False, False);
          ClearCDAMessage;
        end
        else begin
          Result := False;
        end;
      end
      else if (aConfResult <> mrYes) then
      begin
        Result := False;
      end
      else
      begin
        CancelCashSale;
      end;
    end
    else
    begin
      CancelCashSale;
    end;
  end;

  if Result then
    formEFTPOSIntegration.ClearTransactionID;

  SFiscalPrinting.SupressFiscalErrorMessage := True;
  if ((Result) and
      (AppDetails.EnableFiscalPrinting) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0))) and
      (SFiscalPrinting.GetFiscalprinterStatusFlags)) then
  begin
    SFiscalPrinting.SupressFiscalErrorMessage := False;
    if SFiscalPrinting.OpenedFiscalInvoice then
      SFiscalPrinting.CancelFiscalPrinterInvoice;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdHideClick(Sender: TObject);
begin
  StopInactiveTimer;
  if (ButtonsDeactivated) then begin    //Jon 25-02-2002
    Exit;
  end;
  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  tmrQSInactivate.Enabled := False;
  GlbLogin.ShowLastReceipt := False;
  if ((Sender <> nil) and (not OkToClose)) then begin
    StartInactiveTimer;
    Exit;
  end;
  sExternalAccountIntegration.ClearPresentedCards;
  inherited; // This executes the event associated with this same procedure in
             // basedetailform -ie closes.
//  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTLUpDesClick(Sender: TObject);
begin
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdTLs.Handle, WM_VSCROLL, SB_PAGEUP, 0);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTLDownDesClick(Sender: TObject);
begin
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  SendMessage(grdTLs.Handle, WM_VSCROLL, SB_PAGEDOWN, 0);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCancelDesClick(Sender: TObject);
var
  ParkSale: Integer;
  aConfResult: TModalResult;
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  //confirm cancel ONLY if have added order or tender lines
  if ((OLList.Count > 0) or (TLList.Count > 0)) then
  begin
    if (AppDetails.CashSaleRemoveParkSale) then
    begin
      aConfResult := ShowConf(sCloseWithoutCashSale);
    end
    else
    begin
      aConfResult := ShowConfWithExtra(sCloseWithoutCashSale, sParkSale, gstParkSale);
    end;

    if (aConfResult = mrAll) then
    begin
      if (not TryParkSale(ParkSale, GlbLogin.LoginID)) then
      begin
        Exit;
      end;
    end
    else if (aConfResult <> mrYes) then
    begin
      Exit;
    end;
  end;
  CancelCashSale;
  if (AppDetails.EnableFiscalPrinting and CheckFiscalPrinterFunctions and ShowCPFNUmberInActivate and AppDetails.FiscalIsCFPRequired) then
  begin
    FCPFNumber := GetCPFNumber;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.CancelCashSale(DoCancelFiscalPrinterInvoice: Boolean = False);
var
  OL: TOrderLine;
  VoidOL: TOrderLine;
  I: Integer;
  TheSP: TIB_StoredProc;
  TheTR: TIB_Transaction;
  SPNumber, TheInvoiceID: Integer;
begin
  SFiscalPrinting.FiscalOpTryAgainCommand := False;
  TheInvoiceID := 0;
  try
    if AppDetails.EnableFiscalPrinting then
    begin
      if ((AppDetails.EFTPOSIntegrated) and (EFTPOSTransExists)) then
      begin
        if AppDetails.EFTPOSType in [ecTEFIPDaruma] then
          formEFTPOSIntegration.DoEftposTrans(0, 0, ettFinalize, '');
      end;
      SWBEncryption.AuxiliaryData.TEFDone := '0';
      SWBEncryption.GenerateEncryptedAuxFile;
    end
    else
    begin
      if (EFTPOSTransExists) then
      begin
        ShowMsg(sEftPosTenderLineFound);
        Exit;
      end
      else if ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard))) then
      begin
        ShowMsg(sClearSmartCardTenderLines);
        Exit;
      end
    end;
    // RPC: we need to copy all order lines to void lines and set type of void
    for I := 0 to (OLList.Count - 1)do
    begin
      OL := OLList.Items[I];
      if ((OL.ItemID > 0) and (not OL.AlreadyOrdered)) then begin
        VoidOL := TOrderLine.Create;
        VoidOL.CopyOL(OL);
        //if an order line as opposed to a modifier
        VoidOLList.Add(VoidOL);
      end;
    end;
    if (AppDetails.OrdersLockWait) then
    begin
      SPNumber := 3;
    end
    else
    begin
      SPNumber := 1;
    end;

    TheTR := dm.GetTR(SPNumber);
    TheSP := dm.GetSP(SPNumber);
    if (not dm.TRStartTest(SPNumber)) then begin
      Exit;
    end;
    DM.qrGeneral.IB_Transaction := TheTR;
    VoidClearedOL(SPNumber, True);

    if ((AppDetails.EnableFiscalPrinting) and (AppDetails.ClearItemsVoid > 0) and (SFiscalPrinting.OpenedFiscalInvoice)) then
    begin
      with TheSP do
      begin
        if (TheVoidOrderID > 0) then
        begin
          SetStoredProcName('INSERT_INVOICE', SPNumber);   //insert_invoice
          ParamByName('discountrate').AsCurrency := FDiscountPercent;
          IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
          ParamByName('outletid').AsInteger := AppDetails.OutletID;
          IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, OnTable);
          IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);
          ParamNull(ParamByName('refundnote'));
          ParamByName('invoicetype').AsString := 'I';
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamByName('salestax').AsCurrency := 0;
          IntegerAsParam(ParamByName('cashsaleorderid'), TheVoidOrderID);
          ParamNull(ParamByName('REFERENCENO'));
          StringAsParam(ParamByName('cpfnumber'), FCPFNumber);
          ParamNull(ParamByName('FiscalCOO'));
          ParamNull(ParamByName('FiscalCCF'));
          IntegerAsParam(ParamByName('eventid'), GlbEvent.EventID);
          if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode))) then
          begin
            StringAsParam(ParamByName('FiscalCOO'), SFiscalPrinting.COO);
            StringAsParam(ParamByName('FiscalCCF'), SFiscalPrinting.CCF);
          end;
          ExecProc;
          if (not FieldByName('errorcode').IsNull) then
          begin
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
            TheTR.Rollback;
            Exit;
          end
          else
          begin
            TheInvoiceID := FieldByName('invoiceid').AsInteger;
            if (not DoingPhoneOrder) and (not (OnTable and (GlbTable.GroupID > 0))) then
            begin
              SetStoredProcName('UPDATEFISCALDETAILS', SPNumber);
              ParamByName('MANUFACTURENO').AsString := GlbFiscal.ManufactureNo;
              ParamByName('ECFMODEL').AsString := GlbFiscal.ECFModel;
              ParamByName('ADDITIONALMF').AsString := GlbFiscal.MFAdditional;
              ParamByName('THEID').AsInteger := TheInvoiceID;
              ParamByName('TABLETYPE').AsInteger := Ord(fttInvoice); // INVOICE Table
              ParamByName('MANUALSERIES').AsString := '';
              ParamByName('MANUALSUBSERIES').AsString := '';
              ParamByName('FISCALINVNO').AsString := '';
              ParamByName('FISCALSEQNO').AsInteger := AppDetails.FiscalPrinterSeqNo;
              if NFManual or NFEMode then
              begin
                ParamByName('MANUFACTURENO').AsString := '';
                ParamByName('ECFMODEL').AsString := '';
                ParamByName('ADDITIONALMF').AsString := '';
              end;
              if NFManual then
              begin
                ParamByName('MANUALSERIES').AsString := FManualSeries;
                ParamByName('MANUALSUBSERIES').AsString := FManualSubSeries;
                ParamByName('FISCALINVNO').AsString := FManualInvNo;
              end;
              ParamByName('FISCALINVTYPE').AsString := '2D';
              if NFManual then
                ParamByName('FISCALINVTYPE').AsString := '02'
              else if NFEMode then
                ParamByName('FISCALINVTYPE').AsString := '55';
              ExecSQL;
            end;
          end;
        end;
      end;

      if ((FVoidID > 0) and (TheInvoiceID > 0)) then
      begin
        with DM.qrGeneral do
        begin
          Close;
          SQL.Clear;
          SQL.Add('UPDATE VOIDLN SET');
          SQL.Add('       INVOICEID = :invoiceid');
          SQL.Add(' WHERE (VOIDID = :VOIDID)');
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          ParamByName('voidid').AsInteger := FVoidID;
          ExecSQL;
        end;
      end;

      SFiscalPrinting.UpdateInvoiceChecksum(TheSP, TheInvoiceID, True, SPNumber);

{      if (not SFiscalPrinting.UpdateTableCount(0, 0, 0, 0, 0, 1, 0, 0, 0, SPNumber)) then
      begin
        TheTR.Rollback;
        Exit;
      end;   }

      // Cancel Fiscal Invoice
      if ((not (NFManual or NFEMode)) and
          (not DoingPhoneOrder) and
          (not (OnTable and (GlbTable.GroupID > 0))) and
          (SFiscalPrinting.GetFiscalPrinterStatusFlags) and (SFiscalPrinting.OpenedFiscalInvoice)) then
      begin
        SFiscalPrinting.CancelFiscalPrinterInvoice;
      end;
    end;

    TheTR.Commit;
    cmdTable.Enabled := True;
    if AppDetails.EnableFiscalPrinting then
      SWBEncryption.GenerateEncryptedAuxFile;

    ResetCashSale(False, False);
    ClearCDAMessage;
    DoPhoneOrder(PhoneOrderPersistant, PhoneOrderPersistant);

    // Here lets start the timer again!
    if (AppDetails.CashSaleStaffPromptTimeout > 0) then
    begin
      StaffPromptTimeoutTimer.Interval := (AppDetails.CashSaleStaffPromptTimeout * 1000);
      StaffPromptTimeoutTimer.Enabled := True;
      StaffPromptTimeoutTimerActive := True;
    end;

    if (AppDetails.PDEnabled) then
    begin
      DMComponents.PDOutputLine('TRANSACTION', 0);
      DMComponents.PDOutputLine('CANCELLED', 1);
      DMComponents.tmrPoleDisplay.Interval := 30000;
      DMComponents.tmrPoleDisplay.Enabled := True;
    end;

  finally
    begin
      if ((DoCancelFiscalPrinterInvoice) and (SFiscalPrinting.OpenedFiscalInvoice)) then
      begin
        SFiscalPrinting.CancelFiscalPrinterInvoice;
      end;
    end;
  end;

  RSC.AllowUpdate := False;
  RSC.ReasonAlreadyOrdered := False;
  RSC.CardCode := '';
  RSC.ItemID := 0;
  RSC.CancelPressed := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ClearAllOrderLines;
begin
  ClearListObjects(OLList);
  ClearListObjects(VoidOLList);
  with grdOLs do begin
    RowCount := 1;
    Cells[0, 0] := '';
    Cells[1, 0] := '';
    Cells[2, 0] := '';
    Cells[3, 0] := '';
    Row := 0;
  end;
  CalculateToPay;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ClearAllSurcharges;
begin
  ClearListObjects(SurchargeList);
  SaleCategorySurchargeItem := nil;
end;
{******************************************************************************}
procedure TformQuickSaleUser.VoidClearedOL(SPNumber: Integer; DoCancel: Boolean);
var
  TheSP: TIB_StoredProc;
  I: Integer;
  J: Integer;

  OL: TOrderLine;
  VoidOL: TOrderLine;

  QtyToVoid: Currency;
  QtyToWaste: Currency;
  ToWaste: Boolean;
  VoidReasonID: Integer;
  Exp1: string;
  Exp2: string;

  VoidDisplayMode: TVoidDisplayMode;
  Note: string;
  ItemstoVoid: Boolean;
  aHash: string;

begin
  Exp1 := '';
  Exp2 := '';
  // RPC: Here we create a void record for any orderlines that have been cleared
  //let see if have any un ordered items
  ItemstoVoid := False;
  for I := 0 to (VoidOLList.Count - 1) do
  begin
    VoidOL := VoidOLList.Items[I];
    if ((VoidOL.ItemID > 0) and (not VoidOL.AlreadyOrdered)) then
    begin //RPC: Added so we only void items that have already been ordered before
      ItemstoVoid := True;
    end
  end;

  TheSP := dm.GetSP(SPNumber);
  if (DoCancel) then
  begin
    VoidReasonID := AppDetails.OrderClearVoidReasonID;
    VoidDisplayMode := vdmCancelOrder;
    Note := sReasonforcancel;
  end
  else
  begin
    VoidReasonID := AppDetails.OLClearVoidReasonID;
    VoidDisplayMode := vdmClearItemVoid;
    Note := sReasonforclear;
  end;
  if ((AppDetails.ClearItemsVoid > 0) and (VoidOLList.Count > 0) and (ItemstoVoid)) then
  begin
    if (AppDetails.ClearItemsVoid > 2) then
    begin
      ShowVoid(QtyToVoid, QtyToWaste, ToWaste, 0, VoidReasonID, Exp1, Exp2, VoidDisplayMode, False);
    end
    else if (AppDetails.ClearItemsVoid = 2) then
    begin
      GetSingleNote(Exp1, 30, sReason, Note, tiPencil, False);
    end;
    with TheSP do
    begin
      if (TheVoidOrderID <= 0) then
      begin
        //create order record
        SetStoredProcName('INSERT_ORDER', SPNumber); //insert_order

        ParamByName('outletid').AsInteger := AppDetails.OutletID;

        //if (chkTable.State = cbChecked) then
        IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, ((OnTable) and (not DoingPhoneOrder)));

        ParamByName('rush').AsInteger := 0;
        StringAsParam(ParamByName('location'), FDelivery);
        ParamByName('loginid').AsInteger := GlbLogin.LoginID;

        //may be null from tables already open when Sale Categories introduced
        IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);

        //if (chkTable.State = cbChecked) then
        IntegerAsParamIfBool(ParamByName('tableno'), GlbTable.TableNo, OnTable);
        IntegerAsParamIfBool(ParamByName('eatintakeaway'), EatInTakeawayPromptValue, ((not OnTable) and (AppDetails.CashSaleEatInTakeawayPrompt or DoingPhoneOrder)));
        IntegerAsParam(ParamByName('heldgroupid'), FHeldGroupID);
        ParamByName('CPFNumber').AsString := Trim(FCPFNumber);
        ExecProc;

        TheVoidOrderID := FieldByName('orderid').AsInteger;
        WBOrders.RefreshID := TheVoidOrderID;
      end;

      //now lets create te order line for each item (excluding already ordered items)
      for I := 0 to (VoidOLList.Count - 1) do
      begin
        VoidOL := VoidOLList.Items[I];
        if ((VoidOL.ItemID > 0) and (not VoidOL.AlreadyOrdered)) then
        begin //RPC: Added so we only void items that have already been ordered before
          //create order line
          SetStoredProcName('INSERT_ORDERLINE', SPNumber); //insert_orderline

          IntegerAsParam(ParamByName('courseid'), VoidOL.CourseID);
          ParamByName('qty').AsCurrency := VoidOL.Qty;
          ParamByName('happyhour').AsInteger := 0;
          ParamNull(ParamByName('positions'));
          ParamByName('itemid').AsInteger := VoidOL.ItemID;
          ParamByName('orderid').AsInteger := TheVoidOrderID;

          IntegerAsParam(ParamByName('printerid'), VoidOL.PrinterID);
          IntegerAsParam(ParamByName('repeatprinterid'), VoidOL.RepeatPrinterID);

          ParamByName('held').AsInteger := 0;

          ParamByName('openprice').AsCurrency := VoidOL.UnitPrice - VoidOL.ModPrice;
          //Use this regardless, as we can't invoice/tender and order with different prices.
          if (VoidOL.OpenPrice) then
          begin
            StringAsParamIfBool(ParamByName('openpriceitemabbrev'), VoidOL.ItemString, (AppDetails.OrderOpenPriceNameChange and (VoidOL.ItemString <> '')));
          end
          else
          begin
            ParamNull(ParamByName('openpriceitemabbrev'));
          end;

          if ((VoidOL.OLPriceLevel >= 1) and (VoidOL.OLPriceLevel <= 6)) then
          begin
            ParamByName('pricelevel').AsInteger := VoidOL.OLPriceLevel;
          end
          else
          begin
            ParamByName('pricelevel').AsInteger := 0;
          end;

          ParamByName('currenthour').AsInteger := CurrentHour; //Jon 24-06-2002
          ParamByName('currentminute').AsInteger := CurrentMin; //Jon 24-06-2002
          ParamByName('currentday').AsInteger := CurrentDay; //RPC 28-09-2010
          ParamNull(ParamByName('orderlineorder'));
          IntegerAsParam(ParamByName('comboid'), VoidOL.ComboID);
          IntegerAsParam(ParamByName('combogroup'), VoidOL.ComboGroupID);
          IntegerAsParam(ParamByName('comboindex'), VoidOL.ComboIndex);
          ParamByName('comboqty').AsCurrency := VoidOL.ComboQty;
          BoolAsParam(ParamByName('fixthecombo'), VoidOL.FixThisComboPart);
          BoolAsParam(ParamByName('freecomboitem'), VoidOL.FreeComboItem);
          ParamByName('notes').AsString := '';
          if AppDetails.EnableFiscalPrinting then
            ParamByName('notes').AsString := Format(sCancelItemNote, [FormatFloat('#####.00#', ParamByName('qty').AsFloat)]);

          if AppDetails.EnableFiscalPrinting then
          begin
            SWBEncryption.Initialise;
            SWBEncryption.AddInt(VoidOL.ItemID);
            SWBEncryption.AddStr(VoidOL.ItemString);
            SWBEncryption.AddStr(VoidOL.TaxSituation);
            SWBEncryption.AddStr(FloatToStr(VoidOL.SalesTaxPercent));
            SWBEncryption.AddCurrency(VoidOL.Qty);
            SWBEncryption.AddCurrency(VoidOL.UnitPrice);
            SWBEncryption.AddCurrency(VoidOL.Qty * VoidOL.UnitPrice);
            SWBEncryption.AddCurrency(VoidOL.Qty * VoidOL.UnitPrice);
            aHash := SWBEncryption.GetHash;
            ParamByName('Checksum').AsString := aHash;
          end;

          ExecProc;
          VoidOL.OrderLineID := FieldByName('orderlineid').AsInteger;
        end;
      end;
      //now we can insert void & void lines
      SetStoredProcName('INSERT_VOID', SPNumber); //insert_void

      IntegerAsFindParam(FindParam('outletid'), AppDetails.OutletID);
      IntegerAsParamIfBool(ParamByName('groupid'), GlbTable.GroupID, ((OnTable) and (not DoingPhoneOrder)));
      ParamByName('voidreasonid').AsInteger := VoidReasonID;
      StringAsParam(ParamByName('voidexplanation1'), Exp1);
      StringAsParam(ParamByName('voidexplanation2'), Exp2);
      ParamByName('loginid').AsInteger := GlbLogin.LoginID;
      ExecProc;
      FVoidID := FieldByName('voidid').AsInteger;

      J := 0;
      for I := 0 to (VoidOLList.Count - 1) do
      begin
        OL := VoidOLList.Items[J];
        if ((OL.ItemID > 0) and (not OL.AlreadyOrdered)) then
        begin //RPC: Added so we only void items that have already been ordered before
          SetStoredProcName('VOID_ORDERLINE', SPNumber);
          ParamByName('voidid').AsInteger := FVoidID;
          ParamByName('orderlineid').AsInteger := OL.OrderLineID;
          ParamByName('qtyvoided').AsCurrency := OL.Qty;
          ParamByName('qtywasted').AsCurrency := 0;
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
          OL.Free;
          VoidOLList.Delete(J);
        end
        else
        begin
          J := J + 1;
        end;
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.TableSelected(Selected: Boolean); //AMANDA 12/10/00
begin
  OnTable := Selected;
  if (Selected) then begin
//    Skin.ChangeButton(cmdTable, bstToolbar, gstNoChange, biNone, IntToStr(GlbTable.TableNo));
//    cmdTable.Font.Size := 9;

    if ((AppDetails.CashSaleUseBigScreen) and (AppDetails.CashSaleBigScreenButtonType in [2..4])) then begin
      cmdSecondControlParkedSale.Caption := IntToStr(GlbTable.TableNo);
      cmdSecondControlParkedSale.Font.Size := 9;
    end;

    if (GlbTable.Destination <> '') then begin
      FDelivery := GlbTable.Destination; //GIH: 18/01/2013 if we have set a destination value on the table, lets use it.
    end;

    if ((AppDetails.PMSType in [pmsLibica, pmsNZAGold, pmsEvolution]) and AppDetails.CashSaleAutoComplete) then begin
      cmdToPayPMS.Visible := False;
      cmdQuickCash.Visible := True;
    end;

//    Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
    Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
    ReIndexCombo(GlbTable.GroupID, OLList);
    cmdQuickCash.Tag := 0;
  end
  else begin
    if ((GlbTable.GroupID > 0) and (GlbTable.TableLocked)) then begin
      CheckTableLock(False, True);
    end;
    GlbTable.GroupID := 0;     //Jon 02-10-2002
    GlbTable.Uninvoiced := 0;     //Jon 02-10-2002
//    Skin.ChangeButton(cmdTable, bstToolbar, gstTabs, biNone, sTab);
//    cmdTable.Font.Size := 8;
    if ((AppDetails.CashSaleUseBigScreen) and (AppDetails.CashSaleBigScreenButtonType in [2..4])) then begin
      case AppDetails.CashSaleBigScreenButtonType of
        2: cmdSecondControlParkedSale.Caption := cmdTable.Caption;
        3: cmdSecondControlParkedSale.Caption := sDrive;
        4: cmdSecondControlParkedSale.Caption := sKiosk;
      end;
      cmdSecondControlParkedSale.Font.Size := 8;
    end;

//    Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
    Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
    cmdQuickCash.Tag := 1;

    if ((AppDetails.PMSType in [pmsLibica, pmsNZAGold, pmsEvolution]) and AppDetails.CashSaleAutoComplete) then begin
      cmdToPayPMS.Visible := True;
      cmdQuickCash.Visible := False;
    end;
    {            do in SelectSaleCategory
    if (GlbTable.SaleCategoryType >= 1) then begin
      Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
      cmdQuickCash.Tag := 0;
    end; }
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdSelectClick(Sender: TObject);
var
  InvoiceID: Integer;
  InvoiceType: string;
begin
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  PauseStaffPromptTimeoutTimer;

  with dm.sp do begin
    Close;
    Unprepare;
    StoredProcName := 'getlogindetails';
    Prepare;
    ParamByName('loginid').AsInteger := GlbLogin.LoginID;
    ExecProc;
    SelectedStaffid := FieldByName('staffid').AsInteger;
    dm.tr.Commit;
  end;
  SetQueryCashSales;

  if (SelectCashSale(InvoiceID, InvoiceType)) then
  begin
    if (InvoiceID = -1) then
    begin
      ShowQuickMsg(sNoCashSalesToPrint);
    end
    else if (not AppDetails.EnableFiscalPrinting) then
    begin
      if (InvoiceType = 'I') then
      begin
        if (AppDetails.FiscalType = fiscalTaxCore) then
          PrintTaxCoreInvoice(InvoiceID, True)
        else
        begin
        InsertPrintJob('C Invoice', InvoiceID, AppDetails.PrinterID);
        ShowQuickMsg(sCashSalePrinted);
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
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdLastClick(Sender: TObject);
var
  InvoiceID: Integer;
  InvoiceType: string;
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001

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
        ShowQuickMsg(sCashSalePrinted);
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
    try
      if (not dm.TRStartTest) then begin
        Exit;
      end;
      with dm.sp do begin
        SetStoredProcName('PRINTLASTINVOICE');        //printlastinvoice

        ParamByName('terminalid').AsInteger := AppDetails.TerminalID;
        ParamByName('printerid').AsInteger := AppDetails.PrinterID;
        if (AppDetails.CashSaleLastReceiptStaff) then begin
          IntegerAsParam(ParamByName('staffid'), GlbLogin.StaffID);
        end
        else begin
          ParamNull(ParamByName('staffid'));
        end;
        ExecProc;
        InvoiceID := FieldAsInt(FieldByName('invoiceid'));
        dm.tr.Commit;
        if (InvoiceID = 0) then begin
          ShowQuickMsg(sNoCashSalesToPrint);
        end
        else begin
          ShowQuickMsg(sCashSalePrinted);
        end;
      end;
    except
      dm.tr.Rollback;
    end;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTableClick(Sender: TObject);
begin
  StopInactiveTimer;
  InCashSale := False;
  try
    if (DoingTransaction) then
    begin    //Jon 25-02-2002
      Exit;
    end;
    if (FDoingRefund) then
    begin
      if (ShowConf(sConfirmCancellingRefund) <> mrYes) then
      begin
        Exit;
      end;
      SetDoingRefund(False, False);
    end;
    if (DoingWaste) then
    begin
      if (ShowConf(sConfirmCancellingWastage) <> mrYes) then
      begin
        Exit;
      end;
      SetDoingWaste(False, False);
    end;
    if (DoingExchange) then
    begin
      if (ShowConf(sConfirmCancellingExchange) <> mrYes) then
      begin
        Exit;
      end;
      SetDoingExchange(False, False);
    end;

    if (DoingPhoneOrder) then
    begin
      if (ShowConf(sConfirmCancelPhOrder) <> mrYes) then
      begin
        Exit;
      end;
      CancelCashSale;
    end;

    if ((AppDetails.EnableFiscalPrinting) and (SFiscalPrinting.OpenedFiscalInvoice) and (not (NFManual or NFEMode)) and (not OkToClose)) then
    begin
      Exit;
    end;

    PauseStaffPromptTimeoutTimer;
    //AMANDA 19/10/00 - must be no TLs
    DoSelectTable;
    BroadcastCDAMessage;
    ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  finally
    StartInactiveTimer;
    InCashSale := True;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoSelectTable;
var
  PreviousIsDriveThrough, FEnableOpen: Boolean;
  TableFunction: TTableFunction;
  PreviousSalesCategoryID: Integer;
  PrevGroupID, PrevAccountID: Integer;

begin
  FEnableOpen := True;
  if (AppDetails.CallerIDSelected) and (AppDetails.CallerIDType = ctWhozzCalling) then
    FEnableOpen := False;

  if (RSC.AllowUpdate) then begin
    if (ShowConf(sNoReplacementCardsOnTABs) <> mrYes) then begin
      //StartInactiveTimer;
      Exit;
    end;
    DeselectAccount;
    RemoveSwipeCardReplacement;
  end;

  if (TLList.Count > 0) then begin
    ShowMsg(sNoAcctSelectTillTenderRemove);
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  //if doing Goody sale, confirm clear all Goody details first
  if InGoodySale then
  begin
    if (ShowConf(sConfirmClearGoodyDetails) <> mrYes) then
    begin
      Exit;
    end;
    IniGoodyTrans;
  end;

  OrderBeingPlaced := (OLList.Count > 0);  //AKM 3.3.1

  PreviousIsDriveThrough := (GlbTable.SaleCategoryType > scStandard);
  PreviousSalesCategoryID := GlbTable.SaleCategoryID;
  PrevGroupID := GlbTable.GroupID;
  PrevAccountID := GlbAccount.AccountID;
  //True
  if (SelectTable(False, FEnableOpen, True, CurrentTime, CurrentDay, False, TableFunction)) then
  begin
    SetSaleCategory(GlbTable.SaleCategoryID);
    ResetTabDiscounts;

    if (TableFunction = tfOrder) then
    begin
      if ((PreviousIsDriveThrough) or
          ((GlbTable.GroupID <= 0) and (GlbTable.SaleCategoryType > scStandard))) then
      begin
        if (FDriveThroughGroupIDs.Find(GlbTable.GroupID) >= 0) then
        begin
          Exit;
        end;
        if ((GlbTable.GroupID <= 0) or (PrevGroupID <= 0) or
            (PreviousSalesCategoryID <> GlbTable.SaleCategoryID) or
            (GlbTable.SaleCategoryType <= scStandard) or
            (not VerifyPriv(1)) or
            (AppDetails.CashSaleMergeDriveThroughs <= 0) or
            ((AppDetails.CashSaleMergeDriveThroughs = 2) and (ShowConf(sConfirmMergeTabs) <> mrYes))) then
        begin
          ClearDriveThroughOLs;
          ClearListObjects(VoidOLList);
          ClearComboDiscounts;
          FDriveThroughGroupIDs.Clear;
        end;
      end;
      if (GlbTable.SaleCategoryType > scStandard) then
      begin
        FDriveThroughGroupIDs.Add(GlbTable.GroupID);
      end
      else
      begin
        FDriveThroughGroupIDs.Clear;
      end;

      if (GlbTable.GroupID > 0) then
      begin
        //Selected a table
        TableSelected(True);

        if ((GlbTable.SaleCategoryType = scDriveThru) or (GlbTable.SaleCategoryType = scRunningTab)) then
        begin
          //Load Orders here
          LoadDriveThroughOLs;
        end
        else // salecategory type = 0
        begin
          DeselectEvent;
        end;

        if ((GlbTable.SaleCategoryType = scStandard) and (LoyaltyReward.LoyaltyRewardID > 0)) then
        begin
          //If we are doing a drive through payment then we want to allow loyaltyrewards.
          LoyaltyReward.LoyaltyRewardID := 0;
          LoyaltyReward.RewardOffer := roNone;

          TestAllOLsLoyaltyItem(False);

          ReDisplayOLGrid;
        end;
        if (PrevGroupID <> GlbTable.GroupID) then
        begin
          DiscCalculator.ClearPrevTabItems;
          GlbAccount.Initialise;
          txtAccountName.Text := '';
          txtDiscountPercent.Text := '';
          txtDiscountAmount.Text := '';
          txtComboDisc.Text := '';
          LoadingAccount := True;
          try
            if GlbTable.AccountID > 0 then
            begin
              if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
              begin
                ShowMsg(sEventAccountWillBeLoaded, True);
                LoadAccount(GlbEvent.AccountID)
              end
              else if ((not AppDetails.EnableFiscalPrinting) or (not SFiscalPrinting.OpenedFiscalInvoice))  then
              begin
                GlbAccount.AccountID := GlbTable.AccountID;
                LoadAccount(GlbAccount.AccountID);
              end;
            end
            else if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
              LoadAccount(GlbEvent.AccountID)
            else if PrevAccountID > 0 then
              LoadAccount(PrevAccountID);

            if (GlbTable.SaleCategoryType > scStandard) {and ((not OnTable) or (GlbTable.Uninvoiced = 0))} then
              SetOnAccount(False);
          finally
            LoadingAccount := False;
          end;
        end;
        CalculateToPay; //AMANDA 18/10/00 - so that avail is recalculated
        txtTChange.Text := FormatCurrencyNoSign(FChange);
        txtCChange.Caption := FormatCurrencyNoSign(FChange);
        BroadcastCDAMessage;
      end
      else
      begin
        DiscCalculator.ClearPrevTabItems;
        RevertedSaleCategory := False; //AKM 4.3.1

        //Cleared a table
        if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
          LoadAccount(GlbEvent.AccountID)
        else if GlbAccount.AccountID > 0 then
          LoadAccount(GlbAccount.AccountID);

        if ((AppDetails.QSSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.QSSaleCategoryID))) then begin
          if ((AppDetails.CashSaleCategory = '') or (not SelectSaleCategory(AppDetails.CashSaleCategory))) then begin
            if ((AppDetails.DefaultSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.DefaultSaleCategoryID))) then begin
              //Well we didn't find one
            end;
          end;
        end;

        //only deselect if reverting to default SC above worked (ie no half way through an order and trying to change price levels)
        if (not RevertedSaleCategory) then begin    //AKM 4.3.1
          TableSelected(False);
          CalculateToPay; //AMANDA 19/10/00 - so that avail is cleared
        end;
      end;

      if (GlbTable.GroupID > 0) and (((GlbTable.SaleCategoryType = scDriveThru) and (GlbTable.Uninvoiced = 0)) or (GlbTable.SaleCategoryType <> scDriveThru)) then begin
//        Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
        Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
        cmdQuickCash.Tag := 0;

//        if (AppDetails.PMSType in [pmsFidelio, pmsPhoenix]) then begin
        if ((AppDetails.PMSType <> pmsNone) and (AppDetails.CashSaleAutoComplete)) then begin
          cmdToPayPMS.Visible := False;
          cmdQuickCash.Visible := True;
        end;
      end
      else begin
//        Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
        Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
        cmdQuickCash.Tag := 1;
//        if (AppDetails.PMSType in [pmsFidelio, pmsPhoenix]) then begin
        if ((AppDetails.PMSType <> pmsNone) and (AppDetails.CashSaleAutoComplete)) then begin
          cmdToPayPMS.Visible := True;
          cmdQuickCash.Visible := False;
        end;
      end;

      if (OLList.Count > 0) then begin    //Jon 08-10-2003
        ChangeCurrentOrdersPriceLevel(GlbTable.PriceLevel);
        CalculateToPay;
        if ((AppDetails.CashSaleAutoOrderTable) and ((OnTable) and (GlbTable.SaleCategoryType = scStandard))) then begin
          if ((FDoingRefund) or (DoingWaste)) then begin   //We don't ever want to do a refund onto a table this way
            DoCashSaleRefund(True);
          end
          else begin
            DoCashSale(True,3);
          end;
        end;
      end;
    end
    else if (TableFunction = tfPayment) then begin
      ReturnToCashSaleAfterPayment := True;
      cmdHideClick(nil);
      try
        if CheckTableLock(True, False) then
          PaymentTable;
      finally
        CheckTableLock(False, True);
      end;
    end
    else if ((TableFunction = tfNone) and (GlbTable.GroupID = 0)) then
      TableSelected(False);
  end
  else begin
    SetSaleCategory(PreviousSalesCategoryID);
    if (AppDetails.CashSaleStaffPromptTimeout = 0) and (OLList.Count = 0) then begin
      // Call the Logon Box!
      StaffPromptTimeoutTimerTimer(self);
    end;
    ResetTabDiscounts;

    if GlbTable.GroupID = 0 then
    begin
      TableSelected(False);
      if Assigned(DiscCalculator) then
        DiscCalculator.ClearPrevTabItems;
    end
    else if (PrevGroupID <> GlbTable.GroupID) then
    begin
      DiscCalculator.ClearPrevTabItems;
      GlbAccount.Initialise;
      txtAccountName.Text := '';
      txtDiscountPercent.Text := '';
      txtDiscountAmount.Text := '';
      txtComboDisc.Text := '';
      if GlbTable.AccountID > 0 then
      begin
        if ((not AppDetails.EnableFiscalPrinting) or (not SFiscalPrinting.OpenedFiscalInvoice))  then
        begin
          GlbAccount.AccountID := GlbTable.AccountID;
          LoadAccount(GlbAccount.AccountID);
        end;
        if (GlbTable.SaleCategoryType > scStandard){ and ((not OnTable) or (GlbTable.Uninvoiced = 0))} then
          SetOnAccount(False);
      end;
    end;

    CalculateToPay;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.TryLookupCashSaleAccount(AccountID: Integer; const SwipingCard: Boolean = True);
begin
  if (CanSelectAccount) then begin
    if (not dm.TestConnection) then begin
      Exit;
    end;

    ResetGlobalVariables;
    if (GlbAccount.LoadAccount(AccountID, GlbTable.SaleCategoryID, CurrentTime, CurrentDay)) then
    begin
      if (not sExternalAccountIntegration.AllowOnetapLoyaltyAccounts) then
      begin
        DeselectAccount;
        Exit;
      end;

      FCPFNumber := GlbAccount.PerorgTaxNumber;

      if (not ((OnTable) and (GlbTable.Destination <> '')) and
          (((not FDeliverySetByUser) and (FPrevAccountID <> AccountID)) or (FDelivery = '')) and
          (GlbAccount.AccountID <> 0)) then
      begin
        FDelivery := GlbAccount.PerorgFirstName;
      end;

      if FPrevAccountID <> AccountID then
        FPrevAccountID := AccountID;

      if (GlbAccount.AccountRank > GlbLogin.RankAccount) then begin
        ShowQuickMsg(sAccountRankNotAllowSelAccount);
        DeselectAccount;
        Exit;
      end;

      if ((AppDetails.AccountPinCheckRank <= GlbAccount.AccountRank) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
        DeselectAccount;
        Exit;
      end;

      if (GlbAccount.AccountID > 0) and (SwipingCard) then begin
        if (AppDetails.AccountDetailScan >= 0) then begin
          ShowAccountScanned(AppDetails.AccountDetailScan);
        end
        else begin
          ShowQuickMsg(Format(sCustAcctScanned, [GlbAccount.AccountName]));
        end;
      end;

      DisplayAccount;
    end
    else begin
      if (SwipingCard) then begin
        ShowQuickMsg(Format(sCustScctLookup, [GlbScan.ScannedCode]));
      end;
    end;
    BroadcastCDAMessage;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.LoadDriveThroughOLs;
var
  aOL: TOrderLine;
  ItemPOL: TOrderLine;

  procedure AddModifier;
  var
    lComboMod: TComboModifier;
  begin
    with dm.qrDriveThroughOLs do
    begin
      lComboMod := TComboModifier.Create;
      lComboMod.ModID := FieldByName('MODIFIERid').AsInteger;
      lComboMod.ModGroupID := FieldByName('MODgroupid').AsInteger;
      lComboMod.Modifier := FieldByName('MODIFIER').AsString;
      lComboMod.ModGroupOrder := FieldByName('modgrouporder').AsInteger;
      lComboMod.ModPrice := FieldByName('MODPRICE').AsCurrency;
      lComboMod.OriginalModPrice := aOL.OriginalModPrice;
      lComboMod.ModFunction := aOL.ModifierFunction;
      lComboMod.PrinterID := FieldByName('PRINTERID').AsInteger;
      lComboMod.RepeatPrinterID := FieldByName('REPEATPRINTERID').AsInteger;
      lComboMod.ModQty := FieldAsDouble(FieldByName('modqty'));
      lComboMod.ModQtyOption := FieldAsInt(FieldByName('modqtyoption'));
      lComboMod.ModQtyOrdered := FieldAsDouble(FieldByName('modqtyordered'));

      ItemPOL.ComboMods.Add(lComboMod);
    end;
  end;

  function OrderLineInList(aList: TList; aOrderLineID: Integer): Boolean;
  var
    lListOL: TOrderLine;
    I: Integer;

  begin
    for I := 0 to (aList.Count - 1) do
    begin
      lListOL := aList.Items[I];
      if (lListOL.OrderLineID = aOrderLineID) then
      begin
        Result := True;
        Exit;
      end;
    end;
    Result := False;
  end;

begin
  SetQueryDriveThroughOLs;
  with dm.qrDriveThroughOLs do begin
    ParamByName('groupid').AsInteger := GlbTable.GroupID;
    Open;
    while (not EOF) do begin
      if (OrderLineInList(OLList, FieldByName('orderlineid').AsInteger) or
          OrderLineInList(VoidOLList, FieldByName('orderlineid').AsInteger)) then
      begin
        //This Orderline is already in the list. Don't add it again.

        Next;
        while ((not EOF) and ((FieldByName('modifierid').IsNotNull) or (FieldByName('modifier').IsNotNull))) do
        begin
          Next;
        end
      end
      else
      begin
        aOL := TOrderLine.Create;
        with aOL do begin
          IsExistingOL := False;
          AllowEdit := FieldAsBool(FieldByName('allowedit'));
          OrderLineID := FieldByName('orderlineid').AsInteger;
          ItemID := FieldByName('itemid').AsInteger;
          Qty := FieldByName('qty').AsCurrency;
          ILQty := FieldByName('qtyinvoiced').AsCurrency;
          FORB := FieldByName('forb').AsString;
          PrinterID := FieldByName('printerid').AsInteger;
          RepeatPrinterID := FieldByName('repeatprinterid').AsInteger;
          UnitPrice := FieldByName('unitprice').AsCurrency;
          OriginalUnitPrice := UnitPrice;
          if (allowedit) then
          begin
            ToPay := UnitPrice * Qty;
          end
          else
          begin
            ToPay := UnitPrice;
          end;
          ToPay := GetRoundedUpDown(ToPay, AppDetails.DecimalPlaces);
          ILAmount := ToPay;
          DiscountedAmount := ToPay;

          PriceExcludesTax := FieldAsBool(FieldByName('priceexcludestax'));
          SalesTaxPercent := FieldByName('salestaxpercent').AsFloat;
          ItemGrpType := TItemGrpType(FieldAsInt(FieldByName('ItemGrpType')));
          OpenPrice := FieldAsBool(FieldByName('openprice'));

          if (OpenPrice) then begin
            OpenPriceMin := FieldByName('openpricemin').AsCurrency;
            OpenPriceMax := FieldByName('openpricemax').AsCurrency;
          end
          else begin
            OpenPriceMin := 0.00;
            OpenPriceMax := 0.00;
          end;
          WeighedItem := FieldAsBool(FieldByName('weigheditem'));
          ItemString := FieldAsString(FieldByName('itemabbrev'));
          OLPriceLevel := FieldAsInt(FieldByName('pricelevel'));
          CourseID := FieldAsInt(FieldByName('courseid'));
          if (CourseID > 0) then begin
            CourseIsMain := FieldAsBool(FieldByName('courseismain'));
          end
          else begin
            CourseIsMain := False;
          end;
          AlreadyOrdered := True;         //Jon 10-03-2004
          AlreadyOrderedQty := Qty;

          FixThisComboPart := FieldAsBool(FieldByName('FIXTHECOMBO'));
          FreeComboItem := FieldAsBool(FieldByName('FREECOMBOITEM'));
          LastComboItem := False;
          LastNormalItem := False;

          ComboID := FieldAsInt(FieldByName('comboid'));
          ComboGroupID := FieldAsInt(FieldByName('combogroup'));
          ComboIndex := FieldAsInt(FieldByName('comboindex'));
          if (FieldByName('comboqty').IsNotNull) then
          begin
            ComboQty := FieldByName('comboqty').AsDouble;
            ComboTempQty := FieldByName('comboqty').AsDouble;
            FreeComboQty := 0;
            if FreeComboItem then
              FreeComboQty := ComboQty;
          end;

          if ((ComboID > 0) and (ComboGroupID > 0) and (ComboIndex > 0)) then
          begin
            AllowedDiscount := FieldByName('OLDISCOUNT').AsCurrency;
            ComboDiscount := FieldByName('OLDISCOUNT').AsCurrency;
          end;

          LoyaltyPoints1RatioNum := FindFieldAsDouble(FindField('loyaltypoints1rationum'));
          LoyaltyPoints1RatioDen := FindFieldAsDouble(FindField('loyaltypoints1ratioden'), 1);
          LoyaltyPoints1Qty := FindFieldAsDouble(FindField('loyaltypoints1qty'));
          LoyaltyPoints2RatioNum := FindFieldAsDouble(FindField('loyaltypoints2rationum'));
          LoyaltyPoints2RatioDen := FindFieldAsDouble(FindField('loyaltypoints2ratioden'), 1);
          LoyaltyPoints2Qty := FindFieldAsDouble(FindField('loyaltypoints2qty'));

          if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem)) then begin
            LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, ItemID);
            AllocateItemLoyalty(aOL);
          end;

          GLCode := FindFieldAsString(FindField('glcode'));
          GLCode2 := FindFieldAsString(FindField('glcode2'));

          TaxSituation := FieldByName('TaxSituation').AsString;
          CSOSN := FieldByName('CSOSN').AsString;
          CSTICMS := FieldByName('CST_ICMS').AsString;
          CFOP := FieldByName('CFOP').AsInteger;
          GTIN := FieldByName('gtin').AsString;
          NCMProductCode := FieldByName('NCMProductCode').AsString;
          CST_PIS := FieldByName('cst_pis').AsString;
          CST_COFFINS := FieldByName('cst_coffins').AsString;
          CEST := FieldAsInt(FieldByName('cest'));
          ProductOrigin := 0;
          if Length(CSTICMS) >= 3 then
          begin
            ProductOrigin := StrToIntDef(Copy(CSTICMS, 1, 1), 0);
            CSTICMS := Copy(CSTICMS, 2, Length(CSTICMS) - 1);
          end;
        end;

        OLList.Add(aOL);
        ItemPOL := aOL;

        Next;

        while ((not EOF) and ((FieldByName('modifierid').IsNotNull) or (FieldByName('modifier').IsNotNull))) do begin
          //Load Modifier OLs
          aOL := TOrderLine.Create;
          with aOL do begin
            AlreadyOrdered := True;        //Jon 10-03-2004

            PrinterID := FieldAsInt(FieldByName('printerid'));
            if (PrinterID > 0) then begin
              RepeatPrinterID := FieldAsInt(FieldByName('repeatprinterid'));
              ItemPOL.ModsChangePrinter := True;
            end
            else begin
              RepeatPrinterID := 0;
            end;

            ModifierID := FieldAsInt(FieldByName('modifierid'));
            ModGroupID := FieldAsInt(FieldByName('modgroupid'));
            ModGroupOrder := FieldAsInt(FieldByName('modgrouporder'));
            ModPrice := FieldByName('modprice').AsCurrency;
            OriginalModPrice := FieldByName('originalmodprice').AsCurrency;
            ModifierFunction := FieldAsInt(FieldByName('modfunction'));
            ModQty := FieldAsDouble(FieldByName('modqty'));
            ModQtyOption := FieldAsInt(FieldByName('modqtyoption'));
            ModQtyOrdered := FieldAsDouble(FieldByName('modqtyordered'));
            ItemString := FieldByName('Modifier').AsString;

            ComboID := FieldAsInt(FieldByName('comboid'));
            ComboGroupID := FieldAsInt(FieldByName('combogroup'));
            ComboIndex := FieldAsInt(FieldByName('comboindex'));
            if FieldByName('comboqty').IsNotNull then
            begin
              ComboQty := FieldByName('comboqty').AsCurrency;
              ComboTempQty := FieldByName('comboqty').AsCurrency;
              FreeComboQty := 0;
              if FreeComboItem then
                FreeComboQty := ComboQty;
            end;
            AddModifier;

            ItemPOL.ModPrice := ItemPOL.ModPrice + (ModPrice * (ModQtyOrdered - ModQtyIncluded));
          end;

          OLList.Add(aOL);

          Next;
        end;
      end;
    end;
    LoadOLGrid;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ClearDriveThroughOLs;
var
  I: Integer;
  aOL: TOrderLine;

begin
  for I := OLList.Count-1 downto 0 do
  begin
    aOL := OLList.Items[I];
    if (aOL.AlreadyOrdered)  then
    begin
      if aOL.ItemID > 0 then
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;

      aOL.Free;
      OLList.Delete(I);
    end;
  end;
  ReDisplayOLGrid;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DeselectAccount;
begin
  GlbAccount.Initialise;
  lblAllowItemLevelDiscount.Caption := '';
  FPrevAccountID := 0;
  SetOnAccount(False);              //Jon 25-03-2005
  ResetGlobalVariables;

  CheckLoyaltyReward(True);
  CalculateToPay;   //Jon 30-5-2001
  ChangeAllowCreditLabel(False);    //Jon 16-10-2003
  RemoveSwipeCardReplacement;
  BroadcastCDAMessage;
end;
{******************************************************************************}
Procedure TformQuickSaleUser.InitialiseAccountInfo;
begin
  if (GlbEvent.EventID = 0) then
    DeselectAccount;
  //GlbLogin.LoginPIN := 0;
end;
{******************************************************************************}
procedure TformQuickSaleUser.BuildAccountButtons;
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
      if (not FieldAsBool(FieldByName('accinactive'))) then
      begin
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
function TformQuickSaleUser.CheckLoyaltyReward(Prompt: Boolean): Boolean;
var
  OldLoyaltyRewardOffer: TRewardOffer;
begin
  Result:= True;
  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then // no loyalty reward for events
    Exit;

  //if IsNormalLoyalty then//checkzapa
  begin
    OldLoyaltyRewardOffer := LoyaltyReward.RewardOffer;

    if (Prompt) then
    begin
      LoyaltyReward.LoyaltyRewardID := 0;
      LoyaltyReward.RewardOffer := roNone;
    end;

    if ((not Prompt) or
      (((GlbAccount.AccountID > 0) and (GlbAccount.BestLoyaltyRewardID > 0) and (((GlbTable.SaleCategoryType <> scStandard) and (AppDetails.OrderDiscountEnable)) or (not OnTable))) and
       (ShowConf(sReviewLoyaltyRewardOffers) = mrYes))) then
    begin
      //if previous loyalty was item level loyalty, clear the item level loyalty details before go to another
      if (OldLoyaltyRewardOffer = roItem) then
        ClearItemLevelLoyalty;

      if (SelectLoyaltyReward(LoyaltyReward, [roVoucher, roCredit, roDiscount, roItem, roVariableDiscount])) then
      begin
        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roCredit)) then
        begin
          GlbAccount.CurrentBalance := GetRoundedUpDown(GlbAccount.CurrentBalance - LoyaltyReward.Qty, AppDetails.DecimalPlaces);
          FCurrentBalance := GlbAccount.CurrentBalance;
        end
      end
      else
        Result:= False;
    end;
    if (OldLoyaltyRewardOffer = roItem) then
      ClearItemLevelLoyalty;
    if ((OldLoyaltyRewardOffer = roItem) or (LoyaltyReward.RewardOffer = roItem)) then
    begin
      TestAllOLsLoyaltyItem(False);
      ReDisplayOLGrid;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AccountClick(Sender: TObject);
begin
  StopInactiveTimer;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    StartInactiveTimer;
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    StartInactiveTimer;
    Exit;
  end;

  if ((AppDetails.EnableFiscalPrinting) and
      (SFiscalPrinting.OpenedFiscalInvoice) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0)))) then
  begin
    StartInactiveTimer;
    Exit;
  end;
  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  if (CanSelectAccount) then begin
    with dm.qrAccountButtons do begin
      KeyFieldByName('accountid').AsInteger := TfcCustomImageBtn(Sender).Tag;
      if (not LookupKeyForFields) then
      begin
        ShowQuickMsg(sNoCustAcctSelectAcctOpt);
        StartInactiveTimer;
        Exit;
      end
      else begin
        GlbAccount.LookupAccountID := FieldByName('accountid').AsInteger;
        LoadAccount(GlbAccount.LookupAccountID);
      end;
    end;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDeselectAccountClick(Sender: TObject);
begin
  StopInactiveTimer;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    StartInactiveTimer;
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    StartInactiveTimer;
    Exit;
  end;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    StartInactiveTimer;
    Exit;
  end;
  if ((GlbAccount.AccountID > 0) and (OLList.Count <= 0) and (TLList.Count > 0) and (AppDetails.CashSaleAllowAccPayment)) then begin
    ShowQuickMsg(sNoSelectAcctWhilePay);
  end;

  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  DeselectAccount;
  {ConfirmedAccountDiscountSelection := False;
  ConfirmComboDiscountSelection := False;}
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdUpAccountsClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  try
    if (scrAccounts.VertScrollBar.Position - scrAccounts.Height) > 0 then
      scrAccounts.VertScrollBar.Position :=
        scrAccounts.VertScrollBar.Position - scrAccounts.Height
    else
      scrAccounts.VertScrollBar.Position := 0;
  except; end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDownAccountsClick(Sender: TObject);
begin
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  try
    if ((scrAccounts.VertScrollBar.Position + scrAccounts.Height) < scrAccounts.VertScrollBar.Range) then begin
      scrAccounts.VertScrollBar.Position := scrAccounts.VertScrollBar.Position + scrAccounts.Height;
    end
    else begin
      scrAccounts.VertScrollBar.Position := scrAccounts.VertScrollBar.Range;
    end;
  except;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
function TformQuickSaleUser.CanSelectAccount: Boolean;
begin
  Result := False;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  //check if have privilege
  //  if (not VerifyPriv(11)) then
  if (GlbLogin.RankAccount = 0) then
  begin
    ShowMsg(sGetHelpToPutInvThruCustAcct);
    Exit;
  end;

  if ((AppDetails.EnableFiscalPrinting) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0)))) then
  begin
    cmdParkedSale.Enabled := not SFiscalPrinting.OpenedFiscalInvoice;
    cmdTable.Enabled := not SFiscalPrinting.OpenedFiscalInvoice;
    cmdTables.Enabled := not SFiscalPrinting.OpenedFiscalInvoice;
    cmdPhoneOrder.Enabled := (not SFiscalPrinting.OpenedFiscalInvoice);
    if SFiscalPrinting.OpenedFiscalInvoice then
    begin
      ShowMsg(sFiscalInvoiceAlreadyOpened);
      Exit;
    end;
  end;

  //if AppDetails.wiGroupEnabled then
  //  wiGroupPOSSrv.CheckWarnAccDiscNotAllowed;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformOTAcctNotAllowed(True, True) then
    Exit;

  Result := True;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdSelectAccountClick(Sender: TObject);
begin
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

  if (DoingTransaction) then
    Exit;

  StopInactiveTimer;
  PauseStaffPromptTimeoutTimer;
  //fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and
      (not (NFManual or NFEMode)) and
      (Trim(AppDetails.FiscalAccountPaymentMethod) = '') and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0)))) then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    StartInactiveTimer;
    Exit;
  end;

  if ((CanSelectAccount) and (SelectAccount(True, True))) then
  begin
    if (GlbAccount.AccountID > 0) then begin
      LoadAccount(GlbAccount.AccountID);
    end
    else begin
      DeselectAccount;
    end;
  end;
  StartInactiveTimer;
  ResetStaffPromptTimeoutTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDiscountPercentClick(Sender: TObject);
begin
  StopInactiveTimer;

  //if in a Goody discounted sale, don't allow discount to be changed
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sCannotChangeGoodyDiscAmt);
    StartInactiveTimer;
    Exit;
  end;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    StartInactiveTimer;
    Exit;
  end;

  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (GlbAccount.AccountID = 0) then begin
    ShowQuickMsg(sSelectCustAcct);
    StartInactiveTimer;
    Exit;
  end;
  if ((not AppDetails.OrderDiscountEnable) and ((GlbTable.GroupID > 0) or (DoingPhoneOrder))) then
  begin
    ShowQuickMsg(sDiscountOnOrderIsNotEnabled);
    StartInactiveTimer;
    Exit;
  end;
  if ((GlbPrivs[13].AvoidPINCheck) or (GlbLogin.LoginPIN <> 0) or (VerifyPinNo)) then
  begin
    if GetDiscountPercent(FDiscountPercent) then begin
      FOrigDiscountPercent := FDiscountPercent;    //Jon 17-12-2002
      DiscCalculator.OrigDiscountPercent := FOrigDiscountPercent;
      FUseItemDiscounts := False;
      FUseDiscountAmount := False;
      FUseDiscountPeriods := False;
      lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;
      CalculateToPay;       //Jon 30-5-2001
    end;
  end
  else
  begin
    StartInactiveTimer;
    Exit;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdDiscountAmountClick(Sender: TObject);
begin
  StopInactiveTimer;

  //if in a Goody discounted sale, don't allow discount to be changed
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sCannotChangeGoodyDiscAmt);
    StartInactiveTimer;
    Exit;
  end;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    StartInactiveTimer;
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (GlbAccount.AccountID = 0) then begin
    ShowQuickMsg(sSelectCustAcct);
    StartInactiveTimer;
    Exit;
  end;

  if DoingPhoneOrder then
  begin
    ShowQuickMsg(sNoDiscountChangeForPhoneOrders);
    StartInactiveTimer;
    Exit;
  end;

  if ((not AppDetails.OrderDiscountEnable) and ((GlbTable.GroupID > 0) or (DoingPhoneOrder))) then
  begin
    ShowQuickMsg(sDiscountOnOrderIsNotEnabled);
    StartInactiveTimer;
    Exit;
  end;

  if ((GlbPrivs[13].AvoidPINCheck) or (GlbLogin.LoginPIN <> 0) or (VerifyPinNo)) then begin
    if GetDiscountAmount(FToPay, FDiscountAmount) then
    begin
      FOrigDiscountAmount := FDiscountAmount;
      DiscCalculator.OrigDiscountAmount := FOrigDiscountAmount;
      FUseDiscountAmount := True;
      FUseItemDiscounts := False;   //Jon 13-07-2004
      FUseDiscountPeriods := False;
      lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;

      CalculateToPay;       //Jon 30-5-2001
    end;
  end
  else begin
    StartInactiveTimer;
    Exit;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdAccountNoLookupClick(Sender: TObject);
begin
  if not sExternalAccountIntegration.AllowOnetapAccounts then
    Exit;

  if (DoingTransaction) then
    Exit;

  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0)) then
  begin
    ShowQuickMsg(sNoAccountChangeForEvents);
    StartInactiveTimer;
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    StartInactiveTimer;
    Exit;
  end;

  //fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0))) and
      (Trim(AppDetails.FiscalAccountPaymentMethod) = '')) then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    StartInactiveTimer;
    Exit;
  end;

  if ((CanSelectAccount) and (AccountNoLookup) and (LoadAccount(GlbAccount.LookupAccountID)) and (AppDetails.AccountDetailScan >= 0)) then begin
    ShowAccountScanned(AppDetails.AccountDetailScan);
  end;

  StartInactiveTimer;
end;
{******************************************************************************}
function TformQuickSaleUser.LoadAccount(AccountID: Integer; ForGoodyDiscount: Boolean = False): Boolean;
begin
  Result := False;

  //Check goody discount account is only used if a goody discount has just been scanned
  if (AccountID = AppDetails.GoodyDiscountAccountID) and (not ForGoodyDiscount) then
  begin
    //NB: Goody discount account may be partially selected, e.g., cmdSelectAccountClick > SelectAccount sets GlbAccount.AccountID (which don't want)
    //As we don't know the previously selected account, we deselect the account to be sure the Goody discount account isn't partially selected
    DeselectAccount;

    ShowMsg(sGoodyDiscAcctExclusiveToGoodyDiscSale);
    Exit;
  end;

  //if in a Goody discounted sale, disallow loading a different account
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoAcctSelectionForGoodyDiscSale);
    Exit;
  end;

  with dm.sp do begin
    if (not dm.TestConnection) then begin
      Exit;
    end;

    SetStoredProcName('GET_ACCOUNT');
    ParamByName('paccountid').AsInteger := AccountID;
    ParamByName('salecategoryid').AsInteger := GlbTable.SaleCategoryID;
    ParamByName('currenttime').AsInteger := CurrentTime;
    ParamByName('currentday').AsInteger := CurrentDay;
    ExecProc;

    ResetGlobalVariables;

    if (FieldByName('accountid').IsNull) then begin
      ShowQuickMsg(sNoCustAcctSelectAcctOpt);
      Exit;
    end
    else if not sExternalAccountIntegration.AllowOnetapAccounts then
    begin
      Exit;
    end
    else begin
      if (FieldByName('acctyperank').AsInteger > GlbLogin.RankAccount) then begin
        ShowQuickMsg(sAccountRankNotAllowSelAccount);
        DeselectAccount;
        Exit;
      end;

      if ((AppDetails.AccountPinCheckRank <= FieldByName('acctyperank').AsInteger) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then begin
        DeselectAccount;
        Exit;
      end;

      if Assigned(DiscCalculator) then
        DiscCalculator.ClearItemLevelDiscounts;

      GlbAccount.SetGlbAccount(dm.sp);

      if (not sExternalAccountIntegration.AllowOnetapLoyaltyAccounts) then
      begin
        DeselectAccount;
        Exit;
      end;

      FCPFNumber := GlbAccount.PerorgTaxNumber;

      if (not ((OnTable) and (GlbTable.Destination <> '')) and
          (((not FDeliverySetByUser) and (FPrevAccountID <> AccountID)) or (FDelivery = '')) and
          (GlbAccount.AccountID <> 0)) then
      begin
        FDelivery := GlbAccount.PerorgFirstName;
      end;

      if FPrevAccountID <> AccountID then
        FPrevAccountID := AccountID;

      DisplayAccount;
      BroadcastCDAMessage;
      Result := True;
    end;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.QSCreateAccount(AccountTypeID: Integer): Boolean;
var
  SafeGlbAccount: TGlbAccount;
  SafePeopleAccountCreation: Integer;
  SafeScrollableAdmin: Boolean;
  newAccountID: Integer;
begin
  Result := True;
  StopInactiveTimer;

  if (DoingTransaction) then begin
    StartInactiveTimer;
    Result := False;
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;

  if (not VerifyPriv(10)) then
    ShowMsg(sCannotUseThisBtn);
  if ((not VerifyPriv(10) or (not GlbPrivs[10].AvoidPINCheck) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo))) then begin
    StartInactiveTimer;
    Result := False;
    Exit;
  end;

  if ((AccountTypeID > 0)) then begin
    if (WBAccountTypes.QueryType <> qtAccountTypesCurrent) then begin
      FilterQuery(dm.qrAccountTypes, qtAccountTypesCurrent);
    end;
    if (not dm.qrAccountTypes.Active) then
      dm.qrAccountTypes.Open;

    dm.qrAccountTypes.Filter := '((accounttypeid = ''' + IntToStr(AccountTypeID) + ''')' +
                                'and (acctypeaddaccountrank <= ''' + IntToStr(GlbLogin.RankAccount) + '''))';
    dm.qrAccountTypes.Filtered := True;

    if (dm.qrAccountTypes.Eof) then begin
      dm.qrAccountTypes.Filter := '';
      dm.qrAccountTypes.Filtered := False;
      ShowMsg(sAccountRankNotAllowAddAccount);
      StartInactiveTimer;
      Result := False;
      Exit;
    end;

    dm.qrAccountTypes.Filter := '';
    dm.qrAccountTypes.Filtered := False;
  end;

  if (not TestRemoteLocationOption(rlsoRemote)) or
     ((TestRemoteLocationOption(rlsoRemote)) and HeadOfficeOnline) then begin
    DeselectAccount;
    SafeGlbAccount := TGlbAccount.Create;
    SafeGlbAccount.Copy(GlbAccount);
    SafePeopleAccountCreation := AppDetails.PeopleAccountCreation;
    SafeScrollableAdmin := AppDetails.ScrollableAdmin;
    try
      if (AccountTypeID = 0) then begin
        GlbAccount.PerorgType := 'P';
        ShowDetail(adNewAccount);
      end
      else begin
        GlbAccount.PersonCreateAccount := True;
        GlbPerOrg.PerorgID := 0;
        GlbPerOrg.MasPerorgID := 0;
        GlbAccount.LastFilteredAccountTypeID := AccountTypeID;
        AppDetails.PeopleAccountCreation := 2;
        AppDetails.ScrollableAdmin := True;

        ShowDetail(adQSPersonDetail);

        GlbAccount.PersonCreateAccount := False;
      end;
    finally
      newAccountID := GlbAccount.AccountID;
      GlbAccount.Copy(SafeGlbAccount);
      SafeGlbAccount.Free;
      AppDetails.PeopleAccountCreation := SafePeopleAccountCreation;
      AppDetails.ScrollableAdmin := SafeScrollableAdmin;
    end;
    if (newAccountID > 0) then
      if (ShowConf(sSelectAccountAfterQSCreate) = mrYes) then begin
        LoadAccount(newAccountID);
      end;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DisplayAccount;
begin
  if Assigned(DiscCalculator) then
    DiscCalculator.ClearItemLevelDiscounts;
  ClearItemLevelDiscounts(OLList);

  If (GlbAccount.AccountInactive = accInactive) then
  begin
    ShowMsg(sAccountInactive);
    DeselectAccount;
    Exit;
  end;

  txtAccountName.Text := GlbAccount.AccountName;

  //Only if account is active, so dont discount or show loyalty reward options
  if (GlbAccount.AccountInactive = accActive) then
  begin
    FDiscountPercent := GlbAccount.AccountDiscount;
    FLimitMaximum := GlbAccount.LimitMaximum;                          //Jon 13-12-2002
    FMaxDiscount := GlbAccount.MaxDiscount;                          //Jon 13-12-2002
    FOrigDiscountPercent := GlbAccount.AccountDiscount;                     //Jon 17-12-2002
    FCurrentBalance := GlbAccount.CurrentBalance;                    //Jon 17-10-2003
    FUseDiscountAmount := False;                                       //Jon 14-07-2004
    FUseItemDiscounts := (GlbAccount.UseItemDiscounts) or ((AppDetails.EnableFiscalPrinting) and (AppDetails.FiscalLiveInvoice) and (FOrigDiscountPercent = 100));                //Jon 13-07-2004
    FUseDiscountPeriods := GlbAccount.UseTimePeriods;

    if ((GlbTable.GroupID > 0) and (AppDetails.OrderDiscountEnable)) then
    begin
      if GlbTable.DiscountRate > 0 then
      begin
        FOrigDiscountPercent := GlbTable.DiscountRate;//FDiscountPercent;    //Jon 17-12-2002
        FUseItemDiscounts := False;
        FUseDiscountAmount := False;
        FDiscountPercent := GlbTable.DiscountRate;
      end;
      {else if GlbTable.TabDiscountAmt > 0 then
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
      end;}
    end;
  end;

  if ((GlbAccount.AccountInactive <> accActive) or ((GlbTable.GroupID > 0) and (not AppDetails.OrderDiscountEnable))) then
  begin
    FDiscountPercent := 0;
    FDiscountAmount := 0;
    FOrigDiscountPercent := 0;
    FCurrentBalance := GlbAccount.CurrentBalance;
    FUseDiscountAmount := False;
    FUseItemDiscounts := False;
  end;
  lblAllowItemLevelDiscount.Caption := '';
  if FUseItemDiscounts then
    lblAllowItemLevelDiscount.Caption := sAllowedItemLevelDiscount;

  if (AppDetails.PaymentLoyaltyRewardPrompt and (GlbAccount.AccountInactive = accActive) and (not DoingPhoneOrder)) then
    CheckLoyaltyReward(True);                                     //Jon 21-12-2004

  ChangeAllowCreditLabel(GlbAccount.AllowCredit and (GlbAccount.AccountInactive = accActive));             //do before CalculateDiscounts, so OnAccount is set
  CalculateDiscountSchemeItemDiscount(True);  //Again we using this to force a full update...yes it run again under CaltoPay
  CalculateDiscount;// show discounts
  LoadTLs(LANone); //ensure FStillDue/FRequired accounts for current tenders

  //if Goody discount account, don't allow discount to be changed
  if GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID then
  begin
    cmdDiscountPercent.Enabled := False;
    cmdDiscountAmount.Enabled := False;
  end
  else if ((VerifyPriv(13))) then
  begin
    cmdDiscountPercent.Enabled := (not AppDetails.EnableFiscalPrinting) or (OLList.Count <= 0);
    cmdDiscountAmount.Enabled := (not AppDetails.EnableFiscalPrinting) or (OLList.Count <= 0); //(not DoingPhoneOrder);
  end
  else
  begin
    cmdDiscountPercent.Enabled := False;
    cmdDiscountAmount.Enabled := False;
  end;

  if ((AppDetails.PDEnabled) and (GlbAccount.PoleDisplayShow)) then
  begin
    DMComponents.pdOutputLine(GlbAccount.AccountType + ' SCANNED', 0, False);
    if (GlbAccount.AllowCredit and (GlbAccount.AccountInactive = accActive)) then begin
      DMComponents.pdOutputCreditDebitLine('BALANCE', RoundCurrency(- GlbAccount.CurrentBalance, 2), 1, False);
    end
    else begin
      DMComponents.pdOutputLine(' ', 1, False);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdAllGroupsClick(Sender: TObject);
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (cmdAllGroups.Down) then begin
    scrF.Height := pnlFood.Height;
    scrB.Height := pnlBev.Height;
  end
  else begin
    if (pnlFood.Height > (2 * 48)) then begin
      scrF.Height := (2 * 48); //+ 4;
    end
    else begin
      scrF.Height := pnlFood.Height;
    end;
    if (pnlBev.Height > (2 * 48)) then begin
      scrB.Height := (2 * 48); //+ 4;
    end
    else begin
      scrB.Height := pnlBev.Height;
    end;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddMods;
var
  OL: TOrderLine;
  VoidOL: TOrderLine;
  ModOL: TOrderLine;
  aMod: TMod;
  I: Integer;
  TheRow: Integer;
  aSymbol: string;
  tmpMod: TComboModifier;
begin
  if not Assigned(Mods) then
    Exit;

  if (Mods.Count = 0) then
    Exit;

  //assume that ol was selected before this 'AddMods' proc
  //row changes as each mod added, so need a constant value to calculate ol price
  TheRow := grdOLs.Row;
  OL := OLList.Items[TheRow];
  {unsure on this but maybe GlbTable.SaleCategoryType should be >= 1????}
  if ((GlbTable.SaleCategoryType >= scDriveThru) and (OnTable) and (OL.AlreadyOrdered)) then
  begin
    VoidOL := TOrderLine.Create;
    VoidOL.CopyOL(OL);
    VoidOLList.Add(VoidOL);
    OL.AlreadyOrdered := False;
    OL.AlreadyOrderedQty := 0;
    OL.OrderLineID := 0;
  end;

  {if (AppDetails.ZeroPriceItemsStayZero and
      ((OL.UnitPrice = 0) or (OL.LoyaltyFreeQty = OL.Qty))) then begin}
  if ((AppDetails.ZeroPriceItemsStayZero) and (OL.UnitPrice = 0)) then
  begin
    ZeroModPricing;
  end
  else
  begin
    IncludedModsFree;
    case AppDetails.SmartModPricing of
      1: SmartModPricing1;
      2: SmartModPricing2;
      3: SmartModPricing3;
    end;
  end;

  OL.ModsChangePrinter := False;
  OL.Qty := OL.OriginalQty;
  for I := 0 to (Mods.Count - 1) do
  begin
    aMod := Mods.Items[I];

    GlbOLMod.ModifierID := aMod.ModID;
    GlbOLMod.Modifier := aMod.Modifier;
    GlbOLMod.ModGroupID := aMod.ModGroupID;
    GlbOLMod.ModGroupOrder := aMod.ModGroupOrder;
    GlbOLMod.ModPrice := aMod.ModPrice;

    GlbOLMod.OriginalModPrice := aMod.OriginalModPrice;     //Jon 10-9-2001
    GlbOLMod.ModifierFunction := aMod.ModFunction;
    GlbOLMod.PrinterID := aMod.PrinterID;     //Jon 10-9-2001
    GlbOLMod.RepeatPrinterID := aMod.RepeatPrinterID;
    GlbOLMod.ModQty := aMod.ModQty;
    GlbOLMod.ModQtyOption := aMod.ModQtyOption;
    GlbOLMod.ModQtyOrdered := aMod.ModQtyOrdered;
    GlbOLMod.ModQtyIncluded := aMod.ModQtyIncluded;

    if (GlbOLMod.PrinterID > 0) then begin
      OL.ModsChangePrinter := True;
    end;

    with grdOLs do
    begin
      //set the mod's ol's unit price and price using TheRow constant
      // JEH 18/08/2001 1 -> 0
      //RPC set this modprice for Item (aka totalmodprice)
      if ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice)) and ((OL.WeighedItem) and (GlbOLMod.ModQtyOption = 1)) then
      begin   //Change the ol item qty only for weighted items
        OL.Qty := OL.Qty + GlbOLMod.ModQty;
        if (OL.Qty < 0) then OL.Qty := 0;
        Cells[1, TheRow] := FormatFloat(QTYFORMAT, OL.Qty);
        OL.ModPrice := OL.ModPrice + GlbOLMod.ModPrice;
        OL.UnitPrice := OL.UnitPrice + GlbOLMod.ModPrice;
      end
      else if ((not AppDetails.EnableFiscalPrinting) or (not AppDetails.FiscalLiveInvoice)) and (GlbOLMod.ModQtyOption = 0) then
      begin
        OL.ModPrice := OL.ModPrice + (GlbOLMod.ModPrice * (GlbOLMod.ModQtyOrdered - GlbOLMod.ModQtyIncluded));
        OL.UnitPrice := OL.UnitPrice + (GlbOLMod.ModPrice * (GlbOLMod.ModQtyOrdered - GlbOLMod.ModQtyIncluded));
      end
      else begin
        OL.ModPrice := OL.ModPrice + GlbOLMod.ModPrice;
        OL.UnitPrice := OL.UnitPrice + GlbOLMod.ModPrice;
      end;
      OL.ToPay := OL.UnitPrice * OL.Qty;
      if (OL.LoyaltyFreeQty > 0) then begin
        OL.LoyaltyFreeDiscount := OL.LoyaltyFreeQty * OL.UnitPrice;
      end;

      UpdateOLGridRow(TheRow, OL);

      ModOL := TOrderLine.Create;
      ModOL.IsExistingOL := False;
      ModOL.OrderLineOrder := OLList.Count + 1;
      ModOL.WhenOrdered := Now;
      //if not last row
      if (Row <> (RowCount - 1)) then begin
        //move down
        Row := Row + 1;

        //while not an ol or last line move down
        while (((TOrderLine(OLList[Row]).ItemID = 0) and (TOrderLine(OLList[Row]).ModifierID <> 0)) and (Row <> RowCount-1)) do begin
          Row := (Row + 1);
        end;

        //if on last row and it's a mod then add a line
        if ((Row = (RowCount - 1)) and (TOrderLine(OLList[Row]).ItemID = 0)) then begin
          OLList.Add(ModOL);
          AddLine;
        //else insert a line
        end
        else begin
          OLList.Insert(Row, ModOL);
          InsertLine;
        end;
      //else if on last row to start with
      end
      else begin
        OLList.Add(ModOL);
        AddLine;
      end;

      //set mod details in list
      with (ModOL) do
      begin
        AllowEdit := True;
        OrderLineOrder := OLList.Count+1;
        WhenOrdered := Now;
        //PrinterID := 0;
        //RepeatPrinterID := 0;
        PrinterID := GlbOLMod.PrinterID;
        RepeatPrinterID := GlbOLMod.RepeatPrinterID;
        ModifierID := GlbOLMod.ModifierID;
        ModGroupID := GlbOLMod.ModGroupID;
        ModGroupOrder := GlbOLMod.ModGroupOrder;
        ModPrice := GlbOLMod.ModPrice;
        OriginalModPrice := GlbOLMod.OriginalModPrice;    //Jon 10-9-2001
        ModQty := GlbOLMod.ModQty;
        ModQtyOption := GlbOLMod.ModQtyOption;
        ModQtyOrdered := GlbOLMod.ModQtyOrdered;
        ModQtyIncluded := GlbOLMod.ModQtyIncluded;
        ModifierFunction := GlbOLMod.ModifierFunction;
        ItemString := GlbOLMod.Modifier;
        ComboID := OL.ComboID;
      end;
      UpdateOLGridRow(Row, ModOL);
    end;
  end;

  OL.AllModDesc := '';
  OL.ComboMods.Clear;
  for I := 0 to (Mods.Count - 1) do
  begin
    aMod := Mods.Items[I];
    aMod.OriginalModPrice := aMod.ModPrice;

    aSymbol := '';
    if aMod.ModFunction = 1 then
      aSymbol := ''
    else if aMod.ModFunction = 2 then
      aSymbol := '(-)'
    else if aMod.ModFunction = 3 then
      aSymbol := '(EO)';

    if Trim(OL.AllModDesc) = '' then
      OL.AllModDesc := Trim(aSymbol) + Trim(aMod.Modifier)
    else
      OL.AllModDesc := OL.AllModDesc +','+ Trim(aSymbol) + Trim(aMod.Modifier);

    tmpMod := TComboModifier.Create;
    tmpMod.ModID := aMod.ModID;
    tmpMod.ModGroupID := aMod.ModGroupID;
    tmpMod.Modifier := aMod.Modifier;
    tmpMod.ModGroupOrder := aMod.ModGroupOrder;
    tmpMod.ModPrice := aMod.ModPrice;
    tmpMod.OriginalModPrice := aMod.OriginalModPrice;
    tmpMod.ModFunction := aMod.ModFunction;
    tmpMod.PrinterID := aMod.PrinterID;
    tmpMod.RepeatPrinterID := aMod.RepeatPrinterID;
    tmpMod.ModQty := aMod.ModQty;
    tmpMod.ModQtyOption := aMod.ModQtyOption;
    tmpMod.ModQtyOrdered := aMod.ModQtyOrdered;
    tmpMod.ModQtyIncluded := aMod.ModQtyIncluded;
    OL.ComboMods.Add(tmpMod);
  end;

end;
{******************************************************************************}
{******************************************************************************}
//Only Void if an OL - cannot void a mod like this, have to go into mod form
{******************************************************************************}
procedure TformQuickSaleUser.VoidLine;
var
  FromRow, ToRow: Integer;
  RowsToDelete: Integer;
  I: Integer;
  OL: TOrderLine;
  aOL: TOrderLine;
  SPNumber: Integer;
begin
  with grdOLs do
  begin
    //Exit if nothing to void
    if (OLList.Count = 0) then begin
      Exit;
    end;

    if (AppDetails.OrdersLockWait) then begin
      SPNumber := 3;
    end
    else begin
      SPNumber := 1;
    end;

    aOL := OLList.Items[Row];

    if (not aOL.AllowEdit) then begin
      Exit;
    end;

    if ((aOL.AlreadyOrdered) and (GlbTable.SaleCategoryType = scRunningTab)) then begin
      if (not VerifyPriv(22)) then begin
        ShowMsg('Priviledge Denied.' + #13#13 +
                'Please ask someone who has the priviledge to clear items from this tab.');
        Exit;
      end;
    end;

    //If clearing the Replacement Card reason, don't update the account on completion of transaction!
    if (TOrderLine(OLList[Row]).ItemID = RSC.ItemID) then begin
      RSC.AllowUpdate := False;
      RSC.ReasonAlreadyOrdered := False;
      RSC.CardCode := '';
      RSC.ItemID := 0;
      RSC.CancelPressed := False;
    end;

    //Exit if not AN OL, ie if selected a mod (which can't void)
    if ((aOL.ItemID = 0) and (aOL.ModifierID <> 0)) then begin // JEH 18/08/2001 1 -> 0
      if (AppDetails.EnableQuickModifierDelete) then begin
        DeleteModifier;
        Exit;
      end
      else begin
        ShowQuickMsg(sSelectOrderLineClickMod);
        Exit;
      end;
    end;

    //get from - to rows
    FromRow := Row;
    ToRow := Row;

    if (not DeletingComboItems) then
    begin
      ComboIDToDelete := 0;
      if ((aOL.ItemID > 0) and (aOL.FixThisComboPart)) then
      begin
        ComboIDToDelete := aOL.ComboID;
        FComboIndexToDelete := aOL.ComboIndex;
      end;
    end;

    if ((ComboIDToDelete > 0) and (FComboIndexToDelete > 0) and (not DeletingComboItems)) then
    begin
      DeleteCombos(Row);
      DeletingComboItems := False;
      BroadcastCDAMessage;
      Exit;
    end;

    //Look if this is a combo Item
    if (((TOrderLine(OLList[ToRow]).ItemID = 0) and (TOrderLine(OLList[ToRow]).ModifierID = 0)) and (ToRow <> (RowCount-1))) then begin
      //go down one
      Inc(ToRow);
    end
    else
    begin
      //Inc torow if there are sub items of the selected item
      //if not a modifier row and not last line
      // JEH 18/08/2001 1 -> 0
      if ((TOrderLine(OLList[ToRow]).ItemID <> 0) and (ToRow <> (RowCount-1))) then
      begin
        //go down one
        Inc(ToRow);

        //keep on going down one until next order line item or last row
        while ((TOrderLine(OLList[ToRow]).ItemID = 0) and (TOrderLine(OLList[ToRow]).ModifierID <> 0) and (ToRow <> (RowCount-1))) do
        begin
          Inc(ToRow);
        end;

        //if not last line then go to last mod of previous order line item
        if (((TOrderLine(OLList[ToRow]).ItemID <> 0) and (TOrderLine(OLList[ToRow]).ModifierID = 0)) or (ToRow <> (RowCount-1))) then
        begin
          Dec(ToRow);
        end;
      end
      else
      begin
        //Pole Display Jon 29-11-2001
        if (AppDetails.PDEnabled) then begin
          DMComponents.PDOutputItemLine('-' + TOrderLine(OLList[ToRow]).ItemString,  - GetRoundedUpDown(TOrderLine(OLList[ToRow]).ToPay, AppDetails.DecimalPlaces), 0, False);
        end;
      end;
    end;

    RowsToDelete := (ToRow - FromRow) + 1;

    if ((AppDetails.EnableFiscalPrinting) and
        (not (NFManual or NFEMode)) and
        (not DoingPhoneOrder) and
        (not (OnTable and (GlbTable.GroupID > 0)))) then
    begin
      if ((Trim(TOrderLine(OLList[ToRow]).FiscalIndex) <> '') and
          (not SFiscalPrinting.CancelFiscalPrinterInvoiceItems(TOrderLine(OLList[ToRow]).FiscalIndex))) then
      begin
        //TheTR.Rollback;
        //CancelFiscalPrinterInvoice; // Cancel Opened invoice
        //cmdVoidDesClick(Self);
        //Result := False;
        //Exit;
      end;
    end;

    //delete rows in grid
    if (FromRow = 0) and (RowCount-ToRow < 2) then begin // JEH 18/08/2001 1 -> 0
      //clear row 1's contents
      RowCount := 1; // JEH 18/08/2001 reduce by 1
      Cells[0, 0] := '';
      Cells[1, 0] := '';
      Cells[2, 0] := '';
      Cells[3, 0] := '';
    end
    else begin
      //move rows up by one
      for I := FromRow to RowCount - 1 do begin
        Cells[0, I] := Cells[0, I + RowsToDelete];
        Cells[1, I] := Cells[1, I + RowsToDelete];
        Cells[2, I] := Cells[2, I + RowsToDelete];
        Cells[3, I] := Cells[3, I + RowsToDelete];
      end;
      RowCount := RowCount - RowsToDelete
    end;

    //delete from the OLList
    for I := FromRow to ToRow do begin
      OL := OLList.Items[FromRow];
      OLList.Delete(FromRow);

      if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (OL.ItemID > 0) and (OL.LoyaltyItemID > 0) and (OL.LoyaltyFreeQty > 0)) then begin
        LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + OL.LoyaltyFreeQty;
        TestAllOLsLoyaltyItem(True);
      end;

      VoidOLList.Add(OL);
      //Dispose(OL);
      if ((AppDetails.ClearItemsVoid > 0) and (not OL.AlreadyOrdered)) then
        VoidClearedOL(SPNumber, False);
    end;

    if not DeletingComboItems then
      CalculateToPay;
  end;
  //Pole Display Jon 29-11-2001
  if ((AppDetails.PDEnabled)and (not DeletingComboItems)) then
    DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);

  CancelPOSInvoiceIfFiscalInvCancelled;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ClearMods;
var
  FromRow, ToRow: Integer;
  RowsToDelete: Integer;
  I: Integer;
  aOL: TOrderLine;
begin
  with grdOLs do begin
    //if on last row then there are no mods to clear
    if (Row = (RowCount - 1)) then begin
      Exit;
    end;

    //if the next row (Row + 1 - 1) is an item, then there are no mods to clear
    if ((TOrderLine(OLList[Row + 1]).ItemID > 0) or (TOrderLine(OLList[Row + 1]).ModifierID = 0)) then begin    // JEH 18/08/2001 add 1 to see next row
      Exit;
    end;

    //set the ol's unit price and price back to original
   // JEH 18/08/2001 1 -> 0
    aOL := OLList.Items[Row];
    // RPC Clear the 'total' Mod Price for Item
    aOL.ModPrice := 0;
    aOL.UnitPrice := aOL.OriginalUnitPrice;
    aOL.Qty := aOL.OriginalQty;
    Cells[1, Row] := FormatFloat(QTYFORMAT, aOL.Qty);
    aOL.ToPay := aOL.UnitPrice * aOL.Qty;
    aOL.ToPay := GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces);

    if ((aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0)) then begin
      Cells[3, Row] := '* ' + FormatCurrencyNoSign(((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.ToPay);
    end
    else begin
      Cells[3, Row] := FormatCurrencyNoSign(aOL.ToPay);
    end;

    //calculate the from and to row of mods to clear
    //get from - to rows
    FromRow := Row+1;
    ToRow := Row+1;

    //keep on going down one until next order line item or last row
    // JEH 18/08/2001 1 -> 0
    while ((TOrderLine(OLList[ToRow]).ItemID = 0) and (TOrderLine(OLList[ToRow]).ModifierID <> 0) and (ToRow <> (RowCount-1))) do begin
      Inc(ToRow);
    end;

    //if not last line then go to last mod of previous order line item
    if ((TOrderLine(OLList[ToRow]).ItemID <> 0) or (TOrderLine(OLList[ToRow]).ModifierID = 0)) then begin
      dec(ToRow);
    end;

    RowsToDelete := (ToRow - FromRow) + 1;

    //move rows up by one
    for I := FromRow to (RowCount - 1) do begin
      Cells[0, I] := Cells[0, I + RowsToDelete];
      Cells[1, I] := Cells[1, I + RowsToDelete];
      Cells[2, I] := Cells[2, I + RowsToDelete];
      Cells[3, I] := Cells[3, I + RowsToDelete];
    end;
    RowCount := (RowCount - RowsToDelete);

    //delete from the OLList
    for I := (FromRow) to (ToRow) do begin
      aOL := OLList.Items[FromRow];
      aOL.Free;
      OLList.Delete(FromRow);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdModsDesClick(Sender: TObject);
var
  RowToSelect: Integer;
  OrderLine: TOrderLine;
begin
  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  if ((AppDetails.EnableFiscalPrinting) and
      (AppDetails.FiscalLiveInvoice) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0))) and
      (GlbTable.SaleCategoryType <= scStandard)) then
  begin
    ShowQuickMsg(sNoModSelectionAfterItemSelection);
    Exit;
  end;

  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  StopInactiveTimer;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  //if there are any ols
  if (OLList.Count > 0) then
  begin
    SelectModifiersItemInGrid;
    RowToSelect := grdOLs.Row;

    OrderLine := OLList.Items[grdOLs.Row];
    //if an OL selected
    if ((OrderLine.ItemID > 0) and (not OrderLine.AlreadyOrdered))then begin // JEH 18/08/2001 1 -> 0
      Update;

      //used for mods form
      GlbItemModifiers.AddingItem := False;
      GlbItem.Item := OrderLine.ItemString;
      GlbItem.ItemID := OrderLine.ItemID;
      GlbItemModifiers.ForB := OrderLine.ForB;
      GlbItem.ThePrice := OrderLine.OriginalUnitPrice; //AMANDA 08.02.01

      //AMANDA 17/10/00
      LoadMods;
      if (formItemModifiers.GetMods) then begin
        ClearMods;
        AddMods;
      end;
      DisposeMods;
      TemporarilyDeactiveButtons;

      //select the OL added
      grdOLs.Row := RowToSelect;
      CalculateToPay;
      //Pole Display Jon 29-11-2001
      if (AppDetails.PDEnabled) then begin
        DMComponents.PDOutputItemLine(OrderLine.ItemString, GetRoundedUpDown(OrderLine.ToPay, AppDetails.DecimalPlaces), 0, False);
        DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
      end;
    end
    else if (OrderLine.AlreadyOrdered) then
    begin
      ShowQuickMsg(sItemAlreadyOrderMods);
    end
    else
    begin
      ShowQuickMsg(sSelectOrderLineClickModToEdit);
    end;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;

{******************************************************************************}
procedure TformQuickSaleUser.LoadMods;
var
  I: Integer;
  FromRow: Integer;
  ToRow: Integer;
  OrderLine: TOrderLine;
  aMod: TMod;

begin
  DisposeMods;
  Mods := TList.Create;

  if ((not GlbItemModifiers.AddingItem) or (FAddingDuplicateItem)) then begin
    with grdOLs do begin
      //if on last row then there are no mods to load
      if (Row = (RowCount-1)) then begin
        Exit;
      end;

      //if the next row (Row + 1 - 1) is an item, then there are no mods to load
      if ((TOrderLine(OLList[Row+1]).ItemID > 0) or (TOrderLine(OLList[Row+1]).ModifierID = 0)) then begin    // JEH 18/08/2001 Added 1
        Exit;
      end;

      //calculate the from and to row of mods to load
      //get from - to rows
      FromRow := Row+1;
      ToRow := Row+1;

      //keep on going down one until next order line item or last row
      while (((TOrderLine(OLList[ToRow]).ItemID = 0) and (TOrderLine(OLList[ToRow]).ModifierID <> 0)) and (ToRow <> (RowCount-1))) do begin
        Inc(ToRow);
      end;

      //if not last line then go to last mod of previous order line item
      if (((TOrderLine(OLList[ToRow]).ItemID <> 0) or (TOrderLine(OLList[ToRow]).ModifierID = 0)) or (ToRow <> (RowCount-1))) then begin
        dec(ToRow);
      end;

      for I := FromRow to ToRow do begin
        OrderLine := OLList.Items[I];
        aMod := TMod.Create;
        aMod.ModID := OrderLine.ModifierID;
        //aMod.Modifier := Cells[1, I];
        aMod.Modifier := OrderLine.ItemString;    //Jon 9-8-2001
        aMod.ModGroupID := OrderLine.ModGroupID;
        aMod.ModGroupOrder := OrderLine.ModGroupOrder;
        //aMod.ModPrice := TOL(OLList[I-0]^).ModPrice;
        aMod.ModPrice := OrderLine.OriginalModPrice;            //Jon 10-9-2001
        aMod.OriginalModPrice := OrderLine.OriginalModPrice;    //Jon 10-9-2001
        aMod.ModFunction := OrderLine.ModifierFunction;
        aMod.PrinterID := OrderLine.PrinterID;
        aMod.RepeatPrinterID := OrderLine.RepeatPrinterID;
        aMod.ModQty := OrderLine.ModQty;
        aMod.ModQtyOption := OrderLine.ModQtyOption;
        aMod.ModQtyOrdered := OrderLine.ModQtyOrdered;
        aMod.ModQtyIncluded := OrderLine.ModQtyIncluded;
        Mods.Add(aMod);
      end;
    end;
  end;
end;
{******************************************************************************}

{******************************************************************************}
procedure TformQuickSaleUser.cmdEditClick(Sender: TObject);
var
  TL: TTL;
begin
  if ((DoingTransaction) or (TLList.Count = 0)) then begin    //RPC 29-03-11 added Count check if not tenders then nothing to edit
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  //if EFTPOS integrated check that not an EFTPOS line, if is showmsg, then Exit
  if ((AppDetails.EftposIntegrated) and
      (TTL(TLList.Items[grdTLs.Row-1]).TLType in [TLTEFTPOS, TLTVisaEFTPOS, TLTAmexEFTPOS, TLTDinersEFTPOS, TLTMasterCardEFTPOS, TLTBarterCard])) then begin
    ShowMsg(sCannotEditEftPosTenderLine);
    Exit;
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
    EditTenderLine(False);
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddTenderLine(TLType: TTLT; pTL: TTL = nil);
var
  Payment: Currency;
  TL: TTL;
  TenderLineType: TTenderLineType;
  LockPayment: Boolean;
  LockMaxPayment: Boolean;
  UsingRequired: Boolean;


  bLockPay: Boolean;
  ContinueWithTender: Boolean;
  SaleCategory: TSaleCategory;

  function GetOlIndex(AnOrderLineID: Integer):Integer;
  var
    I: Integer;
    tmpOl: TOrderLine;
  begin
    Result := -1;
    for I := 0 to OLList.Count - 1 do
    begin
      tmpOl := TOrderLine(OLList.Items[I]);
      if (Assigned(OLList.Items[I]) and (tmpOl.OrderLineID = AnOrderLineID)) then
      begin
        Result := I;
        Exit;
      end;
    end;
  end;

begin
  if (GlbEvent.EventID > 0) then
  begin
    SetGlbEvent(GlbEvent.EventID, True);

    if ((GlbEvent.Available - FHostSubsidy + FDiscountAmount) < 0) then
    begin
      ShowMsg(sEventSpendLimitReached);
      txtAvail.Text := FormatCurrencyNoSign(GlbEvent.Available - FHostSubsidy + FDiscountAmount);
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;
  end;

  StopInactiveTimer;

  ContinueWithTender := True;

  if ((DoingTransaction) or (ButtonsDeactivated)) then
  begin    //Jon 25-02-2002     
    if (Assigned(pTL)) then FreeAndNil(pTL);
    Exit;
  end;

  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  if ((OLList.Count > 0) or ((AppDetails.CashSaleAllowAccPayment) and (GlbAccount.AccountID > 0))) or
      ((TLList.TenderTypeExists(TLTExternalProviders)) and (TLList.TotalPayment < 0)) or
      ((Assigned(pTL)) and (pTL.TLType = TLTExternalProviders)) then
  begin  // JEH 18/08/2001
    if ((OnTable xor (GlbTable.SaleCategoryType = scDriveThru)) and (GlbTable.SaleCategoryType <> scRunningTab)) then begin
      ShowMsg(sTabHasSelected);   
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;

    if ((TLTYpe = TLTPocketVoucher) and (AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID = '')) then
    begin
      ShowMsg(sNoPocketVoucherMerchID);  
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;

    if ((TLType in [TLTLibica, TLTFidelio, TLTPhoenix, TLTMicros4700, TLTMicros4700Account, TLTMews, TLTAgora, TLTRMS, TLTRoomCharge]) and TLList.TenderTypeExists(TLType)) then begin
      ShowMsg(sRoomTransExists);
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;
    if ((TLType = TLTNZAGold) and TLList.TenderTypeExists(TLTNZAGold)) then begin
      ShowMsg(sNZAGoldTransExists);
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;
    if ((TLType = TLTEvolution) and TLList.TenderTypeExists(TLTEvolution)) then begin
      ShowMsg(sEvolTransExists);
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;

    if (DoingWaste) then begin
      ShowMsg(sTendersNotAllowWhenWastage);
      if (Assigned(pTL)) then FreeAndNil(pTL);
      Exit;
    end;

    if TLType = TLTwiGroup then
    begin
      if FDoingRefund then
      begin
        ShowMsg(Format(swiGroupRefundNotAllowed, [swiGroupTL]));
        if (Assigned(pTL)) then
          FreeAndNil(pTL);
        Exit;
      end;

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

      wiGroupPOSSrv.SetPayment(TLList, FToPay, SaleCategorySurchargeItem, OLList);

      if not wiGroupPOSSrv.ValidatePayment then
        Exit;
    end;

    Payment := GetDue(False);
    UsingRequired := False;
    bLockPay := False;
    LockPayment := ((TLType = TLTCash) and ((AppDetails.CashSaleRemoveTenderChange) or (AppDetails.CashSaleCashOnlyTenders = 1)));
    LockMaxPayment := ((OlList.Count = 0) and ((not AppDetails.CashSaleAllowAccPayment) or (not OnAccount))) or ((OlList.Count > 0) and (not (Appdetails.PaymentAllowChangeOnAccount and OnAccount)));
    
    if (not Assigned(pTL)) then
    begin
      TL := TTL.Create;
      TL.TLType := TLType;

      if TL.TLType = TLTwiGroup then
        wiGroupPOSSrv.SetTL(TL)
      else if ((GlbAccount.AccountID > 0) and (OnAccount) and (not FDoingRefund) and
          (FRequired <> 0) and (FRequired <> Payment) and (not AppDetails.CashSaleSwapStillDueForTenders) and
          (ShowConf(sUseRequiredAmt) = mrYes)) then begin
        TL.TLPayment := FRequired;
        UsingRequired := True;
      end
      else begin
        TL.TLPayment := Payment;
      end;

      LockMaxPayment := LockMaxPayment or (bLockPay);

      TL.TLTip := 0.00;
      TL.TLChange := 0.00;
      TL.TLTender := TL.TLPayment;
      TL.TLSurcharge := 0.00;

      TL.TLTender := RoundToRounding(TL.TLTender, RoundForTLType(TL.TLType));
      TL.TLRounding := TL.TLPayment - TL.TLTender;

      SaleCategory := GlobalMenuList.ListSaleCategorys.Find(GlbTable.SaleCategoryID);
      //currently assume we always find the current sales category
      if (SaleCategory.AutoTipPercent > 0) then
      begin
        TL.TLTip := TL.TLTender * (SaleCategory.AutoTipPercent/100);
        TL.TLTip := RoundToRounding(TL.TLTip, RoundForTLType(TL.TLType));
        TL.TLTender := TL.TLTender + TL.TLTip;
      end;
    end
    else
      TL := pTL;

    if (AppDetails.EnableFiscalPrinting and AppDetails.EFTPOSIntegrated and (AppDetails.EFTPOSType in [ecTEFIPDaruma]) and (not(TLType in [TLTNone, TLTCash, TLTCheque]))) then
    begin
      SFiscalTEF.DoesCardDiscountEnabled('1');
    end;

    if ((TLType = TLTExternalProviders) and Assigned(pTL)) or ((ContinueWithTender) and
        (((not FDoingRefund) and (GetTL(TTPayment, TLAAdd, Payment, TL, False, True, LockPayment, LockMaxPayment, False))) or
         ((FDoingRefund) and (GetTL(TTRefund, TLAAdd, Payment, TL, False, True, LockPayment, LockMaxPayment, False))))) then
    begin
      if (GlbTable.SaleCategoryType > scDriveThru) and (DiscCalculator.SaleHasCombos) and (TL.TLTender < Payment) then
      begin
        LoadQuickServiceComboPayToTabPay;
        StartInactiveTimer;
        Exit;
      end;

      if ((TLType = TLTPocketVoucher) and (AppDetails.PocketVoucherMerchantID <> '')) then begin
        TL.PocketVoucherCode := AppDetails.PocketVoucherPrePopCode;
        if ((not GetNumPadString(TL.PocketVoucherCode, ' Voucher Code', tiPOCKETvoucher)) or
            (not DoPocketVoucherTransaction(TL.PocketVoucherCode, TL.TLTender, FToPay, True))) then
            begin
          TL.Free;
          pTL := nil;
          Exit;
        end;
      end;
      TL.TEFCreditCardDiscount := 0;
      if (AppDetails.EnableFiscalPrinting and AppDetails.EFTPOSIntegrated and (AppDetails.EFTPOSType in [ecTEFIPDaruma]) and (not(TLType in [TLTNone, TLTCash, TLTCheque]))) then
      begin
        TL.TEFCreditCardDiscount := SFiscalTEF.LogCreditCardDiscount('LOGESTENDIDO');
      end;

      if (TL.TLType = TLTLibica) then
      begin      //Jon 24-09-2002
        LibAccount := GlbLibicaAccount.Account;  //Jon 03-10-2002
        LibRoom := GlbLibicaAccount.Room;        //Jon 03-10-2002
        LibName := GlbLibicaAccount.Name;        //Jon 03-10-2002
        LibStatus := GlbLibicaAccount.Status;    //Jon 03-10-2002
      end;
      if (TL.TLType = TLTNZAGold) then begin      //Jon 24-09-2002
        NZAAccount := GlbNZAAccount.Account;     //Jon 01-02-2003
        NZAAkey := GlbNZAAccount.AKey;           //Jon 01-02-2003
        NZAName := GlbNZAAccount.Name;           //Jon 01-02-2003
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
        TL.SurchargeGLCode := TenderLineType.SurchargeGLCode;
        TL.SurchargeGLCode2 := TenderLineType.SurchargeGLCode2;
      end
      else begin
        TL.SurchargeItemID := 0;
      end;

      if ((AppDetails.CashSaleCashOnlyTenders > 0) and (TL.TLType = TLTCash)) then
      begin
        AddCashTender(TL.TLTender, UsingRequired);
        BroadcastCDAMessage;
        Exit;
      end
      else
      begin
        TLList.Add(TL);
        LoadTLs(LAAdd);
        BroadcastCDAMessage;
        if AppDetails.EnableFiscalPrinting then
        begin
          SWBEncryption.AuxiliaryData.TEFDone := '0';
          if EFTPOSTransExists then
            SWBEncryption.AuxiliaryData.TEFDone := '1';
          SWBEncryption.GenerateEncryptedAuxFile;
        end;
        if (TL.TLSurcharge <> 0) then begin   //Jon 28-05-2003
          CalculateToPay;
        end;
      end;
      if (AppDetails.PDEnabled) then
      begin
        DMComponents.PDOutputItemLine(GetTLTType(TL.TLType), RoundCurrency(TL.TLTender, 2), 0, False);
        DMComponents.PDOutputItemLine('STILL DUE', RoundCurrency(FStillDue, 2), 1, False);
      end;

      BroadCastCDAMessage;

      if (not Assigned(pTL)) and
        (((AppDetails.CashSaleAutoComplete) or ((AppDetails.PMSType = pmsNZAGold) and AppDetails.NZAOnly)
      or ((AppDetails.PMSType = pmsEvolution) and AppDetails.EvolutionOnly)) and
          ((FStillDue = 0) or
           ((UsingRequired) and (FRequired <= 0)))) then begin
        DoingTransaction := True;
        Application.ProcessMessages;
        DoingTransaction := False;
        if (FDoingRefund) then
        begin
          DoCashSaleRefund(False);
        end
        else begin
          DoCashSale(False,4);
        end;
      end;
    end
    else
    begin
      TL.Free;  
      pTL := nil;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.EditTenderLine(UseDueAmount: Boolean);
var
  TL: TTL;
  Payment: Currency;
  TenderLineType: TTenderLineType;
  LockPayment: Boolean;
begin
  if (TLList.Count = 0) then begin
    Exit;
  end;

  if (GlbTable.SaleCategoryType > scDriveThru) and (not LoadQuickServiceComboPayToTabPay) then
    Exit;

  TL := TLList.Items[grdTLs.Row-1];

  //cannot edit a wiGroup tender line
  if TL.TLType = TLTwiGroup then
    Exit;

  if (UseDueAmount) then begin
    if (AppDetails.TaxExclusivePrices) then begin
      Payment := FToPay + FSurcharge - FDiscountAmount + FSalesTax;
    end
    else begin
      Payment := FToPay + FSurcharge - FDiscountAmount;
    end
  end
  else begin
    Payment := TL.TLPayment;
  end;
  LockPayment := ((TL.TLType = TLTCash) and ((AppDetails.CashSaleRemoveTenderChange) or (AppDetails.CashSaleCashOnlyTenders > 0)));

  if (((not FDoingRefund) and (GetTL(TTPayment, TLAEdit, Payment, TL, False, True, LockPayment, True, False))) or
      ((FDoingRefund) and (GetTL(TTRefund, TLAEdit, Payment, TL, False, True, LockPayment, True, False)))) then begin
    TenderLineType := GetTenderLineTypeRecord(TL.TLType);

    if (TenderLineType <> nil) then begin
      TL.SurchargeItemID := TenderLineType.SurchargeItemID;
      TL.SurchargeItemAbbrev := TenderLineType.SurchargeItemAbbrev;
      TL.SurchargePercent := TenderLineType.SurchargePercent;
      TL.SurchargeFixed := TenderLineType.SurchargeFixed;
      TL.SurchargeSalesTaxPercent := TenderLineType.SurchargeSalesTaxPercent;
      TL.SurchargeGLCode := TenderLineType.SurchargeGLCode;
      TL.SurchargeGLCode2 := TenderLineType.SurchargeGLCode2;
    end
    else begin
      TL.SurchargeItemID := 0;
    end;

      LoadTLs(LAEdit);

      if ((TL.TLSurcharge <> 0) or (FSurcharge <> 0)) then begin
        CalculateToPay;
      end;

    if ((FStillDue = 0) and (UseDueAmount) and ((AppDetails.CashSaleAutoComplete) or ((AppDetails.PMSType = pmsNZAGold) and AppDetails.NZAOnly) or ((AppDetails.PMSType = pmsEvolution) and AppDetails.EvolutionOnly)) and (not DoingWaste)) then begin
      if (FDoingRefund) then begin
        DoCashSaleRefund(False);
      end
      else begin
        DoCashSale(False,5);
      end;
    end;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.GetDue(AllowNegative: Boolean; IncludeExternalTenders: Boolean = True): Currency;
var
  I: Integer;
  AlreadyPaying: Currency;
  TL: TTL;
begin
  //calculate amount towards invoice(s) already paying
  AlreadyPaying := 0;
  if (TLList.Count > 0) then begin
    for I := 0 to (TLList.Count - 1) do begin
      TL := TLList.Items[I];
      if ((TL.TLType = TLTExternalProviders) and (not IncludeExternalTenders)) then
        Continue;
      AlreadyPaying := GetRoundedUpDown(AlreadyPaying + TL.TLPayment, AppDetails.DecimalPlaces);
    end;
  end;

  Result := 0.00;
  if ((GlbEvent.EventID > 0) and (GlbEvent.SubsidyID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount)) then
  begin
    if ((not AppDetails.TaxExclusivePrices) and (((FGuestSubsidy + FSurcharge - AlreadyPaying) > 0) or (AllowNegative))) then
      Result := FGuestSubsidy + FSurcharge - AlreadyPaying
    else if ((AppDetails.TaxExclusivePrices) and (((FGuestSubsidy + FSurcharge  + FSalesTax - AlreadyPaying) > 0) or (AllowNegative))) then
      Result := FGuestSubsidy + FSurcharge + FSalesTax - AlreadyPaying;
  end
  else if ((not AppDetails.TaxExclusivePrices) and (((FToPay + FSurcharge- FComboDiscount - FDiscountAmount - FLoyaltyFreeItemDiscount - AlreadyPaying) > 0) or (AllowNegative))) then
    Result := FToPay + FSurcharge - FComboDiscount - FDiscountAmount - FLoyaltyFreeItemDiscount - AlreadyPaying   //Jon 31-5-2001
  else if ((AppDetails.TaxExclusivePrices) and (((FToPay + FSurcharge - FComboDiscount- FDiscountAmount - FLoyaltyFreeItemDiscount + FSalesTax - AlreadyPaying) > 0) or (AllowNegative))) then
    Result := FToPay + FSurcharge- FComboDiscount - FDiscountAmount - FLoyaltyFreeItemDiscount + FSalesTax - AlreadyPaying;  //Jon 30-5-2001

  Result := GetRoundedUpDown(Result, AppDetails.DecimalPlaces);
end;
{******************************************************************************}
procedure TformQuickSaleUser.LoadTLs(LAction: TLA);
var
  I: Integer;
  TL: TTL;
begin
  with grdTLs do begin
    if (TLList.Count = 0) then begin
      RowCount := 2;
      Cells[0, 1] := '';
      Cells[1, 1] := '';
      Cells[2, 1] := '';
      Cells[3, 1] := '';
      Cells[4, 1] := '';
      Cells[5, 1] := '';
      Row := 1;
    end
    else begin
      RowCount := TLList.Count + 1;
      for I := 0 to (TLList.Count - 1) do begin
        TL := TLList.Items[I];
        if (TL.TLType = TLTExternalProviders) and (TL.TLProvider <> '') then
          Cells[0, I+1] := TL.TLProvider
        else
          Cells[0, I+1] := GetTLTType(TL.TLType);
        Cells[1, I+1] := TL.TLNotes;
        Cells[2, I+1] := FormatCurrencyNoSign(TL.TLPayment);
        Cells[3, I+1] := FormatCurrencyNoSign(TL.TLTip);
        Cells[4, I+1] := FormatCurrencyNoSign(TL.TLTender);
        Cells[5, I+1] := FormatCurrencyNoSign(TL.TLChange);
      end;
    end;
    if LAction = LAAdd then Row := RowCount-1;
  end;

  BuildTenderSurchargeList;   //Jon 27-05-2003

  TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding);

  txtTPayment.Text := FormatCurrencyNoSign(FPayment);
  txtTTip.Text := FormatCurrencyNoSign(FTip);
  txtTChange.Text := FormatCurrencyNoSign(FChange);
  txtTTender.Text := FormatCurrencyNoSign(FTender);
  txtCChange.Caption := FormatCurrencyNoSign(FChange);

  if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
  begin
    FToPay := TruncateTo(FToPay, 2);
    FSurcharge := TruncateTo(FSurcharge, 2);
  end;

  if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (not OnAccount) and (GlbEvent.SubsidyID > 0)) then
    FStillDue := CalculateStillDue(FGuestSubsidy + FSurcharge, 0, FPayment, FSalesTax)
  else
    FStillDue := CalculateStillDue(FToPay + FSurcharge, FComboDiscount + FDiscountAmount + FLoyaltyFreeItemDiscount, FPayment, FSalesTax);

  if (AppDetails.EnableFiscalPrinting and (AppDetails.TruncateAmount = 1)) then
    FStillDue := TruncateTo(FStillDue, 2);

  if ((GlbAccount.AccountID > 0) and (OnAccount) and (not FDoingRefund)) then begin
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

  if (AppDetails.CashSaleSwapStillDueForTenders) then begin   //Jon 21-08-2003
    txtStillDue.Caption := FormatCurrencyNoSign(FTender);
  end
  else begin
    txtStillDue.Caption := FormatCurrencyNoSign(FRequired);
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.EFTPOSTransExists: Boolean;
var
  I, iCount, iErr: Integer;
  TL: TTL;
begin
  Result := False;

  //if EFTPOS integrated check that there are NO EFTPOS lines, if is showmsg, then Exit
  if ((AppDetails.EftposIntegrated) and (TLList.Count > 0)) then
    Result := TLList.EFTPOSTransExists;

  if (Result and AppDetails.EnableFiscalPrinting and (TLList.Count > 0)) then
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
procedure TformQuickSaleUser.ClearAllTenderLines;
begin
  TLList.Clear;

  LoadTLs(LAClearAll);
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetTLCols;
begin
  with grdTLs do begin
    ColCount := 6;

    ColWidths[0] := 86;
    ColWidths[1] := 59;
    ColWidths[2] := 58;
    ColWidths[3] := 58;
    ColWidths[4] := 58;
    ColWidths[5] := 58;

    Cells[0, 0] := sType;
    Cells[1, 0] := sDetails;
    Cells[2, 0] := sPayment;
    Cells[3, 0] := sTip;
    Cells[4, 0] := sTender;
    Cells[5, 0] := sChange;
  end;
  ClearAllTenderLines;
end;
{******************************************************************************}
procedure TformQuickSaleUser.grdTLsDblClick(Sender: TObject);
begin
  cmdEditClick(nil);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQtyDesClick(Sender: TObject);
begin
  StopInactiveTimer;
  ShowQty(not ShowingQty);
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.QuickSaleCashCam;
var
  Line: string;
  I, upto: Integer;
  OL: TOrderLine;
  TL: TTL;
begin
  WipeCashCamTransRecord;
  with CashCamTransRecord do begin
    TransType := 'Quick Sale';
    if (WBTerminalLogins.QueryType <> qtLoginsTerminal) then begin
      FilterQuery(dm.qrTerminalLogins, qtLoginsTerminal);
    end;
    Staff := dm.qrTerminalLogins.FieldByName('staffname').AsString;

    upto := (OLList.Count - 1);

    for I := 0 to upto do begin  // JEH 18/08/2001 1 -> 0
      OL := OLList.Items[I];
      if (OL.ItemID > 0) then begin
        //The OL is an item
        Line := ConcatItemTender(FloatToStr(OL.Qty) + ' ' + OL.ItemString, OL.Qty * OL.UnitPrice);
        Items.Add(Line);
      end
      else begin
        //this is a modifier
        if (OL.Modprice > 0) then begin
          Items.Add('    ' + OL.ItemString);
        end;
      end;
    end;

    if (txtAccountName.Text <> '') then begin
      Items.Add(ConcatItemTender('SubTotal', FToPay));
      Items.Add(ConcatItemTender('Discount', -FDiscountAmount));
      Items.Add('  ' + txtAccountName.Text);
    end;

    Line := ConcatItemTender('Total', FToPay - FDiscountAmount);

    Items.Add(Line);
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
procedure TformQuickSaleUser.NoSaleCashCam;
var
  Line: string;
  I: Integer;
  TL: TTL;
begin
  WipeCashCamTransRecord;
  with CashCamTransRecord do begin
    TransType := 'No Sale';
    if (WBTerminalLogins.QueryType <> qtLoginsTerminal) then begin
      FilterQuery(dm.qrTerminalLogins, qtLoginsTerminal);
    end;
    Staff := dm.qrTerminalLogins.FieldByName('staffname').AsString;

    Items.Add(dm.qrNSTypes.FieldByName('nosaletype').AsString);

    for I := 0 to (TLList.Count - 1) do begin
      TL := TLList.Items[I];
      Line := ConcatItemTender(GetTLTType(TL.TLType), TL.TLTender);
      TenderLines.Add(Line);
    end;

    if (txtAccountName.Text <> '') then begin
      TenderLines.Add(txtAccountName.Text);
    end;

    DoCashCamTrans;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTablesClick(Sender: TObject);
begin
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  if (not OkToClose) then begin
    StartInactiveTimer;
    Exit;
  end;
  cmdHideClick(nil);

  ResetLoginSessionTimeout;

  ReturnToCashSaleAfterTabs := True;
  case TSpeedButton(Sender).Tag of
    1: PostMessage(formMain.Handle, WM_TOTABLES, 0, 0);
    3: PostMessage(formMain.Handle, WM_TOBOOKINGS, 0, 0);
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdViewOrderLinesDesClick(Sender: TObject);
begin
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;   //Jon 12-7-2001
  ViewOrderLines(otCashSale, GetItemCount, grdOLs);
  StartInactiveTimer;
end;
{******************************************************************************}
function TformQuickSaleUser.SaleCategorySelected: Boolean; //AKM 3.3.1
begin
  Result := True;
  //if a cash sale and a sale category button hasn't been selected
  if ((not OnTable) and (GlbTable.SaleCategoryID = 0)) then begin
    ShowMsg(sNoSaleCatSelected);
    Result := False;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.StaffPromptTimeoutTimerTimer(Sender: TObject);
var
  LoginID: Integer;
begin
  inherited;
  StopInactiveTimer;
  StopStaffPromptTimeoutTimer;
  LoginID := GlbLogin.LoginID;
  if (CloseFromCashSales) then
  begin
    cmdHideClick(nil);
  end
  else if (SelectName(True, True, AppDetails.ProveStaffCashSale)) then
  begin
    InitialiseAccountInfo;
    AddStaffLoginButton;
    if ((AppDetails.StaffAutoParkSale > 0) and (LoginID <> GlbLogin.LoginID) and (SelectParkedSale(True))) then
    begin
      TryLoadParkedSale;
    end;
  end
  else
  begin
    cmdHideClick(nil);
  end;
  if (Assigned(FormSelectCustomer) and (screen.ActiveForm = FormSelectCustomer)) then dm.qrCustomers.First;
end;
{******************************************************************************}
// RPC added this function we can stop the timeout timer and start it again by
// calling ResetStaffPromptTimeouttimer it used as alterntive to StopStaffPromptTimeoutTimer
procedure TformQuickSaleUser.PauseStaffPromptTimeoutTimer;
begin
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;
  StaffPromptTimeoutTimer.Enabled := False;
end;
procedure TformQuickSaleUser.pnlSummaryRightDraw(ASender: TObject;
  const ACanvas: ISkCanvas; const ADest: TRectF; const AOpacity: Single);
var
  LPaint: ISkPaint;
begin
  inherited;
  LPaint := TSkPaint.Create;
  LPaint.PathEffect := TSkPathEffect.MakeCorner(6);
  //Shader
  LPaint.Shader := TSkShader.MakeGradientLinear(PointF(0, 0), PointF(ADest.Width, 0), $FF4D4D4D, $FF2F2F2F, TSkTileMode.Clamp);
  LPaint.ImageFilter := TSkImageFilter.MakeDropShadow(2, 2, 2, 2, TAlphaColors.Black);
  LPaint.AntiAlias := True;
  ACanvas.DrawRect(ADest, LPaint);
end;

procedure TformQuickSaleUser.pnlTitleClick(Sender: TObject);
begin
  inherited;

end;

{******************************************************************************}
procedure TformQuickSaleUser.ResetStaffPromptTimeoutTimer;
begin
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    StartInactiveTimer;
    Exit;
  end;

  if (StaffPromptTimeoutTimerActive) then begin
    StaffPromptTimeoutTimer.Enabled := False;
    StaffPromptTimeoutTimer.Interval := (AppDetails.CashSaleStaffPromptTimeout * 1000);
    StaffPromptTimeoutTimer.Enabled := True;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.StopStaffPromptTimeoutTimer;
begin
  StaffPromptTimeoutTimer.Enabled := False;
  StaffPromptTimeoutTimerActive := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ChangePage(QSPageID: Integer);
begin
  OpenArch.CurrentQSPageID := QSPageID;
  ModifierFunctionChange(1);    //Set the Modfunction to + on page change
  PriceLevelChange(0, 1);       //Set the PriceLevel to 0 on page change
end;
{******************************************************************************}
function TformQuickSaleUser.ForBToString(aForB: string): string;
begin
  if (aForB = 'B') then
  begin
    Result := sBev;
  end
  else if (aForB = 'F') then
  begin
    Result := sFood;
  end
  else begin
    Result := '';
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ModifierFunctionChange(ModFunction: Integer);
begin
  FModifierFunction := ModFunction;
  OpenArch.ButtonsDown(qsbaModFunction, ModFunction);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdNoSaleClick(Sender: TObject);
begin
  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  NoSalePINEntered := False;
  if (AppDetails.CashSaleNoSaleFinish) then begin
    AttemptDefaultNoSale;
  end
  else begin
    ResetStaffPromptTimeoutTimer; // JEH 18/08/2001
    if (not OkToClose) then begin
      StartInactiveTimer;
      Exit;
    end;
    cmdHideClick(nil);
    ReturnToCashSaleAfterNoSale := True;
    PostMessage(formMain.Handle, WM_TONOSALE, 0, 0)
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AttemptDefaultNoSale;    //Jon 17-07-2003
begin
  if (DoingTransaction) then begin
    Exit;
  end;
  try
    DoingTransaction := True;
    //check if have privilege
    if (not VerifyPriv(16)) then begin
      ShowMsg(sNoAdminNoSales);
      Exit;
    end;

    //verify pinno
    if ((not NoSalePINEntered) and ((not GlbPrivs[16].AvoidPINCheck) or (AppDetails.ProveStaffNoSale))) then begin
      if (not VerifyPinNo) then begin
        Exit;
      end
      else begin
        NoSalePINEntered := True;
      end;
    end;

    if (DefaultNoSale) then begin
      //Do CashCamm Doofrey if needed
      if (AppDetails.CashCamEnabled) then begin
        NoSaleCashCam;
      end;
    end;
  finally
    DoingTransaction := False;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdHomeClick(Sender: TObject);
begin
  StopInactiveTimer;
  if (OpenArch.CurrentQSPageID <> AppDetails.CashSaleDefaultPageID) then begin
    BackPagesList.Add(OpenArch.CurrentQSPageID);
    PostMessage(formMain.Handle, WM_QSCHANGEPAGE, AppDetails.CashSaleDefaultPageID, 0);
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.txtStaffChange(Sender: TObject);
begin
  inherited;
  lblstaffname.Caption := txtStaff.Text;
{  if (Assigned(cmdSelectNameDes)) then begin
    cmdSelectNameDes.Caption := txtStaff.Text;
  end
  else begin
    Skin.ChangeButton(cmdSelectName, bstToolbar, gstNoChange, biNone, txtStaff.Text);
  end; }
  cmdSecondControlSelectName.Caption := txtStaff.Text;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdBackClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  if (BackPagesList.Count > 0) then begin
    PostMessage(formMain.Handle, WM_QSCHANGEPAGE, BackPagesList.GetLast, 0);
    BackPagesList.DeleteLast;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
//JEH 18/08/2001
procedure TformQuickSaleUser.cmdMoreDesClick(Sender: TObject);
var
  RowToSelect: Integer;
  TheQty: Currency;
  ItemsQty: Currency;
  J: Integer;
  aOL: TOrderLine;
  OLJ: TOrderLine;
  VoidOL: TOrderLine;
  aQty: Double;
  aRounding, aUnit, aItemName, aTax, aQtyType: string; //For fiscal
  aItemID: Integer;
  aUnitPrice, aDiscount: Currency;
  ComboMod: TComboModifier;
  aMod: TMod;
  NCMTax: Currency;
begin
  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  inherited;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  StopInactiveTimer;
  if (DoingTransaction) then begin    //Jon 25-02-2002
    Exit;
  end;

  FAddingDuplicateItem := False;
  ResetStaffPromptTimeoutTimer;
  if (OLList.Count > 0) then
  begin
    // JEH 18/08/2001 1 -> 0
    RowToSelect := grdOLs.Row;
    aOL := OLList.Items[grdOLs.Row];
    if (aOL.ItemID > 0) then
    begin
      TheQty := aOL.Qty;
      if ((aOL.ComboID > 0) and (ShowConf(sAddCombo) = mrYes)) then
      begin
        AddSameCombo(aOL.ComboID, aOL.ComboIndex);
        BroadcastCDAMessage;
        StartInactiveTimer;
        grdOLs.Repaint;
        Exit;
      end;
      if (((AppDetails.EnableFiscalPrinting) and
           (AppDetails.FiscalLiveInvoice) and
           (not (NFManual or NFEMode)) and
           (not DoingPhoneOrder) and
           (not (OnTable and (GlbTable.GroupID > 0))) and
           (GlbTable.SaleCategoryType <= scStandard)) or
          (TheQty = aOL.AlreadyOrderedQty)) then
      begin
        AddNewFiscalItem;
        StartInactiveTimer;
        Exit;
      end;

      if (TheQty < 999) then begin
        AvailQtyTested := False;   //Jon 14-05-2003
        if (AppDetails.OrderDecrementItemQty > 0) then begin    //Jon 14-05-2003
          ItemsQty := 0;

          for J := (OLList.Count - 1) downto 0 do begin
            OLJ := OLList.Items[J];
            if (OLJ.ItemID = aOL.ItemID) then begin
              if (OLJ.AvailQtyOK) then begin
                AvailQtyTested := True;
              end;
              ItemsQty := ItemsQty + OLJ.Qty;
            end;
          end;
          if ((AppDetails.OrderDecrementItemQty in [2, 3]) or (not AvailQtyTested)) then begin
            if (CheckItemQty(ItemsQty + 1, aOL.ItemID, AvailQtyTested)) then begin
              aOL.AvailQtyOK := AvailQtyTested;
            end
            else begin
              StartInactiveTimer;
              Exit;
            end;
          end;
        end;

        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyItemID > 0)) then begin
          //Return old Qty first
          LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        end;

        if aOL.ComboID > 0 then
        begin
          DisposeMods;
          if ((Assigned(aOL.ComboMods)) and (aOL.ComboMods.Count > 0)) then
          begin
            Mods := TList.Create;
            for J := 0 to aOL.ComboMods.Count  - 1 do
            begin
              aMod := TMod.Create;
              ComboMod := TComboModifier(aOL.ComboMods[J]);
              aMod.ModID := ComboMod.ModID;
              aMod.ModGroupID := ComboMod.ModGroupID;
              aMod.Modifier := ComboMod.Modifier;
              aMod.ModGroupOrder := ComboMod.ModGroupOrder;
              aMod.ModPrice := ComboMod.ModPrice;
              aMod.OriginalModPrice := ComboMod.OriginalModPrice;
              aMod.PrinterID := ComboMod.PrinterID;
              aMod.RepeatPrinterID := ComboMod.RepeatPrinterID;
              aMod.ModFunction := ComboMod.ModFunction;
              aMod.ModQty := ComboMod.ModQty;
              aMod.ModQtyOption := ComboMod.ModQtyOption;
              aMod.ModQtyOrdered := ComboMod.ModQtyOrdered;
              aMod.ModQtyIncluded := ComboMod.ModQtyIncluded;
              Mods.Add(aMod);
            end;
          end;
          //ForcedNextQty := (aOL.Qty > 1);
          NextQty := 1;//Ceil(aOL.Qty);
          FAddingDuplicateItem := True;
          OrderItem(aOL.ItemID, False);
          FAddingDuplicateItem := False;

          Exit;
        end;

        // if not a combo item, it comes here
        TheQty := TheQty + 1;
        aOL.Qty := TheQty;
        aOL.OriginalQty := aOL.OriginalQty + 1;

        aOL.ToPay := aOL.UnitPrice * TheQty;
        aOL.ToPay := GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces);
        aOL.DiscountedAmount := aOL.ToPay;
        if (AllocateItemLoyalty(aOL)) then begin
          TestAllOLsLoyaltyItem(True);
        end;

        UpdateOLGridRow(grdOLs.Row, aOL);

        // Here we going look through the void list to see if we have item we can bring back
        for J := 0 to (VoidOLList.Count - 1) do begin
          VoidOL := VoidOLList.Items[J];
          if ((VoidOL.ItemID = aOL.ItemID) and (VoidOL.OrderLineID = aOL.OrderLineID) and (VoidOL.Qty = 1)) then begin
            VoidOLList.Delete(J);  //Deleted the item
            Break;  //We can get out of here now
          end;
        end;
        DiscCalculator.AddingFreeComboItems := True;
        CalculateToPay;
        AddFreeComboItemsToOrderList;
        DiscCalculator.AddingFreeComboItems := False;
        //Pole Display Jon 29-11-2001

        if ((AppDetails.EnableFiscalPrinting) and
            (AppDetails.FiscalLiveInvoice) and
            (not (NFManual or NFEMode)) and
            (not DoingPhoneOrder) and
            (not (OnTable and (GlbTable.GroupID > 0))) and
            (GlbTable.SaleCategoryType <= scStandard)) then
        begin
          CheckForFiscalDiscountChanges;        //CalculateFiscalDiscounts;

          if ((aOL.ItemID > 0) or (aOL.Qty > 0) or (aOL.UnitPrice > 0)) then
          begin
            aItemID := aOL.ItemID;
            aItemName := Copy(aOL.ItemString, 1, 29);
            if Trim(aOL.TaxSituation) = '' then
              aOL.TaxSituation := 'T';

            if (AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC]) then
            begin
              if aOL.TaxSituation = 'T' then
                aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 0)
              else if aOL.TaxSituation = 'S' then
                aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 1)
              else
                aTax := aOL.TaxSituation;
            end
            else if (AppDetails.FiscalPrinterType = fpDaruma) then
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
              aQty := CRoundTo(aOL.Qty, 3);
              aUnitPrice := CRoundTo(aOL.UnitPrice, 3);
              aDiscount := CRoundTo(aOL.ILDiscount, 2);
              aRounding := 'A';
            end;

            if (not aOL.WeighedItem) then
            begin
              aUnit := 'UN';
              aQtyType := 'I';
            end
            else
            begin
              aUnit := 'KG';                       //0 = in Kilos 1 - in 10gm, 2 - in gm
              aQtyType := 'F';
            end;

            if (AppDetails.FiscalPrinterType = fpBematechNFC)then
            begin
              NCMTax := SFiscalPrinting.GetNCMTax(aOL.NCMProductCode, aOL.CSOSN, aOL.ToPay);
              FTotalNCMTax := FTotalNCMTax + NCMTax;
              if not SFiscalPrinting.AddFiscalPrinterNFCInvoiceItems(aItemID, aOL.GTIN, aItemName,
                  '01', aTax, aUnit, aQtyType, '3', aQty, '3', aUnitPrice, '$', '0', aDiscount,
                  aRounding, aOL.NCMProductCode, IntToStr(aOL.CFOP), aOL.Notes, aOL.CSTICMS, IntToStr(aOL.ProductOrigin), '', '', '', '',
                  '', aOL.CSOSN, '', '', '', '', '', '', '', '',
                  '', '', '', '', '', '', FormatFloat(PRICEFORMAT, NCMTax), aOL.CST_PIS, '', '',
                  '', '', '', aOL.CST_COFFINS, '', '', '', '', '', aOL.CEST) then
              begin
                cmdVoidDesClick(Self);
              end;
            end
            else if not SFiscalPrinting.AddFiscalPrinterInvoiceItems(aItemID, aItemName, aTax, aQty, aUnit, aUnitPrice, aDiscount) then
            begin
              cmdVoidDesClick(Self);
            end
            else begin
              aOL.FiscalIndex := SFiscalPrinting.FiscalItemIndex;
            end;
          end;
        end;
        if (AppDetails.PDEnabled) then begin
          DMComponents.PDOutputItemLine(aOL.ItemString, GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces), 0, False);
          DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
        end;

        CancelPOSInvoiceIfFiscalInvCancelled;
      end;
    end
    else if (aOL.ModifierID > 0) then
    begin
      if (aOL.ModQty > 1) then
      begin
        if (aOL.ModQtyOrdered >= aOL.ModQty) then
        begin
          ShowQuickMsg(sModifierLimitReached, False);
        end
        else
        begin
          aOL.ModQtyOrdered := aOL.ModQtyOrdered + 1;
          SelectModifiersItemInGrid;
          LoadMods;
          ClearMods;
          AddMods;
          DiscCalculator.AddingFreeComboItems := True;
          CalculateToPay;
          AddFreeComboItemsToOrderList;
          DiscCalculator.AddingFreeComboItems := False;
          grdOLs.Row := RowToSelect;
        end;
      end;
    end;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
//JEH 18/08/2001
procedure TformQuickSaleUser.cmdLessDesClick(Sender: TObject);
var
  RowToSelect: Integer;
  TheQty: Currency;
  aOL: TOrderLine;
  aVoidOL: TOrderLine;
  SPNumber: Integer;
  bDeleteCombo: Boolean;
begin
  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  RemoveOrderLine := False;
  inherited;
  StopInactiveTimer;
  if (((AppDetails.EnableFiscalPrinting) and
       (AppDetails.FiscalLiveInvoice) and
       (not (NFManual or NFEMode)) and
       (not DoingPhoneOrder) and
       (not (OnTable and (GlbTable.GroupID > 0))) and
       (GlbTable.SaleCategoryType <= scStandard))) then
  begin
    cmdVoidDesClick(Self);
    StartInactiveTimer;
    Exit;
  end;
  if (AppDetails.OrdersLockWait) then begin
    SPNumber := 3;
  end
  else begin
    SPNumber := 1;
  end;

  if (DoingTransaction) then
  begin    //Jon 25-02-2002
    Exit;
  end;
  ResetStaffPromptTimeoutTimer;
  if (OLList.Count > 0) then
  begin
    // JEH 18/08/2001 1 -> 0
    RowToSelect := grdOLs.Row;
    aOL := OLList.Items[grdOLs.Row];
    if ((aOL.ItemID > 0) and (aOL.AllowEdit)) then begin
      TheQty := aOL.Qty;

      if (TheQty > 1) then
      begin              
        if (not sExternalAccountIntegration.CheckLoadedRedemptions) then
        begin
          StartInactiveTimer;
          Exit;
        end;
        if ((aOL.AlreadyOrdered) and (GlbTable.SaleCategoryType = scRunningTab)) then
        begin
          if (not VerifyPriv(22)) then begin
            ShowQuickMsg('Priviledge Denied.' + #13#13 + 'Please ask someone who has the priviledge to clear items from this tab.');
            StartInactiveTimer;
            Exit;
          end;
        end;

        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyItemID > 0)) then
        begin
          //Return old Qty first
          LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        end;

        if (not DeletingComboItems) then
        begin
          ComboIDToDelete := aOL.ComboID;
          FComboIndexToDelete := aOL.ComboIndex;
        end;

        if ((ComboIDToDelete > 0) and (FComboIndexToDelete > 0) and (not DeletingComboItems)) then
        begin
          bDeleteCombo := (TheQty <= aOL.ComboQty);
          if ((TheQty > aOL.ComboQty) and (ShowConf(Format(sConfirmDeleteCombo, [FormatFloat('###0.###', TheQty), FormatFloat('###0.###', aOL.ComboQty)])) = mrYes)) then
            bDeleteCombo := True;

          if bDeleteCombo then
          begin
            DeleteCombos(grdOLs.Row);
            DeletingComboItems := False;
            BroadcastCDAMessage;
            Exit;
          end;
        end;

        TheQty := TheQty - 1;
        aOL.Qty := TheQty;
        aOL.OriginalQty := aOL.OriginalQty - 1;

        aOL.ToPay := aOL.UnitPrice * TheQty;
        aOL.ToPay := GetRoundedUpDown(aOL.ToPay, AppDetails.DecimalPlaces);
        aOL.DiscountedAmount := aOL.ToPay;
        if (AllocateItemLoyalty(aOL)) then begin
          TestAllOLsLoyaltyItem(True);
        end;

        UpdateOLGridRow(grdOLs.Row, aOL);

        aVoidOL := TOrderLine.Create;
        aVoidOL.CopyOL(aOL);
        aVoidOL.Qty := 1; // We removing 1 at a time
        VoidOLList.Add(aVoidOL);
        if ((AppDetails.ClearItemsVoid > 0) and (not aVoidOL.AlreadyOrdered)) then begin;
          VoidClearedOL(SPNumber, False);
        end;
        DiscCalculator.AddingFreeComboItems := True;
        CalculateToPay;
        AddFreeComboItemsToOrderList;
        DiscCalculator.AddingFreeComboItems := False;
        //Pole Display Jon 29-11-2001
        if (AppDetails.PDEnabled) then
        begin
          DMComponents.PDOutputItemLine(TOrderLine(OLList[grdOLs.Row]).ItemString, GetRoundedUpDown(TOrderLine(OLList[grdOLs.Row]).ToPay, AppDetails.DecimalPlaces), 0, False);
          DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
        end;
      end;
    end
    else if (aOL.ModifierID > 0) and (aOL.AllowEdit) then
    begin
      if (aOL.ModQtyOrdered > 1) then
      begin                                           
        if (not sExternalAccountIntegration.CheckLoadedRedemptions) then
        begin
          StartInactiveTimer;
          Exit;
        end;
        aOL.ModQtyOrdered := aOL.ModQtyOrdered - 1;
        SelectModifiersItemInGrid;
        LoadMods;
        ClearMods;
        AddMods;
        DiscCalculator.AddingFreeComboItems := True;
        CalculateToPay;
        AddFreeComboItemsToOrderList;
        DiscCalculator.AddingFreeComboItems := False;
        grdOLs.Row := RowToSelect;
      end;
    end;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DeleteModifier;
var
  ModRow: Integer;
  RowCurrent: Integer;
  I: Integer;
  Modifier: TMod;

begin
  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  if (not sExternalAccountIntegration.CheckLoadedRedemptions) then Exit;

  ModRow := grdOLs.Row;
  RowCurrent := grdOLs.Row;
  while ((TOrderLine(OLList[RowCurrent]).ItemID = 0) and (TOrderLine(OLList[RowCurrent]).ModifierID <> 0) and (RowCurrent > 0)) do begin
    Dec(RowCurrent);
  end;

  grdOLs.Row := RowCurrent;
  GlbItemModifiers.AddingItem := False;   //Jon 24-12-2003
  LoadMods;

  for I := (Mods.Count - 1) downto 0 do begin
    Modifier := Mods.Items[I];
    if ((Modifier.ModID = TOrderLine(OLList[ModRow]).ModifierID) and
      (Modifier.ModGroupID = TOrderLine(OLList[ModRow]).ModGroupID)) then begin
      Modifier.Free;
      Mods.Delete(I);
    end;
  end;
  ClearMods;
  AddMods;
  DisposeMods;
  if (ModRow > (OLList.Count - 1)) then begin
    ModRow := (OLList.Count - 1);
  end;
  grdOLs.Row := ModRow;
  CalculateToPay;
  if (AppDetails.PDEnabled) then begin
    DMComponents.PDOutputItemLine(TOrderLine(OLList[RowCurrent]).ItemString, GetRoundedUpDown(TOrderLine(OLList[RowCurrent]).ToPay, AppDetails.DecimalPlaces), 0, False);
    DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ShowPnlToPay(B: Boolean);
begin
  pnlTender.Visible := (pnlTender.Visible and (not B));
  pnlToPay.Visible := (pnlToPay.Visible and B);
  pnlToPay.Align := alClient;
  pnlTender.Align := alClient;
  pnlTender.Visible := (not B);
  pnlToPay.Visible := B;
  TemporarilyDeactiveButtons;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdRefundClick(Sender: TObject);
var
  QtyToVoid: Currency;
  QtyToWaste: Currency;
begin
  StopInactiveTimer;
  if (TfcCustomImageBtn(Sender).Tag = 1) then
  begin
    //check if have privilege
    if (not VerifyPriv(2)) then
    begin
      ShowMsg(sGetHelpToVoid);
      SetDoingWaste(False, True);
      StartInactiveTimer;
      Exit;
    end;
  end
  else begin
    //check if have privilege
    if (not VerifyPriv(17)) then
    begin
      ShowMsg(sGetHelpToRefundCashSale);
      SetDoingRefund(False, True);
      StartInactiveTimer;
      Exit;
    end;
  end;

  if ((TfcCustomImageBtn(Sender).Tag = 0) and (TLList.Count > 0)) then
  begin
    ShowMsg(sClearTenderLinesFirst);
    SetDoingRefund(not TfcCustomImageBtn(Sender).Down, True);
    StartInactiveTimer;
    Exit;
  end;

  if ((TfcCustomImageBtn(Sender).Tag = 1) and (TfcCustomImageBtn(Sender).Down = True) and (TLList.Count > 0)) then
  begin
    ShowMsg(sDeleteTenderLinesBeforeWaste);
    SetDoingWaste(False, True);
    StartInactiveTimer;
    Exit;
  end;

  if (RSC.AllowUpdate) then begin
    ShowMsg(sClearReplacementSwipeCard);
    Exit;
  end;

  if AppDetails.EnableFiscalPrinting then
  begin
    StartInactiveTimer;
    Exit;
  end;

  if (OnTable) then begin
    ShowMsg(sNoRefundOrWastageAgainstTab);
    TfcCustomImageBtn(Sender).Down := False;
    StartInactiveTimer;
    Exit;
  end;

  if (DoingPhoneOrder) then begin
    if (ShowConf(sConfirmCancelPhOrder) <> mrYes) then begin
      StartInactiveTimer;
      Exit;
    end;
  end;

  if InGoodySale then
  begin
    ShowMsg(sNoRefundForGoodyScan);
    SetDoingRefund(False, True);
    StartInactiveTimer;
    Exit;
  end;

{  if ((AppDetails.PMSType = pmsRMS) and ((AppDetails.RMSAllTransactions > 0) or (TLList.TenderTypeExists(TLTRMS))))  then
  begin
    ShowMsg(sNoRefundForRMS);
    SetDoingRefund(False, True);
    StartInactiveTimer;
    Exit;
  end; }

  //Set flag to be true or false
  SetDoingRefund((not FDoingRefund) and (TfcCustomImageBtn(Sender).Tag = 0), False);
  SetDoingWaste((not DoingWaste) and (TfcCustomImageBtn(Sender).Tag = 1), False);
  if ((TfcCustomImageBtn(Sender).Tag = 1) and (TfcCustomImageBtn(Sender).Down = False)) then
    CalculateToPay;

  DoPhoneOrder(False, False);     //Cannot do a refund while doing a phone order

  if (DoingWaste) then
  begin
    if (not ShowVoid(QtyToVoid, QtyToWaste, FToWaste, 0, FVoidReasonID, FExp1, FExp2, vdmWasteOrder, False)) then begin
      SetDoingWaste(False, True);
      Exit;
    end;
    FToWaste := True;     //Just in case
  end
  else if (FDoingRefund) then
  begin
    if (not ShowVoid(QtyToVoid, QtyToWaste, FToWaste, 0, FVoidReasonID, FExp1, FExp2, vdmRefund, False)) then begin
      SetDoingRefund(False, True);
      Exit;
    end;
  end
  else
  begin
    FToWaste := False;
  end;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.tmrDeactivateButtonsTimer(Sender: TObject);
begin
  tmrDeactivateButtons.Enabled := False;
  ButtonsDeactivated := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.PriceLevelChange(NewPriceLevel: Integer; NewPriceLevelType: Integer);
begin
  if (NewPriceLevel = 0) then begin
    if ((NewPriceLevelType <> 2) and (NewPriceLevelType <> 3) and (SalePriceLevel <> 0)) then begin
      PriceLevel := SalePriceLevel;
      PriceLevelType := 2;
    end
    else if ((NewPriceLevelType <> 3) and (PersistentPriceLevel <> 0)) then begin
      PriceLevel := PersistentPriceLevel;
      PriceLevelType := 3;
    end
    else begin
      PriceLevel := NewPriceLevel;
      PriceLevelType := 1;
    end;
  end
  else begin
    PriceLevelType := NewPriceLevelType;
    PriceLevel := NewPriceLevel;
  end;
  if (NewPriceLevelType = 3) then begin
    SalePriceLevel := 0;
    PersistentPriceLevel := NewPriceLevel;
  end
  else if (NewPriceLevelType = 2) then begin
    SalePriceLevel := NewPriceLevel;
  end;

  OpenArch.ButtonsDown(qsbaPriceLevel, PriceLevel);
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetDoingRefund(State: Boolean; Force: Boolean);
begin
  if (Force) then
  begin
    FDoingRefund := State;
  end
  else if (EFTPOSTransExists) then begin
    ShowMsg(sEftPosTenderLineFound);
  end
  else if ((AppDetails.SmartCardInterface > 0) and (TLList.TenderTypeExists(TLTSmartCard))) then begin
    ShowMsg(sClearSmartCardTenderLines);
  end
  else if ((AppDetails.VoucherType = Integer(vtPocketVoucher)) and (AppDetails.PocketVoucherMerchantID <> '') and (TLList.TenderTypeExists(TLTPocketVoucher))) then begin
    ShowMsg(sClearVoucherTenderLine);
  end
  else if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    //do nothing (as line above shows message if necessary), i.e. can't do a refund
  else begin
    FDoingRefund := State;
  end;
  cmdRefund.Down := FDoingRefund;
  cmdSecondControlRefund.Down := FDoingRefund;
  cmdPhoneOrder.Enabled := ((not FDoingRefund) and (not DoingWaste));
  if (FDoingRefund) then CalculateToPay;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetDoingExchange(State: Boolean; Force: Boolean);
var
  I: Integer;
  TL: TTL;
  FoundTender: Boolean;
begin
  FoundTender := False;
  if (not Force) then
  begin
    for I := 0 to TLList.Count - 1 do
    begin
      grdTLs.Row := I + 1;
      TL := TLList.Items[grdTLs.Row - 1];
      if TL.TLType <> TLTExchange then
      begin
        FoundTender := True;
      end;
    end;
  end;

  if ((Force) or (not FoundTender)) then
  begin
    DoingExchange := State;
  end
  else
  begin
    ShowMsg(sClearTenderLinesFirst);
  end;

  if (not DoingExchange) then
  begin
    //ClearExchangeDetails
    FExchangeInvoiceID := 0;
    FExchangeDiscountAmount := 0;
    FExchangeSalesTax := 0;
    FExchangeDiscountPercent := 0;

    ClearListObjects(FExchangeOLs);
    ClearExchageTender;
  end;
  CalculateToPay;
  cmdExchange.Down := DoingExchange;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetDoingWaste(State: Boolean; Force: Boolean);
begin
  if ((Force) or (TLList.Count <= 0)) then begin
    DoingWaste := State;
  end
  else begin
    ShowMsg(sClearTenderLinesFirst);
  end;
  cmdWaste.Down := DoingWaste;
  cmdPhoneOrder.Enabled := ((not FDoingRefund) and (not DoingWaste));
  if (DoingWaste) then CalculateToPay;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetOnAccount(State: Boolean);    //Jon 16-10-2003
begin
  // fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0))) and
      (Trim(AppDetails.FiscalAccountPaymentMethod) = '')) then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    Exit;
  end;
  OnAccount := (State and GlbRegistrationInfo.WaiterChargeAccounts);
  cmdOnAccount.Down := OnAccount;
  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID >0) and
                                  (GlbEvent.AccountID > 0) and
                                  (OnAccount) and (GlbEvent.SubsidyID > 0);
  if not LoadingAccount then
    CalculateToPay;
  ChangeQuickCashButtonCaption;
end;
{******************************************************************************}
procedure TformQuickSaleUser.CourseChange(NewCourseID: Integer; NewCourseName: string; NewCourseType: Integer; NewCourseIsMain: Boolean);
begin
  if (NewCourseID = 0) then begin
    if ((NewCourseType = 2) or (CurrentCourseType <> 2)) then begin
      CurrentCourseID := NewCourseID;
      CurrentCourseName := NewCourseName;
      CurrentCourseIsMain := NewCourseIsMain;    //Jon 05-05-2003
      CurrentCourseType := 2;
    end;
    //Otherwise leave it as it is
  end
  else begin
    CurrentCourseID := NewCourseID;
    CurrentCourseName := NewCourseName;
    CurrentCourseType := NewCourseType;
    CurrentCourseIsMain := NewCourseIsMain;    //Jon 05-05-2003
  end;

  OpenArch.ButtonsDown(qsbaCourse, CurrentCourseID);
end;
{******************************************************************************}
procedure TformQuickSaleUser.NextItemButtonStateChange(NewNextItemButtonState: Integer);
begin
  NextItemButtonState := NewNextItemButtonState;
  OpenArch.ButtonsDown(qsbaAssociatedItem, NextItemButtonState);
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetNextQty(I: Integer);
begin
  NextQty := I;
  txtNextQty.Text := IntToStr(I);
  if ((ShowingQty) and (Active)) then
  begin
    try
      if txtNextQty.Visible and txtNextQty.Enabled then
        txtNextQty.SetFocus;
    except
    end;
    txtNextQtyEnter(txtNextQty);
    if txtNextQty.Visible and txtNextQty.Enabled then
      txtNextQty.SelectAll;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.txtNextQtyChange(Sender: TObject);
begin
  if (StrToIntDef(txtNextQty.Text, 1) > 32000) then NextQty := 32000
  else NextQty := StrToIntDef(txtNextQty.Text, 1);
  if (IntToStr(NextQty) <> txtNextQty.Text) then begin
    txtNextQty.Text := IntToStr(NextQty);
  end;
  tmrBarCode.Enabled := False;
  tmrBarCode.Interval := BARCODE_SCAN_DELAY;
  tmrBarCode.Enabled := True;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQtyZeroDesClick(Sender: TObject);
begin
  Keybd_event(VkKeyScan(TfcCustomImageBtn(Sender).Caption[1]), 0, 0, 0);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQtyBackDesClick(Sender: TObject);
begin
  Keybd_event(VK_BACK, 0, 0, 0);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQtyMoreDesClick(Sender: TObject);
begin
  if (NextQty < 999) then begin
    SetNextQty(NextQty + 1);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdQtyLessDesClick(Sender: TObject);
begin
  if (NextQty > 1) then begin
    SetNextQty(NextQty - 1);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ShowQty(B: Boolean);
var
  CurrentRow: Integer;
begin
  ShowingQty := B;
  pnlQty.Height := 192;
  pnlQty.Visible := B;
  cmdQty.Down := B;
  pnlSummaryBottom.Visible := (not B);
  if ((B) and (Active)) then
  begin
    if txtNextQty.Enabled then
      txtNextQty.SetFocus;
    txtNextQty.SelectAll;
  end;
  CurrentRow := grdOLs.Row;
  grdOls.Row := 0;
  grdOls.Row := CurrentRow;
end;
{******************************************************************************}
procedure TformQuickSaleUser.txtNextQtyClick(Sender: TObject);
begin
  txtNextQty.SelectAll;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdParkedSaleClick(Sender: TObject);
var
  ParkSale: Integer;
  NoItemsEntered: Boolean;
  SwipedStaffCard: Boolean;
begin


  StopInactiveTimer;
  StopStaffPromptTimeoutTimer;    //Jon 12-7-2001
  ParkSale := 0;
  if ((OLList.Count > 0) or (TLList.Count > 0)) then
  begin
    NoItemsEntered := False;
    if (not TryParkSale(ParkSale, GlbLogin.LoginID, True)) then
    begin
      StartInactiveTimer;
      Exit;
    end;
  end
  else begin
    NoItemsEntered := True;
  end;

{-------Clear Screen---------}
  SwipedStaffCard := GlbScan.SwipedStaffCard;
  ResetCashSale(False, True);
  GlbScan.SwipedStaffCard := SwipedStaffCard;      //Because the reset cash sale was resetting this when we didn't want it to

  if ((not NoItemsEntered) and (AppDetails.CashSaleStaffPromptTimeout = 0)) then begin
    if (SelectName(True, True, AppDetails.ProveStaffCashSale)) then begin
      AddStaffLoginButton;
    end
    else begin
      cmdHideClick(nil);
      StartInactiveTimer;
      Exit;
    end;
  end;

  if ((ParkSale = 0) or (ParkSale = 2)) then begin
    if (not SelectParkedSale(False)) then begin
      StartInactiveTimer;
      Exit;
    end;

    TryLoadParkedSale;
    CalculateToPay;
  end;
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
function TFormQuickSaleUser.TryParkSale(var ParkSale: Integer; LoginID: Integer; Prompt: Boolean = False): Boolean;
var
  I: Integer;
  ParkedSaleID: Integer;
  OL: TOrderLine;
  TL: TTL;
  ItemForB: string;
begin
  Result := False;

  if (DoingExchange) or (FDoingRefund) or (DoingWaste) then
  begin
    if (DoingExchange) then
      ShowMsg(sCannotParkDuringExchangeProcess);
    if (FDoingRefund) then
      ShowMsg(sCannotParkDuringRefundProcess);
    if (DoingWaste) then
      ShowMsg(sCannotParkDuringWasteProcess);
    Exit;
  end;

  if ((sExternalAccountIntegration.ProviderCardsPresent > 0) and (ShowConf(sPresentedCardsParkSale) = mrNo)) then
    Exit;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
    Exit;

  //if doing Goody sale, confirm clear all Goody details first
  if InGoodySale then
  begin
    if (ShowConf(sConfirmClearGoodyDetails) <> mrYes) then
    begin
      Exit;
    end;
    IniGoodyTrans;
  end;

  if (Prompt) and (not ShowParkSale(ParkSale)) then
  begin
    Exit;
  end;

  try
    if (not dm.TRStartTest) then begin
      Exit;
    end;
    sExternalAccountIntegration.ClearPresentedCards;
    with dm.sp do begin
      SetStoredProcName('INSERT_PARKEDSALE');      //insert_parkedsale

      ParamByName('whoparked').AsInteger := LoginID;
      ParamByName('orderlineno').AsInteger := OLList.Count;
      ParamByName('tenderlineno').AsInteger := TLList.Count;

      ParamByName('fdelivery').AsString := FDelivery;
      ParamByName('ftopay').AsCurrency := FToPay;
      ParamByName('fpayment').AsCurrency := FPayment;
      ParamByName('ftip').AsCurrency := FTip;
      ParamByName('fchange').AsCurrency := FChange;
      ParamByName('ftender').AsCurrency := FTender;
      ParamByName('frounding').AsCurrency := FRounding;
      ParamByName('fstilldue').AsCurrency := FStillDue;
      ParamByName('fsalestax').AsCurrency := FSalesTax;

      ParamByName('fdiscountpercent').AsFloat := FDiscountPercent;
      ParamByName('fdiscountamount').AsCurrency := FDiscountAmount;
      BoolAsParam(ParamByName('fusediscountamount'), FUseDiscountAmount);
      if (GlbEvent.EventID > 0) then
        ParamNull(ParamByName('accountid'))
      else
        IntegerAsParam(ParamByName('accountid'), GlbAccount.AccountID);
      BoolAsParam(ParamByName('payingonaccount'), OnAccount);
      ParamByName('currenthour').AsInteger := CurrentHour;
      ParamByName('currentmin').AsInteger := CurrentMin;
      BoolAsParam(ParamByName('doingrefund'), FDoingRefund);

      ExecProc;
      ParkedSaleID := FieldByName('parkedsaleid').AsInteger;

      ItemForB := 'F'; //We set this incase 1st item is combo!
      for I := 0 to (OLList.Count - 1) do
      begin
        SetStoredProcName('INSERT_PSORDERLINE');       //insert_psorderline

        OL := OLList.Items[I];

        ParamByName('parkedsaleid').AsInteger := ParkedSaleID;
        ParamByName('itemid').AsInteger := OL.ItemID;
        if (OL.FORB <> '') then begin
          //Cos FORB must be entered as 'F' or 'B' even for mods without ForB
          ItemForB := OL.FORB;
        end;
        ParamByName('forb').AsString := ItemForB;
        ParamByName('qty').AsCurrency := OL.OriginalQty;
        ParamByName('printerid').AsInteger := OL.PrinterID;
        ParamByName('repeatprinterid').AsInteger := OL.RepeatPrinterID;
        ParamByName('modifierid').AsInteger := OL.ModifierID;
        if (OL.ModifierID <> 0) then begin
          ParamByName('modgroupid').AsInteger := OL.ModGroupID;
          ParamByName('modgrouporder').AsInteger := OL.ModGroupOrder;
          ParamByName('qty').AsDouble := OL.ModQtyOrdered;
          ParamByName('includedmodqty').AsDouble := OL.ModQtyIncluded;
        end;
        {else begin
          ParamByName('modgroupid').AsInteger := OL.ComboGroupID;
          ParamByName('modgrouporder').AsInteger := OL.ComboGroupOrder;
        end;}
        ParamByName('modgrouporder').AsInteger := OL.ModGroupOrder;
        ParamByName('originalunitprice').AsCurrency := OL.OriginalUnitPrice;
        ParamByName('unitprice').AsCurrency := OL.UnitPrice;
        ParamByName('price').AsCurrency := OL.ToPay;
        ParamByName('modprice').AsCurrency := OL.ModPrice;
        ParamByName('originalmodprice').AsCurrency := OL.OriginalModPrice;
        BoolAsParam(ParamByName('priceexcludestax'), OL.PriceExcludesTax);
        ParamByName('salestaxpercent').AsFloat := OL.SalesTaxPercent;
        BoolAsParam(ParamByName('openprice'), OL.OpenPrice);
        ParamByName('openpricemin').AsCurrency := OL.OpenPriceMin;
        ParamByName('openpricemax').AsCurrency := OL.OpenPriceMax;
        ParamByName('modfunction').AsInteger := OL.ModifierFunction;
        ParamByName('itemstring').AsString := OL.ItemString;
        ParamByName('olpricelevel').AsInteger := OL.OLPriceLevel;
        IntegerAsParam(ParamByName('courseid'), OL.CourseID);
        BoolAsParam(ParamByName('courseismain'), OL.CourseIsMain);
        IntegerAsParam(ParamByName('comboid'), OL.ComboID);
        IntegerAsParam(ParamByName('combogroup'), OL.ComboGroupID);
        IntegerAsParam(ParamByName('comboindex'), OL.ComboIndex);
        DoubleAsParam(ParamByName('comboqty'), OL.ComboQty);
        BoolAsParam(ParamByName('fixthecombo'), OL.FixThisComboPart);
        BoolAsParam(ParamByName('freecomboitem'), OL.FreeComboItem);
        ExecProc;
      end;
      for I := 0 to (TLList.Count - 1) do begin
        SetStoredProcName('INSERT_PSTENDERLINE');           //insert_pstenderline

        TL := TLList.Items[I];
        if (TL.TLType = TLTExternalProviders) then
          Continue;

        ParamByName('parkedsaleid').AsInteger := ParkedSaleID;
        ParamByName('tenderlinetypeid').AsInteger := TL.TLTypeID;
        ParamByName('tlnotes').AsString := TL.TLNotes;
        ParamByName('tlpayment').AsCurrency := TL.TLPayment;
        ParamByName('tltip').AsCurrency := TL.TLTip;
        ParamByName('tlchange').AsCurrency := TL.TLChange;
        ParamByName('tltender').AsCurrency := TL.TLTender;
        ParamByName('tlrounding').AsCurrency := TL.TLRounding;

        ExecProc;
      end;
    end;

//    dm.tr.SavePoint;
    dm.tr.Commit;   //Jon 10-07-2003
    //If Replacement Swipe Card item is loaded, clear RSC for next sale
    RSC.AllowUpdate := False;
    RSC.ReasonAlreadyOrdered := False;
    RSC.CardCode := '';
    RSC.ItemID := 0;
    RSC.CancelPressed := False;

    Result := True;
  except
    on e: exception do begin
      ShowMessage(e.message);
      dm.tr.Rollback;
      SelectFirsts;
    end;
  end;
end;
{******************************************************************************}
function TFormQuickSaleUser.TryLoadParkedSale: Boolean;
var
  OL, lItemOL: TOrderLine;
  TL: TTL;

begin
  lItemOL := nil;
  Result := False;
  try
    FDelivery := GlbParkedSale.FDelivery;
    FToPay := GlbParkedSale.FToPay;
    FPayment := GlbParkedSale.FPayment;
    FTip := GlbParkedSale.FTip;
    FChange := GlbParkedSale.FChange;
    FTender := GlbParkedSale.FTender;
    FRounding := GlbParkedSale.FRounding;
    FStillDue := GlbParkedSale.FStillDue;
    FSalesTax := GlbParkedSale.FSalesTax;
    CurrentHour := GlbParkedSale.CurrentHour;
    CurrentMin := GlbParkedSale.CurrentMin;
    CurrentTime := (GlbParkedSale.CurrentHour * 100) + GlbParkedSale.CurrentMin;
//    SetDoingRefund(GlbParkedSale.FDoingRefund, True);
    SetDoingRefund(False, True);

    if ((GlbParkedSale.AccountID > 0) and (LoadAccount(GlbParkedSale.AccountID))) then begin
      FDiscountPercent := GlbParkedSale.FDiscountPercent;
      FDiscountAmount := GlbParkedSale.FDiscountAmount;
      FUseDiscountAmount := GlbParkedSale.FUseDiscountAmount;
      SetOnAccount(GlbParkedSale.PayingOnAccount);
    end;

    if (not dm.TRStartTest) then begin
      Exit;
    end;
    SetQuery(qtParkedSalesOrderLines);
    with dm.qrPSOrderLines do begin
      First;
      while (not EOF) do begin
        OL := TOrderLine.Create;
        OL.ItemID := FieldByName('itemid').AsInteger;
        OL.ItemGroupID := FieldByName('itemgroupid').AsInteger;
        OL.Qty := FieldByName('qty').AsCurrency;

        if (OL.ItemID <> 0) then
        begin
          lItemOL := OL;
          OL.OriginalQty := FieldByName('qty').AsCurrency;
        end;

        OL.SwipeReplacementGrp := FieldByName('swipereplacementgrp').AsBoolean;
        if (OL.SwipeReplacementGrp) then begin
          RSC.AllowUpdate := True;
          RSC.ReasonAlreadyOrdered := True;
          RSC.CardCode := '';
          RSC.ItemID := OL.ItemID;
          RSC.CancelPressed := False;
        end;
        OL.FORB := FieldByName('forb').AsString;
        OL.PrinterID := FieldByName('printerid').AsInteger;
        OL.RepeatPrinterID := FieldByName('repeatprinterid').AsInteger;
        OL.ModifierID := FieldByName('modifierid').AsInteger;
        OL.ModGroupID := FieldByName('modgroupid').AsInteger;
        OL.ModGroupOrder := FieldByName('modgrouporder').AsInteger;
        OL.OriginalUnitPrice := FieldByName('originalunitprice').AsCurrency;
        OL.UnitPrice := FieldByName('unitprice').AsCurrency;
        OL.ToPay := FieldByName('price').AsCurrency;
        OL.ModPrice := FieldByName('modprice').AsCurrency;
        OL.OriginalModPrice := FieldByName('originalmodprice').AsCurrency;
        OL.PriceExcludesTax := FieldAsBool(FieldByName('priceexcludestax'));
        OL.SalesTaxPercent := FieldByName('salestaxpercent').AsFloat;
        OL.OpenPrice := FieldAsBool(FieldByName('openprice'));
        OL.OpenPriceMin := FieldByName('openpricemin').AsCurrency;
        OL.OpenPriceMax := FieldByName('openpricemax').AsCurrency;
        OL.ModifierFunction := FieldByName('modfunction').AsInteger;
        OL.ItemString := FieldByName('itemstring').AsString;
        OL.OLPriceLevel := FieldByName('olpricelevel').AsInteger;
        OL.CourseID := FieldByName('courseid').AsInteger;
        OL.CourseIsMain := FieldAsBool(FieldByName('courseismain'));    //Jon 07-05-2003
        OL.AvailQtyOK := False;     //Jon 14-05-2003    Don't bother checking in ParkSale. If another added then it will check then. This can create problems, but we can think later about them.
        OL.ModsChangePrinter := False;    //Jon 03-06-2003
        OL.ItemPrices[1] := FieldAsCurrency(FieldByName('itemprice'));
        OL.ItemPrices[2] := FieldAsCurrency(FieldByName('itemprice2'));
        OL.ItemPrices[3] := FieldAsCurrency(FieldByName('itemprice3'));
        OL.ItemPrices[4] := FieldAsCurrency(FieldByName('itemprice4'));
        OL.ItemPrices[5] := FieldAsCurrency(FieldByName('itemprice5'));
        OL.ItemPrices[6] := FieldAsCurrency(FieldByName('itemprice6'));
        OL.WeighedItem := FieldAsBool(FieldByName('weigheditem'));
        OL.AllowEdit := True;
        OL.ApplyDiscountType := adtAllItems;

        OL.ComboID := FieldAsInt(FieldByName('comboid'));
        OL.ComboGroupID := FieldAsInt(FieldByName('combogroup'));
        OL.ComboIndex := FieldAsInt(FieldByName('comboindex'));
        OL.ComboQty := FieldByName('comboqty').AsCurrency;
        OL.FixThisComboPart := FieldAsBool(FieldByName('FixTheCombo'));
        OL.FreeComboItem := FieldAsBool(FieldByName('FreeComboItem'));
        OL.ComboTempQty := OL.ComboQty;
        if OL.FreeComboItem then
          OL.FreeComboQty := OL.ComboQty;
        OL.SetComboPrice := ((OL.ComboID > 0) and (OL.ItemID <> 0));

        OL.LoyaltyPoints1RatioNum := FieldAsCurrency(FieldByName('loyaltypoints1rationum'));
        OL.LoyaltyPoints1RatioDen := FieldAsCurrency(FieldByName('loyaltypoints1ratioden'), 1);
        OL.LoyaltyPoints1Qty := FieldAsCurrency(FieldByName('loyaltypoints1qty'));
        OL.LoyaltyPoints2RatioNum := FieldAsCurrency(FieldByName('loyaltypoints2rationum'));
        OL.LoyaltyPoints2RatioDen := FieldAsCurrency(FieldByName('loyaltypoints2ratioden'), 1);
        OL.LoyaltyPoints2Qty := FieldAsCurrency(FieldByName('loyaltypoints2qty'));

        if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem)) then begin
          OL.LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, OL.ItemID);
          AllocateItemLoyalty(OL);
        end;

        OL.GLCode := FieldAsString(FieldByName('glcode'));
        OL.GLCode2 := FindFieldAsString(FindField('glcode2'));

        if (Assigned(lItemOL) and (OL.ModifierID <> 0)) then
        begin
          FilterQuery(dm.qrModifiers, qtModifiersCurrent);
          dm.qrModifiers.Locate('modifierid', IntToStr(ol.ModifierID), []);
          if (not dm.qrModifiers.FieldByName('modifierid').IsNull) then
          begin
            OL.ModQty := dm.qrModifiers.FieldByName('modqty').AsDouble;
            OL.ModQtyOption := dm.qrModifiers.FieldByName('modqtyoption').AsInteger;
          end;

          if (OL.ModQtyOption = 1) and (lItemOL.WeighedItem) then
          begin
            OL.ModQtyOrdered := 1;
            lItemOL.Qty := lItemOL.Qty + OL.ModQty;
          end
          else if (OL.ModQtyOption = 0) then
          begin
            OL.ModQtyOrdered := FieldByName('qty').AsDouble;  
            OL.ModQtyIncluded := FieldByName('includedmodqty').AsDouble;
          end;
        end;

        OLList.Add(OL);

        Next;
      end;
    end;
    LoadOLGrid;

    SetQuery(qtParkedSalesTenderLines);

    with dm.qrPSTenderLines do begin
      First;
      while (not EOF) do begin
        TL := TLList.NewTL;

        TL.TLTypeID := FieldByName('tenderlinetypeid').AsInteger;
        TL.TLType := GetTLT(FieldByName('tenderlinetypeid').AsInteger);
        TL.TLNotes := FieldByName('tlnotes').AsString;
        TL.TLPayment := FieldByName('tlpayment').AsCurrency;
        TL.TLTip := FieldByName('tltip').AsCurrency;
        TL.TLChange := FieldByName('tlchange').AsCurrency;
        TL.TLSurcharge := 0;
        TL.TLTender := FieldByName('tltender').AsCurrency;
        TL.TLRounding := FieldByName('tlrounding').AsCurrency;
        TL.SurchargeItemID := 0;
        TL.SurchargeItemAbbrev := '';
        TL.SurchargePercent := 0;
        TL.SurchargeFixed := 0;
        TL.SurchargeSalesTaxPercent := 0;
        TL.SurchargeGLCode := '';
        TL.SurchargeGLCode2 := '';
        LoadTLs(LAAdd);

        Next;
      end;
    end;

    TLList.RecalculateTotals(FPayment, FTip, FChange, FTender, FRounding);
    CalculateToPay;

    BroadcastCDAMessage;

    DeleteParkedSale(GlbParkedSale.ParkedSaleID);

    //dm.tr.SavePoint;
    dm.tr.Commit;   //Jon 10-07-2003
    Result := True;
  except
    on e: exception do begin
      ShowMessage(e.message);
      dm.tr.Rollback;
      SelectFirsts;
    end;
  end;
end;
{******************************************************************************}
procedure TFormQuickSaleUser.SetCurrentTime;
//This is used to mark the time at the start of the sale
begin
  DecodeTime(Now, CurrentHour, CurrentMin, CurrentSec, CurrentMSec);    //Jon 24-06-2002
  CurrentTime := ((CurrentHour * 100) + CurrentMin);
  CurrentDay := (DayOfTheWeek(Now));
  if PrevTime <> CurrentTime then
  begin
    PrevTime := CurrentTime;
    if (AppDetails.UseInactiveItems > 0) then
      SetInactiveItems;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.txtNextQtyEnter(Sender: TObject);
begin
  inherited;
  if (Sender is TEdit) then begin
    CurrentSelectedTEdit := TEdit(Sender);
    CurrentSelectedTEditText := TEdit(Sender).Text;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.tmrBarCodeTimer(Sender: TObject);
begin
  inherited;
  tmrBarCode.Enabled := False;
  txtNextQtyEnter(CurrentSelectedTEdit);
end;
{******************************************************************************}
procedure TformQuickSaleUser.BuildTenderSurchargeList;
var
  I, J: Integer;
  SurchargeItem: TSurchargeItem;
  TL: TTL;
begin
  for I := (SurchargeList.Count-1) downto 0 do begin
    SurchargeItem := SurchargeList.Items[I];
    if (SurchargeItem.SurchargeType = stTenderType) then begin
      SurchargeItem.Free;
      SurchargeList.Delete(I);
    end;
  end;

  for J := 0 to (TLList.Count-1) do begin
    TL := TLList.Items[J];
    if (TL.SurchargeItemID > 0) then begin
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
        SurchargeItem.GLCode := TL.SurchargeGLCode;
        SurchargeItem.GLCode2 := TL.SurchargeGLCode2;
        SurchargeList.Add(SurchargeItem);
      end;
      SurchargeItem.Amount := GetRoundedUpDown(SurchargeItem.Amount + TL.TLSurcharge, AppDetails.DecimalPlaces);
    end;
  end;

  FSurcharge := 0.00;
  for I := (SurchargeList.Count-1) downto 0 do begin
    SurchargeItem := SurchargeList.Items[I];
    FSurcharge := GetRoundedUpDown(FSurcharge + SurchargeItem.Amount, AppDetails.DecimalPlaces);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddCashTender(TLTender: Currency; UsingRequired: Boolean = False);
var
  I: Integer;
  AmountDue: Currency;
  TLPayment: Currency;
  TLChange: Currency;
  TLSurcharge: Currency;
  TLRounding: Currency;
  TL: TTL;
begin
  if (OLList.Count > 0) then begin
    AmountDue := GetDue(True);
    if (UsingRequired) and (GlbAccount.AccountID > 0) then
      AmountDue := AmountDue - (GlbAccount.CreditLimit - GlbAccount.CurrentBalance);

    if (TLTender > AmountDue) then begin
      TLPayment := RoundToRounding(AmountDue, RoundForTLType(TLTCash));
    end
    else begin
      TLPayment := TLTender;
    end;
    TLChange := TLTender - TLPayment;

    TLSurcharge := 0;

    TLRounding := TLPayment + TLChange - TLTender;

    TL := nil;
    for I := 0 to (TLList.Count - 1) do begin
      TL := TLList.Items[I];

      if ((TL.TLType = TLTCash) or (TLPayment < 0)) then begin
        Break;
      end
      else begin
        TL := nil;
      end;
    end;
    if (Assigned(TL)) then begin
      TL.TLPayment := GetRoundedUpDown(TL.TLPayment + TLPayment, AppDetails.DecimalPlaces);
      TL.TLTender := GetRoundedUpDown(TL.TLTender + TLTender, AppDetails.DecimalPlaces);
      TL.TLRounding := GetRoundedUpDown(TL.TLRounding + TLRounding, AppDetails.DecimalPlaces);
      TL.TLChange := GetRoundedUpDown(TL.TLChange + TLChange, AppDetails.DecimalPlaces);
      TL.TLSurcharge := GetRoundedUpDown(TL.TLSurcharge + TLSurcharge, AppDetails.DecimalPlaces);
      LoadTLs(LAEdit);
    end
    else begin
      TL := TLList.NewTL;
      TL.TLTypeID := GetTLTID(TLTCash);
      TL.TLType := TLTCash;
      TL.TLNotes := '';
      TL.TLPayment := GetRoundedUpDown(TLPayment, AppDetails.DecimalPlaces);
      TL.TLTip := 0.00;
      TL.TLChange := GetRoundedUpDown(TLChange, AppDetails.DecimalPlaces);
      TL.TLSurcharge := GetRoundedUpDown(TLSurcharge, AppDetails.DecimalPlaces);
      TL.TLTender := GetRoundedUpDown(TLTender, AppDetails.DecimalPlaces);
      TL.TLRounding := GetRoundedUpDown(TLRounding, AppDetails.DecimalPlaces);
      TL.SurchargeItemID := 0;
      TL.SurchargeItemAbbrev := '';

      LoadTLs(LAAdd);
    end;
    CalculateToPay;
    if ((AppDetails.CashSaleAutoComplete) and (FStillDue = 0)) then begin
      DoingTransaction := True;
      Application.ProcessMessages;
      DoingTransaction := False;
      if (FDoingRefund) then begin
        DoCashSaleRefund(False);
      end
      else begin
        DoCashSale(False,6);
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCash5Click(Sender: TObject);
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;
  AddCashTender(5);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCash10Click(Sender: TObject);
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;
  AddCashTender(10);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCash20Click(Sender: TObject);
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;
  AddCashTender(20);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdCash50Click(Sender: TObject);
begin
  if ((DoingTransaction) or (ButtonsDeactivated)) then begin    //Jon 25-02-2002
    Exit;
  end;
  AddCashTender(50);
end;
{******************************************************************************}
procedure TformQuickSaleUser.ChangeCurrentOrdersPriceLevel(NewPriceLevel: Integer);
var
  I: Integer;
  OL: TOrderLine;
  OldRow: Integer;
begin
  OldRow := grdOLs.Row;
  for I := (OLList.Count - 1) downto 0 do begin
    OL := OLList.Items[I];
    if ((OL.ItemID <> 0) and (OL.OLPriceLevel < 1) and (not OL.OpenPrice)) then begin
      grdOLs.Row := I;
      GlbItemModifiers.AddingItem := False;     //Jon 24-12-2003
      LoadMods;

      OL.UnitPrice := OL.ItemPrices[NewPriceLevel];

      //if not AppDetails.EnableFiscalPrinting then
      begin
      if ((AppDetails.TaxExclusivePrices) and (not OL.PriceExcludesTax)) then begin        //Jon 26-6-2001
        OL.UnitPrice := ((OL.UnitPrice * 100) / (100 + OL.SalesTaxPercent));              //Jon 26-6-2001
      end
      else if ((not AppDetails.TaxExclusivePrices) and (OL.PriceExcludesTax)) then begin   //Jon 26-6-2001
        OL.UnitPrice := ((OL.UnitPrice * (100 + OL.SalesTaxPercent)) / 100);              //Jon 26-6-2001
      end;
      end;

      OL.OriginalUnitPrice := OL.UnitPrice;
      OL.ToPay := OL.UnitPrice * OL.Qty;
      OL.ToPay := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);

      if ((OL.LoyaltyItemID > 0) and (OL.LoyaltyFreeQty > 0)) then begin
        grdOLs.Cells[3, grdOLs.Row] := '* ' + FormatCurrencyNoSign(((OL.Qty - OL.LoyaltyFreeQty) / OL.Qty) * OL.ToPay);
      end
      else begin
        grdOLs.Cells[3, grdOLs.Row] := FormatCurrencyNoSign(OL.ToPay);
      end;
      ClearMods;
      AddMods;
      DisposeMods;

      if (AppDetails.PDEnabled) then
      begin
        CalculateToPay;
        DMComponents.PDOutputItemLine(OL.ItemString, GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces), 0, False);
        DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
      end;
    end;
  end;
  grdOLs.Row := OldRow;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ChangeAllowCreditLabel(AllowCredit: Boolean);
begin
  if (AllowCredit) then begin
    lblAllowCredit.Caption := sCreditAllowed;
    lblAllowCredit.Font.Color := clRed;

{ to prevent double pin check we dont verify pin here as:
    outside of fiscal we have already checked pin on TryLookupCashSaleAccount and LoadAccount,
    with fiscal we dont seem to check pin on SetAccount so dont check it on charge rank either

  if you can't charge to an account if you can't USE it.
  }
    if (GlbLogin.RankAccount < GlbAccount.AccountChargeRank) then begin
       {if ((AppDetails.AccChargePinCheckRank <= GlbAccount.AccountChargeRank) and (not VerifyPinNo)) then begin }
      SetOnAccount(False);
      cmdOnAccount.Visible := False;
      lblAllowCredit.Caption := sCreditAllowedNotPriv;
      Exit;
    end
    else if (GlbTable.SaleCategoryType > scStandard){ and ((not OnTable) or (GlbTable.Uninvoiced = 0))} then
    begin
      SetOnAccount(False);
      cmdOnAccount.Visible := True;
    end
    else begin
      cmdOnAccount.Visible := True;
      SetOnAccount (AppDetails.CashSaleAutoSelectPayOnAccount);   //Jon 20-10-2003
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
end;
{******************************************************************************}
procedure TformQuickSaleUser.ChangeHeldGroup(HeldGroupID: Integer; HeldGroupName: string);
begin
{  FHeldGroupID := HeldGroupID;
  if (HeldGroupID > 0) then begin
    Skin.ChangeButton(cmdDestn, bstGen80, gstNoChange, biNone, HeldGroupName);
  end
  else begin
    Skin.ChangeButton(cmdDestn, bstWhere, gstNoChange, biDestination, 'Where');
  end; }
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdOnAccountClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;

  SetOnAccount(not OnAccount);
  BroadcastCDAMessage;
  StartInactiveTimer;
end;
{******************************************************************************}
function TformQuickSaleUser.DoPhoneOrder(Enable: Boolean; PhoneOrderMode: Boolean): Boolean;
var
  StartedPhoneOrder: Boolean;
  ReturnedFromCancel: Boolean;
begin
  Result := False;
  ReturnedFromCancel := False;

  if ((not Enable) and (not DoingPhoneOrder)) then begin
    Exit;
  end;

  try
    if (Enable) then
    begin
      repeat
        EatInTakeawayPromptValue := 0;
        StartedPhoneOrder := False;
        if (AppDetails.PhoneOrderInstructionOrder = 0) then
        begin
  //Eat In Takeaway
          if (not POEatInTakeaway(ReturnedFromCancel)) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            if ((GlbCustomer.PerorgID > 0) and (GlbCustomer.AccountID = GlbAccount.AccountID)) then begin
               DeselectAccount;
            end;
            if (PhoneOrderMode) then begin
              IsShowing := False;
              PostMessage(Self.Handle, WM_CLOSE, 0, 0);
            end;
            Exit;
          end;

          ReturnedFromCancel := False;
  //Select Customer
          if (not POSelectCustomer(0)) then
          begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            ReturnedFromCancel := True;
            Continue;
          end;

  //Check for an existing Tab for this Customer (WhozzCalling Only)
          if (AppDetails.CallerIDSelected) and (AppDetails.CallerIDType = ctWhozzCalling) then begin
            DoSelectTable;
            if (GlbTable.GroupID <> 0) then begin
              DoingPhoneOrder := False;
              AppDetails.CallerIDSelected := False;
              Exit;
            end;
          end;

  //Select Remote Location
          if (not POSelectRemoteLocation) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            Continue;
          end;

  //Set it all up
          if (not POSetUp) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            Continue;
          end;

          DoingPhoneOrder := True;
          AppDetails.CallerIDSelected := False;
          StartedPhoneOrder := True;
          PhoneOrderPersistant := PhoneOrderMode;
          Result := True;
        end
        else begin
  //Select Customer
          if not (AppDetails.PhoneOrderInstructionOrder > 1) then begin
            if (not POSelectCustomer(0)) then begin
              DoingPhoneOrder := False;
              AppDetails.CallerIDSelected := False;
              if (PhoneOrderMode) then begin
                IsShowing := False;
                PostMessage(Self.Handle, WM_CLOSE, 0, 0);
              end;
              Exit;
            end;

   //Check for an existing Tab for this Customer (WhozzCalling only)
            if (AppDetails.CallerIDSelected) and (AppDetails.CallerIDType = ctWhozzCalling) then begin
              DoSelectTable;
              if (GlbTable.GroupID <> 0) then begin
                DoingPhoneOrder := False;
                AppDetails.CallerIDSelected := False;
                Exit;
              end;
            end;
          end;

  //Eat In Takeaway
          if (not POEatInTakeaway(ReturnedFromCancel)) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            if ((GlbCustomer.PerorgID > 0) and (GlbCustomer.AccountID = GlbAccount.AccountID)) then begin
               DeselectAccount;
            end;
            if (AppDetails.PhoneOrderInstructionOrder > 1) then begin
              if (PhoneOrderMode) then begin
                IsShowing := False;
                PostMessage(Self.Handle, WM_CLOSE, 0, 0);
              end;
              Exit;
            end
            else begin
              Continue;
            end;
          end;

          ReturnedFromCancel := False;

  //Get Delivery details
          if ((EatInTakeawayPromptValue = 5) and (AppDetails.PhoneOrderInstructionOrder < 3)) then begin
            if (not POCheckDeliveryDetails) then begin
              DoingPhoneOrder := False;
              AppDetails.CallerIDSelected := False;
              ReturnedFromCancel := True;
              Continue;
            end;
          end;
          {else
            ClearCustomerDeliveryDetails;}

  //Select Remote Location
          if (not POSelectRemoteLocation) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            ReturnedFromCancel := True;
            Continue;
          end;

  //Set it all up
          if (not POSetUp) then begin
            DoingPhoneOrder := False;
            AppDetails.CallerIDSelected := False;
            Continue;
          end;

          DoingPhoneOrder := True;
          StartedPhoneOrder := True;
          PhoneOrderPersistant := PhoneOrderMode;
          Result := True;
        end;
      until ((StartedPhoneOrder) or (not PhoneOrderMode));
    end
    else begin
      if ((DoingPhoneOrder) and (GlbCustomer.PerorgID > 0) and (GlbCustomer.AccountID = GlbAccount.AccountID)) then begin
        DeselectAccount;
      end;
      DoingPhoneOrder := False;
      AppDetails.CallerIDSelected := False;
    end;
  finally
    cmdPhoneOrder.Down := DoingPhoneOrder;
    cmdRefund.Enabled := (not DoingPhoneOrder);

    cmdToPayCash.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));
    cmdToPayEFTPOS.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTEFTPOS).TenderTypeEnabled));
    cmdCash50.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));
    cmdCash20.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));
    cmdCash10.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));
    cmdCash5.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));

    cmdCheque.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCheque).TenderTypeEnabled));
    cmdVoucher.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTVoucher).TenderTypeEnabled));
    cmdOther.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTOther).TenderTypeEnabled));
    cmdEFTPOS.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTEFTPOS).TenderTypeEnabled));
    cmdCash.Enabled := (((not DoingPhoneOrder) or (AppDetails.EnablePhoneOrders = 2)) and (GetTenderLineTypeRecord(TLTCash).TenderTypeEnabled));

    if (DoingPhoneOrder) then begin
//      Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
      Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
      cmdQuickCash.Tag := 0;

      if (AppDetails.CashSaleUseTabNotes) then begin
//        Skin.ChangeButton(cmdDestn, bstNote, gstNoChange, biNone, sNote);
      end;
    end
    else begin
      //Return Sale Category to default
      if not (OnTable) then
      if ((AppDetails.QSSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.QSSaleCategoryID))) then begin
        if ((AppDetails.CashSaleCategory = '') or (not SelectSaleCategory(AppDetails.CashSaleCategory))) then begin
          if ((AppDetails.DefaultSaleCategoryID = 0) or (not SelectSaleCategory('', AppDetails.DefaultSaleCategoryID))) then begin
            //Well we didn't find one
          end;
        end;
      end;
      if ((GlbTable.SaleCategoryType >= scDriveThru) xor (OnTable)) then begin
//        Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
        Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
        cmdQuickCash.Tag := 0;
      end
      else if (cmdQuickCash.Tag <> 1) then begin
//        Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
        Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
        cmdQuickCash.Tag := 1;
      end;

      if (AppDetails.CashSaleUseTabNotes) then begin
//        Skin.ChangeButton(cmdDestn, bstWhere, gstNoChange, biDestination, sWhere);
      end;
    end;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.POEatInTakeaway(ReturnedFromCancel: Boolean): Boolean;
var
  OldEatInTakeawayValue: Integer;
begin
  Result := False;
  OldEatInTakeawayValue := EatInTakeawayPromptValue;
  EatInTakeawayPromptValue := EatInTakeawayPrompt(True, ReturnedFromCancel);

  if (EatInTakeawayPromptValue = 0) then begin
    Exit;
  end;
  if (((EatInTakeawayPromptValue = 1) and (AppDetails.EatInSectionID <= 0)) or
      ((EatInTakeawayPromptValue = 4) and (AppDetails.PickupSectionID <= 0)) or
      ((EatInTakeawayPromptValue = 5) and (AppDetails.DeliverySectionID <= 0))) then begin
    ShowQuickMsg(sNoSecForPhOrder);
    EatInTakeawayPromptValue := OldEatInTakeawayValue;
    DoingPhoneOrder := False;
    AppDetails.CallerIDSelected := False;
    Exit;
  end;
  Result := True;
end;
{******************************************************************************}
function TformQuickSaleUser.POSelectCustomer(PerorgID: Integer): Boolean;
var
  CD: Integer;
  PotentiallyClearAccount: Boolean;
begin
  Result := False;

  PotentiallyClearAccount := ((DoingPhoneOrder) and (GlbCustomer.PerorgID > 0) and (GlbCustomer.AccountID = GlbAccount.AccountID));

  case EatInTakeawayPromptValue of
    5: CD := 1;      //We are doing delivery, so get and check delivery details
    0: CD := 2;      //We don't know what we're doing yet, so just get the defaults for now to potentially check later
  else
    CD := 0;         //We aren't doing delivery so don't get the delivery details
  end;
  
  GlbCustomer.PerorgID := 0;  
  GlbCustomer.MasPerorgID := 0;
  if (SelectCustomer(PerorgID, CD, False)) then
  begin
    Result := True;

    if (GlbCustomer.AccountID > 0) then
    begin
      TryLookupCashSaleAccount(GlbCustomer.AccountID, False);
    end
    else if (PotentiallyClearAccount) then
    begin
      DeselectAccount;
    end;
  end
  else
    GlbCustomer.PerorgID := PerorgID;
    
end;
{******************************************************************************}
function TformQuickSaleUser.POCheckDeliveryDetails: Boolean;
var
  DeliveryDetails: TDeliveryDetails;
  I: Integer;

begin
  Result := False;

  DeliveryDetails.DeliveryName := GlbCustomer.DeliveryName;
  for I := 1 to 3 do begin
    DeliveryDetails.DeliveryAddress[I] := GlbCustomer.DeliveryAddress[I];
  end;
  DeliveryDetails.DeliveryPhone := GlbCustomer.DeliveryPhoneNo;
  DeliveryDetails.DeliveryNotes := GlbCustomer.DeliveryNotes;
  DeliveryDetails.DeliveryName := GlbCustomer.DeliveryName;
  DeliveryDetails.DeliveryStreetNo := GlbCustomer.DeliveryStreetNo;
  DeliveryDetails.DeliveryFlatNo := GlbCustomer.DeliveryFlatNo;
  DeliveryDetails.AddressMapLocationID := GlbCustomer.DeliveryAddressMapLocationID;
  DeliveryDetails.AMLStreetName := GlbCustomer.DeliveryAMLStreetName;
  DeliveryDetails.AMLSuburb := GlbCustomer.DeliveryAMLSuburb;
  DeliveryDetails.AMLCity := GlbCustomer.DeliveryAMLCity;
  DeliveryDetails.AMLPostCode := GlbCustomer.DeliveryAMLPostCode;
  DeliveryDetails.AMLMapPage := GlbCustomer.DeliveryAMLMapPage;
  DeliveryDetails.AMLGridRef1 := GlbCustomer.DeliveryAMLGridRef1;
  DeliveryDetails.AMLGridRef2 := GlbCustomer.DeliveryAMLGridRef2;
  DeliveryDetails.AMLQd := GlbCustomer.DeliveryAMLQd;
  DeliveryDetails.AMLDefaultRemoteLocationID := GlbCustomer.DeliveryAMLDefaultRemoteLocationID;

  if (not CheckDeliveryDetails(DeliveryDetails, True)) then begin
    Exit;
  end;

  GlbCustomer.DeliveryName := DeliveryDetails.DeliveryName;
  for I := 1 to 3 do begin
    GlbCustomer.DeliveryAddress[I] := DeliveryDetails.DeliveryAddress[I];
  end;
  GlbCustomer.DeliveryStreetNo := DeliveryDetails.DeliveryStreetNo;
  GlbCustomer.DeliveryFlatNo := DeliveryDetails.DeliveryFlatNo;
  GlbCustomer.DeliveryPhoneNo := DeliveryDetails.DeliveryPhone;
  GlbCustomer.DeliveryNotes := DeliveryDetails.DeliveryNotes;
  GlbCustomer.DeliveryAddressMapLocationID := DeliveryDetails.AddressMapLocationID;
  GlbCustomer.DeliveryAMLStreetName := DeliveryDetails.AMLStreetName;
  GlbCustomer.DeliveryAMLSuburb := DeliveryDetails.AMLSuburb;
  GlbCustomer.DeliveryAMLPostCode := DeliveryDetails.AMLPostCode;
  GlbCustomer.DeliveryAMLCity := DeliveryDetails.AMLCity;
  GlbCustomer.DeliveryAMLMapPage := DeliveryDetails.AMLMapPage;
  GlbCustomer.DeliveryAMLGridRef1 := DeliveryDetails.AMLGridRef1;
  GlbCustomer.DeliveryAMLGridRef2 := DeliveryDetails.AMLGridRef2;
  GlbCustomer.DeliveryAMLQd := DeliveryDetails.AMLQd;
  GlbCustomer.DeliveryAMLDefaultRemoteLocationID := DeliveryDetails.AMLDefaultRemoteLocationID;

  Result := True;
end;
{******************************************************************************}
function TformQuickSaleUser.POSelectRemoteLocation: Boolean;
var
  DefRemLocID: Integer;

begin
  Result := False;
  if ((TestRemoteLocationOption(rlsoMaster)) and (AppDetails.PhoneOrderUseRemLoc)) then begin
    DefRemLocID := GlbCustomer.AMLDefaultRemoteLocationID;
    if (EatInTakeawayPromptValue = 5) then begin
      DefRemLocID := GlbCustomer.DeliveryAMLDefaultRemoteLocationID;
    end;
    if ((GlbRemoteLocation.RemoteLocationID > 0)) then begin
      DefRemLocID := GlbRemoteLocation.RemoteLocationID;
    end;

    if (not SelectRemoteLocation(DefRemLocID)) then begin
      Exit;
    end;

    if (AppDetails.ROGItemExists and not (DefRemLocID = GlbRemoteLocation.RemoteLocationID)) then begin
      FilterQuery(dm.qrOBItems, qtOItemsB);
      FilterQuery(dm.qrOFItems, qtOItemsF);
      FilterQuery(dm.qrOItemGroups, qtOItemGroups);
      GlobalMenuList.ReloadItems(0);
      GlobalMenuList.ReloadQSItems;
      OpenArch.Free;
      pnlFood.DestroyComponents;
      pnlBev.DestroyComponents;
      BuildItemGroups;
      BuildItems;
    end;
  end;
  Result := True;
end;
{******************************************************************************}
function TformQuickSaleUser.POSetUp: Boolean;
var
  NewSaleCatID: Integer;

begin
  Result := False;
  NewSaleCatID := 0;
  case EatInTakeawayPromptValue of
    1: if (AppDetails.EatInSaleCategoryID > 0) then NewSaleCatID := AppDetails.EatInSaleCategoryID;
    4: if (AppDetails.PickupSaleCategoryID > 0) then NewSaleCatID := AppDetails.PickupSaleCategoryID;
    5: if (AppDetails.DeliverySaleCategoryID > 0) then NewSaleCatID := AppDetails.DeliverySaleCategoryID;
  else
    Exit;
  end;

  if ((NewSaleCatID > 0) and (not SetSaleCategory(NewSaleCatID))) then begin
    DoingPhoneOrder := False;
    Exit;
  end;
  Result := True;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdLoyaltyClick(Sender: TObject);
begin
  inherited;

  //if AppDetails.wiGroupEnabled then
  //  wiGroupPOSSrv.CheckWarnLoyaltyDiscNotAllowed;

  if ((not AppDetails.OrderDiscountEnable) and ((GlbTable.GroupID > 0) or (DoingPhoneOrder))) then
  begin
    ShowQuickMsg(sDiscountOnOrderIsNotEnabled);
    StartInactiveTimer;
    Exit;
  end;
  if (not CheckLoyaltyReward(False)) then
  begin
    StartInactiveTimer;
    Exit;  //user cancels the select loyalty prompt, thus no need to recalulate discounts
  end;

  CalculateDiscountSchemeItemDiscount(True); //RPC using this to clear out discounts
  CalculateDiscount;// show discounts
  FOrigDiscountPercent := GlbAccount.AccountDiscount; //Reset discount % back to account default
//  CalculateDiscountPercent;
  CalculateToPay;
  if CDAServer.Connected then
    CDAServer.CDAData.ShowAccountDetails := False;
  BroadcastCDAMessage;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdPhoneOrderClick(Sender: TObject);
var
  PhoneOrderResult: Integer;
  OldEatInValue: Integer;
begin
  inherited;
  try
    StopInactiveTimer;

    if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
      Exit;

    if ((FDoingRefund) or (DoingWaste)) then begin
      ShowMsg(sNoPhOrderWhenRefundOrWastage);
      StartInactiveTimer;
      Exit;
    end;
    if (OnTable) then begin
       if (ShowConf(sCancelTabSelection) = mrYes) then begin
         CancelCashSale;
       end
       else begin
         StartInactiveTimer;
         Exit;
       end;
    end;
    if (RSC.AllowUpdate) then begin
      ShowMsg(sClearReplacementSwipeCard);
      cmdPhoneOrder.Down := False;
      StartInactiveTimer;
      Exit;
    end;
    PauseStaffPromptTimeoutTimer;

    if (DoingPhoneOrder) then begin
      PhoneOrderResult := DialogPhoneOrder(EatInTakeawayPromptValue = 5);
      if (PhoneOrderResult > 0) then begin
        case PhoneOrderResult of
          1: begin
            OldEatInValue := EatInTakeawayPromptValue;
            if (POEatInTakeaway(False)) then begin
              if ((EatInTakeawayPromptValue <> OldEatInValue) and (EatInTakeawayPromptValue = 5)) then begin
                if (not POSelectCustomer(GlbCustomer.PerorgID)) then begin
                  EatInTakeawayPromptValue := OldEatInValue;
                end;
              end;
            end;
          end;
          2: POSelectCustomer(GlbCustomer.PerorgID);
          3: POCheckDeliveryDetails;
          4: POSelectRemoteLocation;
          5: DoPhoneOrder(False, False);
        end;
      end;
    end
    else begin
      DoPhoneOrder(True, False);
    end;
  finally
    cmdPhoneOrder.Down := DoingPhoneOrder;
    ResetStaffPromptTimeoutTimer;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.TestAllOLsLoyaltyItem(FindNewOnly: Boolean);
var
  I: Integer;
  ListCount: Integer;
  aOL: TOrderLine;
begin
  ListCount := (OLList.Count - 1);
  if (not FindNewOnly) then begin
    LoyaltyReward.AvailableQty := LoyaltyReward.Qty;
  end;

  for I := 0 to ListCount do begin
    aOL := OLList.Items[I];

    if (aOL.ComboID > 0) then
      Continue;

    if (aOL.ItemID > 0) then begin
      if (not FindNewOnly) then begin
        aOL.LoyaltyItemID := -1;
        aOL.LoyaltyFreeQty := 0;
        aOL.LoyaltyFreeDiscount := 0;
      end;
      if ((LoyaltyReward.RewardOffer = roItem) and (aOL.LoyaltyFreeQty < aOL.Qty) and ((aOL.ItemID > 0) or (aOL.LoyaltyItemID > 0))) then begin
        if (aOL.LoyaltyItemID <= 0) then begin
          aOL.LoyaltyItemID := TestLoyaltyItem(LoyaltyReward.LoyaltyRewardID, aOL.ItemID);
        end
        else begin
          LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty + aOL.LoyaltyFreeQty;
        end;
        AllocateItemLoyalty(aOL);
        if (aOL.LoyaltyFreeQty > 0) then begin
          grdOLs.Cells[3, I] := '* ' + FormatCurrencyNoSign(((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.ToPay);
        end
        else begin
          grdOLs.Cells[3, I] := FormatCurrencyNoSign(aOL.ToPay);
        end;
      end;
    end;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.TestLoyaltyItem(LoyaltyRewardID: Integer; ItemID: Integer): Integer;
begin
  try
    with dm.sp do begin
      SetStoredProcName('TEST_LOYALTYITEM');    //test_loyaltyitem

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
function TformQuickSaleUser.AllocateItemLoyalty(var OL: TOrderLine): Boolean;
var
  OldLoyaltyFreeQty: Currency;
  OldLoyaltyFreeDiscount: Currency;
begin
  Result := False;
  if (OL.ComboID > 0) then
    Exit;  //Don't apply Free Item to Item belonging to a combo

  OldLoyaltyFreeQty := OL.LoyaltyFreeQty;
  OldLoyaltyFreeDiscount := OL.LoyaltyFreeDiscount;
  OL.LoyaltyFreeQty := 0;
  OL.LoyaltyFreeDiscount := 0;

  if ((LoyaltyReward.LoyaltyRewardID > 0) and (LoyaltyReward.RewardOffer = roItem) and (LoyaltyReward.AvailableQty > 0) and (OL.LoyaltyItemID > 0)) then begin
    if (LoyaltyReward.AvailableQty > OL.Qty) then begin
      OL.LoyaltyFreeQty := OL.Qty;
    end
    else begin
      OL.LoyaltyFreeQty := LoyaltyReward.AvailableQty;
    end;
    OL.LoyaltyFreeDiscount := GetRoundedUpDown(OL.LoyaltyFreeQty * OL.UnitPrice, AppDetails.DecimalPlaces);
    LoyaltyReward.AvailableQty := LoyaltyReward.AvailableQty - OL.LoyaltyFreeQty;
  end;

  Result := ((OL.LoyaltyFreeQty <> OldLoyaltyFreeQty) or (OL.LoyaltyFreeDiscount <> OldLoyaltyFreeDiscount));
end;
{******************************************************************************}
procedure TformQuickSaleUser.ReDisplayOLGrid;
var
  I: Integer;
  aOL: TOrderLine;

begin
  grdOLs.Perform(WM_SETREDRAW, 0, 0);
  try
    // add to grid again
    with grdOLs do
    begin
      for I := 0 to ColCount - 1 do
      begin
        Cells[I, 0] := '';
      end;
    end;

    if (OLList.Count > 0) then
    begin
      grdOLs.RowCount := OLList.Count;
    end
    else
    begin
      grdOLs.RowCount := 1;
    end;

    for I := 0 to OLList.Count - 1 do
    begin
      aOL := OLList.Items[I];

      UpdateOLGridRow(I, aOL);
    end;
  finally
    grdOLs.Perform(WM_SETREDRAW, 1, 0);
    grdOLs.Invalidate;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.LoadOLGrid;
begin
  ReDisplayOLGrid;
  grdOLs.Row := (grdOLs.RowCount - 1);
end;
{******************************************************************************}
procedure TformQuickSaleUser.UpdateOLGridRow(aRow: Integer; aOL: TOrderLine);
begin
  with grdOLs do
  begin
    Cells[0, aRow] := '';     // course
    Cells[1, aRow] := '';     // qty
    Cells[2, aRow] := '';     // itemname
    Cells[3, aRow] := '';     // price

    if (Assigned(aOL)) then
    begin
      if (aOL.ItemID > 0) then
      begin
        if (aOL.CourseID <> 0) then
        begin
          Cells[0, aRow] := aOL.CourseName;
        end;

        OrderingQty := aOL.Qty;   //set qty, abbrev unitprice and price
        Cells[1, aRow] := FormatFloat(QTYFORMAT, OrderingQty);   // this much unit of items in kg
        if AppDetails.EnableFiscalPrinting then
        begin
          Cells[1, aRow] := FormatFloat(QTYFORMAT, WBRoundTo(OrderingQty, 3));
        end;

        Cells[2, aRow] := aOL.ItemString;

        if ((aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0)) then
        begin
          Cells[3, aRow] := '* ' + FormatCurrencyNoSign(((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.ToPay);
          if AppDetails.EnableFiscalPrinting then
          begin
            Cells[3, aRow] := '* ' + FormatCurrencyNoSign(WBRoundTo(((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.ToPay, 2));
          end;
        end
        else
        begin
          Cells[3, aRow] := FormatCurrencyNoSign(aOL.ToPay);
          if AppDetails.EnableFiscalPrinting then
          begin
            Cells[3, aRow] := FormatCurrencyNoSign(WBRoundTo(aOL.ToPay, 2));
          end;
        end;
        if ((AppDetails.EnableFiscalPrinting) and
            (not (NFManual or NFEMode)) and
            (not DoingPhoneOrder) and
            ((not (OnTable and (GlbTable.GroupID > 0))) or (GlbTable.SaleCategoryType >= scDriveThru))) then
        begin
          if ((aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0)) then
          begin
            Cells[3, aRow] := '* ' + FormatCurrencyNoSign(WBRoundTo(((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.DiscountedAmount, 2));
          end
          else
          begin
            Cells[3, aRow] := FormatCurrencyNoSign(WBRoundTo(aOL.DiscountedAmount, 2));
          end;
          Application.ProcessMessages;
          if ((aOL.ItemID > 0) and ((aOL.Qty - aOL.LoyaltyFreeQty) > 0) and (aOL.UnitPrice > 0)) then
          begin
            if aOL.DiscountSchemeItemAmount > 0 then
            begin
              if ((aOL.LoyaltyItemID > 0) and (aOL.LoyaltyFreeQty > 0)) then
              begin
                Cells[3, aRow] := '* ' + FormatCurrencyNoSign(WBRoundTo((((aOL.Qty - aOL.LoyaltyFreeQty) / aOL.Qty) * aOL.ToPay) - aOL.DiscountSchemeItemAmount, 2));
              end
              else
              begin
                Cells[3, aRow] := FormatCurrencyNoSign(WBRoundTo(aOL.ToPay - aOL.DiscountSchemeItemAmount, 2));
              end;
            end;
          end;
        end;
      end
      else
      begin
        Cells[2, aRow] := ModWithModFunction(aOL.ItemString, aOL.ModifierFunction, aOL.ModQtyOrdered);
      end;
    end;
    Invalidate;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.TemporarilyDeactiveButtons;
begin
  tmrDeactivateButtons.Enabled := False;
  ButtonsDeactivated := True;
  tmrDeactivateButtons.Interval := 500;
  tmrDeactivateButtons.Enabled := True;
end;
{******************************************************************************}
procedure TformQuickSaleUser.CombineOrderLines;
var
  OL: TOrderLine;
  NextOL: TOrderLine;
  FindOL: TOrderLine;
  NextFindOL: TOrderLine;
  I, J: Integer;
  GI: Integer;
  bAddItem: Boolean;
begin
  I := 0;
  while (I < OLList.Count - 1) do begin
    OL := OLList.Items[I];
    if ((OL.ItemID > 0) and (not OL.OpenPrice) and (OL.LoyaltyItemID < 0) and (not OL.AlreadyOrdered)) then begin
      NextOL := OLList.Items[I + 1];
      if (NextOL.ItemID > 0) then begin
        J := OLList.Count - 1;
        while (J > I) do begin
          FindOL := OLList.Items[J];
          if ((FindOL.ItemID > 0) and (not FindOL.OpenPrice) and (FindOL.LoyaltyItemID < 0) and (not FindOL.AlreadyOrdered)) then begin
            NextFindOL := nil;
            if (J < (OLList.Count - 1)) then begin
              NextFindOL := OLList.Items[J + 1];
            end;
            if ((not Assigned(NextFindOL)) or (NextFindOL.ItemID > 0)) then
            begin
              bAddItem := (AppDetails.QSCombineWeigedItems) or (not((not AppDetails.QSCombineWeigedItems) and (OL.WeighedItem)));

              if ((bAddItem) and
                  (OL.ItemID = FindOL.ItemID) and
                  (OL.ComboID = FindOL.ComboID) and (OL.ComboIndex = FindOL.ComboIndex) and (OL.ComboGroupID = FindOL.ComboGroupID) and
                  (OL.ItemString = FindOL.ItemString) and
                  (OL.PrinterID = FindOL.PrinterID) and
                  (OL.RepeatPrinterID = FindOL.RepeatPrinterID) and
                  (OL.UnitPrice = FindOL.UnitPrice) and
                  (OL.CourseID = FindOL.CourseID)) then
              begin
                //We want to combine the found OL with the original OL
                OL.Qty := OL.Qty + FindOL.Qty;
                OL.ToPay := (OL.ToPay + FindOL.ToPay);
                OL.AllowedDiscount := (OL.AllowedDiscount + FindOL.AllowedDiscount);
                OL.ComboQty := OL.ComboQty + FindOL.ComboQty;
                OL.HostSubsidy := OL.HostSubsidy + FindOL.HostSubsidy;
                OL.GuestSubsidy := OL.GuestSubsidy + FindOL.GuestSubsidy;
                FindOL.Free;
                OLList.Delete(J);
                with grdOLs do begin
                  Cells[1, I] := FormatFloat(QTYFORMAT, OL.Qty);
                  Cells[3, I] := FormatCurrencyNoSign(OL.ToPay);

                  for GI := J to (OLList.Count - 1) do begin
                    Cells[0, GI] := Cells[0, GI + 1];
                    Cells[1, GI] := Cells[1, GI + 1];
                    Cells[2, GI] := Cells[2, GI + 1];
                    Cells[3, GI] := Cells[3, GI + 1];
                  end;
                  RowCount := RowCount - 1;
                end;
              end;
            end;
          end;
          Dec(J);
        end;
      end;
    end;
    Inc(I);
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ManualBarCodeEntry;
var
  ScannedCode: string;
  ItemCode: string;
  PriceCode: string;
  ScannedCodeLen: Integer;
  BarCodePrefixLen: Integer;
begin
  ScannedCode := '';
  while (EnterBarCode(ScannedCode)) do begin
    ScannedCodeLen := Length(ScannedCode);
    BarCodePrefixLen := Length(AppDetails.BarCodePrefix);
    if ((Copy(ScannedCode, 1, BarCodePrefixLen) = AppDetails.BarCodePrefix) and (AppDetails.BarCodePrePacketLeftOffset > 0)) then begin
      if (AppDetails.BarCodeCheckBit > 0) then
        ItemCode := Copy(ScannedCode, 1, ScannedCodeLen - AppDetails.BarCodePrePacketLeftOffset - AppDetails.BarCodeCheckBitOffset)
      else
        ItemCode := Copy(ScannedCode, 1, ScannedCodeLen - AppDetails.BarCodePrePacketLeftOffset);
      PriceCode := Copy(ScannedCode, 1 + ScannedCodeLen - AppDetails.BarCodePrePacketLeftOffset, AppDetails.BarCodePrePacketLeftOffset - AppDetails.BarCodePrePacketRightOffset);
    end
    else begin
      if (AppDetails.BarCodeCheckBit > 1) then
        ItemCode := Copy(ScannedCode, 1, ScannedCodeLen - AppDetails.BarCodeCheckBitOffset)
      else
        ItemCode := ScannedCode;
      PriceCode := '';
    end;
    TryLookupCashSaleItem(ItemCode, PriceCode);
    ScannedCode := '';
    if (not AppDetails.KeepManualBarcodeOpen) then
    begin
      Exit;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddStaffLoginButton;
var
  StaffLogin: TStaffLogin;
  I, LastI: Integer;
  NeedToAdd: Boolean;
  LastUsed: TDateTime;
  FirstAvail: Integer;
begin
  if (AppDetails.CashSaleStaffLoginButtons) then begin
    NeedToAdd := True;
    LastUsed := 0;
    LastI := -1;
    FirstAvail := -1;
    for I := (StaffLoginsList.Count - 1) downto 0 do begin
      StaffLogin := StaffLoginsList.Items[I];
      StaffLogin.LoginButton.Font.Color := clBlack;
    end;
    for I := (StaffLoginsList.Count - 1) downto 0 do begin
      StaffLogin := StaffLoginsList.Items[I];
      if (StaffLogin.LoginID = GlbLogin.LoginID) then begin
        NeedToAdd := False;
        StaffLogin.LastUsed := Now;
        StaffLogin.LoginButton.Font.Color := clRed;
        Break;
      end
      else if (StaffLogin.StaffID = GlbLogin.StaffID) then begin
        NeedToAdd := False;
        StaffLogin.LoginID := GlbLogin.LoginID;
        StaffLogin.LastUsed := Now;
        StaffLogin.LoginButton.Font.Color := clRed;
        Break;
      end;
      if (StaffLogin.LoginID = 0) then begin
        FirstAvail := I;
      end
      else if ((LastI < 0) or (StaffLogin.LastUsed < LastUsed)) then begin
        LastUsed := StaffLogin.LastUsed;
        LastI := I;
      end;
    end;
    if (NeedToAdd) then begin
      if (FirstAvail >= 0) then begin
        StaffLogin := StaffLoginsList.Items[FirstAvail];
      end
      else if (StaffLoginsList.Count >= 4) then begin
        StaffLogin := StaffLoginsList.Items[LastI];
      end
      else begin
        StaffLogin := TStaffLogin.Create;
        Skin.CreateNewButton(StaffLogin.LoginButton, pnlStaffLoginsRight, pnlStaffLoginsRight, bstGen60);
        StaffLogin.LoginButton.Font.Name := 'Arial';
        StaffLogin.LoginButton.Font.Size := 8;
        StaffLogin.LoginButton.Font.Style := [fsBold];
        PositionControl(TControl(StaffLogin.LoginButton), (StaffLoginsList.Count * 60), 0, 60, 48);
        StaffLogin.LoginButton.Tag := StaffLoginsList.Count;
        StaffLogin.LoginButton.OnClick := StaffLoginButtonClick;
        StaffLoginsList.Add(StaffLogin);
      end;
      StaffLogin.StaffID := GlbLogin.StaffID;
      StaffLogin.LoginID := GlbLogin.LoginID;
      StaffLogin.LoginButton.Font.Color := clRed;
      StaffLogin.LoginButton.Caption := GlbLogin.StaffName;
      StaffLogin.LoginButton.Visible := True;
      StaffLogin.LastUsed := Now;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.StaffLoginButtonClick(Sender: TObject);
var
  StaffLogin: TStaffLogin;
begin
  if (Sender is TfcCustomImageBtn) then begin
    if (TfcCustomShapeBtn(Sender).Tag < StaffLoginsList.Count) then begin
      StaffLogin := StaffLoginsList.Items[TfcCustomShapeBtn(Sender).Tag];

      DoSelectNameStaffID(StaffLogin.StaffID, AppDetails.ProveStaffCashSale);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.CheckStaffLoginButtons;
var
  StaffLogin: TStaffLogin;
  LoginID: Integer;
  I: Integer;
begin
  if (AppDetails.CashSaleStaffLoginButtons) then begin
    for I := (StaffLoginsList.Count - 1) downto 0 do begin
      StaffLogin := StaffLoginsList.Items[I];
      StaffLogin.BeenChecked := False;
    end;

    with dm.qrTerminalLogins do begin
      BufferFirst;
      while (not BufferEOF) do begin
        LoginID := BufferFieldByName('loginid').AsInteger;
        for I := (StaffLoginsList.Count - 1) downto 0 do begin
          StaffLogin := StaffLoginsList.Items[I];
          if (StaffLogin.LoginID = LoginID) then begin
            StaffLogin.BeenChecked := True;
            Break;
          end;
        end;
        BufferNext;
      end;
    end;

    for I := (StaffLoginsList.Count - 1) downto 0 do begin
      StaffLogin := StaffLoginsList.Items[I];
      if ((not StaffLogin.BeenChecked) and (StaffLogin.LoginID > 0)) then begin
        StaffLogin.LoginButton.Visible := False;
        StaffLogin.LoginID := 0;
        StaffLogin.StaffID := 0;
      end;
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.PrintTicket;
var
  aOL: TOrderLine;
  ErrorMessage: string;
  I, J: Integer;
  Qty: Currency;
  TheLine: Integer;
  Count: Integer;
  Total: Currency;
begin
  try
    Total := 0;
    Count := 1;
    for I := 0 to (OLList.Count - 1) do begin
      aOL := OLList.Items[I];
      if (aOL.ItemID > 0) then begin
        if (not aOL.AlreadyOrdered) then begin
          if (not aOL.WeighedItem) then
            Total := Total + aOL.Qty
          else
            Total := Total + 1;
        end;
      end;
    end;

    with PrintJob do begin
      for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
        aOL := OLList.Items[I];
        if (aOL.ItemID > 0) then begin // JEH 18/08/2001 1 -> 0
          if (not aOL.AlreadyOrdered) then begin
            Qty := 1;
            if (not aOL.WeighedItem) then Qty := aOL.Qty;

            for J := 0 to (Ceil(Qty) - 1) do begin
              if (not OpenThePrinter(ErrorMessage)) then begin
                Exit;
              end;
              PrintLeft(fsSmall, AppDetails.OutletName);
              PrintAlign(PrintJob.TheRight, atRight, inttostr(Count) + '/' + Floattostr(Total));
              PrintLeft(fsSmall, txtStaff.Text);
              PrintLeft(fsSmall, FormatDateTime('d mmm yy', Now));
              PrintLeft(fsSmall, FormatDateTime('h:mm AM/PM', Now));
              if (onTable) then begin
                PrintLeft(fsNormal, 'Name: ' + GlbTable.TableName);
              end;
              if (onAccount) then begin
                PrintWrappedText(fsNormal, atLeft, 'Account: ' + GlbAccount.AccountName);
              end;
              if (onTable) and (GlbTable.RequireGuests) and (GlbTable.Guests > 0) then begin
                if (GlbTable.Children > 0) then
                  PrintLeft(fsNormal, 'Guests: ' + inttostr(GlbTable.Guests) + '/' + inttostr(GlbTable.Children))
                else PrintLeft(fsNormal, 'Guests: ' + inttostr(GlbTable.Guests));
              end;
              BlankLine;
              if (onTable) then begin
                 PrintCentre(fsBig, 'TAB ' + IntToStr(GlbTable.TableNo));
              end;
              if ((GlbTable.Section <> '') and (OnTable)) then begin
                PrintCentre(fsBig, GlbTable.Section);
              end
              else if (GlbTable.SaleCategory <> '') then begin
                PrintCentre(fsBig, GlbTable.SaleCategory);
              end;
              if (FDelivery <> '') then begin
                PrintCentre(fsBig, FDelivery);
              end
              else begin
                PrintCentre(fsBig, AppDetails.WAName);
              end;
              if (EatInTakeawayPromptValue > 0) then begin
                case (EatInTakeawayPromptValue) of
                  1: PrintCentre(fsBig, 'Eat In');
                  2: PrintCentre(fsBig, 'Takeaway');
                  3: PrintCentre(fsBig, 'Phone');
                  4: PrintCentre(fsBig, 'Pickup');
                  5: PrintCentre(fsBig, 'Delivery');
                end;
              end;
              DrawLine;
              if (aOL.WeighedItem) then
              begin
                PrintLeft(fsNormal, Floattostr(aOL.Qty) + ' Kgs ' + aOL.ItemString);

                PrintAlign(PrintJob.TheRight, atRight, FormatCurrencySign(aOL.ToPay));
              end
              else begin
                PrintLeft(fsNormal, '1  ' + aOL.ItemString);
                PrintAlign(PrintJob.TheRight, atRight, FormatCurrencySign(aOL.UnitPrice));
              end;
              TheLine := I;
              While (TheLine < (OLList.Count - 1)) do begin
                Inc(TheLine);
                if (TOrderLine(OLList.Items[TheLine]).ItemID = 0) then begin
                  PrintLeft(fsModifiers, '      ' + TOrderLine(OLList.Items[TheLine]).ItemString);
                end
                else begin
                  break;
                end;
              end;
              BlankLine;
              CheckSpacing;
              CloseThePrinter;
              Inc(Count);
            end;
          end;
        end;
      end;
    end;
  except on e: exception do
    begin
      ErrorMessage := e.message;
      LogMessage(ltGeneral, 'Print Ticket : ' + ErrorMessage);
    end;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.PrintOrder(OrderPrinterList: TIntegerList);
var
  I, J, K: Integer;
  ThePrinterID: Integer;
  ErrorMessage: string;
  aOL: TOrderLine;
  TheLine: Integer;
begin
  for I := 0 to (OrderPrinterList.Count - 1) do begin
    ThePrinterID := OrderPrinterList.Get(I);
    PrintJob.SetupPrinter(ThePrinterID);

    with PrintJob do
    begin
      for J := 1 to NoOfCopies do begin
        if (not OpenThePrinter(ErrorMessage)) then begin
          Exit;
        end;
        //if noofcopies > 1 then print copyno at top in bold
        if (NoOfCopies > 1) then begin
          PrintLeft(fsHuge, Format('Copy %d', [J]));
          BlankLine;
        end;

        PrintLeft(fsNormal, AppDetails.OutletName);
        PrintLeft(fsNormal, txtStaff.Text);
        PrintLeft(fsNormal, FormatDateTime('d mmm yy', Now));
        PrintLeft(fsNormal, FormatDateTime('h:mm AM/PM', Now));
        BlankLine;

        if (onTable) then begin
          PrintLeft(fsNormal, 'Name: ' + GlbTable.TableName);
        end;
        if (onAccount) then begin
          PrintWrappedText(fsNormal, atLeft, 'Account: ' + GlbAccount.AccountName);
        end;
        if (onTable) and (GlbTable.RequireGuests) and (GlbTable.Guests > 0) then begin
          if (GlbTable.Children > 0) then
            PrintLeft(fsNormal, 'Guests: ' + inttostr(GlbTable.Guests) + '/' + inttostr(GlbTable.Children))
          else PrintLeft(fsNormal, 'Guests: ' + inttostr(GlbTable.Guests));
        end;
        BlankLine;
        if (onTable) then begin
           PrintCentre(fsBig, 'TAB ' + IntToStr(GlbTable.TableNo));
        end;
        if ((GlbTable.Section <> '') and (OnTable)) then begin
           PrintCentre(fsBig, GlbTable.Section);
        end
        else if (GlbTable.SaleCategory <> '') then begin
          PrintCentre(fsBig, GlbTable.SaleCategory);
        end;
        if (FDelivery <> '') then begin
          PrintCentre(fsBig, FDelivery);
        end
        else begin
          PrintCentre(fsBig, AppDetails.WAName);
        end;
        if (EatInTakeawayPromptValue > 0) then begin
          case (EatInTakeawayPromptValue) of
            1: PrintCentre(fsBig, 'Eat In');
            2: PrintCentre(fsBig, 'Takeaway');
            3: PrintCentre(fsBig, 'Phone');
            4: PrintCentre(fsBig, 'Pickup');
            5: PrintCentre(fsBig, 'Delivery');
          end;
        end;

      if (AppDetails.PrintLocalOrderNo) then
      begin
          PrintLocalOrderNo(-1, WBInvoices.RefreshID, 0);
      end;
        DrawLine;
        for K := 0 to (OLList.Count - 1) do begin
          aOL := OLList.Items[K];
          if ((aOL.PrinterID = ThePrinterID) or (aOL.RepeatPrinterID = ThePrinterID)) then begin
            if (aOL.WeighedItem) then
            begin
              PrintLeft(fsNormal, Floattostr(aOL.Qty) + 'KGs ' + aOL.ItemString);

              PrintAlign(PrintJob.TheRight, atRight, FormatCurrencySign(aOL.ToPay));
            end
            else begin
              PrintLeft(fsNormal, Floattostr(aOL.Qty) + '  ' + aOL.ItemString);
              if (PrintPriceOnOrders) then
                PrintAlign(PrintJob.TheRight, atRight, FormatCurrencySign(aOL.UnitPrice));
            end;
            TheLine := K;
            While (TheLine < (OLList.Count - 1)) do begin
              Inc(TheLine);
              if (TOrderLine(OLList.Items[TheLine]).ItemID = 0) then begin
                PrintLeft(fsModifiers, '      ' + TOrderLine(OLList.Items[TheLine]).ItemString);
              end
              else begin
                break;
              end;
            end;
          end;
        end;
        BlankLine;
        CheckSpacing;
        CloseThePrinter;
      end; //for NoOfCopies
    end; //with PrintJob
  end; //for OrderPrinterList.Count
end;
{******************************************************************************}
procedure TformQuickSaleUser.PrintInvoice;
var
  ErrorMessage: string;
  aOL: TOrderLine;
  I: Integer;
  InvoiceTotal, SalesTax: Currency;
  FoodTotal, BevTotal: Currency;
  TipTotal, TenderTotal, TheChange: Currency;

begin
  PrintJob.SetupPrinter(AppDetails.PrinterID);
  with PrintJob do begin
    if (not OpenThePrinter(ErrorMessage)) then begin
      Exit;
    end;
    DrawLogo;

    //outlet name
    if (Assigned(AppDetails.PrintLogo)) then begin
      PrintCentre(fsBig, AppDetails.OutletName);
      BlankLine;
    end;

    //
    PrintCentre(fsBoldIt, 'TAX INVOICE');
    BlankLine;

    PrintLeft(fsNormal, 'Cash Sales');

    PrintLeft(fsNormal, 'Destination');
    PrintDestination(FDelivery, fsNormal, 'MidCol');

    if (AppDetails.PrintLocalOrderNo) then
    begin
      PrintLocalOrderNo(-1, WBInvoices.RefreshID, 0);
    end;

    //Invoice #
    PrintLeft(fsNormal, 'Invoice #');
    PrintAlign(CentreCol, atLeft, IntToStr(WBInvoices.RefreshID));

    //StaffName
    PrintLeft(fsNormal, 'Salesperson');
    PrintAlign(CentreCol, atLeft, txtStaff.Text);

    //date
    PrintLeft(fsNormal, sDate);
    PrintAlign(CentreCol, atLeft, FormatDateTime('h:nn AM/PM d mmm yy', Now));
    BlankLine;

    DrawLine;

    InvoiceTotal := 0;
    SalesTax := 0;
    FoodTotal := 0.00;
    BevTotal := 0.00;
    for I := 0 to (OLList.Count - 1) do begin // JEH 18/08/2001 1 -> 0
      aOL := OLList.Items[I];
      if (aOL.ItemID > 0) then begin // JEH 18/08/2001 1 -> 0
        PrintIL(aOL.ItemString, aOL.ToPay, aOL.UnitPrice, aOL.Qty, aOL.WeighedItem);

        InvoiceTotal := InvoiceTotal + aOL.ToPay;
        if (aOL.FORB = 'F') then begin
          FoodTotal := FoodTotal + aOL.ToPay;
        end
        else begin
          BevTotal := BevTotal + aOL.ToPay;
        end;
        SalesTax := SalesTax + ((aOL.ToPay * aOL.SalesTaxPercent) / (100 + aOL.SalesTaxPercent));
        SalesTax := GetRoundedUpDown(SalesTax, AppDetails.DecimalPlaces);
      end;
    end;

    BlankLine;
    //Totals
    PrintLeft(fsBoldIt, 'BALANCE DUE');
    PrintAlign(TheRight, atRight, FormatCurrencySignSpace(InvoiceTotal));

    BlankLine;

    DrawLine;

    //Tenders
    TipTotal := 0;
    TenderTotal := 0;
    TheChange := 0;
    for I := 0 to (TLList.Count - 1) do
    begin
      TipTotal := TipTotal + TLList.Items[I].TLTip;
      TenderTotal := TenderTotal + TLList.Items[I].TLTender;
      TheChange := TheChange + TLList.Items[I].TLChange;
      PrintRL(TLList.Items[I].TLTypeName, TLList.Items[I].TLTip, TLList.Items[I].TLTender);
    end;

    //tip total
    if (TipTotal > 0) then begin
      BlankLine;
      PrintLeft(fsBoldIt, 'TIP');
      PrintAlign(FigCol, atRight, FormatCurrencySignSpace(TipTotal));
     end;

     //tender total
     if TenderTotal > 0 then begin
       if (TipTotal = 0) then begin
         BlankLine;
       end;
       PrintLeft(fsBoldIt, 'TENDERED');
       PrintAlign(TheRight, atRight, FormatCurrencySignSpace(TenderTotal));
     end;

    //change
    if TheChange > 0 then begin
      PrintLeft(fsBoldIt, 'CHANGE');
      PrintAlign(TheRight, atRight, FormatCurrencySignSpace(TheChange));
    end;

    BlankLine;

    //Footer
    PrintFooter(AppDetails.OutletID);

    CheckSpacing;
    CloseThePrinter;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoCashup(DoCashup:Boolean);
begin
  GlbTill.TillID := GlbLogin.TillID;
  // this should never happen but in case it does!
  if (not GlbTill.TillID > 0) then Exit;

  if (AppDetails.OfflineMode) then begin
  //We're in offline mode
    if (not DoCashup) then begin
      if (ShowConf('We are in offline mode.' + #13#10 +
        'Are you sure you want to perform Reading?') <> mrYes) then begin
        Exit;
      end;
    end
    else begin
      if (ShowConf('We are in offline mode.' + #13#10 +
        'Are you sure you want to CashUp?') <> mrYes) then begin
        Exit;
      end;
    end;
  end;

  if (not TestStaffForOwnTill) then begin
  //check if have privilege
    if (not VerifyPriv(7)) then begin
      if (DoCashup) then begin
        ShowMsg('Cannot CashUp.'#13#10'Please ask someone who has the priviledge to CashUp to assist.');
      end
      else begin
        ShowMsg('Cannot perform Reading on another staff members till.'#13#10'Please ask someone who has the priviledge to Perform Reading to assist.');
      end;
      Exit;
    end;
  end;

  if (not DoCashUp) then begin
    if (not VerifyPriv(12)) then begin
      ShowMsg('Cannot perform Reading.'#13#10'Please ask someone who has the priviledge to Perform Reading to assist.');
      Exit;
    end;
  end;

  if (not VerifyPinNo) then begin
    Exit;
  end;

  Update;
  Enabled := False;
  if (DoCashUp) then begin
    if (ShowConf('Are you sure you want to CashUp this till?') <> mrNo) then begin
      if (TryCashUpTill) then
      begin
        if (AppDetails.PromptCashupCount) then
          EditCashupCount(WBCashUps.RefreshID, False);
        //If it works maybe later we can do something here
      end;
    end;
  end
  else begin
    if TryReadingTill then begin
      //If it works maybe later we can do something here
    end;
  end;
  Enabled := True;
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoStats;
var
  FromTime, ToTime: TDateTime;
begin
  if (not VerifyPriv(9)) then begin
    ShowMsg('Cannot print a Stats Report. Please ask someone who has the priviledge to print a Stats Report to assist.');
    Exit;
  end;

  //verify pinno
  if (not VerifyPinNo) then begin
    Exit;
  end;

  //set from and to time to now
  FromTime := Now;
  ToTime := Now;

  //get last time, from and to tables from db
  with dm.sp do begin
    Unprepare;
    StoredProcName := 'getlastsalestatstime';
    Prepare;
    ParamByName('outletid').AsInteger := AppDetails.OutletID;
    ExecProc;

    if (not FieldByName('lastsalestatstime').IsNull) then begin
      FromTime := FieldByName('lastsalestatstime').AsDateTime;
    end;
  end;

  //show from and to time dialog with option to cancel or Ok
  if (not SelectStartStopTimes(FromTime, ToTime, 'Stats Report')) then begin
    Exit;
  end;

  //insert printjob
  try
    dm.tr.StartTransaction;   //Jon 10-07-2003
    with dm.sp do begin
      Unprepare;
      StoredProcName := 'setlastsalestatstime';
      Prepare;
      ParamByName('outletid').AsInteger := AppDetails.OutletID;
      ParamByName('lastsalestatstime').AsDateTime := ToTime;
      //account range not used now, so just set to zero
      ParamByName('fromtable').AsInteger := 0;
      ParamByName('totable').AsInteger := 0;
      ExecProc;

      UnPrepare;
      StoredProcName := 'insert_specialprintjob';
      Prepare;
      ParamByName('printjobtype').AsString := 'Stats';
      ParamByName('theid').AsInteger := AppDetails.OutletID;
      ParamByName('printerid').AsInteger := AppDetails.PrinterID;
      ParamByName('fromtime').AsDateTime := FromTime;
      ParamByName('totime').AsDateTime := ToTime;
      ExecProc;
    end;
//      dm.tr.SavePoint;
    dm.tr.Commit;     //Jon 10-07-2003
  except on e: exception do begin
    dm.tr.Rollback;   //Jon 10-07-2003
    ShowMessage(e.message);
    Exit;
  end; end;
  ShowQuickMsg('Stats Report being prepared. This may take several minutes.');
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetInActiveItems;
var
  I, J: Integer;
  PInactivePeriod: TInactivePeriod;
  PInactivePeriodIndex: Integer;
  PItem: TItem;
begin
  //RPC we using:
  // InactivePeriodIndex < 0 to use as preload of inactive items
  // InactiveperiodIndex = 0 to say not inactive period sellected
  // InactivePeriodIndex > 0 is the current inactive period in use

  if (InactivePeriodIndex < 0) then
  begin
    OpenArch.InactiveItem(False); //Clear all Items of inactive status
    for I := 0 to (GlobalMenuList.ListInactivePeriods.Count - 1) do     //set all items that are part of an inactive period to be InActive
    begin
      PInactivePeriod := GlobalMenuList.ListInactivePeriods.Items[I];
      for J := 0 to (PInactivePeriod.ListInactiveItems.Count - 1) do
      begin
        PItem := GlobalMenuList.ListItems.Find(PInactivePeriod.ListInactiveItems.Get(J));
        if (Assigned(PItem)) then
          PItem.Inactive := True;
        OpenArch.InactiveItem(True, PInactivePeriod.ListInactiveItems.Get(J));
      end;
    end;
  end;

  PInactivePeriodIndex := 0;
  // now we look for time period, stop after the first one we found
  for I := 0 to (GlobalMenuList.ListInactivePeriods.Count - 1) do
  begin
    PInactivePeriod := GlobalMenuList.ListInactivePeriods.Items[I];
    if (TimeIsInPeriod(CurrentTime, CurrentDay, PInactivePeriod.StartTime, PInactivePeriod.EndTime, PInactivePeriod.OnDay)) then
    begin
      PInactivePeriodIndex := I + 1;
      break;
    end;
  end;

  // Clear current Inactive Period
  if (InactivePeriodIndex <> PInactivePeriodIndex) then
  begin
    if (InactivePeriodIndex > 0) then       //set previous inactive period items to be inactive
    begin
      PInactivePeriod := GlobalMenuList.ListInactivePeriods.Items[InactivePeriodIndex - 1];
      for I := 0 to (PInactivePeriod.ListInactiveItems.Count - 1) do
      begin
        PItem := GlobalMenuList.ListItems.Find(PInactivePeriod.ListInactiveItems.Get(I));
        if (Assigned(PItem)) then
          PItem.Inactive := True;
        OpenArch.InactiveItem(True,  PInactivePeriod.ListInactiveItems.Get(I));
      end;
    end;
    if (PInactivePeriodIndex > 0) then     //set current inactive period items to be active
    begin
      PInactivePeriod := GlobalMenuList.ListInactivePeriods.Items[PInactivePeriodIndex - 1];
      for I := 0 to (PInactivePeriod.ListInactiveItems.Count - 1) do
      begin
        PItem := GlobalMenuList.ListItems.Find(PInactivePeriod.ListInactiveItems.Get(I));
        if (Assigned(PItem)) then
          PItem.Inactive := False;
        OpenArch.InactiveItem(False, PInactivePeriod.ListInactiveItems.Get(I));
      end;
    end;
    InactivePeriodIndex := PInactivePeriodIndex;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.grdOLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
Var
  aOL: TOrderLine;
  Italic, Bold: Boolean;
  FontColour: TColor;
  aAlignment: TAlignment;

begin
  inherited;

  if (OLList.Count - 1) >= ARow then
  begin
    aOL := OLList.Items[ARow];
    Italic := aOL.AlreadyOrdered;
    Bold := False;
    FontColour := clWhite;
    Font.Name := 'Roboto';

    if Italic then
      FontColour := $4A4A4A;

    if (aOL.ComboID > 0) then
    begin
      FontColour := aOL.ComboColour;
    end;
  end
  else begin
    Italic := False;
    Bold := False;
    FontColour := 0;
  end;
  aAlignment := taLeftJustify;
  if (aCol in [1, 3]) then
  begin
    aAlignment := taRightJustify;
  end;
  Theme.ApplySkinOnStringGridDraw(TStringGrid(Sender), ARow, ACol, Rect, State, FontColour, Italic, Bold, aAlignment);
  Self.ActiveControl := nil;
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddItemQtyToSpecialNYHappyHour(ItemID: Integer; Qty: Currency);
var
  NYHappyHour: TNYHappyHour;
begin
  // Do SpecialNYHappyHour is set to True at start of sale, so we only mark false if we can't do it
  if (GlobalMenuList.ListNYHappyHour.Count = 0) then begin
    DoSpecialNYHappyHour := False;
    Exit;
  end;
  NYHappyHour := GlobalMenuList.ListNYHappyHour.Find(ItemID);
  if (Assigned (NYHappyHour)) then begin
    NYHappyHour.CurrentLevel := NYHappyHour.CurrentLevel + Qty;
    if (NYHappyHour.CurrentLevel < (NYHappyHour.RequiredLevel * Qty)) then begin
      DoSpecialNYHappyHour := False;
    end;
  end
  else begin
    DoSpecialNYHappyHour := False;
  end;
end;
{******************************************************************************}
procedure TformQuickSaleUser.LogSepicalNYHappyHour(ItemID: Integer; Qty: Currency; Price: Currency; IsFood: Boolean);
var
  IniFile: TIniFile;
  NYHappyHour: TNYHappyHour;
  Total, BevTotal, FoodTotal: Double;
  ItemQty: Integer;
  ItemTotal: Double;
  LF: TFileStream;
  Buffer: Array[0..4095] of Byte;
  BufferI: Integer;
  SLen: Integer;
  SI: Integer;
  S: string;
begin
  if (DoSpecialNYHappyHour) then begin
    IniFile := TIniFile.Create(AppDetails.SpecialNYHappyHourPath + '\log.txt');

    Total := IniFile.ReadFloat('Total', 'Total', 0);
    BevTotal := IniFile.ReadFloat('Total', 'BevTotal', 0);
    FoodTotal := IniFile.ReadFloat('Total', 'FoodTotal', 0);
    ItemQty := IniFile.ReadInteger(inttostr(ItemID), 'ItemQty', 0);
    ItemTotal := IniFile.ReadFloat(inttostr(ItemID), 'ItemTotal', 0);

    NYHappyHour := GlobalMenuList.ListNYHappyHour.Find(ItemID);
    if (Assigned (NYHappyHour)) then begin
      NYHappyHour.CurrentLevel := (NYHappyHour.CurrentLevel - (NYHappyHour.RequiredLevel * Ceil(Qty)));
      ItemQty := ItemQty + Ceil(Qty);
      ItemTotal := ItemTotal + Price;
      Total := Total + Price;
      if (isFood) then
        FoodTotal := FoodTotal + Price else BevTotal := BevTotal + Price;
    end;
    IniFile.WriteFloat('Total', 'Total', Total);
    if (AppDetails.SpecialNYHappyHourLogType = 0) then
    begin
       if (AppDetails.SpecialNYHappyHourForB) then
      begin
        IniFile.WriteFloat('Total', 'BevTotal', BevTotal);
        IniFile.WriteFloat('Total', 'FoodTotal', FoodTotal);
      end;
      IniFile.WriteInteger(inttostr(ItemID), 'ItemQty', ItemQty);
      IniFile.WriteFloat(inttostr(ItemID), 'ItemTotal', ItemTotal);
    end;

    IniFile.Free;

    if (AppDetails.SpecialNYHappyHourLogType = 1) then
    begin
      LF := TFileStream.Create(AppDetails.SpecialNYHappyHourPath + '\log.txt', fmOpenWrite or fmShareDenyNone);
      S := FormatDateTime('dd-mm-yyyy hh:nn:ss.zzz', Now) + ', ' + IntToStr(GlbLogin.StaffID) + ', ' +
        IntToStr(ItemID) + ', ' + IntToStr(ItemQty) + ', ' + FloatToStr(ItemTotal);
      LF.Position := LF.Size;
      SLen := Length(S);
      SI := 1;
      while (SI <= SLen) do begin
        BufferI := 0;
        while ((SI <= SLen) and (BufferI < 4096)) do begin
          Buffer[BufferI] := Ord(S[SI]);
          Inc(BufferI);
          Inc(SI);
        end;
        LF.WriteBuffer(Buffer, BufferI);
      end;

      LF.Free;
    end;
  end;
end;
{******************************************************************************}
function TformQuickSaleUser.GetCPFNumber: string;
var
  frmCPFNo: TformGetCPFNumber;

begin
  SFiscalPrinting.FiscalOpTryAgainCommand := False;
  SFiscalPrinting.SupressFiscalErrorMessage := False;

  Result := '';

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  cmdCPF.Enabled := True;
  cmdCNPJ.Enabled := True;
  ShowCPFNUmberInActivate := True;
  cmdSelectAccount.Enabled := True;
  cmdAccountNoLookup.Enabled := True;
  cmdDeselectAccount.Enabled := True;
  cmdDiscountPercent.Enabled := True;
  cmdDiscountAmount.Enabled := True;

  if ((Trim(GlbAccount.PerorgTaxNumber) <> '') and (not ForceCPFNumber)) then
    FCPFNumber := GlbAccount.PerorgTaxNumber
  else if ((Trim(FCPFNumber) <> '') and (not ForceCPFNumber)) then
  begin
    GlbAccount.PerorgTaxNumber := FCPFNumber;
    FAccountCPFNumberChanged := True;
  end
  else
  begin
    frmCPFNo := TformGetCPFNumber.Create(Self);
    try
      frmCPFNo.UpdateCPF := False;
      frmCPFNo.CPFMode := mSaleCPF;
      frmCPFNo.DisplayTaxDetails := True;
      frmCPFNo.ShowManualSelection := ShowManualSelection;
      frmCPFNo.NFManual := NFManual;
      NFManualSecond := False;
      if frmCPFNo.ShowModal = mrOk then
      begin
        Application.ProcessMessages;
        Result := frmCPFNo.CPFNumber;
        GlbAccount.PerorgTaxNumber := Result;
        FCPFNumber := Result;
        FPerorgID := 0;
        NFManualSecond := frmCPFNo.NFManualSelected;
        NFManual := frmCPFNo.NFManual;
        if frmCPFNo.AccountID > 0 then
        begin
          NFEMode := frmCPFNo.SendToNFE;
          FPerorgID := GlbPerOrg.PerorgID;
          GlbAccount.AccountID := frmCPFNo.AccountID;

          if DoingPhoneOrder then
          begin
            GlbCustomer.PerorgID := GlbPerOrg.PerorgID;
            GlbCustomer.MasPerorgID := 0;
            GlbCustomer.AccountID := GlbAccount.AccountID;
            GetFiscalAutoCustomer(GlbCustomer.PerorgID);
          end;

          SetAccount;
        end
        else
        begin
          RetrieveCPFAccount;
          if GlbAccount.AccountID > 0 then
          begin
            NFEMode := frmCPFNo.SendToNFE;
          end;
        end;
      end
      else //if cpf form is cancelled
      begin
        NFManual := False;
        NFManualSecond := (NFManual and (AppDetails.FiscalManualMode = 1));
      end;

      ClearStatusPage;

      if (NFManual and (not NFManualSecond)) then
        SetStatus(sQuickSalesINManualNFMode, False, True)
      else if (NFManual and (NFManualSecond)) then
        SetStatus(sQuickSalesINManualNFMode, False, False)
      else if NFEMode then
        SetStatus(sQuickSalesINNFEMode, False, NFManual)
      else
        SetStatus(sQuickSales, False, NFManual);

    finally
      if not StaffPromptTimeoutTimer.Enabled then
        ResetStaffPromptTimeoutTimer;

      frmCPFNo.Free;
      lblAcctName.Caption := '';

      if (GlbAccount.AccountID > 0) then
      begin
        EnableAvailBox(False);
        lblAcctName.Caption := DoubleAmpersand(GlbAccount.AccountName);
        lblTitle.Visible := False;
        pnlAvail.Visible := True;
        pnlLoyalty.Visible := True;
      end;
    end;
  end;
{  if (not (NFManual or NFEMode)) then
  begin
    if ((not SFiscalPrinting.CheckAuxFileForManufactureNo) or (not SFiscalPrinting.CheckAuxFileForGrandTotal)) then
    begin
      IsShowing := False;
      PostMessage(Self.Handle, WM_CLOSE, 0, 0);
      Exit;
    end;
  end;  }
end;

procedure TformQuickSaleUser.AddNewFiscalItem(AQty: Integer = 1);
var
  OL: TOrderLine;
  I: Integer;
begin
  if (OLList.Count > 0) then
  begin
    FAddingDuplicateItem := True;
    for I := 1 to AQty do
    begin
      OL := OLList.Items[grdOLs.Row];
      if (OL.ItemID > 0) then
      begin
        ComboFreeItems.Clear;
        OrderItem(OL.ItemID);
        AddFreeComboItemsToOrderList;
      end;
    end;
    FAddingDuplicateItem := False;
  end;
end;

procedure TformQuickSaleUser.RetrieveCPFAccount;
begin
  if (DoingTransaction) then
    Exit;

  //fiscal printing
  if ((AppDetails.EnableFiscalPrinting) and (not (NFManual or NFEMode)) and (Trim(AppDetails.FiscalAccountPaymentMethod) = '')) then
  begin
    ShowMsg(sConfigureAcctPayMethod);
    Exit;
  end;

  if Trim(FCPFNumber) = '' then
    Exit;

  if (CanSelectAccount) then
  begin
    with dm.qrCPFAccount do
    begin
      try
        UnPrepare;
        SQL.Clear;

        SQL.Add('select a.accountid, p.TAXNUMBER, p.Perorgid from account a, perorg p ');
        SQL.Add('where a.PERORGID = p.PERORGID and p.WHENDELETED is null and a.WHENCLOSED is null');
        SQL.Add('and p.TAXNUMBER = ' + QuotedStr(FCPFNumber));//SQL.Add('select accountid, cpfnumber from account where cpfnumber = ' + QuotedStr(FCPFNumber));
        Prepare;
        Open;
        GlbAccount.AccountID := 0;

        if (not EOF) then
        begin
          GlbAccount.AccountID := FieldByName('accountid').AsInteger;

          if DoingPhoneOrder then
          begin
            GlbCustomer.PerorgID := FieldByName('Perorgid').AsInteger;
            GlbCustomer.AccountID := FieldByName('accountid').AsInteger;
            GetFiscalAutoCustomer(GlbCustomer.PerorgID);
          end;
        end;
      finally
        Close;
      end;
    end;

    SetAccount;
  end;
end;

procedure TformQuickSaleUser.GetFiscalAutoCustomer(APerOrgID: Integer);
var
  CustomerName: string;
  DeliveryAddress: array [1..3] of string;
  DeliveryPhoneNo: string;
  DeliveryDetails: TDeliveryDetails;
  I: Integer;
  PhoneNoUsed: Integer;
  DoCheckDeliveryDetails: Integer;
begin
  if (APerOrgID <= 0) then
    Exit;

  //dm.qrCustomers.Locate('perorgid', APerOrgID, []);

  with dm.sp do
  begin
    Unprepare;
    StoredProcName := 'getperson';
    Prepare;
    ParamByName('pperorgid').AsInteger := APerOrgID;
    ExecProc;

    CustomerName := '';
    if (not FieldByName('title').IsNull) then begin
      CustomerName := FieldByName('title').AsString;
    end;
    if (not FieldByName('firstname').IsNull) then begin
      if (CustomerName = '') then begin
        CustomerName := FieldByName('firstname').AsString;
      end
      else begin
        CustomerName := CustomerName + ' ' + FieldByName('firstname').AsString;
      end;
    end;

    if (not FieldByName('middlename').IsNull) then begin
      if (CustomerName = '') then begin
        CustomerName := FieldByName('middlename').AsString;
      end
      else begin
        CustomerName := CustomerName + ' ' + FieldByName('middlename').AsString;
      end;
    end;
    if (not FieldByName('surname').IsNull) then begin
      if (CustomerName = '') then begin
        CustomerName := FieldByName('surname').AsString;
      end
      else begin
        CustomerName := CustomerName + ' ' + FieldByName('surname').AsString;
      end;
    end;

    GlbCustomer.CustomerName := CustomerName;
    GlbCustomer.PerorgID := FieldByName('perorgid').AsInteger;
    GlbCustomer.MasPerorgID := FieldAsInt(FieldByName('masperorgid'));
    GlbCustomer.AccountID := FindFieldAsInt(FindField('accountid'));
    GlbCustomer.TaxNumber := FieldByName('taxnumber').AsString;

    GlbCustomer.Surname := FieldByName('surname').AsString;
    GlbCustomer.FirstName := FieldByName('firstname').AsString;
    GlbCustomer.MiddleName := FieldByName('middlename').AsString;
    GlbCustomer.Title := FieldByName('title').AsString;
    GlbCustomer.Salutation := FieldByName('salutation').AsString;
    GlbCustomer.JobTitle := '';
    GlbCustomer.OrgName := '';
    GlbCustomer.DOB := FieldAsDate(FieldByName('dob'));

    GlbCustomer.BirthDay := FieldAsInt(FieldByName('birthday'));
    GlbCustomer.BirthMonth := FieldAsInt(FieldByName('birthmonth'));

    GlbCustomer.Address1 := FieldByName('address1').AsString;
    GlbCustomer.Address2 := FieldByName('address2').AsString;
    GlbCustomer.Address3 := FieldByName('address3').AsString;

    GlbCustomer.AddressMapLocationID := FieldAsInt(FieldByName('addressmaplocationid'));
    GlbCustomer.AMLStreetName := FieldAsString(FieldByName('amlstreetname'));
    GlbCustomer.AMLSuburb := FieldAsString(FieldByName('amlsuburb'));
    GlbCustomer.AMLPostCode := FieldAsString(FieldByName('amlpostcode'));
    GlbCustomer.AMLCity := FieldAsString(FieldByName('amlcity'));
    GlbCustomer.AMLMapPage := FieldAsString(FieldByName('amlmappage'));
    GlbCustomer.AMLGridRef1 := FieldAsString(FieldByName('amlgridref1'));
    GlbCustomer.AMLGridRef2 := FieldAsString(FieldByName('amlgridref2'));
    GlbCustomer.AMLQd := FieldAsString(FieldByName('amlqd'));
    GlbCustomer.AMLDefaultRemoteLocationID := FieldAsInt(FieldByName('amldefremlocid'));

    GlbCustomer.PostCode := FieldAsString(FieldByName('postcode'));
    GlbCustomer.Country := FieldByName('country').AsString;

    GlbCustomer.POAddress1 := FieldByName('poaddress1').AsString;
    GlbCustomer.POAddress2 := FieldByName('poaddress2').AsString;
    GlbCustomer.POAddress3 := FieldByName('poaddress3').AsString;
    GlbCustomer.POPostCode := FieldByName('popostcode').AsString;
    GlbCustomer.POCountry := FieldByName('pocountry').AsString;

    GlbCustomer.HomeNo := FieldByName('homeno').AsString;
    GlbCustomer.WorkNo := FieldByName('workno').AsString;
    GlbCustomer.MobileNo := FieldByName('mobileno').AsString;
    GlbCustomer.FaxNo := FieldByName('faxno').AsString;

    DeliveryPhoneNo := '';
    PhoneNoUsed := 0;

    if ((not FieldByName('homeno').IsNull) and (FieldByName('homeno').AsString <> '')) then
    begin
      DeliveryPhoneNo := FieldByName('homeno').AsString;
      PhoneNoUsed := 1;
    end
    else if ((not FieldByName('mobileno').IsNull) and (FieldByName('mobileno').AsString <> '')) then
    begin
      DeliveryPhoneNo := FieldByName('mobileno').AsString;
      PhoneNoUsed := 2;
    end
    else if ((not FieldByName('workno').IsNull) and (FieldByName('workno').AsString <> '')) then
    begin
      DeliveryPhoneNo := FieldByName('workno').AsString;
      PhoneNoUsed := 3;
    end;

    GlbCustomer.EMail := FieldByName('email').AsString;
    GlbCustomer.Notes := FieldByName('notes').AsString;
    GlbCustomer.RemoteOverrideGroupID := FindFieldAsInt(FindField('remoteoverridegroupid'));

    GlbCustomer.Gender := FieldAsString(FieldByName('gender'));
    GlbCustomer.Hobbies := FieldAsString(FieldByName('hobbies'));
    GlbCustomer.Sports := FieldAsString(FieldByName('sports'));
    GlbCustomer.Music := FieldAsString(FieldByName('music'));
    GlbCustomer.FavoriteItem := FieldAsString(FieldByName('favoriteitem'));
    GlbCustomer.Occupation := FieldAsString(FieldByName('occupation'));
    GlbCustomer.Nationality := FieldAsString(FieldByName('nationality'));
    GlbCustomer.OptOut := FieldAsBool(FieldByName('optout'));

    BlobFieldToMemoryStream(FieldByName('photo'), GlbCustomer.Photo);

    GlbCustomer.Anniversary := FieldAsDate(FieldByName('anniversary'));
    GlbCustomer.AnniversaryDay := FieldAsInt(FieldByName('anniversaryday'));
    GlbCustomer.AnniversaryMonth := FieldAsInt(FieldByName('anniversarymonth'));

    GlbCustomer.Customer := FieldAsBool(FieldByName('customer'));
    GlbCustomer.VIP := FieldAsBool(FieldByName('vip'));

    GlbCustomer.LinkCode := FieldAsString(FieldByName('linkcode'));

    GlbCustomer.DoingEditCheck := False;
    GlbCustomer.RGRNE := FieldAsString(FieldByName('RGRNE'));

    case EatInTakeawayPromptValue of
      5: DoCheckDeliveryDetails := 1;      //We are doing delivery, so get and check delivery details
      0: DoCheckDeliveryDetails := 2;      //We don't know what we're doing yet, so just get the defaults for now to potentially check later
    else
      DoCheckDeliveryDetails := 0;         //We aren't doing delivery so don't get the delivery details
    end;

    if (DoCheckDeliveryDetails <> 0) then
    begin    //We want to confirm the name, address and phone number
      for I := 1 to 3 do begin
        DeliveryAddress[I] := '';
      end;

      if (not FieldByName('address1').IsNull) then begin
        DeliveryAddress[1] := FieldByName('address1').AsString;
      end;

      if (not FieldByName('address2').IsNull) then begin
        DeliveryAddress[2] := FieldByName('address2').AsString;
      end;

      if (not FieldByName('address3').IsNull) then begin
        DeliveryAddress[3] := FieldByName('address3').AsString;
      end;
      if (not FieldByName('postcode').IsNull) then begin
        if (DeliveryAddress[3] = '') then begin
          DeliveryAddress[3] := FieldByName('postcode').AsString;
        end
        else begin
          DeliveryAddress[3] := DeliveryAddress[3] + ' ' + FieldByName('postcode').AsString;
        end;
      end;

      DeliveryDetails.DeliveryName := CustomerName;
      for I := 1 to 3 do begin
        DeliveryDetails.DeliveryAddress[I] := DeliveryAddress[I];
      end;
      DeliveryDetails.DeliveryStreetNo := FieldAsString(FieldByName('STREETNUMBER'));
      DeliveryDetails.DeliveryFlatNo := FieldAsString(FieldByName('flatnumber'));
      DeliveryDetails.DeliveryPhone := DeliveryPhoneNo;
      DeliveryDetails.DeliveryNotes := FieldByName('notes').AsString;
      DeliveryDetails.AddressMapLocationID := FieldAsInt(FieldByName('addressmaplocationid'));
      DeliveryDetails.AMLStreetName := FieldAsString(FieldByName('amlstreetname'));
      DeliveryDetails.AMLSuburb := FieldAsString(FieldByName('amlsuburb'));
      DeliveryDetails.AMLCity := FieldAsString(FieldByName('amlcity'));
      DeliveryDetails.AMLPostCode := FieldAsString(FieldByName('amlpostcode'));
      DeliveryDetails.AMLMapPage := FieldAsString(FieldByName('amlmappage'));
      DeliveryDetails.AMLGridRef1 := FieldAsString(FieldByName('amlgridref1'));
      DeliveryDetails.AMLGridRef2 := FieldAsString(FieldByName('amlgridref2'));
      DeliveryDetails.AMLQd := FieldAsString(FieldByName('amlqd'));
      DeliveryDetails.AMLDefaultRemoteLocationID := FieldAsInt(FieldByName('amldefremlocid'));

      if (DoCheckDeliveryDetails = 1) then
      begin
        if (CheckDeliveryDetails(DeliveryDetails, True)) then begin
          if ((AppDetails.RemoteLocationLevel <= 2) and ((DeliveryDetails.DeliveryAddress[1] <> DeliveryAddress[1]) or
          (DeliveryDetails.DeliveryAddress[2] <> DeliveryAddress[2]) or (DeliveryDetails.DeliveryAddress[3] <> DeliveryAddress[3]))) then begin
            if (ShowConf(sConfirmSaveDeliveryAddr) = mrYes) then begin
              SelectCustomerRecord(APerOrgID);

              GlbCustomer.Address1 := DeliveryDetails.DeliveryAddress[1];
              GlbCustomer.Address2 := DeliveryDetails.DeliveryAddress[2];
              GlbCustomer.Address3 := DeliveryDetails.DeliveryAddress[3];

              if (DeliveryPhoneNo <> DeliveryDetails.DeliveryPhone) then
              begin
                case PhoneNoUsed of
                  0, 1: GlbCustomer.HomeNo := DeliveryDetails.DeliveryPhone;
                  2: GlbCustomer.WorkNo := DeliveryDetails.DeliveryPhone;
                  3: GlbCustomer.MobileNo := DeliveryDetails.DeliveryPhone;
                end;
              end;
              GlbCustomer.DeliveryStreetNo := DeliveryDetails.DeliveryStreetNo;
              GlbCustomer.DeliveryFlatNo := DeliveryDetails.DeliveryFlatNo;
              GlbCustomer.AddressMapLocationID := DeliveryDetails.AddressMapLocationID;
              GlbCustomer.AMLStreetName := DeliveryDetails.AMLStreetName;
              GlbCustomer.AMLSuburb := DeliveryDetails.AMLSuburb;
              GlbCustomer.AMLPostCode := DeliveryDetails.AMLPostCode;
              GlbCustomer.AMLCity := DeliveryDetails.AMLCity;
              GlbCustomer.AMLMapPage := DeliveryDetails.AMLMapPage;
              GlbCustomer.AMLGridRef1 := DeliveryDetails.AMLGridRef1;
              GlbCustomer.AMLGridRef2 := DeliveryDetails.AMLGridRef2;
              GlbCustomer.AMLQd := DeliveryDetails.AMLQd;
              GlbCustomer.AMLDefaultRemoteLocationID := DeliveryDetails.AMLDefaultRemoteLocationID;

              GlbCustomer.DoingEditCheck := True;

              ShowDetail(adCustomerDetail);

              GlbCustomer.DoingEditCheck := False;
             end;
          end;
        end
        else
        begin
          Exit;
        end;
      end;

      GlbCustomer.DeliveryName := DeliveryDetails.DeliveryName;
      for I := 1 to 3 do begin
        GlbCustomer.DeliveryAddress[I] := DeliveryDetails.DeliveryAddress[I];
      end;
      GlbCustomer.DeliveryStreetNo := DeliveryDetails.DeliveryStreetNo;
      GlbCustomer.DeliveryFlatNo := DeliveryDetails.DeliveryFlatNo;
      GlbCustomer.DeliveryPhoneNo := DeliveryDetails.DeliveryPhone;
      GlbCustomer.DeliveryNotes := DeliveryDetails.DeliveryNotes;
      GlbCustomer.DeliveryAddressMapLocationID := DeliveryDetails.AddressMapLocationID;
      GlbCustomer.DeliveryAMLStreetName := DeliveryDetails.AMLStreetName;
      GlbCustomer.DeliveryAMLSuburb := DeliveryDetails.AMLSuburb;
      GlbCustomer.DeliveryAMLPostCode := DeliveryDetails.AMLPostCode;
      GlbCustomer.DeliveryAMLCity := DeliveryDetails.AMLCity;
      GlbCustomer.DeliveryAMLMapPage := DeliveryDetails.AMLMapPage;
      GlbCustomer.DeliveryAMLGridRef1 := DeliveryDetails.AMLGridRef1;
      GlbCustomer.DeliveryAMLGridRef2 := DeliveryDetails.AMLGridRef2;
      GlbCustomer.DeliveryAMLQd := DeliveryDetails.AMLQd;
      GlbCustomer.DeliveryAMLDefaultRemoteLocationID := DeliveryDetails.AMLDefaultRemoteLocationID;
    end
    else
    begin
      GlbCustomer.DeliveryName := CustomerName;
      for I := 1 to 3 do begin
        GlbCustomer.DeliveryAddress[I] := '';
      end;
      GlbCustomer.DeliveryPhoneNo := DeliveryPhoneNo;
      GlbCustomer.DeliveryNotes := '';
      GlbCustomer.DeliveryOrderReadyFor := 0;
      GlbCustomer.DeliveryAddressMapLocationID := 0;
    end;
  end;
end;

procedure TformQuickSaleUser.cmdCPFDesClick(Sender: TObject);
begin
  ForceCPFNumber := True;
  GetCPFNumber;
end;

procedure TformQuickSaleUser.cmdCNPJDesClick(Sender: TObject);
begin
  ForceCPFNumber := True;
  GetCPFNumber;
end;

procedure TformQuickSaleUser.SetAccount;
begin
  if (GlbAccount.AccountID > 0) then
  begin
    LoadAccount(GlbAccount.AccountID);

    txtAccountName.Text := GlbAccount.AccountName;
    FDiscountPercent := GlbAccount.AccountDiscount;
    FLimitMaximum := GlbAccount.LimitMaximum;
    FMaxDiscount := GlbAccount.MaxDiscount;
    FOrigDiscountPercent := GlbAccount.AccountDiscount;
    FCurrentBalance := GlbAccount.CurrentBalance;
    FUseDiscountAmount := False;

    {CalculateDiscountPercent;
    CalculateToPay;}

    ChangeAllowCreditLabel(GlbAccount.AllowCredit);   //Jon 16-10-2003
  end
  else
    DeselectAccount;
end;

procedure TformQuickSaleUser.EnableAvailBox(AEnable: Boolean);
begin
  txtAvail.Visible := AEnable;
  lblAvail.Visible := AEnable;
  lblAvailDollarSign.Visible := AEnable;
end;

procedure TformQuickSaleUser.CancelPOSInvoiceIfFiscalInvCancelled;
begin
  if ((AppDetails.EnableFiscalPrinting) and
      (AppDetails.FiscalLiveInvoice) and
      (not (NFManual or NFEMode)) and
      (not DoingPhoneOrder) and
      (not (OnTable and (GlbTable.GroupID > 0))) and
      (GlbTable.SaleCategoryType <= scStandard) and
      (not SFiscalPrinting.OpenedFiscalInvoice)) then
  begin
    ConfirmBeforeCancel := False;
    cmdHideClick(Self);
    Exit;
  end;
end;

function TformQuickSaleUser.OpenDefaultTabForWastage:Boolean;
begin
  Result := False;
  with dm.sp do
  begin
    SetStoredProcName('OPENDEFAULTTABFORWASTAGE');

    ParamByName('tableno').AsInteger := AppDetails.DefaultWastageTab;
    ParamByName('outletid').AsInteger := AppDetails.OutletID;
    ParamByName('loginid').AsInteger := GlbLogin.LoginID;
    StringAsParam(ParamByName('groupname'), sQSWastage);
    ExecProc;

    if (not FieldByName('errorcode').IsNull) then
    begin
      ShowErrorMsg(FieldByName('errorcode').AsInteger);
      //dm.tr.Rollback;
      Exit;
    end
    else
    begin
      WBTables.RefreshID := FieldByName('groupid').AsInteger;
      WBTables.JustInserted := True;
      GlbTable.GroupID := FieldByName('groupid').AsInteger;
      GlbTable.TableNo := AppDetails.DefaultWastageTab;
      GlbTable.TableName := sQsWastage;
    end;
  end;
  Result := True;
end;

procedure TformQuickSaleUser.SelectCustomerRecord(APerorgID: Integer);
begin
  if APerorgID <= 0 then
    Exit;

  with dm.sp do
  begin
    Unprepare;
    StoredProcName := 'getperson';
    Prepare;
    ParamByName('pperorgid').AsInteger := APerorgID;
    ExecProc;

    GlbCustomer.PerorgID := FieldByName('perorgid').AsInteger;
    GlbCustomer.MasPerorgID := FieldAsInt(FieldByName('masperorgid'));
    GlbCustomer.AccountID := FindFieldAsInt(FindField('accountid'));
    GlbCustomer.TaxNumber := FieldByName('taxnumber').AsString;

    GlbCustomer.Surname := FieldByName('surname').AsString;
    GlbCustomer.FirstName := FieldByName('firstname').AsString;
    GlbCustomer.MiddleName := FieldByName('middlename').AsString;
    GlbCustomer.Title := FieldByName('title').AsString;
    GlbCustomer.Salutation := FieldByName('salutation').AsString;
    GlbCustomer.JobTitle := '';
    GlbCustomer.OrgName := '';
    GlbCustomer.DOB := FieldAsDate(FieldByName('dob'));

    GlbCustomer.BirthDay := FieldAsInt(FieldByName('birthday'));
    GlbCustomer.BirthMonth := FieldAsInt(FieldByName('birthmonth'));

    GlbCustomer.Address1 := FieldByName('address1').AsString;
    GlbCustomer.Address2 := FieldByName('address2').AsString;
    GlbCustomer.Address3 := FieldByName('address3').AsString;

    GlbCustomer.AddressMapLocationID := FieldAsInt(FieldByName('addressmaplocationid'));
    GlbCustomer.AMLStreetName := FieldAsString(FieldByName('amlstreetname'));
    GlbCustomer.AMLSuburb := FieldAsString(FieldByName('amlsuburb'));
    GlbCustomer.AMLPostCode := FieldAsString(FieldByName('amlpostcode'));
    GlbCustomer.AMLCity := FieldAsString(FieldByName('amlcity'));
    GlbCustomer.AMLMapPage := FieldAsString(FieldByName('amlmappage'));
    GlbCustomer.AMLGridRef1 := FieldAsString(FieldByName('amlgridref1'));
    GlbCustomer.AMLGridRef2 := FieldAsString(FieldByName('amlgridref2'));
    GlbCustomer.AMLQd := FieldAsString(FieldByName('amlqd'));
    GlbCustomer.AMLDefaultRemoteLocationID := FieldAsInt(FieldByName('amldefremlocid'));

    GlbCustomer.PostCode := FieldAsString(FieldByName('postcode'));
    GlbCustomer.Country := FieldByName('country').AsString;

    GlbCustomer.POAddress1 := FieldByName('poaddress1').AsString;
    GlbCustomer.POAddress2 := FieldByName('poaddress2').AsString;
    GlbCustomer.POAddress3 := FieldByName('poaddress3').AsString;
    GlbCustomer.POAddress4 := FieldByName('poaddress4').AsString;
    GlbCustomer.POPostCode := FieldByName('popostcode').AsString;
    GlbCustomer.POCountry := FieldByName('pocountry').AsString;

    GlbCustomer.HomeNo := FieldByName('homeno').AsString;
    GlbCustomer.WorkNo := FieldByName('workno').AsString;
    GlbCustomer.MobileNo := FieldByName('mobileno').AsString;
    GlbCustomer.FaxNo := FieldByName('faxno').AsString;

    GlbCustomer.EMail := FieldByName('email').AsString;
    GlbCustomer.Notes := FieldByName('notes').AsString;
    GlbCustomer.RemoteOverrideGroupID := FindFieldAsInt(FindField('remoteoverridegroupid'));

    GlbCustomer.Gender := FieldAsString(FieldByName('gender'));
    GlbCustomer.Hobbies := FieldAsString(FieldByName('hobbies'));
    GlbCustomer.Sports := FieldAsString(FieldByName('sports'));
    GlbCustomer.Music := FieldAsString(FieldByName('music'));
    GlbCustomer.FavoriteItem := FieldAsString(FieldByName('favoriteitem'));
    GlbCustomer.Occupation := FieldAsString(FieldByName('occupation'));
    GlbCustomer.Nationality := FieldAsString(FieldByName('nationality'));
    GlbCustomer.OptOut := FieldAsBool(FieldByName('optout'));
    BlobFieldToMemoryStream(FieldByName('photo'), GlbCustomer.Photo);

    GlbCustomer.Anniversary := FieldAsDate(FieldByName('anniversary'));
    GlbCustomer.AnniversaryDay := FieldAsInt(FieldByName('anniversaryday'));
    GlbCustomer.AnniversaryMonth := FieldAsInt(FieldByName('anniversarymonth'));

    GlbCustomer.Customer := FieldAsBool(FieldByName('customer'));
    GlbCustomer.VIP := FieldAsBool(FieldByName('vip'));

    GlbCustomer.LinkCode := FieldAsString(FieldByName('linkcode'));

    GlbCustomer.DoingEditCheck := False;
    GlbCustomer.RGRNE := FieldAsString(FieldByName('RGRNE'));
  end;
end;

function TformQuickSaleUser.GetNFManualExtraDetails:Boolean;
var
  frmManual: TformNFEDetail;
begin
  Result := False;
  FManualSeries := '';
  FManualSubSeries := '';
  FManualInvNo := '';

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  frmManual := TformNFEDetail.Create(nil);
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

function TformQuickSaleUser.GetNFEDetails:Boolean;
var
  frmNFE: TformNFEDetail;
begin
  Result := False;
  FManualSeries := '';
  FManualSubSeries := '';
  FManualInvNo := '';

  if not AppDetails.EnableFiscalPrinting then
    Exit;

  frmNFE := TformNFEDetail.Create(nil);
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

procedure TformQuickSaleUser.tmrQSInactivateTimer(Sender: TObject);
var
  iSec: Int64;
begin
  inherited;         
  StopStaffPromptTimeoutTimer;
  if ((formQuickSaleUser.Visible) and (AppDetails.QSInactiveTimeout <= 0))  then
  begin
    tmrQSInactivate.Enabled := False;
    Exit;
  end;
  iSec := SecondsBetween(Now, FQSInactiveTime);
  if (iSec > (AppDetails.QSInactiveTimeout * 60)) then  // Assumption : QSInactiveTimeout is in minutes
  begin
    tmrQSInactivate.Enabled := False;

    DoSelectName; //akm 01.10.01

    tmrQSInactivate.Enabled := True;
    FQSInactiveTime := Now;
  end;
end;

procedure TformQuickSaleUser.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  inherited;
  tmrQSInactivate.Enabled := False;
end;

procedure TformQuickSaleUser.FormKeyPress(Sender: TObject; var Key: Char);
begin
  inherited;
  tmrQSInactivate.Enabled := False;
end;

procedure TformQuickSaleUser.FormKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  inherited;
  tmrQSInactivate.Enabled := True;
  FQSInactiveTime := Now;
end;

procedure TformQuickSaleUser.FormMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  inherited;
  tmrQSInactivate.Enabled := False;
end;

procedure TformQuickSaleUser.FormMouseWheel(Sender: TObject;
  Shift: TShiftState; WheelDelta: Integer; MousePos: TPoint;
  var Handled: Boolean);
begin
  inherited;
  tmrQSInactivate.Enabled := False;
end;

procedure TformQuickSaleUser.FormMouseWheelDown(Sender: TObject;
  Shift: TShiftState; MousePos: TPoint; var Handled: Boolean);
begin
  inherited;
  tmrQSInactivate.Enabled := False;
end;

procedure TformQuickSaleUser.FormMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  inherited;
  tmrQSInactivate.Enabled := True;
  FQSInactiveTime := Now;
end;

procedure TformQuickSaleUser.StartInactiveTimer;
begin
  tmrQSInactivate.Enabled := True;
  FQSInactiveTime := Now;
end;

procedure TformQuickSaleUser.StopInactiveTimer;
begin
  tmrQSInactivate.Enabled := False;
  FQSInactiveTime := Now + 100; // no need to set actually, anyway set to 100 days after
end;

procedure TformQuickSaleUser.scrAccountsClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  StartInactiveTimer;
end;

procedure TformQuickSaleUser.grdTLsClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  StartInactiveTimer;
end;

procedure TformQuickSaleUser.grdTLsDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
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

procedure TformQuickSaleUser.scrBClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  StartInactiveTimer;
end;

procedure TformQuickSaleUser.scrFClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  StartInactiveTimer;
end;
{******************************************************************************}
procedure TformQuickSaleUser.ReplaceSwipeCardQS;
begin
  //if in a Goody discounted sale, disallow replacing an account's swipe card
  if InGoodyDiscountedSale then
  begin
    ShowQuickMsg(sNoReplaceSwipeCardInGoodyDiscSale);
    Exit;
  end;

  StopInactiveTimer;

  if (not TestRemoteLocationOption(rlsoRemote)) or
     ((TestRemoteLocationOption(rlsoRemote)) and HeadOfficeOnline) then begin
     if (RSC.AllowUpdate) then begin
       ShowMsg('Replacement Card already loaded for this transaction');
     end
     else begin
       if ((GlbAccount.AccountID <> 0) and not (RSC.ReasonAlreadyOrdered)) then begin
         case (ShowConfWithCancel(sAccountSelectedSelectAnother)) of
           mrNO: cmdSelectAccountClick(self);
           mrCancel: Exit;
         end
       end
       else if (GlbAccount.AccountID = 0) then begin
          cmdSelectAccountClick(self);
       end;

       if (GlbAccount.AccountID = 0) then begin
         Exit;
       end;
       ComboFreeItems.Clear;
       if (SelectReplacementSwipeCard(@RSC)) then begin
         if (not RSC.ReasonAlreadyOrdered) then
           OrderItem(RSC.ItemID);
         AddFreeComboItemsToOrderList;
         RSC.AllowUpdate := True;
         RSC.ReasonAlreadyOrdered := True;
       end;
     end;
   end;
end;
{******************************************************************************}
function TformQuickSaleUser.UpdateSwipeCardAfterReplacement: Boolean;
var
  errorcode, MasSwipeCardID: Integer;
  ROSSDataCustomer: TROSSDataCustomer;

begin
  Result := True;
  ROSSDataCustomer := TROSSDataCustomer.Create;
  MasSwipeCardID := 0;
  if (not dm.TestConnection) then begin
    Exit;
  end;

  if ((RSC.AllowUpdate) and (RSC.CardCode <> '')) then begin
    try
      if (TestRemoteLocationOption(rlsoRemote)) then begin
        with dm.sp do begin
          SetStoredProcName('TRANSMASSTAFFID');       //transmasstaffid
          ParamByName('pstaffid').AsInteger := GlbLogin.StaffID;
          ParamNull(ParamByName('pmasstaffid'));
          Execproc;
          ROSSDataCustomer.StaffID := FieldAsInt(FieldByName('masstaffid'));

          SetStoredProcName('TRANSMASTERMINALID'); //transmasterminalid
          ParamByName('pterminalid').AsInteger := AppDetails.TerminalID;
          ParamNull(ParamByName('pmasterminalid'));
          Execproc;
          ROSSDataCustomer.TerminalID := FieldAsInt(FieldByName('masterminalid'));

          if ((ROSSDataCustomer.StaffID <= 0) or (ROSSDataCustomer.TerminalID <= 0)) then begin
            Exit;
          end;

          with ROSSDataCustomer.ROSSAccount do begin
            ROSSDataCustomer.SwipeCardExists := True;

            if (GlbAccount.AccountID > 0) then begin
              AccountID := GlbAccount.MasAccountID;
            end;
            ROSSDataCustomer.ROSSAccount.CardNo := RSC.CardCode;
          end;
        end;

        if (SendROSSCustomerTransaction(ROSSDataCustomer)) then begin
          MasSwipeCardID := ROSSDataCustomer.ROSSAccount.MasSwipeCardID;
        end;
      end;

      if (ROSSDataCustomer.ErrorCode <> 0) then begin
        RSC.AllowUpdate := False;
        Result := False;
        Exit;
      end;
    finally
      ROSSDataCustomer.Free;
    end;


    dm.tr.StartTransaction;

    with dm.sp do begin
      Unprepare;
      StoredProcName := 'edit_swipecard';
      Prepare;
      if (MasSwipeCardID <> 0) then
        ParamByName('masswipecardid').AsInteger := MasSwipeCardID;
      ParamByName('loginid').AsInteger := GlbLogin.LoginID;
      ParamByName('cardtype').AsInteger := 1;
      ParamByName('theid').AsInteger := GlbAccount.AccountID;
      ParamByName('cardcode').AsString := RSC.CardCode;
      ParamByName('defaultcard').AsInteger := 1;

      ExecProc;

      errorcode := FieldAsInt(FieldByName('errorcode'));

      if (errorcode <> 0) then begin
        if (errorcode = 120) then begin
          ShowMsg(rsDBErr0120);
        end
        else if (errorcode = 2200) then begin
          ShowMsg(rsDBErr2200);
        end;
        dm.tr.Rollback;
        Result := False;
      end
      else begin
        dm.tr.Commit;
        RSC.AllowUpdate := False;
        RSC.ReasonAlreadyOrdered := False;
        RSC.CardCode := '';
        RSC.ItemID := 0;
        RSC.CancelPressed := False;
      end;
    end;
  end
  else
    Result := False;
end;
{******************************************************************************}
procedure TformQuickSaleUser.RemoveSwipeCardReplacement;
var
  I: Integer;
  OL: TOrderLine;
begin
  with grdOLs do begin
    if (RSC.AllowUpdate) then begin
      for I := (OLList.Count - 1) downto 0 do begin
        OL := TOrderLine(OLList[I]);
        if (OL.ItemID = RSC.ItemID) then begin
          Row := I;
          VoidLine;
        end;
      end;
    end;
  end;
end;

procedure TformQuickSaleUser.ClearCustomerDeliveryDetails;
var
  I: Integer;
begin
  GlbCustomer.DeliveryName := '';
  for I := 1 to 3 do begin
    GlbCustomer.DeliveryAddress[I] := '';
  end;
  GlbCustomer.DeliveryPhoneNo := '';
  GlbCustomer.DeliveryNotes := '';
  GlbCustomer.DeliveryOrderReadyFor := 0;
  GlbCustomer.DeliveryAddressMapLocationID := 0;
  GlbCustomer.DeliveryAMLStreetName := '';
  GlbCustomer.DeliveryAMLSuburb := '';
  GlbCustomer.DeliveryAMLPostCode := '';
  GlbCustomer.DeliveryAMLCity := '';
  GlbCustomer.DeliveryAMLMapPage := '';
  GlbCustomer.DeliveryAMLGridRef1 := '';
  GlbCustomer.DeliveryAMLGridRef2 := '';
  GlbCustomer.DeliveryAMLQd := '';
  GlbCustomer.DeliveryAMLDefaultRemoteLocationID := 0;
  GlbCustomer.DeliveryStreetNo := '';
  GlbCustomer.DeliveryFlatNo := '';
end;

procedure TformQuickSaleUser.ClearSelectedCustomerDetails;
begin
  with GlbCustomer do
  begin
    PerorgID := 0;
    AccountID := 0;
    TaxNumber := '';
    OrgName := '';
    Surname := '';
    FirstName := '';
    MiddleName := '';
    CustomerName := '';   
    Title := '';
    Salutation := '';
    JobTitle := '';
    Address1 := '';
    Address2 := '';
    Address3 := '';
    Address4 := '';
    AddressMapLocationID := 0;
    Country := '';
    PostCode := '';
    POAddress1 := '';
    POAddress2 := '';
    POAddress3 := '';
    POAddress4 := '';
    POCountry := '';
  end;
  ClearCustomerDeliveryDetails;
end;

function TformQuickSaleUser.PromptAndGetDifferentSizeItem(aItemID: Integer; aItemAbbrev: string; aCurrentTime: Integer; aCurrentDay: Integer; var aSelectedItemID: Integer): Boolean;
begin
  PauseStaffPromptTimeoutTimer;
  Result := ShowDifferentSizeItems(aItemID, AItemAbbrev, aCurrentTime, aCurrentDay, aSelectedItemID);
end;

procedure TformQuickSaleUser.AddFreeComboItemsToOrderList;
var
  I, iCount: Integer;
  tmpFreeItem: TFreeComboItemToAdd;
  OL: TOrderLine;
begin
  if ((not AppDetails.EnableFiscalPrinting) and (Assigned(ComboFreeItems)) and (ComboFreeItems.Count > 0)) then
  begin
    for I := 0 to ComboFreeItems.Count-1 do
    begin
      tmpFreeItem := TFreeComboItemToAdd(ComboFreeItems.Items[I]);
      iCount := OLList.Count;
      OrderItem(tmpFreeItem.ItemID, True);
      if (OLList.Count = iCount + 1) then // means one item added to the list
      begin
        OL := OLList.Items[OLList.Count-1];
        FComboDiscount := FComboDiscount + OL.ToPay;

        OL.ComboID := tmpFreeItem.ComboID;
        OL.ComboGroupID := tmpFreeItem.ComboGroupID;
        OL.ComboIndex := tmpFreeItem.ComboIndex;

        OL.AllowedDiscount := OL.ToPay;
        OL.FreeComboItem := True;
        OL.ComboQty := OL.Qty;
        OL.FreeComboQty := tmpFreeItem.FreeQty;
        OL.ApplyDiscountType := adtAllItems; // if it's free combo group, then it should apply to all items in the group
      end;
    end;
    ComboFreeItems.Clear;
    CalculateToPay;
  end;
end;

procedure TformQuickSaleUser.AddSameCombo(aComboID: Integer; aComboIndex: Integer);
var
  I, J, iMaxComboIndex: Integer;
  OL: TOrderLine;
  ComboMod: TComboModifier;
  aMod: TMod;
begin
  iMaxComboIndex := GetMaxComboIndex(aComboID, GlbTable.GroupID, OLList);
  try
    for I := 0 to OLList.Count - 1 do
    begin
      OL := OLList.Items[I];
      if ((OL.ItemID > 0) and (OL.ComboID = aComboID) and (OL.ComboIndex = aComboIndex)) then
      begin
        DisposeMods;
        if ((Assigned(OL.ComboMods)) and (OL.ComboMods.Count > 0)) then
        begin
          Mods := TList.Create;
          for J := 0 to OL.ComboMods.Count  - 1 do
          begin
            aMod := TMod.Create;
            ComboMod := TComboModifier(OL.ComboMods[J]);
            aMod.ModID := ComboMod.ModID;
            aMod.ModGroupID := ComboMod.ModGroupID;
            aMod.Modifier := ComboMod.Modifier;
            aMod.ModGroupOrder := ComboMod.ModGroupOrder;
            aMod.ModPrice := ComboMod.ModPrice;
            aMod.OriginalModPrice := ComboMod.OriginalModPrice;
            aMod.PrinterID := ComboMod.PrinterID;
            aMod.RepeatPrinterID := ComboMod.RepeatPrinterID;
            aMod.ModFunction := ComboMod.ModFunction;  
            aMod.ModQty := ComboMod.ModQty;
            aMod.ModQtyOption := ComboMod.ModQtyOption;
            aMod.ModQtyOrdered := ComboMod.ModQtyOrdered;
            aMod.ModQtyIncluded := ComboMod.ModQtyIncluded;
            Mods.Add(aMod);
          end;
        end;
        AddingComboItems := True;
        ForcedNextQty := (OL.Qty > 1);
        NextQty := Ceil(OL.Qty);
        if (not OrderItem(OL.ItemID, True, OL.ComboID, OL.ComboGroupID, iMaxComboIndex)) then
        begin
          ClearCombo(OLList, OL.ComboID, iMaxComboIndex);
          Exit;
        end;
        AddFreeComboItemsToOrderList;
      end;
    end;
    CalculateToPay;
  finally
    if (AppDetails.EnableFiscalPrinting) then
    begin
      for I := 0 to OLList.Count-1 do
      begin
        OL := OLList.Items[I];
        if ((OL.ItemID > 0) and (OL.ComboID = aComboID) and (OL.ComboIndex = iMaxComboIndex)) then
        begin
          SendOrderLineToFiscalPrinter(OL, I);
        end;
      end;
    end;

    AddingComboItems := False;
  end;
end;

procedure TformQuickSaleUser.DeleteCombos(aIndex: Integer; aDeleteAllItems: Boolean=False);
var
  I: Integer;
  OL: TOrderLine;
  DeleteAllItems: Boolean;
begin
  DeletingComboItems := True;
  try
    DeleteAllItems := (ADeleteAllItems or (ShowConf(sConfirmDeleteOtherComboItems) = mrYes));

    for I := OLList.Count-1 downto 0 do
    begin
      OL := OLList.Items[I];

      if ((OL.ComboID = ComboIDToDelete) and (OL.ComboIndex = FComboIndexToDelete)) then
      begin
        OL.ComboID := 0;
        OL.ComboGroupID := 0;
        OL.ComboIndex := 0;
        OL.ComboQty := 0;
        OL.FreeComboQty := 0;
        OL.ComboColourIndex := -1;
        OL.ComboDiscount := 0;
        OL.ComboTempQty := 0;
        OL.FixThisComboPart := False;
        OL.ApplyDiscountType := adtAllItems;
        OL.FreeComboItem := False;

        grdOLs.Row := I;

        if ((not RemoveOrderLine) and ((not DeleteAllItems) and (I = AIndex))) then
        begin
          OL.Qty := OL.Qty - 1;
          OL.ToPay := OL.UnitPrice * OL.Qty;
          OL.ToPay := GetRoundedUpDown(OL.ToPay, AppDetails.DecimalPlaces);

          if (AllocateItemLoyalty(OL)) then
            TestAllOLsLoyaltyItem(True);

          UpdateOLGridRow(grdOLs.Row, OL);
        end;

        if ((OL.ItemID > 0) and ((((DeleteAllItems) or (I = AIndex)) and (RemoveOrderLine)) or ((not RemoveOrderLine) and ((OL.Qty = 0) or (DeleteAllItems)))))  then
          VoidLine;
      end;
    end;

    ComboIDToDelete := 0;
    FComboIndexToDelete := 0;
    CalculateToPay;
  finally
    DeletingComboItems := False;
  end;

  if ((AppDetails.PDEnabled)and (not DeletingComboItems)) then
    DMComponents.PDOutputItemLine('SUB TOTAL', GetRoundedUpDown(FStillDue, AppDetails.DecimalPlaces), 1, False);
end;

procedure TformQuickSaleUser.CheckForFiscalDiscountChanges;
var
  I: Integer;
  DiscountDelta: Currency;
  aOL: TOrderLine;
  aDiscountType: string;

begin
  for I := 0 to (OLList.Count - 1) do
  begin
    aOL := OLList.Items[I];
    if ((aOL.ItemID > 0) and aOL.IsFiscalDiscountDone) then
    begin
      DiscountDelta := aOL.AllowedDiscount;
      aDiscountType := 'D';
      SFiscalPrinting.ChangeInvoiceItemDiscount(aOL.FiscalIndex, aDiscountType, '$', DiscountDelta);

      aOL.IsFiscalDiscountDone := False;
    end;
  end;
end;

procedure TformQuickSaleUser.UpdateTabDiscount;
var
  tmpDiscountAmount, lComboDiscount: Currency;
begin
  //if ((FComboDiscount > 0) or (FDiscountAmount > 0)) then
  begin
    tmpDiscountAmount := FDiscountAmount;
    lComboDiscount := FComboDiscount;
    if not FUseDiscountAmount then
      FDiscountAmount := 0;

    DiscCalculator.DiscountAmount := FDiscountAmount;
    DiscCalculator.DiscountPercent := FDiscountPercent;

    DiscCalculator.UseDiscountAmount := FUseDiscountAmount;
    DiscCalculator.UseItemDiscounts := FUseItemDiscounts;
    DiscCalculator.UseDiscountPeriods := FUseDiscountPeriods;

    DiscCalculator.Account := nil;
    DiscCalculator.AccountID := 0;
    DiscCalculator.OnAccount := False;
    DiscCalculator.GroupID := 0;

    if ((FDiscountAmount > 0) and (GlbTable.SaleCategoryType >= scDriveThru))  then
    begin
      if not AppDetails.OrderDiscountEnable then
      begin
        ShowQuickMsg(sDiscountOnOrderNotAllowed);
        if Assigned(DiscCalculator) then
        begin
          DiscCalculator.ClearItemLevelDiscounts;
          DiscCalculator.ClearPrevTabItems;
        end;
        ClearItemLevelDiscounts(OLList);
      end;
    end;

    if (AppDetails.OrderDiscountEnable or ((AppDetails.CashSaleGuestCount in [1, 2]) and (NewGuestCount > 0))) then
    begin
      DiscCalculator.Account := GlbAccount;
      DiscCalculator.AccountID := GlbAccount.AccountID;
      DiscCalculator.OnAccount := OnAccount;
      if (OnTable) then
      begin
        DiscCalculator.GroupID := GlbTable.GroupID;
      end;
    end;

    DiscCalculator.StillDue := 0;
    DiscCalculator.MaxDiscount := GlbAccount.MaxDiscount;
    DiscCalculator.LoyaltyReward := LoyaltyReward;

    DiscCalculator.OrigDiscountPercent := FOrigDiscountPercent;
    DiscCalculator.OrigDiscountAmount := FOrigDiscountAmount;

    DiscCalculator.CalculateDiscountAgain := True;
    DiscCalculator.ShowMessageForOneTime := False;
    DiscCalculator.ChangeDiscRateManually := False;
    DiscCalculator.ChangeDiscAmtManually := False;
    DiscCalculator.RebuildCombos := True;
    DiscCalculator.AllCombos := FAllCombos;
    FComboDiscount := 0;
    //ClearItemLevelDiscounts(OLList);
    DiscCalculator.OrderType := otNormal;
    if GlbTable.GroupID > 0 then
      DiscCalculator.OrderType := otDriveThru;
    DiscCalculator.CalculateDiscounts(OLList, True, False, True, FDiscountPercent, FDiscountAmount, nil);

    FComboDiscount := DiscCalculator.ComboDiscount;
    LoyaltyReward := DiscCalculator.LoyaltyReward;

    if (DiscCalculator.IsComboSplit or (tmpDiscountAmount <> FDiscountAmount) or (lComboDiscount <> FComboDiscount)) then
      LoadOLGrid;
  end;

  if ((GlbTable.AccountID > 0) or (GlbAccount.AccountID > 0)) and (GlbTable.GroupID > 0) then
  begin
    with dm.qrGeneral do
    begin
      SQL.Clear;
      SQL.Add('UPDATE AGROUP SET DISCOUNTRATE = :discountrate, ACCOUNTID = :accountid WHERE GROUPID = :groupid');
      ParamByName('accountid').AsInteger := GlbAccount.AccountID;
      ParamByName('discountrate').AsCurrency := FDiscountPercent;
      ParamByName('groupid').AsInteger := GlbTable.GroupID;
      ExecSQL;
    end;
  end;
end;

procedure TformQuickSaleUser.grdOLsDblClick(Sender: TObject);
var
  I, J, K: Integer;
  SelectCombo: TSelectCombo;
  OL, aOL: TOrderLine;
  lCombo: TCombo;
  lComboIndexToUse: Integer;
  lComboGroupItem: TComboGroupItem;
  ComboMod, NewComboMod: TComboModifier;
  aMod: TMod;

begin
  if (DoingTransaction) then
    Exit;

  StopInactiveTimer;
  try
    FAddingDuplicateItem := False;
    ResetStaffPromptTimeoutTimer;
    if (OLList.Count > 0) then
    begin
      OL := OLList.Items[grdOLs.Row];
      if (OL.ItemID > 0) then
      begin
        if (OL.ComboID > 0) then
        begin
          lCombo := FAllCombos.GetCombo(OL.ComboID);
          if Assigned(lCombo) then
          begin
            AddingComboItems := True;
            if (OL.ComboIndex > 0) then
            begin  //get last index and then select all combo items of the same index
              SelectCombo := TSelectCombo.Create;
              try
                lComboIndexToUse := OL.ComboIndex;

                SelectCombo.OLsToShowInEditMode.Clear;
                for i := 0 to OLLIst.Count - 1 do
                begin
                  aOL := OLList.Items[i];
                  if ((aOL.ItemID > 0) and (aOL.ComboID = lCombo.ComboID) and (aOL.ComboIndex = lComboIndexToUse)) then
                    SelectCombo.OLsToShowInEditMode.Add(aOL);
                end;

                SelectCombo.ComboSelectMode := csmEdit;
                SelectCombo.SelectComboItems(lCombo);

                if ((SelectCombo.ComboSelectMode = csmEdit) and (SelectCombo.ComboItemsEdited)) then
                begin
                  ComboIDToDelete := OL.ComboID;
                  FComboIndexToDelete := lComboIndexToUse;

                  if ((ComboIDToDelete > 0) and (FComboIndexToDelete > 0) and (not DeletingComboItems)) then
                    DeleteCombos(grdOLs.Row, True);

                  for I := 0 to SelectCombo.SelectedComboItems.Count - 1 do
                  begin
                    lComboGroupItem := TComboGroupItem(SelectCombo.SelectedComboItems.Items[I]);
                    DisposeMods;
                    Mods := TList.Create;
                    if lComboGroupItem.Mods.Count > 0 then
                    begin
                      for J := 0 to lComboGroupItem.Mods.Count  - 1 do
                      begin
                        aMod := TMod.Create;
                        ComboMod := TComboModifier(lComboGroupItem.Mods[J]);
                        aMod.ModID := ComboMod.ModID;
                        aMod.ModGroupID := ComboMod.ModGroupID;
                        aMod.Modifier := ComboMod.Modifier;
                        aMod.ModGroupOrder := ComboMod.ModGroupOrder;
                        aMod.ModPrice := ComboMod.ModPrice;
                        aMod.OriginalModPrice := ComboMod.OriginalModPrice;
                        aMod.PrinterID := ComboMod.PrinterID;
                        aMod.RepeatPrinterID := ComboMod.RepeatPrinterID;
                        aMod.ModFunction := ComboMod.ModFunction;   
                        aMod.ModQty := ComboMod.ModQty;
                        aMod.ModQtyOption := ComboMod.ModQtyOption;
                        aMod.ModQtyOrdered := ComboMod.ModQtyOrdered;
                        aMod.ModQtyIncluded := ComboMod.ModQtyIncluded;
                        Mods.Add(aMod);
                      end;
                    end;

                    ForcedNextQty := (lComboGroupItem.QtySelected > 1);
                    NextQty := Ceil(lComboGroupItem.QtySelected);
                    if NextQty <= 0 then
                      NextQty := 1;

                    if (not OrderItem(lComboGroupItem.ItemID, True, lCombo.ComboID, lComboGroupItem.ComboGroupID, lComboIndexToUse)) then
                    begin
                      ClearCombo(OLList, lCombo.ComboID, lComboIndexToUse);
                      Exit;
                    end;
                    AddFreeComboItemsToOrderList;
                    if (Assigned(lComboGroupItem.Mods) and (lComboGroupItem.Mods.Count > 0)) then   // if mods are selected for combo items save them along with each order line, we need it for editing the combo
                    begin
                      OL := nil;
                      for K := OLList.Count-1 downto 0 do
                      begin
                        OL := OLList.Items[K];
                        if OL.ItemID > 0 then
                          Break;
                      end;

                      if Assigned(OL) then
                      begin
                        if Assigned(OL.ComboMods) then
                          OL.ComboMods.Clear;

                        for J := 0 to lComboGroupItem.Mods.Count  - 1 do
                        begin
                          ComboMod := TComboModifier(lComboGroupItem.Mods[J]);
                          NewComboMod := TComboModifier.Create;

                          NewComboMod.ModID := ComboMod.ModID;
                          NewComboMod.ModGroupID := ComboMod.ModGroupID;
                          NewComboMod.Modifier := ComboMod.Modifier;
                          NewComboMod.ModGroupOrder := ComboMod.ModGroupOrder;
                          NewComboMod.ModPrice := ComboMod.ModPrice;
                          NewComboMod.OriginalModPrice := ComboMod.OriginalModPrice;
                          NewComboMod.PrinterID := ComboMod.PrinterID;
                          NewComboMod.RepeatPrinterID := ComboMod.RepeatPrinterID;
                          NewComboMod.ModFunction := ComboMod.ModFunction;
                          NewComboMod.ModQty := ComboMod.ModQty;
                          NewComboMod.ModQtyOption := ComboMod.ModQtyOption;
                          NewComboMod.ModQtyOrdered := ComboMod.ModQtyOrdered;
                          NewComboMod.ModQtyIncluded := ComboMod.ModQtyIncluded;
                          OL.ComboMods.Add(NewComboMod);
                        end;
                      end;
                    end;
                  end;
                end;
              finally
                AddingComboItems := False;
                BroadcastCDAMessage;
                SelectCombo.Free;
              end;
              CalculateToPay;
            end;
          end;
          StartInactiveTimer;
          Exit;
        end;
      end;
    end;
  finally
    StartInactiveTimer;
  end;
end;

procedure TformQuickSaleUser.CalculateComboAndAccountDiscounts;
var
  tmpDiscountAmount, lComboDiscount: Currency;
begin
  tmpDiscountAmount := FDiscountAmount;
  lComboDiscount := FComboDiscount;

  if not FUseDiscountAmount then
    FDiscountAmount := 0;

  DiscCalculator.DiscountAmount := FDiscountAmount;
  DiscCalculator.DiscountPercent := FDiscountPercent;

  DiscCalculator.UseDiscountAmount := FUseDiscountAmount;
  DiscCalculator.UseItemDiscounts := FUseItemDiscounts;
  DiscCalculator.UseDiscountPeriods := FUseDiscountPeriods;
  DiscCalculator.CalculateAccountDiscountOnly := False;

  DiscCalculator.Account := nil;
  DiscCalculator.AccountID := 0;

  DiscCalculator.GroupID := 0;

  {if GlbTable.GroupID > 0 then
    DiscCalculator.GroupID := GlbTable.GroupID;}
  DiscCalculator.OnAccount := False;
  if ((tmpDiscountAmount > 0) and (GlbTable.SaleCategoryType >= scDriveThru))  then
  begin
    if not AppDetails.OrderDiscountEnable then
    begin
      ShowQuickMsg(sDiscountOnOrderNotAllowed);
      if Assigned(DiscCalculator) then
        DiscCalculator.ClearItemLevelDiscounts;
      ClearItemLevelDiscounts(OLList);
    end;
  end;

  if ((GlbTable.SaleCategoryType < scDriveThru) or (AppDetails.OrderDiscountEnable and (GlbTable.SaleCategoryType >= scDriveThru)) or ((AppDetails.CashSaleGuestCount in [1, 2]) and (NewGuestCount > 0))) then
  begin
    DiscCalculator.Account := GlbAccount;
    DiscCalculator.AccountID := GlbAccount.AccountID;
    DiscCalculator.OnAccount := OnAccount;
    if (OnTable) then
    begin
      DiscCalculator.GroupID := GlbTable.GroupID;
    end;
  end;

  DiscCalculator.StillDue := 0;
  DiscCalculator.MaxDiscount := GlbAccount.MaxDiscount;
  DiscCalculator.LoyaltyReward := LoyaltyReward;

  DiscCalculator.OrigDiscountPercent := FOrigDiscountPercent;
  DiscCalculator.OrigDiscountAmount := FOrigDiscountAmount;

  if ((FOrigDiscountPercent = 100) and (not FUseDiscountAmount) and (not FUseItemDiscounts) and (not FUseDiscountPeriods)) then
    DiscCalculator.CalculateAccountDiscountOnly := True;

  DiscCalculator.CalculateDiscountAgain := True;
  DiscCalculator.ShowMessageForOneTime := False;
  DiscCalculator.ChangeDiscRateManually := False;
  DiscCalculator.ChangeDiscAmtManually := False;
  DiscCalculator.RebuildCombos := True;

  DiscCalculator.AllCombos := FAllCombos;
  FComboDiscount := 0;
  //ClearItemLevelDiscounts(OLList);
  ComboFreeItems.Clear;
  DiscCalculator.OrderType := otNormal;
  if GlbTable.GroupID > 0 then
    DiscCalculator.OrderType := otDriveThru;
  DiscCalculator.CalculateDiscounts(OLList, True, False, False, FDiscountPercent, FDiscountAmount, ComboFreeItems);

  LoyaltyReward := DiscCalculator.LoyaltyReward;
  FComboDiscount := DiscCalculator.ComboDiscount;

  if (DiscCalculator.IsComboSplit) or (tmpDiscountAmount <> FDiscountAmount) or
     (lComboDiscount <> FComboDiscount) or (AppDetails.EnableFiscalPrinting) then
    ReDisplayOLGrid;

  grdOLs.Refresh;
end;
{******************************************************************************}
procedure TformQuickSaleUser.UpdateOrderLines(TheOrderID, SPNumber: Integer; TheSP: TIB_StoredProc);
var
  I, J, TheLine: Integer;
  aOL, ModOL: TOrderLine;
  OrderLineOrder, OrderLineModifierOrder: Integer;
  aHash: string;

  function SaveOrderLine(APOL: TOrderLine): Integer;
  var
    aOrderLineID, K, OrderedModQty: Integer;

  begin
    //create order line
    aOrderLineID := 0;
    try
      with TheSP do
      begin
        SetStoredProcName('INSERT_ORDERLINE', SPNumber);   //insert_orderline

        IntegerAsParam(ParamByName('courseid'), APOL.CourseID); //Jon 28-02-2002
        ParamByName('qty').AsCurrency := APOL.Qty;
        ParamNull(ParamByName('notes'));
        ParamByName('happyhour').AsInteger := 0;
        ParamNull(ParamByName('positions'));
        ParamByName('itemid').AsInteger := APOL.ItemID; // JEH 18/08/2001 1 -> 0 + below...
        ParamByName('orderid').AsInteger := TheOrderID;

        if (APOL.ModsChangePrinter) then
        begin
          J := I + 1;
          while ((J < OLList.Count) and (TOrderLine(OLList[J]).PrinterID = 0) and (TOrderLine(OLList[J]).ItemID = 0)) do
          begin
            Inc(J);
          end;
          if ((J < OLList.Count) and (TOrderLine(OLList[J]).PrinterID > 0) and (TOrderLine(OLList[J]).ItemID = 0)) then
          begin
            ModOL := OLList.Items[J];
            ParamByName('printerid').AsInteger := ModOL.PrinterID;
            IntegerAsParam(ParamByName('repeatprinterid'), ModOL.RepeatPrinterID);
          end
          else
          begin
            APOL.ModsChangePrinter := False;
          end;
        end;

        if (not APOL.ModsChangePrinter) then
        begin
          IntegerAsParam(ParamByName('printerid'), APOL.PrinterID);
          IntegerAsParam(ParamByName('repeatprinterid'), APOL.RepeatPrinterID);
        end;

        ParamByName('held').AsInteger := 0;

        ParamByName('openprice').AsCurrency := APOL.UnitPrice - APOL.ModPrice; //Use this regardless, as we can't invoice/tender and order with different prices.
        if (APOL.OpenPrice) then
        begin
          StringAsParamIfBool(ParamByName('openpriceitemabbrev'), APOL.ItemString, (AppDetails.OrderOpenPriceNameChange and (APOL.ItemString <> '')));
        end
        else
        begin
          ParamNull(ParamByName('openpriceitemabbrev'));
        end;

        if ((APOL.OLPriceLevel >= 1) and (APOL.OLPriceLevel <= 6)) then
        begin
          ParamByName('pricelevel').AsInteger := APOL.OLPriceLevel;
        end
        else
        begin
          ParamByName('pricelevel').AsInteger := 0;
        end;

        ParamByName('currenthour').AsInteger := CurrentHour;
        ParamByName('currentminute').AsInteger := CurrentMin;
        ParamByName('currentday').AsInteger := CurrentDay;
        IntegerAsParam(ParamByName('orderlineorder'), OrderLineOrder);
        IntegerAsParam(ParamByName('comboid'), APOL.ComboID);
        IntegerAsParam(ParamByName('combogroup'), APOL.ComboGroupID);
        IntegerAsParam(ParamByName('comboindex'), APOL.ComboIndex);
        BoolAsParam(ParamByName('FixTheCombo'), APOL.FixThisComboPart);
        BoolAsParam(ParamByName('FreeComboItem'), APOL.FreeComboItem);
        ParamByName('comboqty').AsCurrency := APOL.ComboQty;

        if AppDetails.EnableFiscalPrinting then
        begin
          SWBEncryption.Initialise;
          SWBEncryption.AddInt(APOL.ItemID);
          SWBEncryption.AddStr(APOL.ItemString);
          SWBEncryption.AddStr(APOL.TaxSituation);
          SWBEncryption.AddStr(FloatToStr(APOL.SalesTaxPercent));
          SWBEncryption.AddCurrency(APOL.Qty);
          SWBEncryption.AddCurrency(APOL.UnitPrice);
          SWBEncryption.AddCurrency(APOL.Qty * APOL.UnitPrice);
          SWBEncryption.AddCurrency(0);
          aHash := SWBEncryption.GetHash;
          ParamByName('Checksum').AsString := aHash;
        end;

        ExecProc;
        aOrderLineID := FieldByName('orderlineid').AsInteger;

        TheLine := I;
        OrderLineModifierOrder := 0; //while not on the last line, try the next line for a mod, if not break loop

        while (TheLine < (OLList.Count - 1)) do
        begin
          Inc(TheLine);
          ModOL := OLList.Items[TheLine];
          if ((ModOL.ItemID = 0) and (ModOL.ModifierID <> 0)) then
          begin
            //create any modifiers for order line
            SetStoredProcName('INSERT_ORDERLINEMODIFIER', SPNumber);
            ParamByName('orderlineid').AsInteger := aOrderLineID;
            IntegerAsParamIfBool(ParamByName('modifierid'), ModOL.ModifierID, (ModOL.ModifierID > 0));
            StringAsParam(ParamByName('modifier'), ModOL.ItemString);
            IntegerAsParam(ParamByName('modgroupid'), ModOL.ModGroupID);
            IntegerAsParamIfBool(ParamByName('modfunction'), ModOL.ModifierFunction, AppDetails.EnableModifierFunction);

            if (ModOL.ModifierFunction = 2) then
            begin
              ParamByName('modprice').AsCurrency := -ModOL.ModPrice;
            end
            else
            begin
              ParamByName('modprice').AsCurrency := ModOL.ModPrice;
            end;
            
            if (ModOL.ModQtyOption = 0) then
              OrderedModQty := Round(ModOL.ModQtyOrdered-ModOL.ModQtyIncluded)
            else
              OrderedModQty := 1;

            for K := 1 to OrderedModQty do
            begin
              Inc(OrderLineModifierOrder);
              IntegerAsParam(ParamByName('orderlinemodifierorder'), OrderLineModifierOrder);
              ExecProc;
            end;

            OrderedModQty := Round(ModOL.ModQtyIncluded);
            ParamByName('modprice').AsCurrency := 0;
            for K := 1 to OrderedModQty do
            begin
              Inc(OrderLineModifierOrder);
              IntegerAsParam(ParamByName('orderlinemodifierorder'), OrderLineModifierOrder);
              ExecProc;
            end;

          end
          else
          begin
            Break;
          end;
        end;
      end;
    finally
      Result := aOrderLineID;
    end;
  end;

begin
  OrderLineOrder := 0;
  with TheSP do
  begin
    for I := 0 to (OLList.Count - 1) do
    begin
      aOL := OLList.Items[I];
      if (aOL.ItemID> 0) then
      begin
        if ((aOL.AlreadyOrdered) and (aOL.Qty > aOL.AlreadyOrderedQty)) then
        begin
          aOL.Qty := aOL.Qty - aOL.AlreadyOrderedQty;
          aOL.AlreadyOrdered := False;
        end;

        if (not aOL.AlreadyOrdered) then
          aOL.OrderLineID := SaveOrderLine(aOL);
      end;
    end;
  end;
end;

procedure TformQuickSaleUser.cmdComboIndexDesClick(Sender: TObject);
var
  frmComboColours: TFormCombosInSale;
begin
  frmComboColours := TFormCombosInSale.Create(nil);
  try
    frmComboColours.OLList := OLList;
    frmComboColours.AllCombos := FAllCombos;
    frmComboColours.LoadComboColours;
    frmComboColours.ShowModal;
  finally
    frmComboColours.Free;
  end;
end;

function TformQuickSaleUser.LoadQuickServiceComboPayToTabPay: Boolean;
var
  FinishedSale: Boolean;
  iGrpID: Integer;
begin
  Result := False;
  if (DiscCalculator.SaleHasCombos and (GlbTable.SaleCategoryType > scDriveThru) and (ShowConf(sSaveCurrentSaleAndGoToTabPay) = mrYes)) then // means running tab
  begin
    iGrpID := GlbTable.GroupID;
    FinishedSale := DoCashSale(True,7);

    if FinishedSale then
    begin
      ClearSelectedCustomerDetails;
      Result := True;
      GlbTable.GroupID := iGrpID;
      with dm.sp do
      begin
        SetStoredProcName('GET_TABLEINFO');
        ParamByName('pgroupid').AsInteger := iGrpID;
        BoolAsParam(ParamByName('getextendeddata'), True);
        ParamByName('pbookingsectionid').AsInteger := AppDetails.BookingsSectionID;
        ExecProc;
        SetTableValuesStatement(dm.sp);
      end;

      ReturnToCashSaleAfterPayment := True;
      StopInactiveTimer;
      try
        if CheckTableLock(True, False) then
          PaymentTable;
      finally
        CheckTableLock(False, True);
      end;
      ClearComboDiscounts;

      StartInactiveTimer;
    end;
  end;
end;

procedure TformQuickSaleUser.ClearComboDiscounts;
begin
  if Assigned(DiscCalculator) then
  begin
    DiscCalculator.ClearComboDiscounts;
    txtComboDisc.Text := '';
    FComboDiscount := 0;

    txtComboDisc.Visible := False;
    lblComboDisc.Visible := False;
    cmdComboIndex.Enabled := ((AppDetails.EnableCombo) and (DiscCalculator.SaleHasCombos));
  end;
end;

procedure TformQuickSaleUser.BroadcastCDAMessage;
begin
  if CDAServer.Connected then
  begin
    CDAServer.ProvideCDAData(ofQuickSale, FDelivery, FDiscountPercent, FDiscountAmount, FComboDiscount, FSalesTax, FToPay, FSurcharge,
                             StrToFloatDef(StripCommas(txtBalDue.Caption), 0), FStillDue, FChange, FFoodTotal,
                             FBevTotal, FTender,
                             OLList, SurchargeList, TLList);
  end;
end;

procedure TformQuickSaleUser.ClearCDAMessage;
begin
  if CDAServer.Connected then
  begin
    CDAServer.ClearCDAMessage;
  end;
end;

function TformQuickSaleUser.CheckFiscalPrinterFunctions: Boolean;
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

      if (iDate = 0) then
        DateNotMoved := True;
      //else if ((AppDetails.FiscalPrinterType = fpDaruma) and ((iDate = 0) or  (iDate = 01012000))) then
      //DateNotMoved := True;
      ShowManualSelection := False;
      if (ExitQS and DateNotMoved) then
      begin
        ExitQS := False;
        if ((not GlbPrivs[11].AvoidPINCheck) and (GlbLogin.LoginPIN = 0) and (not VerifyPinNo)) then
        begin
          ExitQS := True;
        end;
        ShowManualSelection := (AppDetails.FiscalManualMode = 0);
      end
      else
      begin
        ExitQS := False;
      end;

      if ExitQS then
      begin
        SFiscalPrinting.SupressFiscalErrorMessage := False;
        ClearStatus;
        ClearStatusPage;
        Result := False;
        if FormActivated then
        begin
          cmdHideClick(Self);
        end
        else
        begin
          PostMessage(Self.handle, WM_CLOSE, 0, 0);
        end;
        Exit;
      end
      else
      begin
        Application.ProcessMessages;
        if (not SFiscalPrinting.FiscalPrinterEnabled) then
        begin
          NFManual := ((AppDetails.FiscalManualMode = 0) and DateNotMoved);;
          Result := False;
          ShowManualSelection := ((AppDetails.FiscalManualMode = 0) and DateNotMoved);
          FCPFNumber := GetCPFNumber;
        end
        else
        begin
          SFiscalPrinting.GetMovementDate;
          ShowManualSelection := (((AppDetails.FiscalManualMode = 0) and DateNotMoved) or (AppDetails.FiscalManualMode = 1));
          if ((not SFiscalPrinting.GetFiscalPrinterDetails) or (GlbFiscal.ManufactureNo = '')) then
          begin
            PostMessage(self.handle, WM_CLOSE, 0, 0);
            Result := False;
            Exit;
          end;

          if not SFiscalPrinting.GetFiscalPrinterConfigurations then
          begin
            //ShowQuickMsg(sPrinterSystemDateMismatch);
            PostMessage(Self.Handle, WM_CLOSE, 0, 0);
            Result := False;
            Exit;
          end;
        end;
      end;
    end;

    ClearStatusPage;
    if (NFManual and (not NFManualSecond)) then
    begin
      SetStatus(sQuickSalesINManualNFMode, False, True);
    end
    else if (NFManual and NFManualSecond) then
    begin
      SetStatus(sQuickSalesINManualNFMode, False, False);
    end
    else if NFEMode then
    begin
      SetStatus(sQuickSalesINNFEMode, False, NFManual);
    end
    else
    begin
      SetStatus(sQuickSales, False, NFManual);
    end;
  finally
    SFiscalPrinting.SupressFiscalErrorMessage := False;
    Screen.Cursor := crDefault;
  end;
end;

procedure TformQuickSaleUser.ResetGlobalVariables;
begin
  ClearItemLevelDiscounts(OLList);
  //ClearItemLevelLoyalty;
  if Assigned(DiscCalculator) then
  begin
    DiscCalculator.ClearItemLevelDiscounts;
    DiscCalculator.ClearPrevTabItems;
  end;
  ResetTabDiscounts;
  FDiscountPercent := 0;
  FDiscountAmount := 0;
  FOrigDiscountAmount := 0;
  FOrigDiscountPercent := 0;
  FMaxDiscount := 0.00;
  FCurrentBalance := 0.00;

  FToPay := 0;
  FPayment := 0;
  FTip := 0;
  FTender := 0;

  if (LoyaltyReward.LoyaltyRewardID > 0) then
    ReDisplayOLGrid;
  LoyaltyReward.LoyaltyRewardID := 0;
  LoyaltyReward.LoyaltyReward := '';
  LoyaltyReward.LoyaltyPoints1 := 0;
  LoyaltyReward.LoyaltyPoints2 := 0;
  LoyaltyReward.Qty := 0;
  LoyaltyReward.RewardOffer := roNone;

  FLoyaltyFreeItemDiscount := 0;
  FLoyaltyPoints1 := 0;
  FLoyaltyPoints2 := 0;

  FUseDiscountAmount := False;
  FUseItemDiscounts := False;
  FUseDiscountPeriods := False;
  FLimitMaximum := False;

  txtDiscountPercent.Text := '';
  txtDiscountAmount.Text := '';
  txtAccountName.Text := '';
end;

procedure TformQuickSaleUser.ResetTabDiscounts;
begin
  pnlDiscounts.Visible := True; //(GlbTable.GroupID = 0) or ((GlbTable.GroupID > 0) and (GlbTable.SaleCategoryType > 0));
  lblComboDisc.Visible := pnlDiscounts.Visible;
  txtComboDisc.Visible := pnlDiscounts.Visible;
  if Assigned(DiscCalculator) then
  begin
    DiscCalculator.LoadPreviousOrders := ((GlbTable.GroupID > 0) and (GlbTable.SaleCategoryType = scStandard));
  end;
end;

procedure TformQuickSaleUser.SendOrderLineToFiscalPrinter(aOL: TOrderLine; GridIndex: Integer);
var
  aItemName, aTax, aUnit: string;
  aItemID: Integer;
  aQtyType, aRounding: string;
  aQty: Double;
  aUnitPrice, aDiscount: Currency;
  NCMTax: Currency;
begin
  if ((not AppDetails.EnableFiscalPrinting) or (not SFiscalPrinting.OpenedFiscalInvoice)) then
    Exit;

  aItemName := Trim(Copy(aOL.ItemString, 1, 29));
  aItemID := aOL.ItemID;

  if Trim(aOL.AllModDesc) <> '' then
  begin
    if AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC] then
    begin
      aItemName := Trim(Copy(aOL.ItemString, 1, 20)) + ':' + Trim(Copy(aOL.AllModDesc, 1, 179));
      SFiscalPrinting.ExtendItemDescription(aItemName);
      aItemName := Trim(Copy(aOL.ItemString, 1, 29));
    end
    else if AppDetails.FiscalPrinterType = fpDaruma then
    begin
      aItemName := Trim(Copy(aOL.ItemString, 1, 30)) + ':' + Trim(Copy(aOL.AllModDesc, 1, 200));
    end;
  end;

  if Trim(aOL.TaxSituation) = '' then
    aOL.TaxSituation := 'T';

  if AppDetails.FiscalPrinterType in [fpBematech, fpBematechNFC] then
  begin
    if (aOL.TaxSituation = 'T') then
    begin
      aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 0);
    end
    else if aOL.TaxSituation = 'S' then
    begin
      aTax := SFiscalPrinting.GetFiscalTaxIndex(aOL.SalesTaxPercent, 1);
    end
    else
    begin
      aTax := aOL.TaxSituation;
    end;
  end
  else if AppDetails.FiscalPrinterType = fpDaruma then
  begin
    if aOL.TaxSituation = 'T' then
    begin
      aTax := 'I' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100));
    end
    else if aOL.TaxSituation = 'S' then
    begin
      aTax := 'S' + FormatFloat('###########0000', (aOL.SalesTaxPercent * 100));
    end
    else if ((aOL.TaxSituation = 'II') or (aOL.TaxSituation = 'NN') or (aOL.TaxSituation = 'FF')) then
    begin
      aTax := aOL.TaxSituation;
    end
    else if aOL.TaxSituation = 'SI' then
    begin
      aTax := 'ISS';
    end
    else if aOL.TaxSituation = 'SN' then
    begin
      aTax := 'NS';
    end
    else if aOL.TaxSituation = 'SF' then
    begin
      aTax := 'FS';
    end;
  end;
  if AppDetails.TruncateAmount = 1 then
  begin
    aQty := TruncateTo(aOL.Qty - aOL.LoyaltyFreeQty, 3);
    aUnitPrice := TruncateTo(aOL.UnitPrice, 3);
    aDiscount := TruncateTo(aOL.ILDiscount, 2);
    aRounding := 'T';
  end
  else
  begin
    aQty := CRoundTo(aOL.Qty - aOL.LoyaltyFreeQty, 3);
    aUnitPrice := CRoundTo(aOL.UnitPrice, 3);
    aDiscount := CRoundTo(aOL.ILDiscount, 2);
    aRounding := 'A';
  end;

  if (not aOL.WeighedItem) then
  begin
    aUnit := 'UN';
    aQtyType := 'I';
  end
  else
  begin
    aUnit := 'KG';                              //0 = in Kilos 1 - in 10gm, 2 - in gm
    aQtyType := 'F';
  end;

  Application.ProcessMessages;

  if (AppDetails.FiscalPrinterType = fpBematechNFC)then
  begin
    NCMTax := SFiscalPrinting.GetNCMTax(aOL.NCMProductCode, aOL.CSOSN, aOL.ToPay);
    FTotalNCMTax := FTotalNCMTax + NCMTax;
    if not SFiscalPrinting.AddFiscalPrinterNFCInvoiceItems(aItemID, aOL.GTIN, aItemName,
        '01', aTax, aUnit, aQtyType, '3', aQty, '3', aUnitPrice, '$', '0', aDiscount,
        aRounding, aOL.NCMProductCode, IntToStr(aOL.CFOP), aOL.Notes, aOL.CSTICMS, IntToStr(aOL.ProductOrigin), '', '', '', '',
        '', aOL.CSOSN, '', '', '', '', '', '', '', '',
        '', '', '', '', '', '', FormatFloat(PRICEFORMAT, NCMTax), aOL.CST_PIS, '', '',
        '', '', '', aOL.CST_COFFINS, '', '', '', '', '', aOL.CEST) then
    begin
      DeletingComboItems  := AddingComboItems;
      grdOLs.RowCount := GridIndex;
      cmdVoidDesClick(Self);
    end;
  end
  else if (not SFiscalPrinting.AddFiscalPrinterInvoiceItems(aItemID, aItemName, aTax, aQty, aUnit, aUnitPrice, aDiscount)) then
  begin
    DeletingComboItems := AddingComboItems;
    grdOLs.RowCount := GridIndex;
    cmdVoidDesClick(Self);
  end;

  aOL.FiscalIndex := SFiscalPrinting.FiscalItemIndex;// will be calculated in the UFiscalPrinting unit.
end;

procedure TformQuickSaleUser.ClearItemLevelLoyalty;
var
  OL: TOrderLine;
  I: Integer;
begin
  for I := 0 to OLList.Count - 1 do
  begin
    OL := OLList.Items[I];
    OL.LoyaltyItemID := 0;
    OL.LoyaltyFreeQty := 0;
    OL.LoyaltyFreeDiscount := 0;
    OL.LoyaltyPoints1RatioNum := 0;
    OL.LoyaltyPoints1RatioDen := 1;
    OL.LoyaltyPoints1Qty := 0;
    OL.LoyaltyPoints2RatioNum := 0;
    OL.LoyaltyPoints2RatioDen := 1;
    OL.LoyaltyPoints2Qty := 0;
  end;
end;

procedure TformQuickSaleUser.ChangeQuickCashButtonCaption;
begin
  if (GlbTable.SaleCategoryType >= scDriveThru) then
  begin
    if ((not OnTable) or ((not OnAccount) and (((GlbTable.SaleCategoryType = scDriveThru) and (GlbTable.Uninvoiced = 0)) or (GlbTable.SaleCategoryType = scRunningTab)))) then
    begin
//      Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
      Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
      cmdQuickCash.Tag := 0;
      if (AppDetails.CashSaleUseTabNotes) then
//        Skin.ChangeButton(cmdDestn, bstNote, gstNoChange, biNone, sNote);
    end
    else if (OnAccount and (GlbTable.Uninvoiced > 0)) then
    begin
//      Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
      Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
      cmdQuickCash.Tag := 1;
    end;
  end
  else // if normal sale (means if salecategorytype = 0)
  begin
    if (OnTable) or (DoingPhoneOrder) then
    begin
//      Skin.ChangeButton(cmdQuickCash, bstOrder, gstNoChange, biNone, sOrder);
      Skin.ChangeButton(cmdQuickCashOK, bstOrder, gstNoChange, biNone, sOrder);
      cmdQuickCash.Tag := 0;
    end
    else if (cmdQuickCash.Tag <> 1) then
    begin
//      Skin.ChangeButton(cmdQuickCash, bstOK, gstNoChange, biNone, sOK);
      Skin.ChangeButton(cmdQuickCashOK, bstOK, gstNoChange, biNone, sOK);
      cmdQuickCash.Tag := 1;
    end;

    if (AppDetails.CashSaleUseTabNotes) then begin
//      Skin.ChangeButton(cmdDestn, bstWhere, gstNoChange, biDestination, sWhere);
    end;
  end;
  cmdQuickCashOK.Tag := cmdQuickCash.Tag;

end;

function TformQuickSaleUser.GetItemCount: Integer;
var
  I: Integer;
  aOL: TOrderLine;
begin
  Result := 0;
  for I := 0 to OLList.Count - 1 do
  begin
    aOL := TOrderLine(OLList.Items[I]);
    if (not aOL.WeighedItem) then
      Result := Result + Floor(aOL.Qty)
    else
      Inc(Result); // increment by 1 for weighted items
  end;
end;

procedure TformQuickSaleUser.GetSingleItemNote;
begin
  if (not AppDetails.EnableFiscalPrinting) then
    Exit;

  if (not GetSingleNote(FItemNote, 25, sItemName, sItemNameWithColon, tiPieSlice, True)) then
  begin
    StartInactiveTimer;
    StillAddingAnItem := False;
    Exit;
  end;
end;

procedure TformQuickSaleUser.AddFiscalItemNote;
var
  OL, ModOL: TOrderLine;
  TheRow: Integer;
  tmpMod: TComboModifier;

begin
  if ((not AppDetails.EnableFiscalPrinting) or (Trim(FItemNote) = '')) then
    Exit;

  TheRow := grdOLs.Row;
  OL := OLList.Items[TheRow];

  if Trim(OL.AllModDesc) = '' then
    OL.AllModDesc := Trim(FItemNote)
  else
    OL.AllModDesc := OL.AllModDesc + ',' + Trim(FItemNote);

  tmpMod := TComboModifier.Create;
  tmpMod.ModID := -1;
  tmpMod.Modifier := FItemNote;
  tmpMod.ModFunction := 1;
  tmpMod.PrinterID := OL.PrinterID;
  tmpMod.RepeatPrinterID := OL.RepeatPrinterID;
  tmpMod.ModQty := 1;
  tmpMod.ModQtyOption := 0;
  tmpMod.ModQtyOrdered := 1;
  OL.ComboMods.Add(tmpMod);

  ModOL := TOrderLine.Create;
  ModOL.ItemString := FItemNote;
  ModOL.ModifierID := -1;
  ModOL.ModifierFunction := 1;
  ModOL.PrinterID := tmpMod.PrinterID;
  ModOL.RepeatPrinterID := tmpMod.RepeatPrinterID;
  ModOL.ModQty := 1;
  ModOL.Qty := 1;
  ModOL.IsExistingOL := False;
  ModOL.OrderLineOrder := OLList.Count + 1;
  ModOL.WhenOrdered := Now;
  ModOL.ModQty := tmpMod.ModQty;
  ModOL.ModQtyOption := tmpMod.ModQtyOption;
  ModOL.ModQtyOrdered := tmpMod.ModQtyOrdered;
  ModOL.ModQtyIncluded := tmpMod.ModQtyIncluded;
  Inc(TheRow);

  OLList.Add(ModOL);
  AddLine;

  UpdateOLGridRow(TheRow, ModOL);

  FItemNote := '';
end;

procedure TformQuickSaleUser.cmdExchangeDesClick(Sender: TObject);
var
  frmGetCPF: TformGetCPFNumber;
  CPFNumber: string;
begin
  if (not DoingExchange) then
  begin
    if ((TLList.Count > 0) or (OLList.Count > 0)) then
    begin
      ShowMsg(sDeleteTenderLinesBeforeExchange);
      SetDoingExchange(False, True);
      Exit;
    end;

    if (OnTable) then
    begin
      ShowMsg(sNoExchangeAgainstTable);
      SetDoingExchange(False, True);
      Exit;
    end;

    frmGetCPF := TformGetCPFNumber.Create(Application);
    formSelectExchangeItems := TformSelectExchangeItems.Create(Application);
    try
      CPFNumber := '';
      if (not SelectInvoiceNo(FExchangeInvoiceID, CPFNumber, FExchangeNotes)) then
      begin
        SetDoingExchange(False, True);
        Exit;
      end;
      if AppDetails.EnableFiscalPrinting then
      begin
        if (Trim(CPFNumber) <> '') then
          FCPFNumber := CPFNumber;
        frmGetCPF.CPFNumber := FCPFNumber;
        frmGetCPF.CPFMode := mExchangeCPF;
        frmGetCPF.UpdateCPF := False;
        frmGetCPF.DisplayTaxDetails := True;
        frmGetCPF.ShowManualSelection := False;
        FCPFNumber := frmGetCPF.CPFNumber;
        if (frmGetCPF.ShowModal <> mrOk) then
        begin
          SetDoingExchange(False, True);
          Exit;
        end;
      end;
      with formSelectExchangeItems do
      begin
        ExInvoiceID := FExchangeInvoiceID;
        ExSelectedOLs := FExchangeOLs;
        FExchangeAmount := 0;

        if (ShowModal = mrOk) then
        begin
          FExchangeAmount := ExToPay - ExDiscountAmount;
          FExchangeDiscountAmount := ExDiscountAmount;
          FExchangeAccountID := ExAccountID;
          FExchangeDiscountSchemeID := ExDiscountSchemeID;
          FExchangeSalesTax := ExSalesTax;
          FExchangeDiscountPercent := ExDiscountPercent;
          if ((ExAccountID > 0) and (GlbAccount.AccountID <> ExAccountID)) then
            GlbAccount.AccountID := ExAccountID;
          SetAccount;
          SetDoingExchange(True, True);
        end
        else
        begin
          SetDoingExchange(False, True);
          Exit;
        end;
      end;
    finally
      formSelectExchangeItems.Free;
      frmGetCPF.Free;
    end;

    if (FExchangeAmount > 0) then
      AddAutoExchangeTender(FExchangeAmount);
  end
  else
  begin
    SetDoingExchange(False, True);
  end;
end;

procedure TformQuickSaleUser.ClearExchageTender;
var
  I: Integer;
  TL: TTL;
begin
  for I := 0 to TLList.Count - 1 do
  begin
    grdTLs.Row := I + 1;
    TL := TLList.Items[grdTLs.Row - 1];
    if TL.TLType = TLTExchange then
    begin
      if ((not AppDetails.EnableFiscalPrinting) or ((AppDetails.EnableFiscalPrinting) and (not TL.IsEFTPOSTrans))) then
        DeleteTenderLine;
    end;
  end;
end;

procedure TformQuickSaleUser.AddAutoExchangeTender(AmountToAdd: Currency);
var
  TL: TTL;
  TenderLineType: TTenderLineType;
begin
  TL := TTL.Create;
  TL.TLType := TLTExchange;
  TL.TLTypeID := GetTLTID(TL.TLType);
  TL.TLPayment := AmountToAdd;
  TL.TLTip := 0.00;
  TL.TLChange := 0.00;
  TL.TLTender := TL.TLPayment;
  TL.TLSurcharge := 0.00;

  TL.TLTender := RoundToRounding(TL.TLTender, RoundForTLType(TL.TLType));
  TL.TLRounding := TL.TLPayment - TL.TLTender;
  TL.TEFCreditCardDiscount := 0;
  TL.TLNotes := '';
  TenderLineType := GetTenderLineTypeRecord(TL.TLType);
  if (TenderLineType <> nil) then
  begin
    TL.SurchargeItemID := TenderLineType.SurchargeItemID;
    TL.SurchargeItemAbbrev := TenderLineType.SurchargeItemAbbrev;
    TL.SurchargePercent := TenderLineType.SurchargePercent;
    TL.SurchargeFixed := TenderLineType.SurchargeFixed;
    TL.SurchargeSalesTaxPercent := TenderLineType.SurchargeSalesTaxPercent;
    TL.SurchargeGLCode := TenderLineType.SurchargeGLCode;
    TL.SurchargeGLCode2 := TenderLineType.SurchargeGLCode2;
  end
  else
  begin
    TL.SurchargeItemID := 0;
  end;
  TLList.Add(TL);
  LoadTLs(LAAdd);
end;

function TformQuickSaleUser.TryExchangeRefund(TheSP: TIB_StoredProc; SPNumber: Integer): Boolean;
var
  I: Integer;
  ExchangeTL: TTL;
  ExchangeReasonID: Integer;
  BalanceDelta, SoFarDiscount, ILDiscount: Currency;
  TheInvoiceID, TheTenderID, TheVoidID: Integer;
  PrinterList: TIntegerList;
  QtyToVoid, QtyToWaste, FExchangeChange: Currency;
  Exp1, Exp2, aHash: string;
  OrderLine: TOrderLine;
  ToWaste: Boolean;
  VoidDisplayMode: TVoidDisplayMode;

begin
  Result := False;

  PrinterList := TIntegerList.Create;
  PrinterList.AutoSort := False;
  PrinterList.AllowDuplicates := False;
  FExchangeChange := 0;
  Exp1 := '';
  Exp2 := '';
  //Transaction will be started from TryCashSale;
  try
    BalanceDelta := 0.00;
    TheTenderID := 0;
    TheVoidID := 0;
    ExchangeTL := TLList.GetTLOfTypeID(GetTLTID(TLTExchange));

    if (not Assigned(ExchangeTL)) then
    begin
      Exit;
    end;

    ExchangeReasonID := AppDetails.ExchangeVoidReasonID;

    VoidDisplayMode := vdmRefund;
    if AppDetails.EnableFiscalPrinting then
      VoidDisplayMode := vdmExchange;

    if (ExchangeReasonID <= 0) and (not ShowVoid(QtyToVoid, QtyToWaste, ToWaste, 0, ExchangeReasonID, Exp1, Exp2, VoidDisplayMode, False)) then
      Exit;

    //loop through order lines
    for I := 0 to (FExchangeOLs.Count - 1) do
    begin
      OrderLine := FExchangeOLs.Items[I];
      //for each order line loop through walist twice, first for printer, then for ccprinter
      if ((OrderLine.PrinterID <> 0) and (not OrderLine.ModsChangePrinter)) then
      begin
        AddPrinterToPrinterList(PrinterList, OrderLine.PrinterID);

        //loop through walist, if repeat printer not found add
        if ((OrderLine.RepeatPrinterID > 0) and (not AppDetails.CashSaleDisableRepeatPrinter)) then
        begin
          AddPrinterToPrinterList(PrinterList, OrderLine.RepeatPrinterID);
        end;
      end;
    end;
    AddCCPrintersToPrinterList(PrinterList);

    //if not on table, will be printing, and no destination specified, set to WAName
    if ((not OnTable) and (PrinterList.Count > 0) and (FDelivery = '')) then
    begin
      FDelivery := AppDetails.WAName;
    end;

    Result := True;

    try
      with TheSP do
      begin
        if (AppDetails.OrdersLockWait) then
        begin
          SetStoredProcName('SETORDERMUTEX', SPNumber);
          ///setordermutex
          I := 0;
          repeat
            Inc(I);
            ExecProc;
          until ((FieldByName('errorcode').IsNull) or (I >= 3));

          if (not FieldByName('errorcode').IsNull) then
          begin
            ShowErrorMsg(FieldByName('errorcode').AsInteger);
          end;
        end;

        //create invoice record
        SetStoredProcName('INSERT_INVOICE', SPNumber); //insert_invoice

        ParamByName('discountrate').AsCurrency := FExchangeDiscountPercent;
        IntegerAsParam(ParamByName('accountid'), FExchangeAccountID);
        ParamByName('outletid').AsInteger := AppDetails.OutletID;
        //may be null from tables already open when Sale Categories introduced
        IntegerAsParam(ParamByName('salecategoryid'), GlbTable.SaleCategoryID);
        ParamNull(ParamByName('refundnote'));
        ParamByName('invoicetype').AsString := 'C';
        ParamByName('loginid').AsInteger := GlbLogin.LoginID;
        ParamByName('salestax').AsCurrency := -FExchangeSalesTax;
        ParamNull(ParamByName('cashsaleorderid'));
        if AppDetails.EnableFiscalPrinting then
        begin
          ParamByName('cpfnumber').AsString := FCPFNumber;
          StringAsParam(ParamByName('notes'), FExchangeNotes);
        end;

        ParamNull(ParamByName('FiscalCOO'));
        ParamNull(ParamByName('groupid'));
        if AppDetails.EnableFiscalPrinting then
        begin
          SWBEncryption.Initialise;
          SWBEncryption.AddCurrency(FExchangeAmount);
          SWBEncryption.AddCurrency(0);
          aHash := SWBEncryption.GetHash;
          StringAsParam(ParamByName('checksum'), aHash);
        end;

        ExecProc;

        if (not FieldByName('errorcode').IsNull) then
        begin
          Result := False;
          ShowErrorMsg(FieldByName('errorcode').AsInteger);
          Exit;
        end
        else
        begin
          TheInvoiceID := FieldByName('invoiceid').AsInteger;
        end;

        //for each cash sale line added
        //create order line, invoice line and the order line's modifier record
        SoFarDiscount := 0.00;

        for I := 0 to (FExchangeOLs.Count - 1) do
        begin
          OrderLine := FExchangeOLs.Items[I];
          //if an order line as opposed to a modifier
          if (OrderLine.ItemID > 0) then
          begin
            if (((OrderLine.ToPay > 0) or (AppDetails.CashSaleAllowZeroPriceInvoices)) and (TheInvoiceID > 0)) then
            begin
              SetStoredProcName('INSERT_INVOICELINE', SPNumber);

              OrderLine.ILAmount := -OrderLine.ToPay;
              ParamByName('ilamount').AsFloat := OrderLine.ILAmount;

              //if it's the last line, allocate whatever hasn't been pro rata-ed
              if I = FExchangeOLs.Count - 1 then // last item
              begin
                ILDiscount := FExchangeDiscountAmount - SoFarDiscount;
              end
              else
              begin //pro rata discount and accumulate discount pro rata-ed so far
                ILDiscount := OrderLine.AllowedDiscount;
                ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
                SoFarDiscount := SoFarDiscount + ILDiscount;
                SoFarDiscount := GetRoundedUpDown(SoFarDiscount, AppDetails.DecimalPlaces);
              end;

              ILDiscount := GetRoundedUpDown(ILDiscount, AppDetails.DecimalPlaces);
              OrderLine.ILDiscount := ILDiscount;
              ParamByName('discountamount').AsCurrency := -OrderLine.ILDiscount;
              ParamByName('invoiceid').AsInteger := TheInvoiceID;
              ParamByName('orderlineid').AsInteger := OrderLine.OrderLineID;
              IntegerAsParam(ParamByName('discountschemeid'), FExchangeDiscountSchemeID);
              ParamNull(ParamByName('FISCALSEQNO'));
              ParamNull(ParamByName('FISCALTAX'));
              if AppDetails.EnableFiscalPrinting then
              begin
                SWBEncryption.Initialise;
                //ManufactureNo
                //InvCOO
                //InvCCF
                SWBEncryption.AddInt(OrderLine.ItemID);
                SWBEncryption.AddStr(OrderLine.ItemString);
                SWBEncryption.AddStr(OrderLine.TaxSituation);
                SWBEncryption.AddStr(FloatToStr(OrderLine.SalesTaxPercent));
                SWBEncryption.AddCurrency(OrderLine.Qty);
                SWBEncryption.AddCurrency(OrderLine.UnitPrice);
                SWBEncryption.AddCurrency(ParamByName('ilamount').AsFloat - ParamByName('discountamount').AsFloat);
                SWBEncryption.AddCurrency(0);
                aHash := SWBEncryption.GetHash;
                ParamByName('CHECKSUM').AsString := aHash;
              end;
              //Account Balance
              if (AppDetails.TaxExclusivePrices) then
              begin
                BalanceDelta := BalanceDelta + ((OrderLine.ILAmount + OrderLine.ILDiscount) * (100 + OrderLine.SalesTaxPercent)) / 100;
              end
              else
              begin
                BalanceDelta := BalanceDelta + (OrderLine.ILAmount + OrderLine.ILDiscount);
              end;
              BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);
              ExecProc;
              OrderLine.InvoiceLineID := FieldByName('INVOICELINEID').AsInteger;

              if (AppDetails.FiscalType = fiscalTaxCore) then
              begin
                SetStoredProcName('INSERT_INVOICELINETAXCORE', SPNumber);
                ParamByName('invoicelineid').AsInteger := OrderLine.InvoiceLineID;
                StringAsParam(ParamByName('taxlabel'), OrderLine.TaxCoreLabel);
                ExecProc;
            end;
          end;
        end;
        end;
        // for time being surcharge is not handled - TODO
        // it also looks that combo discount not handled as no SoFarComboDiscount which is generally used when SoFarDiscount is

        if ((Assigned(ExchangeTL))) then
        begin
          //create tender record
          SetStoredProcName('INSERT_TENDER', SPNumber);
          ParamByName('tendertype').AsString := 'R';
          IntegerAsParam(ParamByName('accountid'), FExchangeAccountID);
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          BoolAsParam(ParamByName('printed'), AppDetails.PrintCashSale);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ParamNull(ParamByName('groupid'));
          ParamNull(ParamByName('nosaletypeid'));
          ParamNull(ParamByName('nosalenotes'));
          if AppDetails.EnableFiscalPrinting then
          begin
            SWBEncryption.Initialise;
            // ManufactureNo
            // InvCOO
            // CCF
            // CDC
            // GNF
            aHash := SWBEncryption.GetHash;
            ParamByName('CHECKSUM').AsString := aHash;
          end;
          ExecProc;
          TheTenderID := FieldByName('tenderid').AsInteger;

          //create tender lines

          if (ExchangeTL.TLTender > 0) then
          begin
            SetStoredProcName('INSERT_TENDERLINE', SPNumber);
            ParamByName('tenderid').AsInteger := TheTenderID;
            ParamByName('tenderlinetypeid').AsInteger := ExchangeTL.TLTypeID;
            ParamByName('tenderlineamount').AsCurrency := -ExchangeTL.TLTender;
            ParamByName('tenderlinetip').AsCurrency := -ExchangeTL.TLTip;
            ParamByName('roundingamount').AsCurrency := -ExchangeTL.TLRounding;
            BoolAsParam(ParamByName('changeamount'), False);
            ParamByName('tenderlinechange').AsCurrency := -ExchangeTL.TLChange;
            StringAsParam(ParamByName('tenderlinenotes'), ExchangeTL.TLNotes);
            //Account Balance
            BalanceDelta := BalanceDelta + (ExchangeTL.TLTender - ExchangeTL.TLTip + ExchangeTL.TLRounding);
            BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);

            ParamNull(ParamByName('eftpostxnref'));
            ParamNull(ParamByName('eftpostxnauthcode'));
            ParamNull(ParamByName('eftposcardref'));
            ParamNull(ParamByName('COO'));
            ParamNull(ParamByName('GNF'));
            ParamNull(ParamByName('CCF'));
            ParamNull(ParamByName('checksum'));
            IntegerAsParam(ParamByName('tenlinetypeproviderid'), ExchangeTL.TLProviderID);
            IntegerAsParam(ParamByName('instalments'), ExchangeTL.TLInstalments);
            ExecProc;
            ExchangeTL.TLID := FieldByName('tenderlineid').AsInteger;
          end;
          FExchangeChange := 0;
          //create a tender line for the change
          if (FExchangeChange > 0) then
          begin
            SetStoredProcName('INSERT_TENDERLINE', SPNumber);

            ParamByName('tenderid').AsInteger := TheTenderID;
            ParamByName('tenderlinetypeid').AsInteger := 4; //cash
            ParamByName('tenderlineamount').AsCurrency := FExchangeChange; //total change
            ParamByName('tenderlinetip').AsCurrency := 0;
            ParamByName('roundingamount').AsCurrency := 0;
            BoolAsParam(ParamByName('changeamount'), True);
            ParamByName('tenderlinechange').AsCurrency := 0;
            ParamNull(ParamByName('tenderlinenotes'));
            ParamNull(ParamByName('eftpostxnref'));
            ParamNull(ParamByName('eftposcardref'));
            //Account Balance
            BalanceDelta := BalanceDelta - FExchangeChange;
            BalanceDelta := GetRoundedUpDown(BalanceDelta, AppDetails.DecimalPlaces);

            ParamNull(ParamByName('COO'));
            ParamNull(ParamByName('GNF'));
            ParamNull(ParamByName('CCF'));
            ParamNull(ParamByName('checksum'));
            ParamNull(ParamByName('tenlinetypeproviderid'));
            ParamNull(ParamByName('instalments'));
            ExecProc;
          end;
        end;

        if (TheTenderID > 0) then
        begin
          SetStoredProcName('INSERT_VOID', SPNumber); //insert_void

          IntegerAsFindParam(FindParam('outletid'), AppDetails.OutletID);
          ParamNull(ParamByName('groupid'));
          ParamByName('voidreasonid').AsInteger := ExchangeReasonID;
          StringAsParam(ParamByName('voidexplanation1'), Exp1);
          StringAsParam(ParamByName('voidexplanation2'), Exp2);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          ExecProc;
          TheVoidID := FieldByName('voidid').AsInteger;

          for I := 0 to (FExchangeOLs.Count - 1) do
          begin
            OrderLine := FExchangeOLs.Items[I];
            //if an order line as opposed to a modifier
            if (OrderLine.ItemID > 0) then
            begin
              SetStoredProcName('VOID_ORDERLINE', SPNumber);

              ParamByName('voidid').AsInteger := TheVoidID;
              ParamByName('orderlineid').AsInteger := OrderLine.OrderLineID;
              ParamByName('qtyvoided').AsCurrency := OrderLine.Qty;
              ParamByName('qtywasted').AsCurrency := 0;
              if OrderLine.VoidType = vtWaste then
                ParamByName('qtywasted').AsCurrency := OrderLine.Qty;
              ParamByName('loginid').AsInteger := GlbLogin.LoginID;
              ExecProc;
            end;
          end;

          if (not AppDetails.EnableFiscalPrinting) then
          begin
            SetStoredProcName('RESTORE_STOCKVOID', SPNumber);
            ParamByName('voidid').AsInteger := TheVoidID;
            IntegerAsParamIfBool(ParamByName('terminalid'), AppDetails.TerminalID, AppDetails.StockEnabled);
            BoolAsParam(ParamByName('restoreavailqty'), True);
            ExecProc;
            if (not FieldByName('errorcode').IsNull) then
            begin
              Result := False;
              ShowErrorMsg(FieldByName('errorcode').AsInteger);
              Exit;
            end;
          end;
        end;

        if ((AppDetails.PrintRefund) and (not AppDetails.EnableFiscalPrinting)) then
        begin //insert print job
          InsertPrintJob('Credit Note', TheInvoiceID, AppDetails.PrinterID);
        end;

        if ((AppDetails.CashSalePrintVoids) and (TheVoidID > 0)) then
        begin
          for I := 0 to (PrinterList.Count - 1) do
          begin
            If (DoingWaste) then
              InsertPrintJob('Waste', TheVoidID, PrinterList.Get(I))
            else
              InsertPrintJob('Void', TheVoidID, PrinterList.Get(I));
          end;
        end;

        if ((GlbAccount.AccountID > 0) and (not DoingWaste)) then
        begin // if going on an account, set account balance
          SetStoredProcName('INSERTACCOUNTINVOICE', SPNumber); //insertaccountinvoice

          ParamByName('accountid').AsInteger := GlbAccount.AccountID;
          ParamByName('invoiceid').AsInteger := TheInvoiceID;
          IntegerAsParam(ParamByName('tenderid'), TheTenderID);
          ParamByName('loginid').AsInteger := GlbLogin.LoginID;
          CurrencyAsParam(ParamByName('currentbalancechange'), BalanceDelta);
          ExecProc;
        end;
      end;

      if (AppDetails.PDEnabled) then
      begin
        DMComponents.PDOutputItemLine('REFUNDED', GetRoundedUpDown(ExchangeTL.TLTender, AppDetails.DecimalPlaces), 0);
        if FExchangeChange > 0 then
          DMComponents.PDOutputItemLine('CHANGE', GetRoundedUpDown(FChange, AppDetails.DecimalPlaces), 1);
      end;

      //TheTR.Commit; called in trycashsale
      cmdTable.Enabled := True;
    except
      on e: exception do
      begin
        Result := False;
        ShowMessage(e.message);
        SelectFirsts;
      end;
    end;
  finally
    PrinterList.Free;
  end;
end;

procedure TformQuickSaleUser.cmdEventDesClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  PauseStaffPromptTimeoutTimer;
  try
    if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformCompletePaymentFirst then
      Exit;

    if (DoingWaste) then begin
      if (ShowConf(sConfirmCancellingWastage) <> mrYes) then begin
        Exit;
      end;
      SetDoingWaste(False, False);
    end;
    if (DoingExchange) then
    begin
      if (ShowConf(sConfirmCancellingExchange) <> mrYes) then
      begin
        Exit;
      end;
      SetDoingExchange(False, False);
    end;

    if ((OnTable xor (GlbTable.SaleCategoryType = scDriveThru)) and (GlbTable.SaleCategoryType <> scRunningTab)) then begin
      ShowMsg(sNoEventSelectWhileOrdering);
      Exit;
    end;

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
        DeselectEvent;
        Exit;
      end;

      lblEventName.Caption := '  ' + GlbEvent.EventName;
      if (GlbEvent.AccountID > 0) and (not LoadEventAccount) then  //LoadEventAccount sets up SubsidyCalc and calls CalulateToPay
        DeselectEvent;    //handled inactive accounts and Account Rank issues, so deselecting Event if account is not available to be used

    end
    else // an event is selected and button is down
    begin
      DeselectEvent;
    end;
  finally
    StartInactiveTimer;
    ResetStaffPromptTimeoutTimer;
  end;
end;

procedure TformQuickSaleUser.ClearItemLevelSubsidy;
var
  I: Integer;
  OL: TOrderLine;
begin
  for I := 0 to OLList.Count - 1 do
  begin
    OL := TOrderLine(OLList.Items[I]);
    OL.HostSubsidy := 0.00;
    OL.GuestSubsidy := 0.00;
  end;
end;

procedure TformQuickSaleUser.CalculateSubsidy(RecalculateAll:Boolean);
var
  I: Integer;
  aOL: TOrderLine;
begin
  {Calculate subsidy - have to apply combo discount before calculating subsidy
  to consider combo discount for guest. }

  if (OLList.Count > 0) then
  begin
    if ((GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (GlbEvent.SubsidyID > 0)) then
    begin
      SubsidyCalc.CalculateEventSubsidy(OLList);
      CalculateDiscountSchemeItemDiscount(RecalculateAll);

      FGuestSubsidy := 0;
      FHostSubsidy := 0;
      for I := 0 to (OLList.Count - 1) do
      begin
        aOL := OLList.Items[I];
        if (aOL.ItemID > 0) then
        begin
          FGuestSubsidy := FGuestSubsidy + aOL.GuestSubsidy;
          FHostSubsidy := FHostSubsidy + aOL.HostSubsidy;
        end;
      end;
    end;
  end;
end;

function TformQuickSaleUser.LoadEventAccount: Boolean;
begin
  Result := LoadAccount(GlbEvent.AccountID);
  if (not Result) then Exit;

  cmdOnAccount.Down := False;
  OnAccount := False;

  SubsidyCalc.EventID := GlbEvent.EventID;
  SubsidyCalc.AccountID := GlbEvent.AccountID;
  SubsidyCalc.SubsidyOverrided := GlbEvent.SubsidyOverrided;
  SubsidyCalc.SubsidyID := GlbEvent.SubsidyID;
  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (OnAccount) and (GlbEvent.SubsidyID > 0);
  //OnAccount is set to False above, thus looks like IsHostTransaction will also be False here
  CalculateToPay;
end;

procedure TformQuickSaleUser.DeselectEvent;
begin
  GlbEvent.Clear;
  SubsidyCalc.EventID := 0;
  SubsidyCalc.AccountID := 0;
  SubsidyCalc.SubsidyOverrided := False;
  SubsidyCalc.SubsidyID := 0;

  SubsidyCalc.ClearSubsidyDetails;
  ClearItemLevelSubsidy;
  DeselectAccount; // to deselect event account
  SubsidyCalc.IsHostTransaction := (GlbEvent.EventID > 0) and (GlbEvent.AccountID > 0) and (OnAccount) and (GlbEvent.SubsidyID > 0);

  CalculateToPay;

  cmdEvent.Down := False;
  lblEventName.Caption := '';
end;

{******************************************************************************}
procedure TformQuickSaleUser.cmdSearchDesClick(Sender: TObject);
begin
  inherited;
  StopInactiveTimer;
  PauseStaffPromptTimeoutTimer;

  grdItems.Perform(WM_SETREDRAW, 0, 0);

  SelectItems(True, False, False);
  SetView;
  
  grdItems.Perform(WM_SETREDRAW, 1, 0);
  grdItems.Invalidate;
  
  ResetStaffPromptTimeoutTimer;
  StartInactiveTimer;
end;

{******************************************************************************}
procedure TformQuickSaleUser.SelectModifiersItemInGrid;
var
  I: Integer;
  aOL: TOrderLine;
begin
  aOL := OLList.Items[grdOLs.Row];
  if ((aOL.ItemID <= 0) and (aOL.ModifierID <> 0)) then
  begin
    I := grdOLs.Row - 1;
    while (I >= 0) do
    begin
      if (TOrderLine(OLList[I]).ItemID > 0) then
      begin
        grdOLs.Row := I;
        Break;
      end;
      Dec(I);
    end;
  end;
end;

{******************************************************************************}
procedure TformQuickSaleUser.ShowExternalAccounts(TL: TTL = nil);
begin      
  if (not AllowTendering) then
  begin
    ShowMsg('Please de-select Tab, Phone Order or Waste before processing External Account payments');
    Exit;
  end;
  StopInactiveTimer;
  StopStaffPromptTimeoutTimer;
  sExternalAccountIntegration.SetOrderChanged(csCleanSent);
  SelectExternalAccount(GetDue(False, False), OLList, FDoingRefund, TL);

end;

{******************************************************************************}
procedure TformQuickSaleUser.CloseExternalAccounts;
begin
  if (Assigned(formSelectExternalAccount)) and (formSelectExternalAccount.Showing) then
    formSelectExternalAccount.cmdCancelClick(nil);
  
  ResetStaffPromptTimeoutTimer;
end;

{******************************************************************************}
procedure TformQuickSaleUser.UpdateTLListExternalAccounts;
begin
  DeleteCurrentExternalTenders;
  AddExternalTenders;
end;

{******************************************************************************}
procedure TformQuickSaleUser.DeleteCurrentExternalTenders;
var
  i: Integer;
  TL: TTL;
begin
  for i := TLList.Count-1 downto 0 do
  begin
    TL := TTL(TLList.Items[i]);
    if (tl.TLType = TLTExternalProviders) and (not tl.isProtected) then
      TLList.DisposeTL(i);
  end;
  LoadTLs(LADelete);
end;
{******************************************************************************}
procedure TformQuickSaleUser.AddExternalTenders;
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
              if (Assigned(FTenderLineCredit) and (FTenderLineCredit.TLTender <> 0) and (TLList.IndexOf(FTenderLineCredit) = -1)) then
              begin
                AddTenderLine(FTenderLineCredit.TLType, FTenderLineCredit);
                FTenderLineCredit.AddedToTLList := True;
              end;
              if (Assigned(FTenderLineLoyalty) and (FTenderLineLoyalty.TLTender > 0) and (TLList.IndexOf(FTenderLineLoyalty) = -1)) then
              begin
                AddTenderLine(FTenderLineLoyalty.TLType, FTenderLineLoyalty);
                FTenderLineLoyalty.AddedToTLList := True;
              end;
            end;
          end;
        end;
      end;
    end;
  end;
  CalculateToPay;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdExternalAccLookupDesClick(Sender: TObject);
begin
  DoExternalAccountButtonClick;
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdExternalAccRIPDesClick(Sender: TObject);
begin
  DoExternalAccountButtonClick(True);
end;
{******************************************************************************}
procedure TformQuickSaleUser.DoExternalAccountButtonClick(RIPButtonClicked: Boolean = False; ExternalAccType: Integer = 0);
begin
  if not sExternalAccountIntegration.AllowExternalAccounts(RIPButtonClicked) then
    Exit;

  if AppDetails.wiGroupEnabled and wiGroupPOSSrv.CheckInformExtAcctNotAllowed(True, True) then
    Exit;

  StopInactiveTimer;
  StopStaffPromptTimeoutTimer;

  if (not RIPButtonClicked) and (sExternalAccountIntegration.ProviderCardsPresent > 0) then
  begin
    ShowExternalAccounts;
    Exit;
  end;

  ExternalAccountLookup(OLList, RIPButtonClicked, ExternalAccType);
end;
{******************************************************************************}
procedure TformQuickSaleUser.cmdTaxCoreBuyerDetailDesClick(Sender: TObject);
begin
  inherited;
  GetTaxCoreBuyerDetail;
end;
{******************************************************************************}
procedure TformQuickSaleUser.SetGoodyAccountDiscountAmount;
begin
  FDiscountAmount := GlbGoody.TotalDiscount;

  //Code from TformQuickSaleUser.cmdDiscountAmountClick, after call to GetDiscountAmount
  FOrigDiscountAmount := FDiscountAmount;
  DiscCalculator.OrigDiscountAmount := FOrigDiscountAmount;
  FUseDiscountAmount := True;
  FUseItemDiscounts := False;
  FUseDiscountPeriods := False;
  lblAllowItemLevelDiscount.Caption := sEditedDiscountApplied;

  //Calculate discount and to pay amount
  CalculateToPay;

  BroadcastCDAMessage;
end;
{******************************************************************************}
function TformQuickSaleUser.TryAddGoodyDiscountAmount(ScannedAmount: string): Boolean;
var
  JustLoadedGoodyDiscAcc: Boolean;
  GoodyDiscountAmount, GoodyAccMaxDisc: Currency;
begin
  //NB: Code to load an account was originally copied from OpenArchButtonClick where qsbaAccount and OpenArchButton.AssociatedItem2 <= 0.

  Result := False;
  JustLoadedGoodyDiscAcc := False;
  GoodyDiscountAmount := 0;
  GoodyAccMaxDisc := 0;

  if DoingTransaction then
    Exit;

  //Check not doing a Refund
  if FDoingRefund then
  begin
    ShowMsg(sNoRefundForGoodyScan);
    Exit;
  end;

  //Check don't have an Event selected
  if GlbEvent.EventID <> 0 then
  begin
    ShowMsg(sNoEventForGoodyScan);
    Exit;
  end;

  //Check not on a Tab
  if OnTable then
  begin
    ShowMsg(sNoGoodyScanWhileTabSelected);
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

    //NB: LoadAccount: DisplayAccount ensures account is active and user has privilege to Edit Invoice Discount (13)
    GlbAccount.LookupAccountID := AppDetails.GoodyDiscountAccountID;
    if not LoadAccount(GlbAccount.LookupAccountID, True) then
    begin
      DeselectAccount;
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

  StartInactiveTimer;

  Result := True;
  ShowQuickMsg(Format(sGoodyDiscApplied, [FormatCurrencySign(GoodyDiscountAmount)]));
end;
{******************************************************************************}
procedure TformQuickSaleUser.DeselectGoodyAccount;
begin
  //if Goody discount account loaded, deselect now
  if GlbAccount.AccountID = AppDetails.GoodyDiscountAccountID then
    DeselectAccount;
end;
{******************************************************************************}
end.
