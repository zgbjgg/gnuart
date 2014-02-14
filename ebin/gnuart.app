{application, gnuart,
 [
  {description, "UART erlang"},
  {vsn, "0.2"},
  {registered, []},
  {applications, [
                  kernel,
                  stdlib
                 ]},
  {mod, { gnuart_app, []}},
  {modules,[gnuart, gnuart_app, gnuart_sup]}
 ]}.
