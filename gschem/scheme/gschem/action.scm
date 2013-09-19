;; gEDA - GPL Electronic Design Automation
;; gschem - gEDA Schematic Capture - Scheme API
;; Copyright (C) 2013 Peter Brett <peter@peter-b.co.uk>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
;;

(define-module (gschem action)
  #:use-module (gschem window)
  #:use-module (ice-9 optargs))

(define current-action-position (make-fluid))

;; Define an eval-in-currentmodule procedure
(define (eval-cm expr) (eval expr (current-module)))

;; Evaluates a gschem action.  A gschem action is expected to be a
;; symbol naming a thunk variable in the current module.
;;
;; The special-case symbol repeat-last-command causes the last action
;; executed to be repeated.
(define-public (eval-action! action)
  (define (invalid-action-error)
    (error "~S is not a valid gschem action." action))

  (cond

   ;; Normal actions
   ((symbol? action)
    (let ((proc (false-if-exception (eval-cm action))))
      (if (thunk? proc)
          (begin
            (proc)
            #t)
          (invalid-action-error))))

   ;; Otherwise, fail
   (else (invalid-action-error))))

;; Evaluate an action at a particular point on the schematic plane.
;; If the point is omitted, the action is evaluated at the current
;; mouse pointer position.
(define*-public (eval-action-at-point!
                 action
                 #:optional (point (pointer-position)))

  (with-fluids ((current-action-position point))
               (eval-action! action)))

;; Return the current action pointer position.  This should be the
;; location at which the action was invoked (set via
;; eval-action-at-point!).
(define-public (action-position)
  (fluid-ref current-action-position))
