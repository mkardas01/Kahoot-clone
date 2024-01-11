socket.addEventListener('message', function (event) {
    console.log(event.data);

    const jsonMessages = event.data.split('\n');

    for (const json of jsonMessages) {

        // Sprawdź, czy json nie jest pusty
        if (json.trim() === '') {
            continue;
        }

        data = JSON.parse(json);

        console.log(data);

        switch (data['type']) {
            case 'createGame':
                handleCreateGame(data);
                break;

            case 'startWatingForPlayer':
                handleStartWaitingForPlayer(data);
                break;

            case 'reJoinOwner':
                handleReJoinOwner(data)
                break;

            case 'joinGame':
                handleJoinGame(data);
                break;

            case 'playerJoin':
                handlePlayerJoin(data);
                break;

            case 'startGame':
                handleStartGame();
                break;

            case 'question':
                handleQuestion(data);
                break;

            case 'questionOwner':
                handleQuestionOwner(data);
                break;

            case 'userAnswer':
                handleUserAnswer(data);
                break;

            case 'pointsSummary':
                handlePointsSummary(data);
                break;

            case 'endOfGame':
                handleEndOfGame(data);
                break;

            case 'serverStatus':
                handleServerStatus(data)
                break;

            default:
                // Handle default case if needed
                break;
        }
    }

});

function handleCreateGame(data) {
    if (data['type'] == 'createGame' && data['status'] == 'success') {

        goToNextTab()
        gameIDCreated.innerText = data['gameID'];
        gamePinCreated.innerText = data['gamePin'];
        sendStatus('Gra stworzona :)', true); //send success notification 

    }
}

function handleStartWaitingForPlayer(data) {
    switch (data['status']) {
        case 'success':
            goToNextTab();
            sendStatus('Uruchomiono oczekiwanie graczy', true);
            break;

        case 'waiting':
            goToNextTab(false);
            sendStatus('Gra jest już uruchomiona', false);
            break;

        case 'userAlreadyInGame':
            goToNextTab(false);
            sendStatus('Uczesniczysz już w innej grze', false);
            break;

        default:
            goToNextTab(false);
            sendStatus('Gra nie istnieje', false);
            break;
    }



}

function handleReJoinOwner(data) {
    switch (data['status']) {

        case 'waitingForPlayers':
            goToNextTab()
            sendStatus('Wrócono do oczekiwania na graczy', true); //send success notification  
            break;

        case 'started':
            goToNextTab(false);
            sendStatus('Wrócono do gry', true);
            startGameTab2.click();
            break;

        default:
            goToNextTab(false);
            sendStatus('Wystąpił błąd podczas ponownego dołaczania', false);
            break;
    }
}

function handleJoinGame(data) {

    switch (data['status']) {
        case 'success':
            document.querySelector('#loaderInfo').innerText = 'Oczekiwanie na rozpoczęcie gry przez gospodarza';
            goToNextTab();
            sendStatus('Dołączono do gry', true);
            break;

        case 'started':
            goToNextTab(false);
            sendStatus('Nie można dołączyć do gry, która jest w toku', false);
            break;

        case 'nickNameNotAvailable':
            goToNextTab(false);
            sendStatus('Nickname niedostępny', false);
            break;

        case 'reJoin':
            document.querySelector('#loaderInfo').innerText = 'Dołączysz do gry przy następnym pytaniu';
            goToNextTab();
            sendStatus('Dołączono ponownie', true);
            break;

        case 'userAlreadyInGame':
            goToNextTab(false);
            sendStatus('Dołączyłeś już do istniejącej gry', false);
            break;

        default:
            goToNextTab(false);
            sendStatus('Gra nie istnieje lub nie została uruchomiona', false);
            break;
    }

}

function handlePlayerJoin(data) {
    playersList.innerHTML += data['player'] + "<br>";
}

function handleStartGame() {
    goToNextTab();
    sendStatus('Gra wystartowała :)', true);
    playersList.innerText = '';

}

function handleQuestion(data) {
    document.querySelector('#loaderInfo').innerText = 'Oczekiwanie na następne pytanie'; // send info for waiting for next question
    putQuestion(data);
    goToTimerFunction.click();
}

function handleQuestionOwner(data) {
    putQuestion(data, 'owner');
}

function handleUserAnswer(data) {
    putUserAnswer(data);
}

function handlePointsSummary(data) {
    putRank(data);
}

function handleEndOfGame(data) {
    goToRank.click(); // turn on goToRankFunction
}

function handleServerStatus(data){
    
    switch(data['serverStatus']){
        case true:
            sendStatus('Połączono z serwerem', true);
            break;
        
        case false:
            sendStatus('Rozłączono z serwrem', false);
            isEndOfGame = true;
            backButton.click();
            isEndOfGame = false;
            break;
    }

}

