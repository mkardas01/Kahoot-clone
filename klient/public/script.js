function putQuestion(data, type = 'game') {

    document.getElementById(type + 'Question').innerText = data.questionList.question;

    // Aktualizuj odpowiedzi
    for (i = 0; i < 4; i++) {
        document.getElementById(type + 'Answer' + i).innerText = data.questionList.answers[i];
    }

    if (type === 'owner') {
        for (i = 0; i < 4; i++) {
            document.getElementById('ownerUserAnswer' + i).innerText = "";
        }
    }

}

function putUserAnswer(data) {
    let answer = document.getElementById('ownerUserAnswer' + data.answerIndex);
    answer.innerHTML += ' ' + data.nickname + '<br>';

}

function putRank(data) {
    playersRankElement.innerHTML = "";

    for (var i = 0; i < data.users.length; i++) {
        playersRankElement.innerHTML += "<p class='mt-2 text-center'>" + (i + 1) +
            ". " + data.users[i].nickname + "<br>" +
            " punkty: " + data.users[i].points + "</p>";
    }
}

function goToNextTab(nextTab = null) { // if null go to next tab if false stay in current
    clearTimeout(respondTimeOut);
    respondTimeOut = nextTab;
}

function sendStatus(message, success) { //show notifcation
    const statusbar = document.querySelector('#statusbar');
    const status = document.querySelector('#status');

    statusbar.classList.remove('hidden');

    statusbar.style.transition = 'transform 0.3s ease-out, opacity 0.3s ease-out';
    statusbar.style.transform = 'translateY(-100%)';
    statusbar.style.opacity = '0';

    statusbar.offsetHeight;

    statusbar.style.transform = 'translateY(0)';
    statusbar.style.opacity = '1';

    if (success) {
        statusbar.classList.add('bg-green-500');
    } else {
        statusbar.classList.add('bg-red-500');
    }

    status.innerHTML = message;

    setTimeout(() => {

        statusbar.style.transition = 'transform 0.3s ease-out, opacity 0.3s ease-out';
        statusbar.style.transform = 'translateY(-100%)';
        statusbar.style.opacity = '0';


        setTimeout(() => {
            statusbar.classList.remove('bg-green-500', 'bg-red-500');
            statusbar.classList.add('hidden');
            statusbar.style.transition = '';
        }, 300);
    }, 4000);
}

function sendMessage(message, waitForRespond = true) {
    // Wysyłaj wiadomość do serwera Node.js przez SSE
    message = message + '\n'

    socket.send(message);

    if (waitForRespond) {
        respondTimeOut = setTimeout(() => {
            sendStatus('Coś poszło nie tak - brak odpowiedzi od serwera', false);
        }, waitTime);
    }
}

function getQuestionsToJson(questionNumber) {
    const questionsObject = {
        'type': 'createGame',
        'questions': [],  // Inicjalizacja pustej tablicy dla pytań,
        'questionsNumber': 0
    };

    for (let i = 0; i < questionNumber; i++) { // get all questions
        const question = document.querySelector('#Question' + i);
        const answerA = document.querySelector('#AnswerA' + i);
        const answerB = document.querySelector('#AnswerB' + i);
        const answerC = document.querySelector('#AnswerC' + i);
        const answerD = document.querySelector('#AnswerD' + i);
        const correctAnswer = document.querySelector('#CorrectAnswer' + i);

        if (!dataValid('Wypełnij wszystkie pola poprawnie w pytaniu nr ' + (i + 1), [question, answerA, answerB, answerC, answerD, correctAnswer])) {
            return;
        }
        console.log(i);
        const questionObject = {
            question: question.value,
            answers: [answerA.value, answerB.value, answerC.value, answerD.value],
            indexOfCorrectAnswer: parseInt(correctAnswer.value) - 1
        };

        questionsObject['questions'].push(questionObject);  // Dodaj pytanie do tablicy


    }

    questionsObject['questionsNumber'] = parseInt(questionNumber);

    // Spakuj dane do JSON
    return JSON.stringify(questionsObject);
}

function dataValid(message, idOfObjects) {
    console.log('data valid')


    for (const id of idOfObjects) {
        console.log('data valid if');
        if (id.value === '') {
            console.log('data valid status');
            sendStatus(message, false);
            return false; // Validation failed
        }
    }
    // If no validation failed, return true
    return true;
}

function createGame() {

    jsonData = getQuestionsToJson(this.questionNumber);

    if (typeof jsonData !== "undefined") {
        // Wyślij dane 
        sendMessage(jsonData);
        loadSpiner.call(this, 2, 'tabForm'); // Adjust waitTime as needed
    }
}

function startWatingForPlayer() {

    if (dataValid('Wypełnij wszysktkie pola poprawnie', [gameIDStart, gamePinStart])) {

        const startGameMessage = {
            'type': 'startWatingForPlayer',
            'gameID': parseInt(gameIDStart.value),
            'gamePin': parseInt(gamePinStart.value)
        }

        // Spakuj dane do JSON
        const jsonData = JSON.stringify(startGameMessage);
        console.log(jsonData);

        // Wyślij dane 
        sendMessage(jsonData);
        loadSpiner.call(this, 1, 'startGameTab'); // Adjust waitTime as needed
    }
}

function checkQuestionNumber() {
    const parsedQuestionNumber = parseInt(this.questionNumber, 10);

    if (isNaN(parsedQuestionNumber) || parsedQuestionNumber <= 0) {
        sendStatus('Wypełnij liczbę pytań poprawnie', false);
    } else {
        this.tabForm = 1;
    }
}

function joinGame() {
    console.log('test')

    if (dataValid('Wypełnij wszysktkie pola', [gameIDJoin, gamePinJoin, gameNicknameJoin])) {
        console.log('data validated')

        const joinGameMessage = {
            'type': 'joinGame',
            'gameID': parseInt(gameIDJoin.value),
            'gamePin': parseInt(gamePinJoin.value),
            'nickname': gameNicknameJoin.value
        }

        // Spakuj dane do JSON
        const jsonData = JSON.stringify(joinGameMessage);
        console.log(jsonData);

        // Wyślij dane 
        sendMessage(jsonData);

        loadSpiner.call(this, 1, 'questionTab', true);

    }
}

function startGame() {

    playersRankElement.innerHTML = "";

    const startGameMessage = {
        'type': 'startGame',
        'gameID': parseInt(gameIDStart.value)
    }

    // Spakuj dane do JSON
    const jsonData = JSON.stringify(startGameMessage);
    console.log(jsonData);

    // Wyślij dane 
    sendMessage(jsonData);

    loadSpiner.call(this, 2, 'startGameTab');


}

function sendAnswer(event) {
    console.log('test');

    const clickedElement = event.currentTarget;
    const answerID = clickedElement.querySelector('p').id.replace('gameAnswer', '');
    console.log('Answer ID:', answerID);

    const userAnswer = {
        'type': 'answer',
        'gameID': parseInt(gameIDJoin.value),
        'answer': parseInt(answerID)
    }

    // Spakuj dane do JSON
    const jsonData = JSON.stringify(userAnswer);
    console.log(jsonData);

    // Wyślij dane 
    sendMessage(jsonData, false);


    loadSpiner.call(this, 1, 'questionTab', true);

}

function loadSpiner(newTab, targetVariable, leaveSpinner = false) {

    this.showSpinner = true;
    let timeout;

    console.log('test')
    let intervalId = setInterval(() => {
        console.log('interwal')

        if (respondTimeOut === null) {
            clearInterval(intervalId);
            clearTimeout(timeout);

            this[targetVariable] = newTab; // wait if server aproved

            if (leaveSpinner)
                this.showSpinner = true;
            else
                this.showSpinner = false;

        } else if (respondTimeOut === false) {
            clearInterval(intervalId);
            this.showSpinner = false;

        }
    }, 200);

    // Clear the interval after 5 seconds (waitTime milliseconds)
    timeout = setTimeout(() => {
        clearInterval(intervalId); // stop checking after 5 seconds if server dosent approved
        this.showSpinner = false;
    }, waitTime);
}

function goToQuestion() { //goToQuestionFunction

    clearInterval(goToQuestionInterval);
    clearTimeout(goToQuestionTimeOut);
    
    questionTimer.innerHTML = timeToRespond;

    this.showSpinner = false;
    this.questionTab = 3;

    goToQuestionInterval = setInterval(() => {
        questionTimer.innerText = questionTimer.innerText - 1;
    }, 1000)

    goToQuestionTimeOut = setTimeout(() => {
        clearInterval(goToQuestionInterval);

        setTimeout(() => {
            questionTimer.innerHTML = timeToRespond;
        }, 100)

    }, timeToRespond * 1000)
}

function goToTimer() { // goToTimerFunction
    console.log('timer');
    clearInterval(goToQuestionInterval);
    clearTimeout(goToQuestionTimeOut);
    
    questionTimer.innerHTML = timeToRespond;
    
    // this.showSpinner = false;
    // this.questionTab = 2;
    loadSpiner.call(this, 2, 'questionTab', false);

    this.spinner = false;

    goToTimerInterval = setInterval(() => {
        nextQuestionTimer.innerHTML = nextQuestionTimer.innerHTML - 1;
    }, 1000)

    goToTimerTimeOut = setTimeout(() => {
        clearInterval(goToTimerInterval);

        goToQuestionFunction.click();

        setTimeout(() => {
            nextQuestionTimer.innerHTML = 10;
        }, 100)

    }, 10000)
}

function goToRankFunction() {
    this.tab = 1;
    this.showSpinner = false;
    this.questionTab = 4;
    isEndOfGame = true;
}

function backButtonFunction() {
    this.tab = 0;
    this.tabForm = 0;

    if (isEndOfGame) {
        this.questionNumber = 1,
        this.showSpinner = false,
        this.questionTab = 0,
        this.startGameTab = 0
        playersRankElement.innerHTML = "";
        isEndOfGame = false;
    }
}