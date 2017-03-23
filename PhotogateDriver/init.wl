(* ::Package:: *)

 


BeginPackage["PhotogateDriver`"];
(*Begin["Private`"];*)


classPath=FileNameJoin[{$UserBaseDirectory,"Applications","PhotogateDriver","Java","dist","PhotogateDriver.jar"},OperatingSystem->"Unix"];
Needs["JLink`"];
InstallJava[ClassPath->classPath];
serialLink = JavaNew[LoadJavaClass["photogatedriver.PhotogateDriver"]];

initalise:=(
Needs["JLink`"];
InstallJava[ClassPath->classPath];
serialLink = JavaNew[LoadJavaClass["photogatedriver.PhotogateDriver"]];
findPorts;
If[Length[ports]>0,portString=ports[[1]],portString=""];
isConnected=False;
);

findPorts:=ports = serialLink@listPorts[];

listPorts:=serialLink@listPorts[];

clearLog:=serialLink@clearLog[];

eventMode :=serialLink@modeEventTime[];

gapMode:=serialLink@modeGapTime[];

returnLog:=ToExpression/@StringSplit[#,"_"]&/@StringSplit[serialLink@printLog[],"~"];

endSession:=(
serialLink@disconnectArduino[];
UninstallJava[];
);

connectArduino[portString_]:=connectionSuccess=serialLink@connectArduino[portString];

newSession[portString_]:=(
serialLink@disconnectArduino[];
connectArduino[portString];
If[connectionSuccess,
ShowJavaConsole["stdout"];
isConnected=True;
]
);

portSelectionMenu:=PopupMenu[Dynamic[portString], ports,Enabled->Dynamic[!isConnected],ImageSize->{90,30}];

portRefreshButton:=Button["Refresh",findPorts,Enabled->Dynamic[!isConnected],ImageSize->{90,30}];

connectButton:=Button["Connect",findPorts;If[Length[ports]>0,newSession[portString]],Method->"Queued",Enabled->Dynamic[(Length[ports]>0&&!isConnected)],ImageSize->{90,30}];

disconnectButton:=Button["Disconnect",endSession;initalise,Method->"Queued",Enabled->Dynamic[isConnected],ImageSize->{90,30}];

printLogButton:=Button["Return Data",Print[returnLog],Method->"Queued",Enabled->Dynamic[isConnected],ImageSize->{90,30}];

modeSelectionMenu:=ActionMenu["Mode",{"Gap Time":>gapMode,"Event Time":> eventMode},ImageSize->{90,30},Enabled->Dynamic[isConnected]];

toolbar:=Row[{
Dynamic@portSelectionMenu,
portRefreshButton,
Spacer[5],
connectButton,
Spacer[5],
disconnectButton,
Spacer[50],
printLogButton,
modeSelectionMenu
}];

endSession;
initalise;
CreateWindow[DockedCells->Cell[BoxData[ToBoxes[Dynamic[toolbar]]],"DockedCell",Background->RGBColor[1, 0.991791, 0.866468]],NotebookEventActions->{"WindowClose":>UninstallJava[]}];


(*End[];*)

EndPackage[];
