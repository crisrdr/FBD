SELECT e3.employeenumber,
       e3.lastname
FROM   employees e3
WHERE  e3.reportsto IN (SELECT e2.employeenumber
                        FROM   employees e2
                        WHERE  e2.reportsto IN (SELECT e.employeenumber
                                                FROM   employees e
                                                WHERE  e.reportsto IS NULL));