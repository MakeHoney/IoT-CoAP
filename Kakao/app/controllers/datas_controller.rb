class DatasController < ApplicationController
	def keyboard
		@msg = {
			type: "buttons",
			buttons: ["요청 로그 확인"]
		}
		render json: @msg, status: :ok
	end

	def chat
		if File.exist?('../../../log.txt')
			fd = File.new('../../../log.txt', 'r');
			fs = fd.stat.size
			content = fd.sysread(fs)
		else
			content = '작성된 로그가 없습니다.'
		end

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
