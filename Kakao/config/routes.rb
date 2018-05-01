Rails.application.routes.draw do
  	get '/keyboard' => 'datas#keyboard'
	post '/message' => 'datas#chat'
end
