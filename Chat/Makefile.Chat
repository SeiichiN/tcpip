#-------------------------------------------#
# TCPChatClient と                       #
# TCPChatServer を作成する               #
#                    2017.03.02             #
#                    by Seiichi Nukayama    #
#-------------------------------------------#
CC=gcc
CFLAGS=-g -Wall
OBJ1=TCPChatClient.o DieWithError.o
OBJ2=TCPChatServer.o ChatTCP.o DieWithError.o

all:	TCPChatClient TCPChatServer

TCPChatClient: $(OBJ1)
	$(CC) $(CFLAGS) -o TCPChatClient $(OBJ1)

TCPChatClient.o: TCPChatClient.c
#	$(CC) $(CFLAGS) -c TCPChatClient.c

TCPChatServer: $(OBJ2)
	$(CC) $(CFLAGS) -o TCPChatServer $(OBJ2)

TCPChatServer.o: TCPChatServer.c

ChatTCP.o: ChatTCP.c

DieWithError.o: DieWithError.c
#	$(CC) $(CFLAGS) -c DieWithError.c

clean:
	rm -f TCPChatClient $(OBJ1) TCPChatServer $(OBJ2)

cleanobj:
	rm -f $(OBJ1) $(OBJ2)

#------------------------------------------#
# 疑問点                                   #
#     各オブジェクトファイルを作るのに     #
#     #(CC) 以下の記述はいらなかったけど   #
#     どうなんだろ？                       #
#     記述があったほうが早いみたいだけど。 #
#------------------------------------------#
