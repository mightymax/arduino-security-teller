const char* JS = R"=====(const server = './wachtenden'
const MAX = 10
let C =  0
const counter = () => {
  fetch(server)
    .then(r => r.text())
    .then(c =>  {
      c = parseInt(c)
      const h4 = document.querySelector('h4')
      h4.innerHTML=`<p>${c}</p>`
      if (c >= MAX) {
        h4.classList.add('stop')
        h4.classList.remove('go')
        h4.classList.remove('warn')
      } else if ( c > 0.5 * MAX) {
        h4.classList.remove('stop')
        h4.classList.remove('go')
        h4.classList.add('warn')
      } else {
        h4.classList.remove('stop')
        h4.classList.add('go')
        h4.classList.remove('warn')
      }
      document.querySelector('pre').innerText=''
    })
    .catch(_ => document.querySelector('pre').innerText='Fout bij het ophalen van de teller.')
  setTimeout(counter, 1000)
}

window.onload = () => {
  counter()
})=====";
