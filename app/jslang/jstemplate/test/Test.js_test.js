// Copyright 12-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/** Server */
export default class Server {

  /* .
  _rc_ Employ : serial
  firstName: string
  # Is married?
  +married: boolean
  age: number=
  #  Pets names
  ## Include all pets
  +pets: !Array<string>
  +accs: Map<Person>
  parent: Person
  sons: Array<Person>
  loans: Map<number>
  */


  // CONSTRUCTOR ---------------------------------
  // TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

  /*--*/
  /**
   * @param {string} firstName
   * @param {boolean} married Is married?
   * @param {number=} age
   * @param {!Array<string>} pets Pets names
   *        Include all pets
   * @param {Map<string, Person>} accs
   * @param {Person} parent
   * @param {Array<Person>} sons
   * @param {Map<string, number>} loans
   */
  constructor (
    firstName,
    married,
    age,
    pets,
    accs,
    parent,
    sons,
    loans
  ) {

    /**
     * @private
     * @type {string}
     */
    this._firstName = firstName;

    /**
     * @private
     * @type {boolean}
     */
    this._married = married;

    /**
     * @private
     * @type {number=}
     */
    this._age = age;

    /**
     * @private
     * @type {!Array<string>}
     */
    this._pets = pets;

    /**
     * @private
     * @type {Map<string, Person>}
     */
    this._accs = accs;

    /**
     * @private
     * @type {Person}
     */
    this._parent = parent;

    /**
     * @private
     * @type {Array<Person>}
     */
    this._sons = sons;

    /**
     * @private
     * @type {Map<string, number>}
     */
    this._loans = loans;

  }

  /**  @return {string} */
  get firstName () {
    return this._firstName;
  }

  /**  @return {boolean} */
  get married () {
    return this._married;
  }

  /**
   * @param {boolean} value
   * @return void
   */
  set married (value) {
    this._married = value;
  }

  /**  @return {number=} */
  get age () {
    return this._age;
  }

  /**  @return {!Array<string>} */
  get pets () {
    return this._pets;
  }

  /**
   * @param {!Array<string>} value
   * @return void
   */
  set pets (value) {
    this._pets = value;
  }

  /**  @return {Map<string, Person>} */
  get accs () {
    return this._accs;
  }

  /**
   * @param {Map<string, Person>} value
   * @return void
   */
  set accs (value) {
    this._accs = value;
  }

  /**  @return {Person} */
  get parent () {
    return this._parent;
  }

  /**  @return {Array<Person>} */
  get sons () {
    return this._sons;
  }

  /**  @return {Map<string, number>} */
  get loans () {
    return this._loans;
  }

  /** @return {!Array<?>} */
  toJs () {
    return [
      this._firstName,
      this._married,
      this._age,
      this._pets,
      this._accs ? this._accs.entries().map(kv =>
        [kv[0], kv[1] ? kv[1].toJs() : null]
      ) : null,
      this._parent ? this._parent.toJs() : null,
      this._sons ? this._sons.map(e => e ? e.toJs() : null) : null,
      this._loans ? this._loans.entries() : null
    ];
  }

  /**
   * @param {!Array<?>} serial
   * @return {!Employ}
   */
  static fromJs (serial) {
    return new Employ(
      serial[0],
      serial[1],
      serial[2],
      serial[3],
      serial[4] ? new Map(serial[4].map(kv =>
        [kv[0], kv[1] ? Person.fromJs(kv[1]) : null]
      )) : null,
      serial[5] ? Person.fromJs(serial[5]) : null,
      serial[6] ? serial[6].map(e =>
        e ? Person.fromJs(e) : null
      ) : null,
      serial[7] ? new Map(serial[7]) : null
    );
  }
  /*--*/

  /**
   * @param {number} id Identifier
   * @param {string} shortName
   * @param {string} longName
   * @param {Conf} companyConf
   * @param {Conf} dailyConf
   * @param {Conf} historicConf
   * @param {Array<Code>} codes
   */
  constructor (
    id,
    shortName,
    longName,
    companyConf,
    dailyConf,
    historicConf,
    codes
  ) {
    this.id = id;
    this.shortName = shortName;
    this.longName = longName;
    this.companyConf = companyConf;
    this.dailyConf = dailyConf;
    this.historicConf = historicConf;
    this.codes = codes;
  }

  // FUNCTIONS -----------------------------------
  // TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

  /**
   * @param {!Array<?>} serial
   * @return {Server}
   */
  static fromJson (serial) {
    return null;
  }
}

export class Conf {
  /**
   * @param {string} url
   * @param {string} templateDate
   * @param {boolean} isIsoNumbers
   * @param {string} fields
   * @param {string} tableStart
   * @param {string} tableEnd
   * @param {string} rowStart
   * @param {string} rowEnd
   * @param {!Array<string>} cellsStart
   * @param {!Array<string>} cellsEnd
   */
  constructor (
    url,
    templateDate,
    isIsoNumbers,
    fields,
    tableStart,
    tableEnd,
    rowStart,
    rowEnd,
    cellsStart,
    cellsEnd
  ) {
    this.url = url;
    this.templateDate = templateDate;
    this.fields = fields;
    this.tableStart = tableStart;
    this.tableEnd = tableEnd;
    this.rowStart = rowStart;
    this.rowEnd = rowEnd;
    this.cellsStart = cellsStart;
    this.cellsEnd = cellsEnd;
  }
}

export class Code {
  /**
   * @param {number} nickId;
   * @param {string} code;
   */
  constructor (
    nickId,
    code
  ) {
    this.nickId = nickId;
    this.code = code;
  }
}

