%%
%% Copyright (C) 2014 
%% Authors: Patricia Nuñez <cpattynue@gmail.com<>
%%          Jorge Garrido <zgbjgg@gmail.com>
%% All rights reserved.
%%
%% This Source Code Form is subject to the terms of the Mozilla Public
%% License, v. 2.0. If a copy of the MPL was not distributed with this
%% file, You can obtain one at http://mozilla.org/MPL/2.0/.
%%
%%

-module(gnuart).

-behaviour(gen_server).

%% API
-export([start_link/0, flush/1, subscribe/0, unsubscribe/1]).

%% gen_server callbacks
-export([init/1, handle_call/3, handle_cast/2, handle_info/2,
	 terminate/2, code_change/3]).

-define(SERVER, ?MODULE). 

-record(state, {driver = [], subscription = []}).

-include("gnuart.hrl").

%%%===================================================================
%%% API
%%%===================================================================

%%--------------------------------------------------------------------
%% @doc Flush bytes to port driver, write and read data.
%%
%% @spec flush(Bytes :: string()) -> {ok, string()}
%%--------------------------------------------------------------------
-spec flush(Bytes :: string()) -> {ok, string()}.
flush(Bytes) ->
    gen_server:call(?MODULE, {flush, Bytes}).
    

%%--------------------------------------------------------------------
%% @doc Subscribe pid to send data
%%
%% @spec subscribe() -> {ok, reference()}
%%--------------------------------------------------------------------
-spec subscribe() -> {ok, reference()}.
subscribe() ->
    gen_server:call(?MODULE, subscribe).


%%--------------------------------------------------------------------
%% @doc Unsubscribe pid via reference to avoid send data
%%
%% @spec unsubscribe( Ref :: reference()) -> {ok, unsubscribe}
%%--------------------------------------------------------------------
-spec unsubscribe(Ref :: reference()) -> {ok, unsubscribe}.
unsubscribe(Ref) ->
    gen_server:call(?MODULE, {unsubscribe, Ref}).

%%--------------------------------------------------------------------
%% @doc
%% Starts the server
%%
%% @spec start_link() -> {ok, Pid} | ignore | {error, Error}
%% @end
%%--------------------------------------------------------------------
start_link() ->
    gen_server:start_link({local, ?SERVER}, ?MODULE, [], []).

%%%===================================================================
%%% gen_server callbacks
%%%===================================================================

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Initiates the server
%%
%% @spec init(Args) -> {ok, State} |
%%                     {ok, State, Timeout} |
%%                     ignore |
%%                     {stop, Reason}
%% @end
%%--------------------------------------------------------------------
init([]) ->
    process_flag(trap_exit, true),
    Ebin = filename:dirname(code:which(?MODULE)),
    DriverDir = filename:join(filename:dirname(Ebin), "priv"),
    ok = case erl_ddll:load_driver(DriverDir, ?DRIVER) of
	     ok -> 
		 io:format("[gnuart] Succesfully loaded driver \n"),
	         ok;
	     {error, already_loaded} -> 
		 io:format("[gnuart] Already loaded driver \n"),
		 ok;
	     {error, Error}          -> 
		 io:format("[gnuart] Error loading driver ~p", [erl_ddll:format_error(Error)]),
	         exit(erl_ddll:format_error(Error))
         end,
    Port = open_port({spawn, ?DRIVER}, []),
    {ok, #state{driver=Port, subscription = []}}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling call messages
%%
%% @spec handle_call(Request, From, State) ->
%%                                   {reply, Reply, State} |
%%                                   {reply, Reply, State, Timeout} |
%%                                   {noreply, State} |
%%                                   {noreply, State, Timeout} |
%%                                   {stop, Reason, Reply, State} |
%%                                   {stop, Reason, State}
%% @end
%%--------------------------------------------------------------------
handle_call(subscribe, {Pid, _}, #state{driver=Port, subscription=S})         ->
    Reference = erlang:make_ref(),
    {reply, {ok, Reference}, #state{driver=Port, subscription=[ {Reference, Pid} | S]}};
handle_call({unsubscribe, Ref}, _From, #state{driver=Port, subscription=S})     ->
    NewS = [ Scr || {Reference, _}=Scr <- S, Ref =/= Reference],
    {reply, {ok, unsubscribe},  #state{driver=Port, subscription=NewS}};    
handle_call({flush, Bytes}, _From, State=#state{driver=Port, subscription=_S}) ->
    Port ! {self(), {command, Bytes}}, 
    {reply, {ok, flush}, State}; 
handle_call(stop, _From, State=#state{driver=Port}) ->
    Port ! {self(), close},
    {stop, normal, ok, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling cast messages
%%
%% @spec handle_cast(Msg, State) -> {noreply, State} |
%%                                  {noreply, State, Timeout} |
%%                                  {stop, Reason, State}
%% @end
%%--------------------------------------------------------------------
handle_cast(_Msg, State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Handling all non call/cast messages
%%
%% @spec handle_info(Info, State) -> {noreply, State} |
%%                                   {noreply, State, Timeout} |
%%                                   {stop, Reason, State}
%% @end
%%--------------------------------------------------------------------
handle_info({'EXIT', _Port, _Reason}, _State) 				      ->
    exit(port_terminated),
    {noreply, #state{driver=nil, subscription=[]}};
handle_info({Port, {data, Bytes}}, State=#state{driver=Port, subscription=S}) ->
    [ Pid ! {got, Ref, list_to_binary(Bytes)} || {Ref, Pid}<- S ],
    {noreply, State};
handle_info({_Port, closed}, _State) 					      ->
    {noreply, #state{driver=nil, subscription=[]}}.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% This function is called by a gen_server when it is about to
%% terminate. It should be the opposite of Module:init/1 and do any
%% necessary cleaning up. When it returns, the gen_server terminates
%% with Reason. The return value is ignored.
%%
%% @spec terminate(Reason, State) -> void()
%% @end
%%--------------------------------------------------------------------
terminate(_Reason, _State) ->
    ok.

%%--------------------------------------------------------------------
%% @private
%% @doc
%% Convert process state when code is changed
%%
%% @spec code_change(OldVsn, State, Extra) -> {ok, NewState}
%% @end
%%--------------------------------------------------------------------
code_change(_OldVsn, State, _Extra) ->
    {ok, State}.
