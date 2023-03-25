const char* CSS = R"=====(body {
  text-align: center;
}

body > div {
  width: 500px;
}

h1 {
  color: rgb(3, 3, 50);
  font-family: Impact, Haettenschweiler, 'Arial Narrow Bold', sans-serif;
  font-size: 3em;
  margin: 0;
}

h1 img {
  width: 90%;
}
h2 {
  font-family: Impact, Haettenschweiler, 'Arial Narrow Bold', sans-serif;
  text-transform: uppercase;
  color: rgb(8, 8, 143);
  font-size: 6em;
  margin: 0;
}

h3 {
  font-family: Arial, Helvetica, sans-serif;
  text-transform: uppercase;
  color: rgb(8, 8, 143);
  font-size: 1.5em;
}

h4 {
  font-family: Impact, Haettenschweiler, 'Arial Narrow Bold', sans-serif;
  text-transform: uppercase;
  color: #ffffff;
  font-size: 6em;
  margin: auto;
  margin-top: -50px;
  width: 300px;
  height: 300px;
  background-image: url('https://raw.githubusercontent.com/mightymax/arduino-security-teller/main/stop-and-go.jpg');
  background-position: -600px 0px;
  background-repeat: no-repeat;
}

h4.stop {
  background-position: 0px 0px;
}

h4.warn {
  background-position: -300px 0px;
}

h4 p {
  line-height: 300px;
}

h4.hidden {
  display: none
})=====";