#-------------------------------------------#
# TCPEchoClient と                          #
# TCPEchoServer を作成する                  #
#                    2016.12.30             #
#                    by Seiichi Nukayama    #
#-------------------------------------------#
CC=gcc
CFLAGS=-g -Wall
OBJ1=TCPEchoClient.o DieWithError.o
OBJ2=TCPEchoServer.o HandleTCPClient.o DieWithError.o

all:	TCPEchoClient TCPEchoServer

TCPEchoClient: $(OBJ1)
	$(CC) $(CFLAGS) -o TCPEchoClient $(OBJ1)

TCPEchoClient.o: TCPEchoClient.c
#	$(CC) $(CFLAGS) -c TCPEchoClient.c

TCPEchoServer: $(OBJ2)
	$(CC) $(CFLAGS) -o TCPEchoServer $(OBJ2)

TCPEchoServer.o: TCPEchoServer.c

HandleTCPClient.o: HandleTCPClient.c

DieWithError.o: DieWithError.c
#	$(CC) $(CFLAGS) -c DieWithError.c

clean:
	rm -f TCPEchoClient $(OBJ1) TCPEchoServer $(OBJ2)

cleanobj:
	rm -f $(OBJ1) $(OBJ2)

#------------------------------------------#
# 疑問点                                   #
#     各オブジェクトファイルを作るのに     #
#     #(CC) 以下の記述はいらなかったけど   #
#     どうなんだろ？                       #
#     記述があったほうが早いみたいだけど。 #
#------------------------------------------#
