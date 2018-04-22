# TCP Client

require 'socket'
require 'coap'

socket = TCPSocket.open("localhost", 80) # virtual hosting하여 port변경하기

puts "클라이언트를 시작합니다."
while message = socket.gets # 소켓으로부터 라인을 읽어들인다.
socket
	puts message.chomp
end

puts "클라이언트를 종료합니다."
socket.close				# 소켓을 닫는다.
