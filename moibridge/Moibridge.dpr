program Moibridge;

uses
  Forms,
  Main in 'Main.pas' {frmMain},
  SpriteView in 'SpriteView.pas' {frmSpriteView};

{$R *.RES}

begin
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmSpriteView, frmSpriteView);
  Application.Run;
end.
