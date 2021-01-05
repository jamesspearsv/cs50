-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find description of the crime
SELECT description FROM crime_scene_reports
WHERE day=28 AND month=7 AND year=2020
AND street="Chamberlin Street";
    -- description
    -- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
    -- Interviews were conducted today with three witnesses who were present
    -- at the time — each of their interview transcripts mentions the courthouse.

-- Find interviews with witnesses from day of crime
SELECT name, transcript FROM interviews
WHERE day=28 AND month=7 AND year=2020
AND transcript LIKE "%courthouse%";

-- Thief planned to take earliest flight out of town next day
-- Query airports table for Fiftyville airport info
SELECT id, full_name, abbreviation FROM airports WHERE city="Fiftyville";
    -- 8 | Fiftyville Regional Airport | CSF

-- Query flights table
SELECT id, destination_airport_id, hour, minute FROM flights
WHERE month=7 AND day=29 AND year=2020
AND origin_airport_id IN
(SELECT id FROM airports WHERE city="Fiftyville")
ORDER BY hour, minute;
    -- id | destination_airport_id | hour | minute
    -- 36 | 4 | 8 | 20 <---- FIRST FLIGHT OUT OF CSF
    -- 43 | 1 | 9 | 30
    -- 23 | 11 | 12 | 15
    -- 53 | 9 | 15 | 20
    -- 18 | 6 | 16 | 0

-- Find destination of flight 36 out of CSF on 7/29/2020
SELECT id, full_name, city, abbreviation FROM airports WHERE id=4;
    -- id | full_name | city | abbreviation
    -- 4 | Heathrow Airport | London | LHR

-- Find passengers on flight 36 from Fiftyville(8) to London(4)
SELECT passport_number, seat FROM passengers WHERE flight_id=36;
    -- 7214083635 | 2A
    -- 1695452385 | 3B
    -- 5773159633 | 4A
    -- 1540955065 | 5C
    -- 8294398571 | 6C
    -- 1988161715 | 6D
    -- 9878712108 | 7A
    -- 8496433585 | 7B

-- Get names of passengers on flight 36
SELECT name, phone_number, license_plate FROM people
WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id=36);
-- SELECT all passangers on Flight 36
SELECT name, phone_number, people.passport_number, license_plate FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id=36
ORDER BY name;
    -- Bobby | (826) 555-1652 | 30G67EN
    -- Roger | (130) 555-0289 | G412CB7
    -- Madison | (286) 555-6063 | 1106N58
    -- Danielle | (389) 555-5198 | 4328GD8
    -- Evelyn | (499) 555-9472 | 0NTHK55
    -- Edward | (328) 555-1152 | 130LD9Z
    -- Ernest | (367) 555-5533 | 94KL13X
    -- Doris | (066) 555-9701 | M51FA04

-- Witness mentions seeing thief using ATM, query bank_accounts table
-- against people.id to find acount numbers of people on flight 36
-- Join people and bank_accounts tables to display names and account numbers together
SELECT name, account_number FROM bank_accounts
JOIN people ON people.id = bank_accounts.person_id
WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id=36);
    -- name | account_number
    -- Ernest | 49610011
    -- Bobby | 28296815
    -- Danielle | 28500762
    -- Madison | 76054385

-- Query bank account numbers against atm_transactions before 10:15 on 7/28/2020
SELECT id, account_number, transaction_type FROM atm_transactions
WHERE month=7 AND day=28 AND year=2020
AND atm_location="Fifer Street"
AND account_number IN
(SELECT account_number FROM bank_accounts
JOIN people ON people.id = bank_accounts.person_id
WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id=36));
    -- id | account_number | transaction_type
    -- 246 | 28500762 | withdraw        //All 4 account numbers
    -- 264 | 28296815 | withdraw        //match people on
    -- 266 | 76054385 | withdraw        //flight 36
    -- 267 | 49610011 | withdraw

-- Witness recalls seeing thief make a phone call
-- Query phone calls table against passangers' phone numbers on flight 36
SELECT caller, receiver, duration FROM phone_calls
WHERE month=7 AND day=28 AND year=2020
AND duration < 60
AND caller IN
(SELECT phone_number FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id=36);
    -- caller | receiver | duration
    -- (130) 555-0289 | (996) 555-8899 | 51
    -- (499) 555-9472 | (892) 555-8872 | 36
    -- (367) 555-5533 | (375) 555-8161 | 45
    -- (499) 555-9472 | (717) 555-1342 | 50
    -- (286) 555-6063 | (676) 555-6554 | 43
    -- (826) 555-1652 | (066) 555-9701 | 55

-- Check courthouse security logs against passenger license plates
SELECT license_plate, activity, hour, minute FROM courthouse_security_logs
WHERE month=7 AND day=28 AND year=2020
AND license_plate IN
(SELECT license_plate FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id=36);
license_plate | activity | hour | minute
    -- 94KL13X | entrance | 8 | 23
    -- 1106N58 | entrance | 8 | 34
    -- 0NTHK55 | entrance | 8 | 42
    -- 4328GD8 | entrance | 9 | 14
    -- G412CB7 | entrance | 9 | 28
    -- 94KL13X | exit | 10 | 18
    -- 4328GD8 | exit | 10 | 19
    -- G412CB7 | exit | 10 | 20
    -- 0NTHK55 | exit | 10 | 23
    -- 1106N58 | exit | 10 | 35

-- Consolidated query for passengers of Flight 36
SELECT DISTINCT(name), phone_number, people.passport_number, license_plate, bank_accounts.account_number FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE flight_id=36;
    -- name | phone_number | passport_number | license_plate | account_number
    -- Ernest | (367) 555-5533 | 5773159633 | 94KL13X | 49610011 <-- Guilty!
    -- Bobby | (826) 555-1652 | 9878712108 | 30G67EN | 28296815
    -- Danielle | (389) 555-5198 | 8496433585 | 4328GD8 | 28500762 <-- CLEARED. DID NOT MAKE OR RECIEVE A CALL ON 7/28
    -- Madison | (286) 555-6063 | 1988161715 | 1106N58 | 76054385

SELECT caller, receiver, duration FROM phone_calls
WHERE month=7 AND day=28 AND year=2020
AND duration < 60
AND caller IN
(SELECT phone_number FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE flight_id=36)
ORDER BY id;
    -- caller | receiver | duration
    -- (367) 555-5533 | (375) 555-8161 | 45 <-- Ernest
    -- (286) 555-6063 | (676) 555-6554 | 43
    -- (826) 555-1652 | (066) 555-9701 | 55

SELECT id, account_number, transaction_type, amount FROM atm_transactions
WHERE month=7 AND day=28 AND year=2020
AND atm_location="Fifer Street"
AND account_number IN
(SELECT bank_accounts.account_number FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE flight_id=36)
ORDER BY id;
    -- id | account_number | transaction_type
    -- 246 | 28500762 | withdraw
    -- 264 | 28296815 | withdraw <-- Ernest
    -- 266 | 76054385 | withdraw
    -- 267 | 49610011 | withdraw

SELECT license_plate, activity, hour, minute FROM courthouse_security_logs
WHERE month=7 AND day=28 AND year=2020
AND license_plate IN
(SELECT license_plate FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE flight_id=36)
ORDER BY id;
    -- license_plate | activity | hour | minute
    -- 94KL13X | entrance | 8 | 23 <-- Ernest; first one in
    -- 1106N58 | entrance | 8 | 34
    -- 4328GD8 | entrance | 9 | 14
    -- 94KL13X | exit | 10 | 18 <-- Ernest; first one out after theft(10:15)
    -- 4328GD8 | exit | 10 | 19
    -- 1106N58 | exit | 10 | 35

-- Query who Ernest called
SELECT id, name, passport_number, license_plate FROM people WHERE phone_number="(375) 555-8161";
    -- id | name | passport_number | license_plate
    -- 864400 | Berthold |  | 4V16VO0