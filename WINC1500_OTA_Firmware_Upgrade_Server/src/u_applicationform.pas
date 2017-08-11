unit u_applicationform;

{$mode objfpc}{$H+}

interface

uses
  Forms, StdCtrls, Buttons, ExtCtrls, Dialogs
  , SysUtils
  , fphttpserver, fpwebfile
  , u_http_server_thread
  , Classes;

const
  PORT = 80;

type

  { TApplicationForm }

  TApplicationForm = class(TForm)
    BitBtnStart    : TBitBtn;
    BitBtnStop     : TBitBtn;
    LabeledEditPort: TLabeledEdit;
    Memo           : TMemo;
    PanelButtons   : TPanel;
    StaticText     : TStaticText;
    //
    procedure BitBtnStartClick( {%H-}Sender: TObject );
    procedure BitBtnStopClick ( {%H-}Sender: TObject );
  private
    function GetPort: UInt16;
  private
    FHandler     : TFPCustomFileModule;
    FServerThread: THTTPServerThread;
    FURL         : String;
    //
    procedure HttpServerOnRequest(
          {%H-}Sender   : TObject;
      var      ARequest : TFPHTTPConnectionRequest;
      var {%H-}AResponse: TFPHTTPConnectionResponse );
    //
    procedure ShowURL;
    //
    property Port: UInt16 read GetPort;
  public
    procedure AfterConstruction; override;
  end;

resourcestring
  PortOutOfRange = 'Portnumber is out of range (1...65535)';

implementation

{$R *.lfm}

{ TApplicationForm }

procedure TApplicationForm.AfterConstruction;
begin
  inherited;
  BitBtnStop.Enabled := False;
  FHandler := TFPCustomFileModule.CreateNew(Self);
//  FHandler.BaseURL:=’/’;
end;

procedure TApplicationForm.BitBtnStartClick(
  Sender: TObject );
begin
  if( Port < $01 ) then
    Exit;
  //
  BitBtnStart.Enabled := False;
  Memo.Lines.Add('Starting server');
  FServerThread := THTTPServerThread.Create( Port, @HttpServerOnRequest );
  BitBtnStop.Enabled := True;
end;

procedure TApplicationForm.BitBtnStopClick(
  Sender: TObject );
begin
  BitBtnStop.Enabled := False;
  Memo.Lines.Add('Stopping server');
  try
    if( Assigned( FServerThread ) ) then begin
      try
        FServerThread.Server.Active := False;
        FServerThread.Terminate;
        FServerThread.WaitFor;
      except
        on e:Exception do;
      end;
      FreeAndNil( FServerThread );
    end;
  finally
    BitBtnStart.Enabled := True;
  end;
end;

function TApplicationForm.GetPort: UInt16;
var
  Input: LongInt;
begin
  Result := $00;
  try
    Input := StrToInt( LabeledEditPort.Text );
    if( (Input < $01) or (Input > $FFFF) ) then
      raise EConvertError.Create( PortOutOfRange )
    else
      Result := Input;
  except
    on E: EConvertError do
      MessageDlg('Portnumber error', PortOutOfRange, mtError, [mbOK], 0);
  end;
end;

procedure TApplicationForm.HttpServerOnRequest(
      Sender   : TObject;
  var ARequest : TFPHTTPConnectionRequest;
  var AResponse: TFPHTTPConnectionResponse );
begin
  FURL := ARequest.URL;
  FServerThread.Synchronize( FServerThread, @ShowURL );
  FHandler.HandleRequest( ARequest, AResponse );
end;

procedure TApplicationForm.ShowURL;
begin
  Memo.Lines.Add( 'Handling request : '+FURL );
end;

end.

