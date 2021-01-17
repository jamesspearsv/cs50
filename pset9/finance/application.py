import os

from sqlite3 import *
from flask import Flask, flash, redirect, render_template, request, session, json, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database (Reworked to use Python sqlite3 module)
db = connect("finance.db", check_same_thread=False)
cursor = db.cursor()
db.commit()
try:
    db.execute("""CREATE TABLE users (
                id INTEGER,
                username TEXT NOT NULL,
                hash TEXT NOT NULL,
                cash NUMERIC NOT NULL DEFAULT 10000.00,
                PRIMARY KEY(id));""")
    db.execute("CREATE UNIQUE INDEX username ON users (username);")
    db.execute("""CREATE TABLE portfolio (
                symbol TEXT NOT NULL,
                stock_name TEXT NOT NULL,
                total_shares INTEGER NOT NULL,
                user_id INTEGER NOT NULL,
                PRIMARY KEY(symbol, user_id),
                FOREIGN KEY(user_id) REFERENCES users(id));""")
    db.execute("""CREATE TABLE ledger (
                id INTEGER,
                symbol TEXT NOT NULL,
                transaction_type TEXT NOT NULL,
                price NUMERIC NOT NULL,
                shares INTEGER NOT NULL,
                datetime TEXT NOT NULL,
                user_id INTEGER,
                PRIMARY KEY(id),
                FOREIGN KEY(symbol, user_id) REFERENCES portfolio(symbol, user_id),
                FOREIGN KEY(user_id) REFERENCES users(id));""")
    db.commit()
except:
    pass

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = []
    temp_row = db.execute("SELECT cash FROM users WHERE id=?;", (session["user_id"],)).fetchall()
    user_cash = float(temp_row[0][0])
    portfolio_value = user_cash
    
    # Get data for table
    for row in db.execute("SELECT symbol, stock_name, total_shares FROM portfolio WHERE user_id=?;", (session["user_id"],)):
        quote = lookup(row[0])
        price = quote["price"]
        stock_value = float(price) * float(row[2])
        tempDict = {
            'symbol' : row[0],
            'stock_name' : row[1],
            'total_shares' : row[2],
            'current_price' : price,
            'value' : stock_value
        }
        # Append data from database and lookup to Dict
        portfolio.append(tempDict)
        portfolio_value = portfolio_value + stock_value
    
    return render_template("index.html", portfolio=portfolio, user_cash=user_cash, portfolio_value=portfolio_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        transaction_type = "buy"

        # Get lookup information
        quote = lookup(request.form.get("symbol"))
        if quote==None:
            return apology("Looks like nothing was found", code=400)

        # Store necessary information in variables
        shares = request.form.get("shares")
        if not shares.isdigit():
            return apology("Error", code=400)
        name = quote["name"]
        price = quote["price"]
        symbol = quote["symbol"]
        date = datetime.utcnow()
        user = session["user_id"]
        cost = price * float(shares)

        temp = cursor.execute("SELECT cash FROM users WHERE id=?;", (user,)).fetchall()
        user_funds = temp[0][0]

    
        # Check that user has funds to make purchase
        if cost > user_funds:
            return apology("Insufficient funds", 400)
                

        # Update users cash account balance and portfolio
        # Check is user does not already own desired stock
        rows = cursor.execute("SELECT * from portfolio WHERE user_id=? AND symbol=?;", (user, symbol)).fetchall()
        if len(rows) == 0:
            db.execute("INSERT INTO portfolio (symbol, stock_name, total_shares, user_id) VALUES(?, ?, ?, ?);", (symbol, name, shares, user))
            db.execute("UPDATE users SET cash=cash - ? WHERE id=?;", (cost, user))
            db.commit()
        else:
            db.execute("UPDATE portfolio SET total_shares=total_shares + ? WHERE user_id=? AND symbol=?;", (shares, user, symbol))
            db.execute("UPDATE users SET cash=cash - ? WHERE id=?;", (cost, user))
            db.commit()

        # Insert transaction into ledger. 
        db.execute("""INSERT INTO ledger (symbol, transaction_type, price, shares, datetime, user_id) 
        VALUES (?, ?, ?, ?, ?, ?);""", (symbol, transaction_type, price, shares, date, user))
        db.commit()

        flash(f"Bought {shares} share(s) of {name}")
        return redirect("/")
    # GET requests to route
    else: 
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Gets transaction history from DB
    history = cursor.execute(""" SELECT symbol, shares, price, datetime 
    FROM ledger WHERE user_id=? ORDER by id DESC;""", (session["user_id"], )).fetchall()
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = cursor.execute("SELECT * FROM users WHERE username =?;", (request.form.get("username"),)).fetchall()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0][2], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0][0] # User ID from DB (Row 0 Column 0)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Get needed lookup information from lookup()
        quote = lookup(request.form.get("symbol"))
        # If symbol provided is invalid
        if quote==None:
            return apology("Looks like nothing was found", code=400)
        
        name = quote["name"]
        symbol = quote["symbol"]
        price = quote["price"]

        # Gets user cash balance
        row = db.execute("SELECT cash FROM users WHERE id=?;", (session["user_id"],)).fetchall()
        cash = row[0][0]
        

        price_js = json.dumps( float(price) )
        return render_template("quoted.html", name=name, price=price, symbol=symbol, cash=cash, price_js=price_js)
    # GET requests to quote route
    else:
       return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Validate form
        if not request.form.get("username") or not request.form.get("password"):
            return apology("Please provide a username and password", code=400)
        
        # Get user input from registration form
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirm = request.form.get("confirmation")


        # Check user confirmed password
        if password != password_confirm:
            return apology("Passwords do not match", code=400)
        # Check username is available
        rows = cursor.execute("SELECT username FROM users WHERE username=?;", (username,)).fetchall()
        if len(rows) != 0:
            return apology("Username taken")

        hashword = generate_password_hash(password)

        # Insert new username and hashword into DB
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", (username, hashword))
        db.commit()
        return redirect("/")
    # GET requests to route
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        transaction_type = "sell"

    
        # Get lookup information
        quote = lookup(request.form.get("symbol"))
        if quote==None:
            return apology("Looks like nothing was found", 400)

        shares = request.form.get("shares")
        if not shares.isdigit(): 
            return apology("Error", 400)

        # Store necessary information in variables
        name = quote["name"]
        price = quote["price"]
        symbol = quote["symbol"]
        date = datetime.utcnow()
        user = session["user_id"]
        cost = price * float(shares)

        temp = cursor.execute("SELECT total_shares FROM portfolio WHERE user_id=? AND symbol=?;", (user, symbol)).fetchall()
        user_shares = temp[0][0]

        
        # Check that user has enough shares to make the sale
        if int(shares) > user_shares:
            return apology("Insufficient funds", 400)
        shares = int(shares) * -1
                

        # Update users cash account balance and portfolio
        # If user already owns desired stock.
        db.execute("UPDATE portfolio SET total_shares=total_shares + ? WHERE user_id=? AND symbol=?;", (shares, user, symbol))
        db.execute("UPDATE users SET cash=cash + ? WHERE id=?;", (cost, user))

        # Check that user has shares remaining after sale. If not, delete row.
        rows = db.execute("SELECT total_shares FROM portfolio WHERE symbol=? AND user_id=?;", (symbol, user)).fetchall()
        if int(rows[0][0]) == 0:
            db.execute("DELETE FROM portfolio WHERE user_id=? AND symbol=?;", (user, symbol))
            db.commit()
        else: 
            db.commit()

        # Insert transaction into ledger. 
        db.execute("""INSERT INTO ledger (symbol, transaction_type, price, shares, datetime, user_id) 
        VALUES (?, ?, ?, ?, ?, ?);""", (symbol, transaction_type, price, shares, date, user))
        db.commit()

        flash(f"Sold {(shares * -1)} share(s) of {name}")
        return redirect("/")
    # GET requests to sell route
    else: 
        # TODO get list of owned stock symbol
        portfolio = db.execute("SELECT symbol FROM portfolio WHERE user_id=?;", (session["user_id"],)).fetchall()
        return render_template("sell.html", portfolio=portfolio)

@app.route("/settings")
@login_required
def settings():
        # Grabs username to display on settings page
        row = db.execute("SELECT username, hash FROM users WHERE id=?;", (session["user_id"],)).fetchall()
        return render_template("settings.html", username=row[0][0])

@app.route("/username", methods=["GET"])
def username():
    # Checks username availbility and returns to client
    q = request.args.get('q')
    available = []
    rows = db.execute("SELECT username FROM users WHERE username=?;", (q,)).fetchall()
    if len(rows) == 0:
        available.append('true')
        return jsonify(available)
    else:
        available.append('false')
        return jsonify(available)

@app.route("/password", methods=["POST"])
@login_required
def password():
    # Check that user inputs valid password when attempting to update it.
    data = request.form['password']
    result = []
    row = db.execute("SELECT hash FROM users WHERE id=?;", (session["user_id"],)).fetchall()
    if check_password_hash(row[0][0], data):
        result.append('true')
        return jsonify(result)
    else:
        result.append('false')
        return jsonify(result)

@app.route("/update", methods=["GET", "POST"])
@login_required
def update():
    # inserts user's new password into database. 
    if request.method == "POST":
        password = request.form.get("password")
        hashword = generate_password_hash(password)
        db.execute("UPDATE users SET hash =? WHERE id=?;", (hashword, session["user_id"]))
        flash("Password updated")
        return redirect("/")
    else:
        return render_template("update.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
