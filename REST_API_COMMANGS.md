## GW40 BREMSINTERFACE REST API


* eine visualisierung aller state gibt es unter http://__HOST__:3345/


* http://__HOST__:3345/rest/ventile listet alle ventile inkl states und IDs auf
* http://__HOST__:3345/rest/__ID__/get_state gibt den aktuellen state zurück dabei 0 -> geschlossen 1-> geöffnet *
* http://__HOST__:3345/rest/__ID__/get_info gibt alle informationen zum ventil an


* http://127.0.0.1:3000/rest/__ID__/set_state/__STATE__ setzt den status des ventils mit der ID dabei muss der state valide sein und in der valid_states vorkommen (in der regel 0 oder 1)


* *beim uic ventil ist der state 0 = P, 1 = ??, 2 =???

* http://__HOST__:3345/restet kompletter reset aller ventile -> ausgangszustand nach dem start 