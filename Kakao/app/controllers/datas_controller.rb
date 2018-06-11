class DatasController < ApplicationController
	def keyboard
		@msg = {
			type: "buttons",
			buttons: ["출입정보 열람"]
		}
		render json: @msg, status: :ok
	end

	def chat
		@res = params[:content]

		if @res.eql?("출입정보 열람")
			sql = "SELECT * FROM logs ORDER BY id desc LIMIT 20";
			logs = Log.connection.exec_query(sql).rows;
			content = "";
			logs.each do |tuple|

				content += "이름: #{tuple[1]}\n연락처: #{tuple[2]}\n주소: #{tuple[4]}\n출입시간: #{tuple[3]}\n\n";
			end

			content.chomp!

			@msg = {
				message: {
					text: content
				},
				keyboard: {
					type: "buttons",
			buttons: ["출입정보 열람"]
				}
			}
			render json: @msg, status: :ok
		end
	end
end
