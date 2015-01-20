#|
  This file is a part of cl-voxelize project.
  Copyright (c) 2014 Masayuki Takagi (kamonama@gmail.com)
|#

(in-package :cl-user)
(defpackage cl-voxelize-test
  (:use :cl
        :cl-voxelize
        :cl-test-more)
  (:import-from :cl-voxelize
                :triangle-intersect-p
                :quadtree
                :intersection-z
                :point-intersect-p
                :query-quadtree))
(in-package :cl-voxelize-test)

(plan nil)


;;;
;;; test TRIANGLE-INTERSECT-P function
;;;

(diag "TRIANGLE-INTERSECT-P")

(is (triangle-intersect-p '(0.0 0.0 2.0 2.0) '((1.0 1.0 0.0)
                                               (3.0 1.0 0.0)
                                               (2.0 3.0 0.0)))
    t "basic case 1")

(is (triangle-intersect-p '(0.0 0.0 2.0 2.0) '((-1.0 1.0 0.0)
                                               (1.0 1.0 0.0)
                                               (0.0 3.0 0.0)))
    t "basic case 2")

(is (triangle-intersect-p '(0.0 0.0 2.0 2.0) '((-1.0 1.0 0.0)
                                               (3.0 1.0 0.0)
                                               (1.0 3.0 0.0)))
    t "basic case 3")

(is (triangle-intersect-p '(0.0 0.0 2.0 2.0) '((1.0 3.0 0.0)
                                               (3.0 3.0 0.0)
                                               (2.0 5.0 0.0)))
    nil "basic case 4")


;;;
;;; test INTERSECTION-Z function
;;;

(diag "INTERSECTION-Z")

(is (intersection-z 0.25 0.25 '((0.0 0.0 1.0) (1.0 0.0 1.0) (0.0 1.0 1.0)))
    1.0 "basic case 1 - a ray intersects with a triangle")

(is (intersection-z 0.75 0.75 '((0.0 0.0 1.0) (1.0 0.0 1.0) (0.0 1.0 1.0)))
    nil "basic case 2 - a ray does not intersect with a triangle")

(is (intersection-z 0.0 0.0 '((0.0 0.0 0.0) (0.0 1.0 0.0) (0.0 0.0 1.0)))
    nil "basic case 3 - a triangle parallel to z-axis")


;;;
;;; test POINT-INTERSECT-P function
;;;

(diag "POINT-INTERSECT-P")

(is (point-intersect-p '(0.0 0.0 2.0 2.0) 1.0 1.0)
    t "basic case 1 - a point intersects with a boundary rectangular")

(is (point-intersect-p '(0.0 0.0 2.0 2.0) -1.0 -1.0)
    nil "basic case 2 - a point does not intersect with a boundary rect.")


;;;
;;; test QUADTREE and QUERY-QUADTREE functions
;;;

(diag "QUADTREE and QUERY-QUADTREE")

(let ((qt (quadtree '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 1.0 0.0))
                      ((0.0 1.0 0.0) (1.0 1.0 1.0) (1.0 2.0 0.0))))))
  (is (query-quadtree qt 1.0 1.0)
      '(((0.0 1.0 0.0) (1.0 1.0 1.0) (1.0 2.0 0.0))
        ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 1.0 0.0)))
      "basic case 1"))

(let ((qt (quadtree '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.9 0.0))
                      ((1.1 1.0 0.0) (2.0 1.0 1.0) (2.0 2.0 0.0))))))
  (cl-voxelize::devide-quadtree qt)
  (is (query-quadtree qt 0.5 0.5)
      '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.9 0.0)))
      "basic case 2"))

(let ((qt (quadtree '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.80 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.81 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.82 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.83 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.84 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.85 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.86 0.0))
                      ((0.0 1.0 0.0) (1.0 1.0 0.0) (1.0 2.0 0.0))))))
  (is (cl-voxelize::quadtree-leaf-p qt)
      t "basic case 3")
  (cl-voxelize::insert-quadtree qt '((0.0 0.0 0.0) (1.0 0.0 0.0)
                                     (0.0 0.87 0.0)))
  (is (cl-voxelize::quadtree-node-p qt)
      t "basic case 4")
  (is (query-quadtree qt 0.5 1.5)
      '(((0.0 1.0 0.0) (1.0 1.0 0.0) (1.0 2.0 0.0)))
      "basic case 5"))

(let ((qt (quadtree '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.80 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.81 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.82 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.83 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.84 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.85 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.86 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.87 0.0))
                      ((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.88 0.0))))))
  (is (cl-voxelize::quadtree-depth (cl-voxelize::quadtree-nw
                                     (cl-voxelize::quadtree-nw
                                       (cl-voxelize::quadtree-nw qt))))
      cl-voxelize::*max-depth* "basic case 6"))


;;;
;;; test VOXELIZE function
;;;

(diag "VOXELIZE")

(let ((triangles '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 1.0 0.0))
                   ((1.0 0.0 0.0) (1.0 1.0 0.0) (0.0 1.0 0.0))
                   ((0.0 1.0 0.0) (1.0 1.0 0.0) (0.0 1.0 1.0))
                   ((1.0 1.0 0.0) (1.0 1.0 1.0) (0.0 1.0 1.0))
                   ((0.0 0.0 1.0) (1.0 0.0 1.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (1.0 0.0 0.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (0.0 0.0 1.0) (1.0 1.0 1.0))
                   ((0.0 0.0 1.0) (0.0 1.0 1.0) (1.0 1.0 1.0))
                   ((0.0 0.0 1.0) (0.0 0.0 0.0) (0.0 1.0 1.0))
                   ((0.0 0.0 0.0) (0.0 1.0 0.0) (0.0 1.0 1.0))
                   ((1.0 0.0 0.0) (1.0 0.0 1.0) (1.0 1.0 0.0))
                   ((1.0 0.0 1.0) (1.0 1.0 1.0) (1.0 1.0 0.0))))
      (delta 0.5))
  (is (length (voxelize triangles delta))
      8 "basic case 1"))

(let ((cl-voxelize::*max-depth* 0)
      (triangles '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 0.7 0.0))
                   ((1.0 0.0 0.0) (1.0 0.7 0.0) (0.0 0.7 0.0))
                   ((0.0 0.7 0.0) (1.0 0.7 0.0) (0.0 0.7 1.0))
                   ((1.0 0.7 0.0) (1.0 0.7 1.0) (0.0 0.7 1.0))
                   ((0.0 0.0 1.0) (1.0 0.0 1.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (1.0 0.0 0.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (0.0 0.0 1.0) (1.0 0.7 1.0))
                   ((0.0 0.0 1.0) (0.0 0.7 1.0) (1.0 0.7 1.0))
                   ((0.0 0.0 1.0) (0.0 0.0 0.0) (0.0 0.7 1.0))
                   ((0.0 0.0 0.0) (0.0 0.7 0.0) (0.0 0.7 1.0))
                   ((1.0 0.0 0.0) (1.0 0.0 1.0) (1.0 0.7 0.0))
                   ((1.0 0.0 1.0) (1.0 0.7 1.0) (1.0 0.7 0.0))
                   ;; to control quadtree's processing area
                   ((0.99 1.0 0.0) (1.0 0.99 0.0) (1.0 1.0 0.0))))
      (delta 0.5))
  (is (length (voxelize triangles delta))
      4 "basic case 1 - not antialiased")
  (is (length (voxelize triangles delta :antialias-p t))
      8 "basic case 2 - antialiased"))


;;;
;;; test DO-VOXELIZE macro
;;;

(diag "DO-VOXELIZE")

(let ((triangles '(((0.0 0.0 0.0) (1.0 0.0 0.0) (0.0 1.0 0.0))
                   ((1.0 0.0 0.0) (1.0 1.0 0.0) (0.0 1.0 0.0))
                   ((0.0 1.0 0.0) (1.0 1.0 0.0) (0.0 1.0 1.0))
                   ((1.0 1.0 0.0) (1.0 1.0 1.0) (0.0 1.0 1.0))
                   ((0.0 0.0 1.0) (1.0 0.0 1.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (1.0 0.0 0.0) (0.0 0.0 0.0))
                   ((1.0 0.0 1.0) (0.0 0.0 1.0) (1.0 1.0 1.0))
                   ((0.0 0.0 1.0) (0.0 1.0 1.0) (1.0 1.0 1.0))
                   ((0.0 0.0 1.0) (0.0 0.0 0.0) (0.0 1.0 1.0))
                   ((0.0 0.0 0.0) (0.0 1.0 0.0) (0.0 1.0 1.0))
                   ((1.0 0.0 0.0) (1.0 0.0 1.0) (1.0 1.0 0.0))
                   ((1.0 0.0 1.0) (1.0 1.0 1.0) (1.0 1.0 0.0))))
      (delta 0.5))
  (let (ret)
    (do-voxelize ((x y z) triangles delta)
      (push (list x y z) ret))
    (is (length ret) 8 "basic case 1")))


(finalize)
