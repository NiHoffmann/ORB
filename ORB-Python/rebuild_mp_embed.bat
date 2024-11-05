cd /d "%~dp0"
cd libs
cd mp_embed
make -f micropython_embed.mk clean
make -f micropython_embed.mk