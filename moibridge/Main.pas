unit Main;

interface

uses
  SysUtils, Windows, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, Buttons, ExtCtrls, Menus, ComCtrls, ToolWin, Registry;

type
  TfrmMain = class(TForm)
    MainMenu: TMainMenu;
    FileNewItem: TMenuItem;
    FileOpenItem: TMenuItem;
    FileSaveItem: TMenuItem;
    FileSaveAsItem: TMenuItem;
    FileExitItem: TMenuItem;
    StatusLine: TStatusBar;
    OpenDialog: TOpenDialog;
    SaveDialog: TSaveDialog;
    ToolBar1: TToolBar;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    OpenSprites1: TMenuItem;
    OpenDialog1: TOpenDialog;
    SpeedButton6: TSpeedButton;
    SpriteList: TListBox;
    Animation: TPaintBox;
    Timer: TTimer;
    ToolButton4: TToolButton;
    SpeedButton7: TSpeedButton;
    SpeedButton8: TSpeedButton;
    SpeedButton9: TSpeedButton;
    SpeedButton10: TSpeedButton;
    SpeedButton11: TSpeedButton;  { E&xit }
    procedure FormCreate(Sender: TObject);
    procedure ShowHint(Sender: TObject);
    procedure FileNew(Sender: TObject);
    procedure FileOpenAnimation(Sender: TObject);
    procedure FileSave(Sender: TObject);
    procedure FileSaveAs(Sender: TObject);
    procedure FileExit(Sender: TObject);
    procedure FileOpenSprites(Sender: TObject);
    procedure SpeedButton8Click(Sender: TObject);
    procedure SpeedButton7Click(Sender: TObject);
    procedure TimerTimer(Sender: TObject);
    procedure SpriteListDblClick(Sender: TObject);
    procedure SpeedButton9Click(Sender: TObject);
    procedure SpeedButton10Click(Sender: TObject);
    procedure SpriteListClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private

  public
     OpenSpritesInitialDir: String;
     OpenAnimationInitialDir: String;
     Registry: TRegistry;
     AnimIndex: Integer;
     ListDiff: Integer;
  end;

var
  frmMain: TfrmMain;

implementation

uses SpriteView;

{$R *.DFM}

procedure TfrmMain.FormCreate(Sender: TObject);
begin
   Application.OnHint := ShowHint;
   ListDiff := frmMain.Height - SpriteList.Height;
   AnimIndex := 0;
   Registry := TRegistry.Create;
   If Registry.KeyExists('MOIBRIDGE') then
      begin
      Registry.OpenKey('MOIBRIDGE', FALSE);
      if Registry.ValueExists('OpenSpritesInitialDir') then
         OpenSpritesInitialDir := Registry.ReadString('OpenSpritesInitialDir')
      else
         begin
         Registry.WriteString('OpenSpritesInitialDir', 'C:\');
         OpenSpritesInitialDir := 'C:\';
         end;
      if Registry.ValueExists('OpenAnimationInitialDir') then
         OpenAnimationInitialDir := Registry.ReadString('OpenAnimationInitialDir')
      else
         begin
         Registry.WriteString('OpenAnimationInitialDir', 'C:\');
         OpenAnimationInitialDir := 'C:\';
         end
      end
   else
      begin
      Registry.CreateKey('MOIBRIDGE');
      Registry.OpenKey('MOIBRIDGE', FALSE);
      Registry.WriteString('OpenSpritesInitialDir', 'C:\');
      OpenSpritesInitialDir := 'C:\';
      Registry.WriteString('OpenAnimationInitialDir', 'C:\');
      OpenAnimationInitialDir := 'C:\';
      end;
end;

procedure TfrmMain.ShowHint(Sender: TObject);
begin
  StatusLine.SimpleText := Application.Hint;
end;

procedure TfrmMain.FileNew(Sender: TObject);
begin
  { Add code to create a new file }
end;

procedure TfrmMain.FileOpenAnimation(Sender: TObject);
begin
  if OpenDialog.Execute then
  begin
    { Add code to open OpenDialog.FileName }
  end;
end;

procedure TfrmMain.FileSave(Sender: TObject);
begin
   { Add code to save current file under current name }
end;

procedure TfrmMain.FileSaveAs(Sender: TObject);
begin
  if SaveDialog.Execute then
  begin
    { Add code to save current file under SaveDialog.FileName }
  end;
end;

procedure TfrmMain.FileExit(Sender: TObject);
begin
  Close;
end;

procedure TfrmMain.FileOpenSprites(Sender: TObject);
   var filename: string;
   var finalfn: string;
   var fn_ind: Integer;
   var tc: Char;
begin
   OpenDialog1.FileName := '*.bmp';
   OpenDialog1.InitialDir := OpenSpritesInitialDir;
   if OpenDialog1.Execute then
      begin
      frmSpriteView.DoLoadSprites(OpenDialog1.FileName);
      filename := OpenDialog1.FileName;
      fn_ind := Length(filename);
      repeat
         tc := filename[fn_ind];
         fn_ind := fn_ind - 1;
      until(tc = '\');
      finalfn := Copy(filename, 0, fn_ind+1);
      Registry.WriteString('OpenSpritesInitialDir',finalfn);
      end;
end;

procedure TfrmMain.SpeedButton8Click(Sender: TObject);
begin
   Timer.Interval := StrToInt(InputBox('Animation Display Interval', 'Enter Interval In MS', ''));
end;

procedure TfrmMain.SpeedButton7Click(Sender: TObject);
begin
   if SpeedButton7.Down = FALSE then
      begin
      Timer.Enabled := FALSE;
      SpriteList.Enabled := TRUE;
      end
   else if SpriteList.Items.Count > 0 then
      begin
      AnimIndex := 0;
      Timer.Enabled := TRUE;
      SpriteList.Enabled := FALSE;
      end
   else
      SpeedButton7.Down := FALSE;
end;

procedure TfrmMain.TimerTimer(Sender: TObject);
   var Frame: Integer;
begin
   if SpriteList.Items.Count > 0 then
      begin
      Frame := StrToInt(SpriteList.Items.Strings[AnimIndex]);
      Animation.Canvas.Draw(0, 0, frmSpriteView.Sprites[Frame]);
      StatusLine.SimpleText := Format('Frame: %d/%d   Sprite Index: %d   Interval: %d ms', [AnimIndex,SpriteList.Items.Count-1,Frame,Timer.Interval]);
      AnimIndex :=  AnimIndex + 1;
      If AnimIndex > (SpriteList.Items.Count - 1) then
         AnimIndex := 0;
      end;
end;

procedure TfrmMain.SpriteListDblClick(Sender: TObject);
begin
   SpriteList.Items.Delete(SpriteList.ItemIndex);
end;

procedure TfrmMain.SpeedButton9Click(Sender: TObject);
   var Frame: Integer;
begin
   if SpriteList.Items.Count > 0 then
      begin
      AnimIndex :=  AnimIndex - 1;
      If AnimIndex < 0 then
         AnimIndex := (SpriteList.Items.Count - 1);
      Frame := StrToInt(SpriteList.Items.Strings[AnimIndex]);
      Animation.Canvas.Draw(0, 0, frmSpriteView.Sprites[Frame]);
      StatusLine.SimpleText := Format('Frame: %d/%d   Sprite Index: %d   Interval: %d ms', [AnimIndex,SpriteList.Items.Count-1,Frame,Timer.Interval]);
      end;
end;

procedure TfrmMain.SpeedButton10Click(Sender: TObject);
   var Frame: Integer;
begin
   if SpriteList.Items.Count > 0 then
      begin
      AnimIndex :=  AnimIndex + 1;
      If AnimIndex > (SpriteList.Items.Count - 1) then
         AnimIndex := 0;
      Frame := StrToInt(SpriteList.Items.Strings[AnimIndex]);
      Animation.Canvas.Draw(0, 0, frmSpriteView.Sprites[Frame]);
      StatusLine.SimpleText := Format('Frame: %d/%d   Sprite Index: %d   Interval: %d ms', [AnimIndex,SpriteList.Items.Count-1,Frame,Timer.Interval]);
      end;

end;

procedure TfrmMain.SpriteListClick(Sender: TObject);
   var Frame: Integer;
begin
   Frame := StrToInt(SpriteList.Items.Strings[SpriteList.ItemIndex]);
   Animation.Canvas.Draw(0, 0, frmSpriteView.Sprites[Frame]);
   StatusLine.SimpleText := Format('Frame: %d/%d   Sprite Index: %d   Interval: %d ms', [SpriteList.ItemIndex,SpriteList.Items.Count-1,Frame,Timer.Interval]);
end;

procedure TfrmMain.FormResize(Sender: TObject);
begin
   SpriteList.Height := frmMain.Height - ListDiff;
end;

end.
