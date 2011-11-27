unit SpriteView;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

type
  TfrmSpriteView = class(TForm)
    SpriteBox: TPaintBox;
    procedure SpriteBoxPaint(Sender: TObject);
    procedure SpriteBoxDblClick(Sender: TObject);
    procedure frmSpriteViewClose(Sender: TObject;
      var Action: TCloseAction);
  private
     { Private declarations }
  public
     { Public declarations }
     Sprites: array of TBitmap;
     SpriteBitmap: TBitmap;
     NumSpritesX: Integer;
     NumSpritesY: Integer;
     SpriteWidth: Integer;
     SpriteHeight: Integer;
     procedure DoLoadSprites(fname: string);
  end;

var
  frmSpriteView: TfrmSpriteView;

implementation

uses Main;

{$R *.DFM}

procedure TfrmSpriteView.SpriteBoxPaint(Sender: TObject);
   var x: Integer;
   var y: Integer;
begin
   SpriteBox.Canvas.Draw(0,0,SpriteBitmap);
   SpriteBox.Canvas.Pen.Color := $00FFFFFF;
   SpriteBox.Canvas.Pen.Style := psDot;
   x := 0;
   y := 0;
   if frmMain.SpeedButton6.Down = True then
      begin
      repeat
         x := x + SpriteWidth;
         SpriteBox.Canvas.MoveTo(x, 0);
         SpriteBox.Canvas.LineTo(x, SpriteBox.Height);
      until x >= SpriteBox.Width;
      repeat
         y := y + SpriteHeight;
         SpriteBox.Canvas.MoveTo(0, y);
         SpriteBox.Canvas.LineTo(SpriteBox.Width, y);
      until y >= SpriteBox.Height;
      end;
end;

procedure TfrmSpriteView.DoLoadSprites(fname: string);
   var cx: Integer;
   var cy: Integer;
   var cur_sprite: Integer;
begin
   SpriteBitmap := TBitmap.Create;
   SpriteBitmap.LoadFromFile(fname);
   frmSpriteView.ClientWidth := SpriteBitmap.Width;
   frmSpriteView.ClientHeight := SpriteBitmap.Height;
   SpriteBox.Width := ClientWidth;
   SpriteBox.Height := ClientHeight;
   SpriteWidth := StrToInt(InputBox('Input Sprite Dimensions', 'Enter Sprite Width',''));
   SpriteHeight := StrToInt(InputBox('Input Sprite Dimensions', 'Enter Sprite Height',''));
   frmMain.Animation.Width := frmSpriteView.SpriteWidth;
   frmMain.Animation.Height := frmSpriteView.SpriteHeight;
   Show;
   NumSpritesX := SpriteBitmap.Width div SpriteWidth;
   NumSpritesY := SpriteBitmap.Height div SpriteHeight;
   SetLength(Sprites, (NumSpritesY * NumSpritesY));
   for cy := 0 to (NumSpritesY-1) do
      begin
      for cx := 0 to (NumSpritesX-1) do
         begin
            cur_sprite := (cy * NumSpritesY) + cx;
            Sprites[cur_sprite] := TBitmap.Create;
            Sprites[cur_sprite].Width := SpriteWidth;
            Sprites[cur_sprite].Height := SpriteHeight;
            Sprites[cur_sprite].Canvas.CopyMode := cmSrcCopy;
            Sprites[cur_sprite].Canvas.CopyRect(Rect(0,0,SpriteWidth,SpriteHeight),SpriteBitmap.Canvas,Rect(cx*SpriteWidth,cy*SpriteHeight,cx*SpriteWidth+SpriteWidth,cy*SpriteHeight+SpriteHeight));
         end;
      end;
end;

procedure TfrmSpriteView.SpriteBoxDblClick(Sender: TObject);
   var mx: Integer;
   var my: Integer;
   var wherex: Integer;
   var wherey: Integer;
   var border: Integer;
   var titlebar: Integer;
   var spritenum: Integer;
begin
   border := (frmSpriteView.Width - frmSpriteView.ClientWidth) div 2;
   titlebar := (frmSpriteView.Height - frmSpriteView.ClientHeight) - border;
   mx := Mouse.CursorPos.X - (frmSpriteView.Left + border);
   my := Mouse.CursorPos.Y - (frmSpriteView.Top + titlebar);
   wherex := mx div frmSpriteView.SpriteWidth;
   wherey := my div frmSpriteView.SpriteHeight;
   spritenum := (wherey * frmSpriteView.NumSpritesY) + wherex;
   frmMain.SpriteList.Items.Add(IntToStr(spritenum));
end;

procedure TfrmSpriteView.frmSpriteViewClose(Sender: TObject; var Action: TCloseAction);
   var tmp_x: Integer;
   var tmp_y: Integer;
begin
   If not SpriteBitmap.Empty then
      SpriteBitmap.Destroy;
   for tmp_y := 0 to (NumSpritesY-1) do
      begin
      for tmp_x := 0 to (NumSpritesX-1) do
         begin
         if not Sprites[(tmp_y * NumSpritesY) + tmp_x].Empty then
            Sprites[(tmp_y * NumSpritesY) + tmp_x].Destroy;
         end;
      end;
   frmMain.SpriteList.Clear;
end;

end.
