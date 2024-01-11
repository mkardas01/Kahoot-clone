let socket = new WebSocket('ws://localhost:3000/events');
let timeToRespond = 20;
let respondTimeOut = null;

const nextQuestionTimeout = 10;
const waitTime = 10000;

const gameIDCreated = document.querySelector('#gameIDCreated');
const gamePinCreated = document.querySelector('#gamePinCreated');

const playersList = document.querySelector('#playersList');

const spinner = document.querySelector('#spinner');

const backButton = document.querySelector('#backButton');

const gameIDJoin = document.querySelector('#gameIDJoin');
const gamePinJoin = document.querySelector('#gamePinJoin');
const gameNicknameJoin = document.querySelector('#gameNicknameJoin');

const gameIDStart = document.querySelector('#gameIDStart');
const gamePinStart = document.querySelector('#gamePinStart');
const gameNicknameStart = document.querySelector('#gameNicknameStart');

const nextQuestionTimer = document.querySelector('#nextQuestionTimer');
const questionTimer = document.querySelector('#questionTimer');

const goToTimerFunction = document.querySelector('#goToTimer');
const goToQuestionFunction = document.querySelector('#goToQuestion');

const startGameTab2 = document.querySelector('#startGameTab2');

const playersRankElement = document.getElementById("playersRank");;

let goToTimerInterval;
let goToTimerTimeOut;

let goToQuestionInterval;
let goToQuestionTimeOut;

const goToRank = document.querySelector('#goToRank');


let isEndOfGame = false;

questionTimer.innerText = timeToRespond;
nextQuestionTimer.innerText = nextQuestionTimeout;