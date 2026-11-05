# Testing Field Guide

Keep this open while you test. It has everything you need to write a real test case. The examples come from the Fright Night app you tested in class.

---

## Part 1: Verification vs. Validation

| | Question it asks | A failure looks like |
|---|---|---|
| **Verification** | Did we build it **right**? | The code does not match the requirements or the diagrams. |
| **Validation** | Did we build the **right thing**? | The code matches the requirements perfectly, but the requirements do not meet the stakeholder's actual goal. |

Every failing test case you write gets labeled one or the other. Check the Stakeholder Goals (G1, G2, G3) in the requirements when you suspect a validation bug.

---

## Part 2: Seven Types of Testing

The example ideas below test features that **work correctly**. They show you the format. They are not bugs.

| Type | What it checks | Example idea |
|---|---|---|
| **Unit** | One function by itself | `formatVoteCount(1)` returns `"1 vote"`, not `"1 votes"` |
| **Integration** | Two parts of the system working together | Submit a Spooky costume, then confirm it appears in the Gallery |
| **System (End-to-End)** | A whole user flow, start to finish | Register a new account, log in, and see your username in the header |
| **Acceptance** | Does it meet the stakeholder's real need? | A judge can identify the leading costume within 5 seconds of opening the leaderboard (see G2) |
| **Boundary Value** | The edges of a valid range | A costume name of exactly 50 characters is accepted |
| **Negative** | Bad input is rejected gracefully | Logging in with the wrong password shows an error and does not log you in |
| **Security** | Can someone do what they should not? | A logged-out user who tries to submit a costume is blocked |

**Tip on Boundary Value testing:** boundary tests come in threes. Test just below the limit, exactly at the limit, and just above it. For a 50-character limit, that is 49, 50, and 51.

---

## Part 3: Anatomy of a Test Case

| Field | What goes here | Why it matters |
|---|---|---|
| **Test ID** | A unique label, like TC-07 | So you can reference it later |
| **Written by** | Your name | Individual credit |
| **Title** | One line describing what is tested | Someone skimming 200 tests knows what this one does |
| **Traces to** | A REQ ID, NFR ID, goal ID, or diagram name | Proves the test exists for a reason, not vibes |
| **Test type** | One of the seven types above | Shows how you attacked the problem |
| **Preconditions** | What must be true before you start | A test that only works on your laptop is not a test |
| **Test data** | The exact inputs you will use | "Some text" is not reproducible. "Vampire Lord" is. |
| **Steps** | Numbered, one action per step | Anyone should be able to follow them |
| **Expected result** | What **should** happen, specific and checkable | The heart of the test. Decide it **before** you run it. |
| **Actual result** | What **did** happen | Filled in only after running |
| **Status** | Pass or Fail | |
| **Severity** | Critical, Major, or Minor (Fail only) | Helps the team decide what to fix first |
| **V or V** | Verification or Validation (Fail only) | Did we build it wrong, or build the wrong thing? |

### Severity levels

| Severity | Meaning | Example |
|---|---|---|
| **Critical** | Security hole, data loss, or the contest cannot run fairly | Anyone can delete anyone's costume |
| **Major** | A core feature is wrong | A rule in the requirements is not enforced |
| **Minor** | Cosmetic or low impact | Text is slightly off |

---

## Part 4: A Fully Written Sample Test Case

> **Test ID:** TC-00
> **Written by:** Instructor
> **Title:** Costume name at the maximum length is accepted
> **Traces to:** REQ-04 (Costume names must be between 1 and 50 characters)
> **Test type:** Boundary Value
> **Preconditions:** Logged in as `ghost_gary` (password `boo12345`). On the Submit Costume page.
> **Test data:**
> - Name: `TheHeadlessHorsemanOfSleepyHollowRidesAgainTonight` (exactly 50 characters)
> - Description: `Rides a pumpkin-powered scooter.`
> - Category: Spooky
>
> **Steps:**
> 1. Type the test data name into the Costume name field
> 2. Type the test data description into the Description field
> 3. Select Spooky from the Category dropdown
> 4. Click Submit Costume
> 5. Open the Gallery page
>
> **Expected result:** A success message appears. The costume appears in the Gallery with the full 50-character name, not truncated.
> **Actual result:** Success message appeared. The costume appears in the Gallery with the full name.
> **Status:** Pass
> **Severity:** N/A
> **V or V:** N/A

**If this test had failed,** you would fill in Severity and V or V.

**Remember:** boundary tests come in threes. We tested 50. Testing 49 and 51 is up to you.

---

## Part 5: Good vs. Bad Expected Results

The most common rookie mistake is a mushy expected result.

| Bad | Why it is bad |
|---|---|
| "It works correctly." | Works how? Nobody can check this. |
| "The name is saved." | Saved where? Shown how? |

| Good |
|---|
| "The costume appears in the Gallery with the full 50-character name, not truncated." |

**If you cannot tell whether a test passed by reading its expected result, it is not a test. It is a wish.**

---

## Part 6: Where Bugs Hide

Bugs can live in three places:

1. **In the code.** The app does something wrong.
2. **In the gap between code and docs.** The app does one thing, the requirements or diagrams say another.
3. **In the docs themselves.** Two documents contradict each other, or a requirement does not serve the stakeholder goal.

Look in all three. Open the app's code files in the `app/` folder. Open your browser's developer tools (F12) and check the Console and the Application tab (Local Storage). Read every diagram carefully.
