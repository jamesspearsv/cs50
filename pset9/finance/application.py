import os


from sqlite3 import *
from flask import Flask, flash, redirect, render_template, request, session
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
db = connect("finance.db")
cursor = db.cursor()
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
                purchase_price NUMERIC NOT NULL,
                purchase_datetime TEXT NOT NULL,
                shares INTEGER,
                user_id INTEGER,
                FOREIGN KEY(user_id) REFERENCES users(id)
                );""")
except:
    pass

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    return render_template("index.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        quote = lookup(symbol)
        if quote==None:
            return apology("Looks like nothing was found", 404)

        name = quote["name"]
        price = quote["price"]
        symbol = quote["symbol"]
        date = datetime.now()
        user_id = session["user_id"]

        print(f"symbol: {symbol}")
        print(f"shares: {shares}")
        print(f"datetime: {date}")

        """TODO: Finish collection necessary data and insert into databse"""
        """TODO: Need new table to record transaction history"""
        """TODO: Link all tables to session["user_id"]"""

        return apology("TODO")
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


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
        rows = cursor.execute("SELECT * FROM users WHERE username = ?;", (request.form.get("username"),)).fetchall()

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
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote==None:
            return apology("Looks like nothing was found", 404)
        
        name = quote["name"]
        symbol = quote["symbol"]
        price = usd(quote["price"])
        
        return render_template("quoted.html", name=name, price=price, symbol=symbol)
    else:
       return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirm = request.form.get("password_confirm")

        rows = cursor.execute("SELECT username FROM users WHERE username=?;", (username,)).fetchall()
        # Check user provided username and password
        if username == None or password == None:
            return apology("Username or password not provided", 403)
        # Check user confirmed password
        if password != password_confirm:
            return apology("Passwords do not match", 403)
        # Check username is available
        if len(rows) >= 1:
            return apology("Username taken")

        hashword = generate_password_hash(password)

        # Insert new username and hashword into DB
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", (username, hashword))
        db.commit()
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
