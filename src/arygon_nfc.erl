-module(arygon_nfc).
-export([sendTama/1]).

sendTama(CommandTama) ->
        responseReceived(<<"FF00001C4B010100440007048A9A6AD02980\n\n">>).

responseReceived(<<Status:6/binary,Length:2/binary, Packet:2/binary, TargetsInit:2/binary,TargetNum:2/binary, Sens:4/binary, Sel:2/binary, LengthID:2/binary, ID/binary>>) ->
	io:format("Status OK ~p~n Length ~p~n Packet ~p~n TargetsInit ~p~n TargetNum ~p~n Sens ~p~n Sel ~p~n Length ID ~p~n ID ~p~n ~n",
[Status,Length,Packet,TargetsInit,TargetNum,Sens,Sel,LengthID,ID]).
