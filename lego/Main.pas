unit Main;

interface

uses
  SysUtils, Windows, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, StdCtrls, Buttons, ExtCtrls, Menus, ComCtrls, ToolWin, Registry;

type
  TMainForm = class(TForm)
    MainMenu: TMainMenu;
    FileNewItem: TMenuItem;
    FileOpenItem: TMenuItem;
    FileSaveItem: TMenuItem;
    FileSaveAsItem: TMenuItem;
    FileExitItem: TMenuItem;
    StatusLine: TStatusBar;
    OpenDialog: TOpenDialog;
    SaveDialog: TSaveDialog;
    FileOpenTileSetItem: TMenuItem;
    OpenDialog1: TOpenDialog;
    MapBox: TPaintBox;
    ToolBar1: TToolBar;
    ToolButton2: TToolButton;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    ToolButton7: TToolButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    SpeedButton7: TSpeedButton;
    SpeedButton8: TSpeedButton;
    SpeedButton9: TSpeedButton;
    ToolButton1: TToolButton;
    SpeedButton10: TSpeedButton;
    SpeedButton11: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure ShowHint(Sender: TObject);
    procedure FileNew(Sender: TObject);
    procedure FileOpen(Sender: TObject);
    procedure FileSave(Sender: TObject);
    procedure FileSaveAs(Sender: TObject);
    procedure FileExit(Sender: TObject);
    procedure FileOpenTileSet(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure MapPaint(Sender: TObject);
    procedure MapBoxMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure MapBoxMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure ToolbarMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure StatusLineMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure NewMap;
    procedure SaveMap;
    procedure DoLoadMap(fname: string);
    procedure SpeedButton9Click(Sender: TObject);
    procedure SpeedButton8Click(Sender: TObject);
    procedure SpeedButton7Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton11Click(Sender: TObject);
    procedure SpeedButton10Click(Sender: TObject);
  public
    MapArray: array[0..127,0..127] of Integer;
    WalkArray: array[0..127,0..127] of Integer;
    MMColArray: array[0..127,0..127] of TColor;
    WorldOffsetX: Integer;
    WorldOffsetY: Integer;
    ViewableX: Integer;
    ViewableY: Integer;
    CanDraw: Boolean;
    Registry: TRegistry;
    OpenTilesetInitialDir: string;
    OpenMapInitialDir: string;
    NeedsSave: Boolean;
    LoadedTileSet: Boolean;
    procedure Init();
  end;

var
  MainForm: TMainForm;

implementation

uses TileSet, MiniMap;

{$R *.DFM}

procedure TMainForm.FormCreate(Sender: TObject);
begin
  Application.OnHint := ShowHint;
end;

procedure TMainForm.ShowHint(Sender: TObject);
begin
  StatusLine.SimpleText := Application.Hint;
end;

procedure TMainForm.FileNew(Sender: TObject);
begin
   if NeedsSave then
      begin
      If Application.MessageBox('Would You Like To Save Your Current Map?', 'Save?', MB_YESNO) = IDNO then
         begin
         NewMap();
         end
      else
         begin
         FileSave(Sender);
         NewMap();
         end;
      end
   else
      begin
      NewMap();
      end;
end;

procedure TMainForm.FileOpen(Sender: TObject);
   var filename: string;
   var finalfn: string;
   var fn_ind: Integer;
   var tc: Char;
begin
   If NeedsSave then
      If Application.MessageBox('Would You Like To Save Your Current Map?', 'Save?', MB_YESNO) = IDYES then
         SaveMap();
   OpenDialog.FileName := '*.dmf';
   OpenDialog.InitialDir := OpenMapInitialDir;
   if OpenDialog.Execute then
      begin
      DoLoadMap(OpenDialog.FileName);
      filename := OpenDialog.FileName;
      fn_ind := Length(filename);
      repeat
         tc := filename[fn_ind];
         fn_ind := fn_ind - 1;
      until(tc = '\');
      finalfn := Copy(filename, 0, fn_ind+1);
      Registry.WriteString('OpenMapInitialDir',finalfn);
      end;
end;

procedure TMainForm.FileSave(Sender: TObject);
begin
   if SaveDialog.FileName <> '*.dmf' then
      begin
      SaveMap();
      end
   else
      begin
      FileSaveAs(Sender);
      end;
end;

procedure TMainForm.FileSaveAs(Sender: TObject);
   var fn_ind: Integer;
   var tc: Char;
   var filename: String;
begin
   if SaveDialog.Execute then
      begin
      fn_ind := Length(SaveDialog.FileName);
      filename := SaveDialog.FileName;
      repeat
         tc := SaveDialog.FileName[fn_ind];
         fn_ind := fn_ind - 1;
         if fn_ind < 0 then
            begin
            SaveDialog.FileName := Concat(filename, '.dmf');
            break;
            end;
      until(tc = '.');
      SaveMap();
      end;
end;

procedure TMainForm.FileExit(Sender: TObject);
begin
   Registry.Destroy;
   TileSetForm.Close;
   Close;
end;

procedure TMainForm.FileOpenTileSet(Sender: TObject);
   var filename: string;
   var finalfn: string;
   var fn_ind: Integer;
   var tc: Char;
begin
   if LoadedTileSet = FALSE then
      begin
      OpenDialog1.FileName := '*.bmp';
      OpenDialog1.InitialDir := OpenTilesetInitialDir;
      if OpenDialog1.Execute then
         begin
         TileSetForm.DoLoadTileSet(OpenDialog1.FileName);
         filename := OpenDialog1.FileName;
         fn_ind := Length(filename);
         repeat
            tc := filename[fn_ind];
            fn_ind := fn_ind - 1;
         until(tc = '\');
         finalfn := Copy(filename, 0, fn_ind+1);
         Registry.WriteString('OpenTilesetInitialDir',finalfn);
         end;
      end;
end;

procedure TMainForm.FormResize(Sender: TObject);
begin
   MapBox.Width := Integer(MainForm.ClientWidth div 32) * 32;
   MapBox.Height := Integer((MainForm.ClientHeight - StatusLine.Height - Toolbar1.Height) div 32) * 32;
   MainForm.ClientWidth := MapBox.Width;
   MainForm.ClientHeight := MapBox.Height + StatusLine.Height + Toolbar1.Height;
   ViewableX := MapBox.Width div 32;
   ViewableY := MapBox.Height div 32;
end;

procedure TMainForm.MapPaint(Sender: TObject);
   var index_x: Integer;
   var index_y: Integer;
   var x: Integer;
   var y: Integer;
begin
   MapBox.Canvas.Brush.Color := $00000000;
   MapBox.Canvas.Brush.Style := bsSolid;
   MapBox.Canvas.FillRect(Rect(0,0,MapBox.Width,MapBox.Height));
   if CanDraw then
      begin
      for index_y := WorldOffsetY to 127 do
         begin
         for index_x := WorldOffsetX to 127 do
            begin
            If MapArray[index_y,index_x] > -1 then
               MapBox.Canvas.Draw((index_x-WorldOffsetX) * 32, (index_y-WorldOffsetY) * 32, TileSetForm.Tiles[MapArray[index_y, index_x]]);
            end;
         end;
      end;
   MapBox.Canvas.Pen.Color := $00FFFFFF;
   MapBox.Canvas.Pen.Style := psDot;
   x := 0;
   y := 0;
   if MainForm.SpeedButton11.Down = True then
      begin
      repeat
         x := x + 32;
         MapBox.Canvas.MoveTo(x, 0);
         MapBox.Canvas.LineTo(x, MapBox.Height);
      until x >= MapBox.Width;
      repeat
         y := y + 32;
         MapBox.Canvas.MoveTo(0, y);
         MapBox.Canvas.LineTo(MapBox.Width, y);
      until y >= MapBox.Height;
      end;
end;

procedure TMainForm.MapBoxMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
  var where_x: Integer;
  var where_y: Integer;
begin
   if CanDraw then
      begin
      where_x := WorldOffsetX + (X div 32);
      where_y := WorldOffsetY + (Y div 32);

      MapArray[where_y, where_x] := TileSetForm.TileSelectedNum;
      MiniMapForm.MM.Canvas.Pixels[where_x, where_y] := MMColArray[TileSetForm.TileSelectedY,TileSetForm.TileSelectedX];
      MapBox.Repaint;
      MiniMapForm.MiniMapBox.Repaint;
      NeedsSave := TRUE;
      end
   else
      begin
      Application.MessageBox('You Cant Place Tiles Until You Load A TileSet!', 'Please Load TileSet', MB_OK);
      end;
end;

procedure TMainForm.MapBoxMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, WorldOffsetX + (X div 32), WorldOffsetY + (y div 32), MapArray[Y div 32, X div 32]]);
end;

procedure TMainForm.SpeedButton1Click(Sender: TObject);
begin
   if WorldOffsetX > 0 then
      WorldOffsetX := WorldOffsetX - 1;
   MapBox.Repaint;
   MiniMapForm.MiniMapBox.Repaint;
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.SpeedButton2Click(Sender: TObject);
begin
   if WorldOffsetX < 128-ViewableX then
      WorldOffsetX := WorldOffsetX + 1;
   MapBox.Repaint;
   MiniMapForm.MiniMapBox.Repaint;
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.SpeedButton3Click(Sender: TObject);
begin
   if WorldOffsetY > 0 then
      WorldOffsetY := WorldOffsetY - 1;
   MapBox.Repaint;
   MiniMapForm.MiniMapBox.Repaint;
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.SpeedButton4Click(Sender: TObject);
begin
   if WorldOffsetY < 128-ViewableY then
      WorldOffsetY := WorldOffsetY + 1;
   MapBox.Repaint;
   MiniMapForm.MiniMapBox.Repaint;
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.ToolbarMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.StatusLineMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
begin
   StatusLine.SimpleText := Format('World Offsets: (%d,%d)   Current Tile: (%d,%d)   Value: (%d)', [WorldOffsetX, WorldOffsetY, -1, -1, -1]);
end;

procedure TMainForm.NewMap();
   var ix: Integer;
   var iy: Integer;
begin
   for iy := 0 to 127 do
      begin
      for ix := 0 to 127 do
         begin
         MapArray[iy,ix] := -1;
         end;
      end;
   for iy := 0 to 127 do
      begin
      for ix := 0 to 127 do
         begin
         WalkArray[iy,ix] := -1;
         end;
      end;
   for iy := 0 to 127 do
      begin
      for ix := 0 to 127 do
         begin
         MMColArray[iy,ix] := $00000000;
         end;
      end;
   MiniMapForm.ClearMM();
   NeedsSave := FALSE;
   WorldOffsetX := 0;
   WorldOffsetY := 0;
   SaveDialog.FileName := '*.dmf';
   MapBox.Repaint;
end;

procedure TMainForm.SaveMap();
   var F: TextFile;
   var nx: Integer;
   var ny: Integer;
   var sc: char;
begin
   sc := ' ';
   AssignFile(F, SaveDialog.FileName);
   Rewrite(F);
   Writeln(F, OpenDialog1.FileName);
   for ny := 0 to 127 do
      for nx := 0 to 127 do
         begin
         Write(F, MapArray[ny,nx]);
         Write(F, sc);
         end;
   Write(F, TileSetForm.NumTilesY);
   Write(F, sc);
   Write(F, TileSetForm.NumTilesX);
   Write(F, sc);
   for ny := 0 to (TileSetForm.NumTilesY-1) do
      for nx := 0 to (TileSetForm.NumTilesX-1) do
         begin
         Write(F, WalkArray[ny,nx]);
         Write(F, sc);
         end;
   for ny := 0 to (TileSetForm.NumTilesY-1) do
      for nx := 0 to (TileSetForm.NumTilesX-1) do
         begin
         Write(F, MMColArray[ny,nx]);
         Write(F, sc);
         end;
   CloseFile(F);
   NeedsSave := FALSE;
end;

procedure TMainForm.DoLoadMap(fname: string);
   var F: TextFile;
   var nx: Integer;
   var ny: Integer;
   var fn: string;
begin
   AssignFile(F, fname);
   Reset(F);
   Readln(F, fn);
   OpenDialog1.FileName := fn;
   for ny := 0 to 127 do
      for nx := 0 to 127 do
         begin
         Read(F, MapArray[ny,nx]);
         end;
   Read(F, TileSetForm.NumTilesY);
   Read(F, TileSetForm.NumTilesX);
   for ny := 0 to (TileSetForm.NumTilesY-1) do
      for nx := 0 to (TileSetForm.NumTilesX-1) do
         begin
         Read(F, WalkArray[ny,nx]);
         end;
   for ny := 0 to (TileSetForm.NumTilesY-1) do
      for nx := 0 to (TileSetForm.NumTilesX-1) do
         begin
         Read(F, MMColArray[ny,nx]);
         end;
   CloseFile(F);
   for ny := 0 to 127 do
      for nx := 0 to 127 do
         begin
         MiniMapForm.MM.Canvas.Pixels[nx,ny] := MMColArray[MapArray[ny,nx] div TileSetForm.NumTilesY,MapArray[ny,nx] - ((MapArray[ny,nx] div TileSetForm.NumTilesY)* TileSetForm.NumTilesY)];
         end;
   MiniMapForm.MiniMapBox.Repaint;
   if LoadedTileSet then
      TileSetForm.Close;
   if FileExists(OpenDialog1.FileName) then
      TileSetForm.DoLoadTileset(OpenDialog1.FileName)
   else
      FileOpenTileSetItem.Click;
   NeedsSave := FALSE;
   SaveDialog.FileName := fname;
   MapBox.Repaint;
end;

procedure TMainForm.SpeedButton9Click(Sender: TObject);
begin
  FileNew(Sender);
end;

procedure TMainForm.SpeedButton8Click(Sender: TObject);
begin
   FileOpen(Sender);
end;

procedure TMainForm.SpeedButton7Click(Sender: TObject);
begin
   FileOpenTileset(Sender);
end;

procedure TMainForm.SpeedButton6Click(Sender: TObject);
begin
   FileSave(Sender);
end;

procedure TMainForm.SpeedButton5Click(Sender: TObject);
begin
   FileSaveAs(Sender);
end;

procedure TMainForm.SpeedButton11Click(Sender: TObject);
begin
   MapBox.Repaint;
end;

procedure TMainForm.SpeedButton10Click(Sender: TObject);
begin
   TileSetForm.TileSetBox.Repaint;
end;

procedure TMainForm.Init();
begin
   NewMap();
   CanDraw := FALSE;
   LoadedTileSet := FALSE;
   ViewableX := 15;
   ViewableY := 15;
   MainForm.ClientWidth := 15*32;
   MainForm.ClientHeight := 15*32+Toolbar1.Height+StatusLine.Height;
   MapBox.Width := MainForm.ClientWidth;
   MapBox.Height := MainForm.ClientHeight-StatusLine.Height;
   Registry := TRegistry.Create;
   If Registry.KeyExists('LEGO') then
      begin
      Registry.OpenKey('LEGO', FALSE);
      if Registry.ValueExists('OpenTilesetInitialDir') then
         OpenTilesetInitialDir := Registry.ReadString('OpenTilesetInitialDir')
      else
         begin
         Registry.WriteString('OpenTilesetInitialDir', 'C:\');
         OpenTilesetInitialDir := 'C:\';
         end;
      if Registry.ValueExists('OpenMapInitialDir') then
         OpenMapInitialDir := Registry.ReadString('OpenMapInitialDir')
      else
         begin
         Registry.WriteString('OpenMapInitialDir', 'C:\');
         OpenMapInitialDir := 'C:\';
         end
      end
   else
      begin
      Registry.CreateKey('LEGO');
      Registry.OpenKey('LEGO', FALSE);
      Registry.WriteString('OpenTilesetInitialDir', 'C:\');
      OpenTilesetInitialDir := 'C:\';
      Registry.WriteString('OpenMapInitialDir', 'C:\');
      OpenMapInitialDir := 'C:\';
      end;
end;

end.
