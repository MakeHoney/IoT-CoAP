class Log < ApplicationRecord
	establish_connection(:remote_db)
end