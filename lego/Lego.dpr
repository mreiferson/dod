program Lego;

uses
  Forms,
  Main in 'Main.pas' {MainForm},
  TileSet in 'TileSet.pas' {TileSetForm},
  MiniMap in 'MiniMap.pas' {MiniMapForm};

{$R *.RES}

begin
  Application.Title := 'LEGO';
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TTileSetForm, TileSetForm);
  Application.CreateForm(TMiniMapForm, MiniMapForm);
  MainForm.Init();
  Application.Run;
end.
