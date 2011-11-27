unit MiniMap;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

type
  TMiniMapForm = class(TForm)
    MiniMapBox: TPaintBox;
    procedure MiniMapBoxPaint(Sender: TObject);
    procedure MiniMapFormCreate(Sender: TObject);
    procedure MiniMapBoxMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
    MM: TBitmap;
    procedure ClearMM();
  end;

var
  MiniMapForm: TMiniMapForm;

implementation

uses Main;

{$R *.DFM}

procedure TMiniMapForm.MiniMapBoxPaint(Sender: TObject);
begin
   MiniMapBox.Canvas.Draw(0, 0, MM);
   MiniMapBox.Canvas.Brush.Color := $0000FFFF;
   MiniMapBox.Canvas.Brush.Style := bsSolid;
   MiniMapBox.Canvas.FrameRect(Rect(MainForm.WorldOffsetX, MainForm.WorldOffsetY, MainForm.WorldOffsetX+MainForm.ViewableX-1, MainForm.WorldOffsetY+MainForm.ViewableY-1));
end;

procedure TMiniMapForm.MiniMapFormCreate(Sender: TObject);
begin
   MM := TBitmap.Create;
   MM.Width := 128;
   MM.Height := 128;
   Show;
end;

procedure TMiniMapForm.ClearMM();
begin
   MM.Canvas.Brush.Color := $00000000;
   MM.Canvas.Brush.Style := bsSolid;
   MM.Canvas.FillRect(Rect(0,0,127,127));
   MiniMapBox.Repaint;
end;

procedure TMiniMapForm.MiniMapBoxMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
   var leftc: Integer;
   var topc: Integer;
begin
   leftc := X - (MainForm.ViewableX div 2);
   topc := Y - (MainForm.ViewableY div 2);
   if (leftc + MainForm.ViewableX) > 128 then
      leftc := leftc - (leftc + MainForm.ViewableX - 128);
   if (topc + MainForm.ViewableY) > 128 then
      topc := topc - (topc + MainForm.ViewableY - 128);
   if leftc < 0 then
      leftc := 0;
   if topc < 0 then
      topc := 0;
   MainForm.WorldOffsetX := leftc;
   MainForm.WorldOffsetY := topc;
   MiniMapBox.Repaint;
   MainForm.MapBox.Repaint;
end;

end.
