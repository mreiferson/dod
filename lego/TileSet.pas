unit TileSet;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, Menus;

type
  TTileSetForm = class(TForm)
    TileSetBox: TPaintBox;
    ColorDialog1: TColorDialog;
    TilePopupMenu: TPopupMenu;
    mnuMMColChange: TMenuItem;
    mnuChangeWalkability: TMenuItem;
    procedure TileSetBoxPaint(Sender: TObject);
    procedure TileSetFormShow(Sender: TObject);
    procedure TileSetBoxMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure TileSetFormClose(Sender: TObject; var Action: TCloseAction);
    procedure mnuChangeTileWalkabilityOnClick(Sender: TObject);
    procedure mnuMMColChangeOnClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    Tiles: array of TBitmap;
    NumTilesY: Integer;
    NumTilesX: Integer;
    TileSetBitmap: TBitmap;
    TileSelectedX: Integer;
    TileSelectedY: Integer;
    TileSelectedNum: Integer;
    TileSetRect: TRect;
    AspectRatioW: Real;
    AspectRatioH: Real;
    procedure DoLoadTileSet(fname: string);
  end;

var TileSetForm: TTileSetForm;

implementation

uses Main, MiniMap;

{$R *.DFM}

procedure TTileSetForm.TileSetBoxPaint(Sender: TObject);
   var x: Integer;
   var y: Integer;
   var sx: Integer;
   var sy: Integer;
begin
   TileSetBox.Canvas.StretchDraw(TileSetRect,TileSetBitmap);
   TileSetBox.Canvas.Pen.Color := $00FFFFFF;
   TileSetBox.Canvas.Pen.Style := psDot;
   x := 0;
   y := 0;
   if MainForm.SpeedButton10.Down = True then
      begin
      repeat
         x := Round(x + (32 * AspectRatioW));
         TileSetBox.Canvas.MoveTo(x, 0);
         TileSetBox.Canvas.LineTo(x, TileSetBox.Height);
      until x >= TileSetBox.Width;
      repeat
         y := Round(y + 32 * AspectRatioH);
         TileSetBox.Canvas.MoveTo(0, y);
         TileSetBox.Canvas.LineTo(TileSetBox.Width, y);
      until y >= TileSetBox.Height;
      end;
   sx := Round(TileSelectedX * (32*AspectRatioW));
   sy := Round(TileSelectedY * (32*AspectRatioH));
   TileSetBox.Canvas.Brush.Color := $0000FFFF;
   TileSetBox.Canvas.FrameRect(Rect(sx, sy, Round(sx+(32*AspectRatioW))+1, Round(sy+(32*AspectRatioH))+1));
end;

procedure TTileSetForm.TileSetFormShow(Sender: TObject);
begin
   NumTilesX := 0;
   NumTilesY := 0;
   TileSelectedX := 0;
   TileSelectedY := 0;
   TileSelectedNum := 0;
end;

procedure TTileSetForm.TileSetBoxMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
   if Button = mbLeft then
      begin
      TileSelectedX := X div Round(32*AspectRatioW);
      TileSelectedY := Y div Round(32*AspectRatioH);
      TileSelectedNum := (TileSelectedY * NumTilesY) + TileSelectedX;
      TileSetBox.Repaint;
      end;
   if Button = mbRight then
      begin
      TileSelectedX := X div Round(32*AspectRatioW);
      TileSelectedY := Y div Round(32*AspectRatioH);
      TileSelectedNum := (TileSelectedY * NumTilesY) + TileSelectedX;
      TileSetBox.Repaint;
      if MainForm.WalkArray[TileSelectedY,TileSelectedX] = 1 then
         mnuChangeWalkability.Caption := 'Change Tile Walkability (TRUE)'
      else
         mnuChangeWalkability.Caption := 'Change Tile Walkability (FALSE)';
      TilePopupMenu.Popup(TileSetForm.Left+X,TileSetForm.Top+Y);
      end;
end;

procedure TTileSetForm.DoLoadTileSet(fname: string);
   var cx: Integer;
   var cy: Integer;
   var cur_tile: Integer;
begin
   TileSetBitmap := TBitmap.Create;
   TileSetBitmap.LoadFromFile(fname);
   if TileSetBitmap.Width > 256 then
      begin
      ClientWidth := 256;
      AspectRatioW := 256 / TileSetBitmap.Width;
      end
   else
      begin
      ClientWidth := TileSetBitmap.Width;
      AspectRatioW := 1;
      end;
   if TileSetBitmap.Height > 256 then
      begin
      ClientHeight := 256;
      AspectRatioH := 256 / TileSetBitmap.Height;
      end
   else
      begin
      ClientHeight := TileSetBitmap.Height;
      AspectRatioH := 1;
      end;
   TileSetBox.Width := ClientWidth;
   TileSetBox.Height := ClientHeight;
   TileSetRect := Rect(0,0,ClientWidth,ClientHeight);
   Show;
   NumTilesX := TileSetBitmap.Width div 32;
   NumTilesY := TileSetBitmap.Height div 32;
   SetLength(Tiles, (NumTilesY * NumTilesX));
   for cy := 0 to (NumTilesY-1) do
      begin
      for cx := 0 to (NumTilesX-1) do
         begin
            cur_tile := (cy * NumTilesY) + cx;
            Tiles[cur_tile] := TBitmap.Create;
            Tiles[cur_tile].Width := 32;
            Tiles[cur_tile].Height := 32;
            Tiles[cur_tile].Canvas.CopyMode := cmSrcCopy;
            Tiles[cur_tile].Canvas.CopyRect(Rect(0,0,32,32),TileSetBitmap.Canvas,Rect(cx*32,cy*32,cx*32+32,cy*32+32));
         end;
      end;
   MainForm.CanDraw := TRUE;
   MainForm.LoadedTileSet := TRUE;
end;

procedure TTileSetForm.TileSetFormClose(Sender: TObject; var Action: TCloseAction);
  var tmp_x: Integer;
  var tmp_y: Integer;
begin
   MainForm.CanDraw := FALSE;
   If not TileSetBitmap.Empty then
      TileSetBitmap.Destroy;
   for tmp_y := 0 to (NumTilesY-1) do
      begin
      for tmp_x := 0 to (NumTilesX-1) do
         begin
         if not Tiles[(tmp_y * NumTilesY) + tmp_x].Empty then
            Tiles[(tmp_y * NumTilesY) + tmp_x].Destroy;
         end;
      end;
   NumTilesY := 0;
   NumTilesX := 0;
   TileSelectedX := 0;
   TileSelectedY := 0;
   TileSelectedNum := 0;
   MainForm.LoadedTileSet := FALSE;
end;

procedure TTileSetForm.mnuChangeTileWalkabilityOnClick(Sender: TObject);
begin
   if NOT (MainForm.WalkArray[TileSelectedY,TileSelectedX] = 1) then
      MainForm.WalkArray[TileSelectedY,TileSelectedX] := 1
   else
      MainForm.WalkArray[TileSelectedY,TileSelectedX] := 0;
end;

procedure TTileSetForm.mnuMMColChangeOnClick(Sender: TObject);
   var ny: Integer;
   var nx: Integer;
begin
   ColorDialog1.Color := MainForm.MMColArray[TileSelectedY,TileSelectedX];
   if ColorDialog1.Execute then
      begin
      MainForm.MMColArray[TileSelectedY,TileSelectedX] := ColorDialog1.Color;
      for ny := 0 to 127 do
         for nx := 0 to 127 do
            begin
            MiniMapForm.MM.Canvas.Pixels[nx,ny] := MainForm.MMColArray[MainForm.MapArray[ny,nx] div NumTilesY,MainForm.MapArray[ny,nx] - ((MainForm.MapArray[ny,nx] div NumTilesY) * NumTilesY)];
            end;
      MiniMapForm.MiniMapBox.Repaint;
      end;
end;

end.
