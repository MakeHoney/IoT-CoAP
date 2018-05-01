class DatasController < ApplicationController
	def keyboard
		@msg = {
			type: "buttons",
			buttons: ["요청 로그 확인"]
		}
		render json: @msg, status: :ok
	end

	def chat
		fd = File.new('../../../log.txt', 'r');
		fs = fd.stat.size
		if fd
			content = fd.sysread(fs)
		else
			content = '로그가 없습니다.'
		end

		@res = params[:content]
		@user_key = params[:user_key]
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
