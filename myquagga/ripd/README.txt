rip_debug.* deleted

rip_snmp.c deleted 

rip_offset.c deleted

rip_interface.* deleted (maybe change back to get this file, file has cmds for interface operation)

rip_peer.c deleted

rip_routemap.c cannot be deleted -> makes problems in vty files and blocks
than compilation problems

delete anything else makes error in compilation
ripd.c has funktion for adding routes

after debug understand that 
rip compiles wrong have to ask edwin for this

new idea why add doesn´t work need maybe the callback funktion in zclient to work properly

still no idea about rip
reserch resault need callback funktion in i2rsd zclient
looked into ospfd deamon for hints importent part is ospf_exteral_info_add
funktion and file ospf_asbr.c changing the information of an route node

callback funktion research added to github currently not working

trying answrs from quagga dev team
