unit u_http_server_thread;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils
  , fphttpserver, fpwebfile
  ;

type

  { THTTPServerThread }

  THTTPServerThread = Class( TThread )
  private
    FServer: TFPHTTPServer;
  public
    constructor Create(
            Port     : Word;
      const OnRequest: THTTPServerRequestHandler );
    //
    procedure BeforeDestruction; override;
    procedure Execute          ; override;
    //
    property Server: TFPHTTPServer Read FServer;
end;

implementation

constructor THTTPServerThread.Create(
        Port     : Word;
  const OnRequest: THTTPServerRequestHandler);
begin
  FServer := TFPHTTPServer.Create( Nil );
  FServer.Port      := Port     ;
  FServer.OnRequest := OnRequest;
  MimeTypesFile     := './mime.types';
  //
  Inherited Create( {* CreateSuspended := *} False );
end;

procedure THTTPServerThread.BeforeDestruction;
begin
  FServer.Active := False;
  FreeAndNil( FServer );
  //
  inherited BeforeDestruction;
end;

procedure THTTPServerThread.Execute;
begin
  FServer.Active := True;
end;

end.

