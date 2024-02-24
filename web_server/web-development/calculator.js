
            function calculate(){

                const hourlyRate = parseInt(document.querySelector("#hourlyRate").value);
                const numberOfHours = parseInt(document.querySelector("#numberOfHours").value);
                const selectCurrency = document.querySelector("#selectCurrency").value;

                // fetch and get the rate from external apis
              

                fetch('https://open.er-api.com/v6/latest/EUR')
                .then(response=> response.json())
                .then(data =>{
                    const exchangeRate = data.rates[selectCurrency];
                    const finalPrice = hourlyRate * numberOfHours * exchangeRate;

                    document.getElementById("sumOfQuote").textContent = finalPrice;
                    
                })
                
                

            }
                

                

      