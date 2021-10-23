create table dut_book
(
    book_id        int  not null,
    name           text not null,
    isbn           text not null,
    publisher      text not null,
    registered_at  int  not null,
    registered_by  text not null,
    max_amount     int  not null,
    current_amount int  not null,
    authors        text not null,
    primary key (book_id)
);

create table dut_user
(
    uid              int  not null,
    name             text not null,
    email            text not null,
    password         text not null,
    permission_group text not null,
    lent_books       text,
    primary key (uid)
);



INSERT INTO dut_book (book_id, name, isbn, publisher, registered_at, registered_by, max_amount, current_amount, authors) VALUES (10001, 'C Primer Plus', '7-115-13022-1', 'Posts & Telecom Press', 1634389509, 'Zeithrold', 10, 9, '["Stephen Prata"]');
INSERT INTO dut_book (book_id, name, isbn, publisher, registered_at, registered_by, max_amount, current_amount, authors) VALUES (10002, 'Elementary of Advanced Math', '978-7-04-043101-8', 'High Education Press', 1634654324, 'Zeithrold', 12, 12, '["Dukuang Su","Jiguang Yu"]');

INSERT INTO dut_user (uid, name, email, password, permission_group, lent_books) VALUES (0, 'dut-elab-test', 'elab@example.dlut.edu.cn', '5678', 'admin', '[10001]');

