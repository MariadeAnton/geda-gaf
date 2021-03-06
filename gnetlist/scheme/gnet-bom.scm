; -*-scheme-*-
;
;;; gEDA - GPL Electronic Design Automation
;;; gnetlist - gEDA Netlist
;
;;; Copyright (C) 1998-2015 Ales Hvezda
;;; Copyright (C) 1998-2015 gEDA Contributors (see ChangeLog for details)
;
;;; This program is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 2 of the License, or
;;; (at your option) any later version.
;
;;; This program is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.
;
;;; You should have received a copy of the GNU General Public License
;;; along with this program; if not, write to the Free Software
;;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
;;; 02110-1301 USA, <http://www.gnu.org/licenses/>.


;; --------------------------------------------------------------------------
;;
;; Bill of Material backend written by Matt Ettus starts here
;;

;;; Bill Of Materials Generator
;;; You must have a file called attribs in the pwd
;;; The file should be a text list of attributes you want listed,
;;; One per line.  No comments are allowed in the file.
;;; Questions? Contact matt@ettus.com
;;; This software is released under the terms of the GNU GPL

(use-modules (ice-9 rdelim) ;; guile-1.8 fix
             (gnetlist backend-getopt))

(define bom:open-input-file
  (lambda (options)
    (let ((filename (backend-option-ref options 'attrib_file "attribs")))
      (if (file-exists? filename)
          (open-input-file filename)
          (if (backend-option-ref options 'attribs) #f
              (begin
                (format (current-error-port)
"ERROR: Attribute file '~A' not found. You must do one of the following: ~%
         - Create an 'attribs' file ~%
         - Specify an attribute file using -Oattrib_file=<filename> ~%
         - Specify which attributes to include using -Oattribs=attrib1,attrib2,... (no spaces) ~%"
filename)
                (primitive-exit 1)))))))

(define bom
  (lambda (output-filename)
    (let* ((options (backend-getopt (get-backend-arguments)
                     '((attrib_file (value #t)) (attribs (value #t)))))
           (attriblist (bom:parseconfig (bom:open-input-file options) options)))
      (set-current-output-port (output-port output-filename))
      (and attriblist
           (begin (bom:printlist (cons 'refdes attriblist))
                  (bom:components netlist:packages attriblist)
                  ))
      (close-output-port (current-output-port)))))

(define bom:printlist
  (lambda (ls)
    (if (null? ls)
        (newline)
        (begin
          (display (car ls))
          (write-char #\tab)
          (bom:printlist (cdr ls))))))

; Parses attrib file or argument. Returns a list of read attributes.
(define bom:parseconfig
  (lambda (port options)
    (let ((attribs (backend-option-ref options 'attribs)))
      (if attribs (string-split attribs #\,)
          (and port
               (let ((read-from-file (read-delimited " \n\t" port)))
                 (cond ((eof-object? read-from-file)
                        '())
                       ((= 0 (string-length read-from-file))
                        (bom:parseconfig port options))
                       (else
                        (cons read-from-file (bom:parseconfig port options))))))))))

(define bom:components
  (lambda (ls attriblist)
    (if (not (null? ls))
        (let ((package (car ls)))
          (if (not (string=? "1" (get-package-attribute package "nobom")))
              (begin
                (display package)
                (write-char #\tab)
                (bom:printlist (bom:find-attribs package attriblist))))
          (bom:components (cdr ls) attriblist)))))

(define bom:find-attribs
  (lambda (package attriblist)
    (if (null? attriblist)
        '()
        (cons (get-package-attribute package (car attriblist))
              (bom:find-attribs package (cdr attriblist))))))

;;
;; Bill of Material backend written by Matt Ettus ends here
;;
;; --------------------------------------------------------------------------
