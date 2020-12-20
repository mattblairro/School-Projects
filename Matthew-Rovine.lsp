;;;Matthew Rovine Dicegame project 5-10-19

;;;Throws generates a random number betweeen 1-6
(defun throws () (+ 1 (random 6)))
;;;Repeats throws twice and stores it to a list
(defun dicethrow () (list (throws) (throws)))

;;;detects if two 6s are rolled
(defun sixsix (x) (and (equal 6 (first x)) (equal 6 (second x))))
;;;detects if two 1s are rolled
(defun oneone(x) (and (equal 1 (first x)) (equal 1 (second x))))
;;;seven eleven and three are used in the instantwin and instantlose functions
(defun seven (x) (equal 7 (+ (first x) (second x))))
(defun eleven (x) (equal 11 (+ (first x) (second x))))
(defun three (x) (equal 3 (+ (first x) (second x))))

;;;Tests if you get a winning roll
(defun instantwin (x) (or (seven x) (eleven x)))
;;;Tests for a losing roll
(defun instantlose (x) (or (or (three x) (sixsix x) (oneone x))))

;;;outputs the roll you got.  Special output one-one in the event of two ones or six-six in the event of two sixes
(defun saythrow (x) (cond ((oneone x) 'one-one) ((sixsix x) 'six-six) ((+ (first x) (second x)))))

;;;Combines previously made functions into one dicegame function.  Variables are generated when the function is called
(defun dice-game () (let* ((x (dicethrow)) (num (saythrow x))) 
(cond ((instantlose x) (list 'Throw (first x) 'and (second x) '- num '- 'you 'lose))
((instantwin x) (list 'throw (first x) 'and (second x) '- num '- 'you 'win))
((list 'throw (first x) 'and (second x) '- 'Your 'point 'is num)))))
 