unit ini_project;
 
 
interface
 
 
uses
 
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
 
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, IniFiles;
 
 
type
 
  TForm1 = class(TForm)
 
    Username: TEdit;
 
    Passwrod: TEdit;
 
    Submit: TButton;
 
    Status: TLabel;
 
    Content: TLabel;
 
    DeleteBtn: TButton;
 
    procedure FormCreate(Sender: TObject);
 
    procedure SubmitClick(Sender: TObject);
 
    procedure DeleteBtnClick(Sender: TObject);
 
  private
 
  public
 
  end;
 
 
var
 
  Form1: TForm1;
 
  IniFilePath: string;
 
 
implementation
 
 
{$R *.dfm}
 
 
// FUNCTION: OnCreate Event Function
 
procedure TForm1.FormCreate(Sender: TObject);
 
var
 
  Ini: TIniFile;
 
  StoredUsername, StoredPassword: string;
 
begin
 
  IniFilePath := ExtractFilePath(ParamStr(0)) + 'UserData.ini';
 
 
  Ini := TIniFile.Create(IniFilePath);
 
  try
 
    StoredUsername := Ini.ReadString('Login', 'Username', '');
 
    StoredPassword := Ini.ReadString('Login', 'Password', '');
 
 
    if (StoredUsername = '') and (StoredPassword = '') then
 
      Content.Caption := ''
 
    else
 
      Content.Caption := 'Username: ' + StoredUsername + sLineBreak +
 
                         'Password: ' + StoredPassword;
 
  finally
 
    Ini.Free;
 
  end;
 
end;
 
 
// FUNCTION: OnClick Submit Button Function
 
procedure TForm1.SubmitClick(Sender: TObject);
 
var
 
  Ini: TIniFile;
 
  UName, PWord: string;
 
  StoredUsername, StoredPassword: string;
 
begin
 
  UName := Trim(Username.Text);
 
  PWord := Trim(Passwrod.Text);
 
 
  if (UName = '') or (PWord = '') then
 
  begin
 
    Status.Caption := 'Please enter both Username and Password.';
 
    Exit;
 
  end;
 
 
  Ini := TIniFile.Create(IniFilePath);
 
  try
 
    Ini.WriteString(UName, 'Username', UName);
 
    Ini.WriteString(UName, 'Password', PWord);
 
    Status.Caption := 'Entry Added Successfully!';
 
 
    StoredUsername := Ini.ReadString(UName, 'Username', '');
 
    StoredPassword := Ini.ReadString(UName, 'Password', '');
 
 
    if (StoredUsername = '') and (StoredPassword = '') then
 
      Content.Caption := ''
 
    else
 
      Content.Caption := 'Username: ' + StoredUsername + sLineBreak +
 
                         'Password: ' + StoredPassword;
 
  finally
 
    Ini.Free;
 
  end;
 
end;
 
 
// FUNCTION: OnClick Delete Button Function
 
procedure TForm1.DeleteBtnClick(Sender: TObject);
 
var
 
  Ini: TIniFile;
 
  UName, PWord, StoredPassword: string;
 
begin
 
  UName := Trim(Username.Text);
 
  PWord := Trim(Passwrod.Text);
 
 
  if (UName = '') or (PWord = '') then
 
  begin
 
    Status.Caption := 'Please enter both Username and Password to delete.';
 
    Exit;
 
  end;
 
 
  Ini := TIniFile.Create(IniFilePath);
 
  try
 
    if Ini.SectionExists(UName) then
 
    begin
 
      StoredPassword := Ini.ReadString(UName, 'Password', '');
 
      if StoredPassword = PWord then
 
      begin
 
        Ini.EraseSection(UName);
 
        Status.Caption := 'Section deleted successfully.';
 
        Content.Caption := '';
 
      end
 
      else
 
        Status.Caption := 'Incorrect password. Section not deleted.';
 
    end
 
    else
 
      Status.Caption := 'Section not found.';
 
  finally
 
    Ini.Free;
 
  end;
 
end;
 
 
end.
