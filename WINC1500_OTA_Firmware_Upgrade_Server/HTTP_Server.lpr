program HTTP_Server;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, u_applicationform, u_http_server_thread;

{$R *.res}

var
  ApplicationForm: TApplicationForm;

begin
  RequireDerivedFormResource := True;
  Application.Initialize;
  Application.CreateForm(TApplicationForm, ApplicationForm);
  Application.Run;
end.

