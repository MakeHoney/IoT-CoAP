class Info < ApplicationRecord
	establish_connection(:remote_db)
end