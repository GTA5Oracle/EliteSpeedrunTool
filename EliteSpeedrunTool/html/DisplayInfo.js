var websocket = null;

var timerStartTime = null;
var timer = null;
var serverBrowserTimeDelta = 0;

var autoTimerStartTime = null;
var autoTimer = null;
var serverBrowserAutoTimeDelta = 0;

// 判断浏览器是否支持 WebSocket
if ('WebSocket' in window) {
	websocket = new ReconnectingWebSocket("ws://" + document.domain + ":9976/", null, {maxReconnectAttempts: 10});
	websocket.onopen = function() {

	}
	websocket.onerror = function() {

	}
	websocket.onclose = function() {

	}
	websocket.onmessage=function(msg){
		var j = JSON.parse(msg.data);
		if (j.hasOwnProperty('missionData')) {
			document.getElementById("missionData").innerHTML = j.missionData;
		}
		if (j.hasOwnProperty('timerState')) {
			if (j.timerState == 0x03) { // 归零然后开始
				updateStarttedAndServerTimestamp(j);
				clearTimerAndSetNull();		// 清空计时器 timer
				timer = setInterval(function() { timerFunc(timerStartTime) }, 50);
			} else if (j.timerState == 0x02) {  // 继续计时
				updateStarttedAndServerTimestamp(j);
				clearTimerAndSetNull();		// 清空计时器 timer
				timer = setInterval(function() { timerFunc(timerStartTime) }, 50);
			} else if (j.timerState == 0x05) {  // 暂停计时
				clearTimerAndSetNull();		// 清空计时器 timer
				if (j.hasOwnProperty('pausedTimestamp')) {
					updateStarttedAndServerTimestamp(j);
					timerFunc(timerStartTime, new Date(j.pausedTimestamp), 0);		// 更新暂停时间
				}
			} else if (j.timerState == 0x04) {  // 停止计时
				clearTimerAndSetNull();		// 清空计时器 timer
				if (j.hasOwnProperty('pausedTimestamp')) {
					updateStarttedAndServerTimestamp(j);
					timerFunc(timerStartTime, new Date(j.pausedTimestamp), 0);		// 更新暂停时间
				}
				timerStartTime = null;
			} else if (j.timerState == 0x01) {  // 归零
				clearTimerAndSetNull();		// 清空计时器 timer
				timerStartTime = null;
				document.getElementById("timerMinute").innerHTML = "00";
				document.getElementById("timerSecond").innerHTML = "00";
				document.getElementById("timerMs").innerHTML = "00";
			}
		}
		if (j.hasOwnProperty('autoTimerState')) {
			if (j.autoTimerState == 0x02) {  // 继续计时
				updateAutoStarttedAndServerTimestamp(j);
				clearAutoTimerAndSetNull();		// 清空自动计时器 autoTimer
				autoTimer = setInterval(function() { autoTimerFunc(autoTimerStartTime) }, 50);
			} else if (j.autoTimerState == 0x03) {  // 暂停计时
				updateAutoStarttedAndServerTimestamp(j);
				clearAutoTimerAndSetNull();		// 清空自动计时器 autoTimer
				if (j.hasOwnProperty('pausedAutoTimestamp')) {
					updateAutoStarttedAndServerTimestamp(j);
					autoTimerFunc(autoTimerStartTime, j.pausedAutoTimestamp, 0);		// 更新暂停时间
				}
			} else if (j.autoTimerState == 0x01) {  // 归零
				clearAutoTimerAndSetNull();		// 清空自动计时器 autoTimer
				autoTimerStartTime = null;
				document.getElementById("autoTimerMinute").innerHTML = "00";
				document.getElementById("autoTimerSecond").innerHTML = "00";
				document.getElementById("autoTimerMs").innerHTML = "00";
			}
		}
	}
	// 监听窗口关闭事件，当窗口关闭时，主动去关闭websocket连接，防止连接还没断开就关闭窗口，server端会抛异常。
	window.onbeforeunload = function () {
		clearTimerAndSetNull();		// 清空计时器 timer
		timerStartTime = null;
		websocket.close();
	}
}
else {
	alert('The browser not support websocket')
}

function timerFunc(timerStartTime, now = new Date(), sbTimeDelta = serverBrowserTimeDelta) {
	var deltaTime = now - timerStartTime + sbTimeDelta;
	var m = parseInt(deltaTime / 1000 / 60);
	var s = parseInt(deltaTime / 1000) % 60;
	var ms = parseInt((deltaTime % 1000) / 10);
	document.getElementById("timerMinute").innerHTML = m.toString().padStart(2, '0');
	document.getElementById("timerSecond").innerHTML = s.toString().padStart(2, '0');
	document.getElementById("timerMs").innerHTML = ms.toString().padStart(2, '0');
}

function autoTimerFunc(autoTimerStartTime, now = new Date(), sbAutoTimeDelta = serverBrowserAutoTimeDelta) {
	var deltaTime = now - autoTimerStartTime + sbAutoTimeDelta;
	var m = parseInt(deltaTime / 1000 / 60);
	var s = parseInt(deltaTime / 1000) % 60;
	var ms = parseInt((deltaTime % 1000) / 10);
	document.getElementById("autoTimerMinute").innerHTML = m.toString().padStart(2, '0');
	document.getElementById("autoTimerSecond").innerHTML = s.toString().padStart(2, '0');
	document.getElementById("autoTimerMs").innerHTML = ms.toString().padStart(2, '0');
}

// 清空计时器 timer
function clearTimerAndSetNull() {
	if (timer != null) {
		clearInterval(timer);
		timer = null;
	}
}

// 清空自动计时器 autoTimer
function clearAutoTimerAndSetNull() {
	if (autoTimer != null) {
		clearInterval(autoTimer);
		autoTimer = null;
	}
}

// 更新开始计时时间和服务器时间（与服务器的时间差）
function updateStarttedAndServerTimestamp(json) {
	if (json.hasOwnProperty('starttedTimestamp')) {
		if (json.hasOwnProperty('serverTimestamp')) {
			// 服务器与浏览器的时差
			serverBrowserTimeDelta = json.serverTimestamp - new Date();
		}
		timerStartTime = new Date(json.starttedTimestamp);        // 与服务端矫正时间
	}
}

// 更新自动计时开始计时时间和服务器时间（与服务器的时间差）
function updateAutoStarttedAndServerTimestamp(json) {
	if (json.hasOwnProperty('starttedAutoTimestamp')) {
		if (json.hasOwnProperty('serverAutoTimestamp')) {
			// 服务器与浏览器的时差
			serverBrowserAutoTimeDelta = json.serverAutoTimestamp - new Date();
		}
		autoTimerStartTime = new Date(json.starttedAutoTimestamp);        // 与服务端矫正时间
	}
}