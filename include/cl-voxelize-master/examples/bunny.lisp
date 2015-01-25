#|
  This file is a part of cl-voxelize project.
  Copyright (c) 2014 Masayuki Takagi (kamonama@gmail.com)
|#

(in-package :cl-user)
(defpackage cl-voxelize-examples
  (:use :cl :cl-voxelize)
  (:export :main))
(in-package :cl-voxelize-examples)


;;;
;;; PLY format to triangles
;;;

(defun triangles (vertices faces)
  (let (ret)
    (dotimes (i (array-dimension faces 0))
      (let ((face (aref faces i)))
        (let ((v0 (aref vertices (nth 0 face)))
              (v1 (aref vertices (nth 1 face)))
              (v2 (aref vertices (nth 2 face))))
          (push (list v0 v1 v2) ret))))
    ret))

(defun ply-to-triangles (path)
  (cl-ply:with-plyfile (ply path)
    (let ((vertex-size (cl-ply:plyfile-element-size ply "vertex"))
          (face-size (cl-ply:plyfile-element-size ply "face")))
      (let ((vertices (make-array vertex-size))
            (faces (make-array face-size)))
        ;; read vertices
        (loop repeat vertex-size
              for i from 0
           do (cl-ply:with-ply-element ((x y z) ply)
                (setf (aref vertices i) (list x y z))))
        ;; read faces
        (loop repeat face-size
              for i from 0
           do (cl-ply:with-ply-element (vertex-index ply)
                (setf (aref faces i) vertex-index)))
        ;; get triangles from vertices and faces
        (triangles vertices faces)))))


;;;
;;; Output in POV format
;;;

(defun head ()
  "#include \"colors.inc\"
camera {
  location <5.0e-2, 0.2, 0.25>
  look_at  <-3.0e-2, 0.1, 0.0>
  right    <-1.33, 0, 0>
}
light_source {
  <5.0e-2, 0.2, 0.25>
  color White
}
")

(defun sphere (voxel)
  (destructuring-bind (x y z) voxel
    (format nil
"~F,~F,~F
"
x y z)))

(defun output (path voxels)
  (with-open-file (out path :direction :output :if-exists :supersede)
    (dolist (voxel voxels)
      (princ (sphere voxel) out))))


;;;
;;; Main
;;;

(defun main ()
  (let* ((ply-path (make-pathname :defaults "C:/Users/Fabs/Desktop/github/snow/include/cl-voxelize-master/examples/bunny.ply"))
         (pov-path (make-pathname :type "voxel" :defaults "C:/Users/Fabs/Desktop/github/snow/model/bunny")))
    (let ((triangles (ply-to-triangles ply-path))
          (delta 0.002))
      (output pov-path
        (voxelize triangles delta)))))
