class DatasController < ApplicationController
	def keyboard
		@msg = {
			type: "buttons",
			buttons: ["요청 로그 확인", "병헌", "정연", "태성"]
		}
		render json: @msg, status: :ok
	end

	def chat
		@res = params[:content]

		if @res.eql?("요청 로그 확인") 
			if File.exist?('/home/ec2-user/iot_coap/log.txt')
				fd = File.new('/home/ec2-user/iot_coap/log.txt', 'r');
				fs = fd.stat.size
				content = fd.sysread(fs)
			else
				content = '작성된 로그가 없습니다.'
			end

			content.chomp!

			# @res = params[:content]
			# @user_key = params[:user_key]
			@msg = {
				message: {
					text: content
				},
				keyboard: {
					type: "buttons",
					buttons: ["요청 로그 확인", "병헌", "정연", "태성"]
				}
			}
			render json: @msg, status: :ok

		elsif @res.eql?("병헌")
			if File.exist?('/home/ec2-user/iot_coap/byunghun.txt')
				fd = File.new('/home/ec2-user/iot_coap/byunghun.txt', 'r');
				fs = fd.stat.size
				content = fd.sysread(fs)
			else
				content = '0'
			end

			content.chomp!

			# @res = params[:content]
			# @user_key = params[:user_key]
			@msg = {
				message: {
					text: content
				},
				keyboard: {
					type: "buttons",
					buttons: ["요청 로그 확인", "병헌", "정연", "태성"]
				}
			}
			render json: @msg, status: :ok

		elsif @res.eql?("정연")
			if File.exist?('/home/ec2-user/iot_coap/jungyeon.txt')
				fd = File.new('/home/ec2-user/iot_coap/jungyeon.txt', 'r');
				fs = fd.stat.size
				content = fd.sysread(fs)
			else
				content = '0'
			end

			content.chomp!

			# @res = params[:content]
			# @user_key = params[:user_key]
			@msg = {
				message: {
					text: content
				},
				keyboard: {
					type: "buttons",
					buttons: ["요청 로그 확인", "병헌", "정연", "태성"]
				}
			}
			render json: @msg, status: :ok


		elsif @res.eql?("태성")
			if File.exist?('/home/ec2-user/iot_coap/taesung.txt')
				fd = File.new('/home/ec2-user/iot_coap/taesung.txt', 'r');
				fs = fd.stat.size
				content = fd.sysread(fs)
			else
				content = '0'
			end

			content.chomp!

			# @res = params[:content]
			# @user_key = params[:user_key]
			@msg = {
				message: {
					text: content
				},
				keyboard: {
					type: "buttons",
					buttons: ["요청 로그 확인"]
				}
			}
			render json: @msg, status: :ok
		end
	end
end
