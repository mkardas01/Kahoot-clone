cmake_minimum_required(VERSION 3.10)
include_directories(include)

project(serwer CXX)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 20)

# Dodaj flagę -Wall
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")


# Dodaj pliki źródłowe
set(SOURCES
     serwer.cpp
     src/send_data_rank.cpp
     src/game_create.cpp
     src/question_to_json.cpp
     src/send_questions_or_endofgame.cpp
     src/start_game_send_player_to_lobby.cpp
     src/check_if_user_in_game.cpp
     src/check_nickname_availability.cpp
     src/player_join_rejoin_game.cpp
     src/owner_rejon.cpp
     src/owner_start_lobby.cpp
     src/send_players_answer_to_owner.cpp
     src/menage_points.cpp
     src/handle_connection.cpp
     src/handle_data.cpp
     src/check_if_next_question.cpp
     src/message_queue.cpp
     )


# Dodaj wykonywalny plik
add_executable(serwer ${SOURCES})

# Dodaj ścieżki do katalogów include
target_include_directories(serwer PUBLIC include)
