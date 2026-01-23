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
