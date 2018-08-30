Draft
=====

Class structure:

- `import` [name1 `=`] path [ `*` ] `;`
- `struct` name2 `{` [`var`] type id, ...`}`
- `typedef` name2 Type `;`
- [ `private` ] Type `:` `new` new_params -> Block `;` (Only one and if there is
  a `struct` matching the own class name)
- [ `private` ] Type `:` id `=` definition `;`

Notes:

- Imports
  - *path* use points: "foo.bath.Such"
  - `*` in imports only can be used with main types not with subtypes, with
    have to be imported with name.
  - `name1` can not match the own class one.

- `name2` can match the own class one. In such case can be defined a `new`
  function.

