unit UServerContainer;

interface

uses System.SysUtils, System.Classes, System.StrUtils, System.Hash, system.JSON,
  Datasnap.DSServer, Datasnap.DSCommonServer, Datasnap.DSSession,
  Datasnap.DSReflect, Datasnap.DSNames, REST.Types,
  IPPeerServer, IPPeerAPI, Datasnap.DSAuth, Web.HTTPApp, Web.ReqMulti, UDM,
  URegistrationTest;

type
  TServerContainer = class(TDataModule)
    DSServer: TDSServer;
    DSAuthenticationManager: TDSAuthenticationManager;
    DSServerService: TDSServerClass;
    DSServerReports: TDSServerClass;
    DSServerPeople: TDSServerClass;
    DSServerSetup: TDSServerClass;
    DSServerDeliverect: TDSServerClass;
    procedure DSAuthenticationManagerUserAuthorize(Sender: TObject;
      EventObject: TDSAuthorizeEventObject; var valid: Boolean);
    procedure DSAuthenticationManagerUserAuthenticate(Sender: TObject;
      const Protocol, Context, User, Password: string; var valid: Boolean;
      UserRoles: TStrings);
    procedure DSServerSetupGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
    procedure DSServerPeopleGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
    procedure DSServerReportsGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
    procedure DSServerServiceGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
    procedure DSServerDeliverectGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
    procedure DataModuleCreate(Sender: TObject);
    procedure DataModuleDestroy(Sender: TObject);
  private
    FRegistered: Boolean;
    FRegistrationTest: TRegistrationTest;

    procedure LogRequest;
    procedure RegistrationTestFailure(Sender: TObject);
    procedure RegistrationTestPass(Sender: TObject);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  end;

function DSServer: TDSServer;
function DSAuthenticationManager: TDSAuthenticationManager;
function MainDM: TDM;

threadvar
  FRequest: TWebRequest;
  FResponse: TWebResponse;

implementation


{$R *.dfm}

uses
  UServerMethodsSetup, UServerMethodsPeople, UServerMethodsReports, UServerMethodsService, UAppDetails, ULogFile,
  UServerMethodsDeliverect;

var
  FModule: TComponent;
  FDSServer: TDSServer;
  FDSAuthenticationManager: TDSAuthenticationManager;
  FMainDM: TDM;

function DSServer: TDSServer;
begin
  Result := FDSServer;
end;

function DSAuthenticationManager: TDSAuthenticationManager;
begin
  Result := FDSAuthenticationManager;
end;

function MainDM: TDM;
begin
  Result := FMainDM;
end;

{ TServerContainer }

constructor TServerContainer.Create(AOwner: TComponent);
begin
  inherited;
  FDSServer := DSServer;
  FDSAuthenticationManager := DSAuthenticationManager;
  AppDetails.LoadDBDetails;
  FMainDM := DM;
end;

procedure TServerContainer.DataModuleCreate(Sender: TObject);
begin
  FRegistered := False;
  FRegistrationTest := TRegistrationTest.Create(Self);
  FRegistrationTest.TR := dm.tr;
  FRegistrationTest.QR := dm.qr;
  FRegistrationTest.ServerHost := AppDetails.onetapServerHost;
  FRegistrationTest.OnFailure := RegistrationTestFailure;
  FRegistrationTest.OnPass := RegistrationTestPass;
  FRegistrationTest.DoTest;
end;

procedure TServerContainer.DataModuleDestroy(Sender: TObject);
begin
  FRegistrationTest.Free;
  FreeDM;
end;

destructor TServerContainer.Destroy;
begin
  inherited;
  FDSServer := nil;
  FDSAuthenticationManager := nil;
end;

procedure TServerContainer.DSServerDeliverectGetClass(
  DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
begin
  PersistentClass := UServerMethodsDeliverect.Deliverect;
end;

procedure TServerContainer.DSServerPeopleGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
begin
  PersistentClass := UServerMethodsPeople.People;
end;

procedure TServerContainer.DSServerReportsGetClass(DSServerClass: TDSServerClass;
  var PersistentClass: TPersistentClass);
begin
  PersistentClass := UServerMethodsReports.Reports;
end;

procedure TServerContainer.DSServerServiceGetClass(DSServerClass: TDSServerClass;
  var PersistentClass: TPersistentClass);
begin
  PersistentClass := UServerMethodsService.Service;
end;

procedure TServerContainer.DSServerSetupGetClass(DSServerClass: TDSServerClass; var PersistentClass: TPersistentClass);
begin
  PersistentClass := UServerMethodsSetup.Setup;
end;

procedure TServerContainer.LogRequest;
function MyStreamToString(aStream: TStream): string;
var
  SS: TStringStream;
begin
  if aStream <> nil then
  begin
    SS := TStringStream.Create('');
    try
      SS.CopyFrom(aStream, 0); // No need to position at 0 nor provide size
      Result := SS.DataString;
    finally
      SS.Free;
    end;
  end
  else
    Result := '';
end;
var
  s: string;
  I: Integer;
begin
  if (Assigned(FRequest)) then
  begin
    try
      case FRequest.MethodType of
        mtAny:
          s := 'Any';
        mtGet:
          s := 'Get';
        mtPut:
          s := 'Put';
        mtPost:
          s := 'Post';
        mtHead:
          s := 'Head';
        mtDelete:
          s := 'Delete';
        mtPatch:
          s := 'Patch';
      else
        s := 'Unknown'
      end;
      s := s.PadRight(8) + FRequest.PathInfo;
      if (FRequest.Query <> '') then
        s := s + '?' + FRequest.Query;

      if (FRequest.Content <> '') then
        s := s + #13#10 + FRequest.Content;

      LogFileMessage(ltGeneral, FRequest.ContentType);
      LogFileMessage(ltGeneral, s);

      if (FRequest.Files.Count > 0) then
      begin
        LogFileMessage(ltGeneral, 'File Count: ' + FRequest.Files.Count.ToString);
        for I := 0 to FRequest.Files.Count - 1 do
        begin
          LogFileMessage(ltGeneral, FRequest.Files.Items[i].FieldName);
          LogFileMessage(ltGeneral, FRequest.Files.Items[i].FileName);
          LogFileMessage(ltGeneral, FRequest.Files.Items[i].ContentType);
          LogFileMessage(ltGeneral, MyStreamToString(FRequest.Files.Items[i].Stream));
        end;
      end;
    except
      on e: Exception do
      begin
        LogFileException(e, e.ClassName, True);
      end;
    end;
  end;
end;

procedure TServerContainer.RegistrationTestFailure(Sender: TObject);
var
  OI: TOutletInfo;
begin
  //The're a thief
  FRegistered := False;
  LogFileMessage(ltGeneral, Format('Registration Failure - %s',[FRegistrationTest.FailureReasonString]));
  OI := FRegistrationTest.GetOutletInfoIndex(0);
  if Assigned(OI) then
    AppDetails.RegistrationCode := OI.RegistrationCode;
end;

procedure TServerContainer.RegistrationTestPass(Sender: TObject);
var
  I, MaxI: Integer;
  OI: TOutletInfo;
  s: string;
begin
  MaxI := (FRegistrationTest.OutletCount - 1);
  for I := MaxI downto 0 do
  begin
    //Now we just need to whinge if their expiry date is getting close
    OI := FRegistrationTest.GetOutletInfoIndex(I);

{    if (not OI.LicenceInfo.ProgramsKiosk) then
    begin
      LogFileMessage(ltGeneral, 'Not licenced to use onetap API');
      Exit;
    end; }

    if ((not OI.LicenceInfo.ExpiryUnlocked) and ((OI.LicenceInfo.ExpiryDate - Now) < 30)) then
    begin
      if ((OI.LicenceInfo.ExpiryDate - Now) < 1) then
      begin
        LogFileMessage(ltGeneral, 'This is the last day the system will work on the current Licence Key');
      end
      else
      begin
        LogFileMessage(ltGeneral, Format('There are %s days left until the current Licence Key expires', [IntToStr(Trunc(OI.LicenceInfo.ExpiryDate - Now))]));
      end;
    end;
    AppDetails.RegistrationCode := OI.RegistrationCode;
  end;

  FRegistered := True;
end;

procedure TServerContainer.DSAuthenticationManagerUserAuthenticate(
  Sender: TObject; const Protocol, Context, User, Password: string;
  var valid: Boolean; UserRoles: TStrings);
var
  Session: TDSSession;
begin
  { TODO : Validate the client user and password.
    If role-based authorization is needed, add role names to the UserRoles parameter  }
  Session := TDSSessionManager.GetThreadSession;
  if Assigned(FRequest) then
  begin
    if (FRequest.GetFieldByName('x-server-authorization-hmac-sha256') <> '') then
    begin
     LogFileMessage(ltGeneral, FRequest.GetFieldByName('x-server-authorization-hmac-sha256'));
     LogFileMessage(ltGeneral, TEncoding.UTF8.GetString(FRequest.RawContent));
     LogFileMessage(ltGeneral, THashSHA2.GetHMAC(TEncoding.UTF8.GetString(FRequest.RawContent), '63450b8c54015109ca034bc6'));
    end;

    if (FRequest.GetFieldByName('x-forwarded-for') <> '') then
    begin
      Session.PutData('RemoteAddr', FRequest.GetFieldByName('x-forwarded-for'));
      LogFileMessage(ltGeneral, (FRequest.RemoteAddr + ' x-forwarded-for ' + FRequest.GetFieldByName('x-forwarded-for')).PadRight(52) + FRequest.UserAgent);
    end
    else
    begin
      Session.PutData('RemoteAddr', FRequest.RemoteAddr);
      LogFileMessage(ltGeneral, FRequest.RemoteAddr.PadRight(52) + FRequest.UserAgent);
    end;
  end;



  valid := (((uppercase(User) = uppercase(AppDetails.UserName)) and (uppercase(password) = uppercase(AppDetails.Password))) or
            (Assigned(FRequest) and (ContainsText(FRequest.PathInfo.ToLower, '/wizbang/restapi/deliverect'))));  //Allow url containing text to be Authenticated
  if (valid) then
    LogFileMessage(ltGeneral, 'User Authenticated')
  else
    LogFileMessage(ltGeneral, 'User not Authenticated');

  if (not Valid) then
    LogRequest;
end;

procedure TServerContainer.DSAuthenticationManagerUserAuthorize(
  Sender: TObject; EventObject: TDSAuthorizeEventObject;
  var valid: Boolean);
begin
  { TODO : Authorize a user to execute a method.
    Use values from EventObject such as UserName, UserRoles, AuthorizedRoles and DeniedRoles.
    Use DSAuthenticationManager1.Roles to define Authorized and Denied roles
    for particular server methods. }
  valid := True;
  LogRequest;
end;

initialization
  FModule := TServerContainer.Create(nil);
finalization
  FModule.Free;
end.


-----------------------------------------------------------------------------------------------------------

unit UWebModule;

interface

uses
  System.SysUtils, System.Classes, System.StrUtils, Web.HTTPApp, Datasnap.DSHTTPCommon,
  Datasnap.DSHTTPWebBroker, Datasnap.DSServer,
  Web.WebFileDispatcher, Web.HTTPProd,
  DataSnap.DSAuth,
  Datasnap.DSProxyJavaScript, IPPeerServer, Datasnap.DSMetadata, Datasnap.DSServerMetadata, Datasnap.DSClientMetadata,
  Datasnap.DSCommonServer, Datasnap.DSHTTP,
  REST.JsonCustom,
  IdCompressorZLib, IdZLib,
  REST.Types, System.JSON, Data.DBXCommon;

type
  TWebDispatcher = class(TWebModule)
    DSHTTPWebDispatcher: TDSHTTPWebDispatcher;
    WebFileDispatcher: TWebFileDispatcher;
    DSProxyGenerator: TDSProxyGenerator;
    DSServerMetaDataProvider: TDSServerMetaDataProvider;
    procedure WebModule1DefaultHandlerAction(Sender: TObject;
      Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
    procedure WebFileDispatcherBeforeDispatch(Sender: TObject;
      const AFileName: string; Request: TWebRequest; Response: TWebResponse;
      var Handled: Boolean);
    procedure WebModuleCreate(Sender: TObject);
    procedure WebModuleAfterDispatch(Sender: TObject; Request: TWebRequest; Response: TWebResponse;
      var Handled: Boolean);
    procedure WebModuleBeforeDispatch(Sender: TObject; Request: TWebRequest; Response: TWebResponse;
      var Handled: Boolean);
    procedure DSHTTPWebDispatcherFormatResult(Sender: TObject; var ResultVal: TJSONValue; const Command: TDBXCommand;
      var Handled: Boolean);
    procedure DSHTTPWebDispatcherHTTPTrace(Sender: TObject; AContext: TDSHTTPContext; ARequest: TDSHTTPRequest;
      AResponse: TDSHTTPResponse);
    procedure WebDispatcherHealthPageAction(Sender: TObject;
      Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
  private
    { Private declarations }
    function CompressWebResponse(Content: String; UseGZip: Boolean): TMemoryStream;
  public
    { Public declarations }
  end;

var
  WebModuleClass: TComponentClass = TWebDispatcher;

implementation


{$R *.dfm}

uses UServerContainer, Web.WebReq, UAppDetails, ULogFile, UData.Helper;

procedure TWebDispatcher.WebModule1DefaultHandlerAction(Sender: TObject;
  Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
begin
  Response.Content :=
    '<!DOCTYPE html>' +
    '<html>' +
    '<head>' +
    '    <meta charset="utf-8" />' +
    '    <meta name="viewport" content="width=device-width" />' +
    '    <title>onetap API Test Page v' + AppDetails.FileVersion + '</title>' +
    '    <link href="/js/bootstrap.css" rel="stylesheet"/>' +
    '<link href="/js/site.css" rel="stylesheet"/>' +
    '    <script src="/js/modernizr-2.6.2.js"></script>' +
    '</head>' +
    '<body>' +
    '    <div class="navbar navbar-inverse navbar-fixed-top">' +
    '        <div class="container">' +
    '            <div class="navbar-header">' +
    '                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">' +
    '                    <span class="icon-bar"></span>' +
    '                    <span class="icon-bar"></span>' +
    '                    <span class="icon-bar"></span>' +
    '                </button>' +
    '                <a class="navbar-brand" href="/">onetap API</a>' +
    '            </div>' +
    '            <div class="navbar-collapse collapse">' +
    '                <ul class="nav navbar-nav">' +
    '                    <li><a href="/">Home</a></li>' +
    '                </ul>' +
    '            </div>' +
    '        </div>' +
    '    </div>' +
    '    <div class="container body-content">' +
    '<div class="jumbotron">' +
    '    <h1>onetap API <small>v' + AppDetails.APIVersion + '</small></h1>' +
    '    <p>If you can see this, then you''re most of the way there. It means that API has been installed correctly, and the application is running</p>' +
    '</div>' +
    '<div class="row">' +
    '    <div class="col-md-4">' +
    '        <h2>Getting started</h2>' +
    '        <p>You may need to check that the application and database connection is properly setup. Call onetap support on 0800 949 2264.</p>' +
    '    </div>' +
    '    <div class="col-md-4">' +
    '        <h2>Test Page</h2>' +
    '        <p>You may need to check that the application and database connection is properly setup. Call onetap support on 0800 949 2264.</p>' +
    '    </div>' +
    '</div>' +
    '        <hr />' +
    '        <footer>' +
    '            <p>API Version: ' + AppDetails.APIVersion + '<br>File Version: ' + AppDetails.FileVersion + '<br>Database Version: ' + AppDetails.DatabaseVersion +'</p>' +
    '            <p><a href="http://www.wizbang.co.nz">&copy; 2021 - WizBang Technologies</a></p>' +
    '        </footer>' +
    '    </div>' +
    '    <script src="/js/bootstrap.js"></script>' +
    '<script src="/js/respond.js"></script>' +
    '</body>' +
    '</html>';
end;

procedure TWebDispatcher.WebModuleAfterDispatch(Sender: TObject; Request: TWebRequest; Response: TWebResponse;
  var Handled: Boolean);
var
  AcceptEncoding: string;
begin
  AcceptEncoding := Request.GetFieldByName('Accept-Encoding');
  if ((Response.ContentLength > 1024) and (Response.ContentType = CONTENTTYPE_APPLICATION_JSON) and
    ((ContainsText(AcceptEncoding, 'gzip')) or (ContainsText(AcceptEncoding, 'deflate')))) then
  begin
    Response.ContentStream := CompressWebResponse(Response.Content, ContainsText(AcceptEncoding, 'gzip'));
    if (ContainsText(AcceptEncoding, 'gzip')) then
      Response.ContentEncoding := 'gzip'
    else
      Response.ContentEncoding := 'deflate';
    Response.Content := '';
  end;

  FRequest := nil;
  FResponse := nil;
end;

procedure TWebDispatcher.WebModuleBeforeDispatch(Sender: TObject; Request: TWebRequest; Response: TWebResponse;
  var Handled: Boolean);
begin
  FRequest := Request;
  FResponse := Response;
end;

procedure TWebDispatcher.DSHTTPWebDispatcherFormatResult(Sender: TObject; var ResultVal: TJSONValue;
  const Command: TDBXCommand; var Handled: Boolean);
var
  Aux: TJSONValue;
begin
  try
    //Remove second array
    if ((ResultVal is TJSONArray) and (TJSONArray(ResultVal).Count > 0)) and (TJSONArray(ResultVal).Items[0] is TJSONArray) then
    begin
      Aux := ResultVal;
      ResultVal := TJSONArray(Aux).Items[0];
      TJSONArray(Aux).Remove(0); //remove the item so it isn't disposed when array is freed
      Aux.Free;
    end;
  except
    on e : Exception do
    begin
      LogFileException(e, e.ClassName, True);
    end;
  end;
end;

procedure TWebDispatcher.DSHTTPWebDispatcherHTTPTrace(Sender: TObject; AContext: TDSHTTPContext;
  ARequest: TDSHTTPRequest; AResponse: TDSHTTPResponse);
var
  s: string;
begin
  s := 'Sending Result';
  if (AResponse.ContentText <> '') then
    s := s + #13#10 + AResponse.ContentText;

  LogFileMessage(ltGeneral, s);
end;

procedure TWebDispatcher.WebDispatcherHealthPageAction(Sender: TObject;
  Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
var
  JSONObject: TJSONObject;
begin
  if (Request.PathInfo.ToLower = '/status/health.html') then
  begin
    Response.Content := '<html>' + '<head><title>onetap API Service</title></head>' + '<body>onetap API Service is running</body>' +
      '</html>';
  end
  else if (Request.PathInfo.ToLower = '/status/health.json') then
  begin
    JSONObject := TJSONObject.Create;
    try
      StringAsParam(JSONObject, 'Status', 'running');
      StringAsParam(JSONObject, 'API Version', AppDetails.APIVersion);
      StringAsParam(JSONObject, 'File Version', AppDetails.FileVersion);
      StringAsParam(JSONObject, 'Database Version', AppDetails.DatabaseVersion);

      Response.ContentType := 'application/json';
      Response.Content := JSONObject.ToString;
    finally
      JSONObject.Free;
    end;
  end
  else if (Request.PathInfo.ToLower = '/status/settings.json') then
  begin
      Response.ContentType := 'application/json';
      Response.Content := TJsonCustom.ObjectToJsonString(AppDetails);
  end
  else
    WebModule1DefaultHandlerAction(Sender, Request, Response, Handled);
end;

procedure TWebDispatcher.WebFileDispatcherBeforeDispatch(Sender: TObject;
  const AFileName: string; Request: TWebRequest; Response: TWebResponse;
  var Handled: Boolean);
var
  D1, D2: TDateTime;
begin
  Handled := False;
  if SameFileName(ExtractFileName(AFileName), 'serverfunctions.js') then
    if not FileExists(AFileName) or (FileAge(AFileName, D1) and FileAge(WebApplicationFileName, D2) and (D1 < D2)) then
    begin
      DSProxyGenerator.TargetDirectory := ExtractFilePath(AFileName);
      DSProxyGenerator.TargetUnitName := ExtractFileName(AFileName);
      DSProxyGenerator.Write;
    end;
end;

procedure TWebDispatcher.WebModuleCreate(Sender: TObject);
begin
  DSServerMetaDataProvider.Server := DSServer;
  DSHTTPWebDispatcher.DSContext := 'wizbang';
  DSHTTPWebDispatcher.DSPort := AppDetails.DSPort;
  DSHTTPWebDispatcher.RESTContext := 'restapi';
  DSHTTPWebDispatcher.WebDispatch.PathInfo := 'wizbang*';
  DSHTTPWebDispatcher.Server := DSServer;

  if DSServer.Started then
  begin
    DSHTTPWebDispatcher.DbxContext := DSServer.DbxContext;
    DSHTTPWebDispatcher.Start;
  end;
  DSHTTPWebDispatcher.AuthenticationManager := DSAuthenticationManager;
end;

function TWebDispatcher.CompressWebResponse(Content: String; UseGZip: Boolean): TMemoryStream;
var
  LDecompressed: TStringStream;
  LCompress: TIdCompressorZLib;
begin
  LDecompressed := TStringStream.Create(Content);
  LCompress := TIdCompressorZLib.Create();
  try
    Result := TMemoryStream.Create;
    if (UseGZip) then
      LCompress.CompressStream(LDecompressed, Result, 9, GZIP_WINBITS, 9, 0)
    else
      LCompress.CompressHTTPDeflate(LDecompressed, Result, 4);

    Result.Position := 0;
  finally
    LDecompressed.Free;
    LCompress.Free
  end;
end;

initialization
finalization
  Web.WebReq.FreeWebModules;

end.

